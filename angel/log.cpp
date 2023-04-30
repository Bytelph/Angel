#include "log.h"
#include "util.h"
#include <map>
#include <iostream>
#include <functional>
#include <time.h>
#include <string.h>
#include <vector>
#include <set>

#include <mysql/mysql.h>

#include "config.h"

namespace angel
{
/*------------LogLevel------------*/

const char* LogLevel::ToString(LogLevel::Level level)
{
    switch(level)
    {
#define XX(name) \
    case LogLevel::name: \
        return #name; \
        break;
    
    XX(DEBUG);
    XX(INFO);
    XX(WARN);
    XX(ERROR);
    XX(FATAL);
    
#undef XX
    default:
        return "UNKNOW";
    }
    
    return "UNKNOW";
}

LogLevel::Level LogLevel::FromString(const std::string& str)
{
#define XX(level, v) \
    if(str == #v) { \
        return LogLevel::level; \
    }

    XX(DEBUG, debug);
    XX(INFO, info);
    XX(WARN, warn);
    XX(ERROR, error);
    XX(FATAL, fatal);

    
    XX(DEBUG, DEBUG);
    XX(INFO, INFO);
    XX(WARN, WARN);
    XX(ERROR, ERROR);
    XX(FATAL, FATAL);

    return LogLevel::UNKNOW;
#undef XX
};

/*-------------ForamtItems-------------*/
class MessageFormatItem : public LogFormatter::FormatItem
{
public:
    MessageFormatItem(const std::string& str = ""){}
	void format(std::ostream& os,Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
	{
	    os << event->getContent();
	}
};

class LevelFormatItem : public LogFormatter::FormatItem
{
public:
    LevelFormatItem(const std::string& str = ""){}
	void format(std::ostream& os,Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
	{
	    os << LogLevel::ToString(level);
	}
};

class ElapseFormatItem : public LogFormatter::FormatItem
{
public:
    ElapseFormatItem(const std::string& str = ""){}
	void format(std::ostream& os,Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
	{
	    os << event->getElapse();
	}
};

class NameFormatItem : public LogFormatter::FormatItem
{
public:
    NameFormatItem(const std::string& str = ""){}
	void format(std::ostream& os,Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
	{
	    os << event->getLogger()->getName();
	}
};

class ThreadIdFormatItem : public LogFormatter::FormatItem
{
public:
    ThreadIdFormatItem(const std::string& str = ""){}
	void format(std::ostream& os,Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
	{
	    os << event->getThreadId();
	}
};

class ThreadNameFormatItem : public LogFormatter::FormatItem {
public:
    ThreadNameFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getThreadName();
    }
};

class FiberIdFormatItem : public LogFormatter::FormatItem
{
public:
    FiberIdFormatItem(const std::string& str = ""){}
	void format(std::ostream& os,Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
	{
	    os << event->getFiberId();
	}
};

class DateTimeFormatItem : public LogFormatter::FormatItem
{
public:
    DateTimeFormatItem(const std::string& format = "%Y:%m:%d %H:%M:%S"):m_format(format)
    {
        if(m_format.empty()){
            m_format = "%Y-%m-%d %H-%M-%S";
        }
    }
	void format(std::ostream& os,Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
	{
	    struct tm tm;
        time_t time = event->getTime();
        localtime_r(&time, &tm);
        char buf[64];
        strftime(buf, sizeof(buf), m_format.c_str(), &tm);
        os << buf;
	}
	
private:
    std::string m_format;
};

class FilenameFormatItem : public LogFormatter::FormatItem
{
public:
    FilenameFormatItem(const std::string& str = ""){}
	void format(std::ostream& os,Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
	{
	    os << event->getFile();
	}
};

class LineFormatItem : public LogFormatter::FormatItem
{
public:
    LineFormatItem(const std::string& str = ""){}
	void format(std::ostream& os,Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
	{
	    os << event->getLine();
	}
};

class NewLineFormatItem : public LogFormatter::FormatItem
{
public:
    NewLineFormatItem(const std::string& str = ""){}
	void format(std::ostream& os,Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
	{
	    os << std::endl;
	}
};

class StringFormatItem : public LogFormatter::FormatItem
{
public:
    StringFormatItem(const std::string& str) : m_string(str){}
	void format(std::ostream& os,Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
	{
	    os << m_string;
	}
private:
    std::string m_string;
};

class TabFormatItem : public LogFormatter::FormatItem
{
public:
    TabFormatItem(const std::string& str = ""){}
	void format(std::ostream& os,Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override
	{
	    os << "\t";
	}
private:
    std::string m_string;
};

/*---------------Event----------------*/
LogEvent::LogEvent(std::shared_ptr<Logger> logger
        , LogLevel::Level level
        , const char* file
        , int32_t line
        , uint32_t elapse
        , uint32_t thread_id
        , uint32_t fiber_id
        , uint64_t time
        , const std::string& thread_name)
    : m_file(file)
    , m_line(line)
    , m_elapse(elapse)
    , m_threadId(thread_id)
    , m_fiberId(fiber_id)
    , m_time(time)
    , m_threadName(thread_name)
    , m_logger(logger)
    , m_level(level)
{pushMysql();}

bool LogEvent::pushMysql()
{
#define C_DB_SERVER_IP "localhost"
#define C_DB_SERVER_PORT 3306
#define C_DB_USE "angel"
#define C_DB_PSW "123456"
#define C_DB "log"
    //连接Mysql
    if(m_level < m_logger->getLevel()){return false;}
    MYSQL mysql;
    if(mysql_init(&mysql) == NULL)
    {
        return false;
    }

    if(!mysql_real_connect(&mysql, C_DB_SERVER_IP, C_DB_USE, C_DB_PSW, C_DB, C_DB_SERVER_PORT, NULL, 0))
    {
        return false;
    }
    char sql[100] = "insert into %s values('%s',%d,%d,%d,%d,'%s','%s','%s',%d);";
    char insert[1024];
    struct tm tm;
    time_t time = m_time;
    localtime_r(&time, &tm);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H-%M-%S", &tm);

    switch(m_level)
    {
        case LogLevel::Level::UNKNOW:
            sprintf(insert,sql,"unknowlogs",m_file,m_line,m_elapse,m_threadId,m_fiberId,buf,"tname","ooo",0);
            break;
        case LogLevel::Level::DEBUG:
            sprintf(insert,sql,"debuglogs",m_file,m_line,m_elapse,m_threadId,m_fiberId,buf,"tname","ooo",1);
            break;
        case LogLevel::Level::INFO:
            sprintf(insert,sql,"infologs",m_file,m_line,m_elapse,m_threadId,m_fiberId,buf,"tname","ooo",2);
            break;
        case LogLevel::Level::WARN:
            sprintf(insert,sql,"warnlogs",m_file,m_line,m_elapse,m_threadId,m_fiberId,buf,"tname","ooo",3);
            break;
        case LogLevel::Level::ERROR:
            sprintf(insert,sql,"errorlogs",m_file,m_line,m_elapse,m_threadId,m_fiberId,buf,"tname","ooo",4);
            break;
        case LogLevel::Level::FATAL:
            sprintf(insert,sql,"fatallogs",m_file,m_line,m_elapse,m_threadId,m_fiberId,buf,"tname","ooo",5);
            break;
    }
    mysql_query(&mysql, insert);
    mysql_close(&mysql);

    return true;
}

void LogEvent::format(const char* fmt, ...)
{
    va_list al;
    va_start(al, fmt);
    format(fmt, al);
    va_end(al);
}

void LogEvent::format(const char* fmt, va_list al)
{
    char* buf = nullptr;
    int len = vasprintf(&buf, fmt, al);
    if(len != -1)
    {
        m_ss << std::string(buf, len);
        free(buf);
    }
}


/*----------------Logger---------------*/
Logger::Logger(const std::string& name)
    : m_name(name)
    , m_level(LogLevel::DEBUG) 
{  
    m_formatter.reset(new LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
}

void Logger::addAppender(LogAppender::ptr appender)
{
    if(!appender->getFormatter())
    {
        appender->m_formatter = m_formatter;
    }
	m_appenders.push_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender)
{
	for(auto it = m_appenders.begin(); it != m_appenders.end(); ++it)
	{
		if(*it ==appender)
		{
			m_appenders.erase(it);
			break;
		}
	}
}

void Logger::clearAppender()
{
    m_appenders.clear();
}

void Logger::log(LogLevel::Level level,LogEvent::ptr event)
{
    if(level >= m_level)
    {
        auto self = shared_from_this();
        if(!m_appenders.empty())
        {
            for(auto& i : m_appenders)
            {
                i->log(self, level, event);
            }
        } else if(m_root) 
        {
            m_root->log(level, event);
        }
    }	
}

//set the new format of log 
void Logger::setFormatter(LogFormatter::ptr val)
{
    m_formatter = val;
    for(auto& i : m_appenders)
    {
        if(!i->m_hasFormatter)
        {
            i->m_formatter = m_formatter;
        }
    }
}

//trans the format form string to logFormatter::ptr
void Logger::setFormatter(const std::string& val)
{
    angel::LogFormatter::ptr new_val(new angel::LogFormatter(val));
    if(new_val->isError()) 
    {
        std::cout << m_name << ": "<< val << " invalid formatter" << std::endl;
        return;
    }
    setFormatter(new_val);

}
void Logger::debug(LogEvent::ptr event)
{
	log(LogLevel::DEBUG,event);
}
void Logger::info(LogEvent::ptr event)
{
	log(LogLevel::INFO,event);
}
void Logger::warn(LogEvent::ptr event)
{
	log(LogLevel::WARN,event);
}
void Logger::error(LogEvent::ptr event)
{
	log(LogLevel::ERROR,event);
}
void Logger::fatal(LogEvent::ptr event)
{
	log(LogLevel::FATAL,event);
}

std::string Logger::toYamlString()
{
    YAML::Node node;
    node["name"] = m_name;
    if(m_level != LogLevel::UNKNOW)
    {
        node["level"] = LogLevel::ToString(m_level);
    }
    if(m_formatter)
    {
        node["formatter"] = m_formatter->getPattern();
    }
    for(auto& it : m_appenders)
    {
        node["appenders"].push_back(YAML::Load(it->toYamlString()));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();

}


/*-----------------EventWarp------------------*/

LogEventWarp::LogEventWarp(LogEvent::ptr e)
    :m_event(e)
{}
LogEventWarp::~LogEventWarp()
{
    m_event->getLogger()->log(m_event->getLevel(), m_event);

}

std::stringstream& LogEventWarp::getSS()
{
    return m_event->getSS();
}


/*-----------------Formatter------------------*/
LogFormatter::LogFormatter(const std::string& pattern):m_pattern(pattern)
{
	init();
}

std::string LogFormatter::format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
{
	std::stringstream ss;
	for(auto& i : m_items)
	{
		i->format(ss, logger, level, event);
	}
	return ss.str();
}

std::ostream& LogFormatter::format(std::ostream& ofs,std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
{
    for(auto& i : m_items)
    {
            i->format(ofs, logger, level, event);
    }
    return ofs;
}
//%xxx %xxx{xxx} %%
//默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
void LogFormatter::init() {
    //str, format, type
    std::vector<std::tuple<std::string, std::string, int> > vec;
    std::string nstr;
    for(size_t i = 0; i < m_pattern.size(); ++i)
    {
        if(m_pattern[i] != '%')
        {
            nstr.append(1, m_pattern[i]);
            continue;
        }

        if((i + 1) < m_pattern.size())
        {
            if(m_pattern[i + 1] == '%')
            {
                nstr.append(1, '%');
                continue;
            }
        }
        size_t n = i + 1;
        int fmt_status = 0;
        size_t fmt_begin = 0;

        std::string str;
        std::string fmt;
        while(n < m_pattern.size())
        {
            if(!fmt_status && (!isalpha(m_pattern[n]) && m_pattern[n] != '{'
                    && m_pattern[n] != '}'))
            {
                str = m_pattern.substr(i + 1, n - i - 1);
                break;
            }
            if(fmt_status == 0) {
                if(m_pattern[n] == '{')
                {
                    str = m_pattern.substr(i + 1, n - i - 1);
                    fmt_status = 1; //解析格式
                    fmt_begin = n;
                    ++n;
                    continue;
                }
            } else if(fmt_status == 1)
            {
                if(m_pattern[n] == '}')
                {
                    fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
                    fmt_status = 0;
                    ++n;
                    break;
                }
            }
            ++n;
            if(n == m_pattern.size())
            {
                if(str.empty())
                {
                    str = m_pattern.substr(i + 1);
                }
            }
        }

        if(fmt_status == 0)
        {
            if(!nstr.empty())
            {
                vec.push_back(std::make_tuple(nstr, std::string(), 0));
                nstr.clear();
            }
            vec.push_back(std::make_tuple(str, fmt, 1));
            i = n - 1;
        } else if(fmt_status == 1)
        {
            std::cout << "pattern parse error: " << m_pattern << " - " << m_pattern.substr(i) << std::endl;
            m_error = true;
            vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));
        }
    }

    if(!nstr.empty())
    {
        vec.push_back(std::make_tuple(nstr, "", 0));
    }
    static std::map<std::string, std::function<FormatItem::ptr(const std::string& str)> > s_format_items = {
#define XX(str, C) \
        {#str, [](const std::string& fmt) { return FormatItem::ptr(new C(fmt));}}

        XX(m, MessageFormatItem),           //m:消息
        XX(p, LevelFormatItem),             //p:日志级别
        XX(r, ElapseFormatItem),            //r:累计毫秒数
        XX(c, NameFormatItem),              //c:日志名称
        XX(t, ThreadIdFormatItem),          //t:线程id
        XX(n, NewLineFormatItem),           //n:换行
        XX(d, DateTimeFormatItem),          //d:时间
        XX(f, FilenameFormatItem),          //f:文件名
        XX(l, LineFormatItem),              //l:行号
        XX(T, TabFormatItem),               //T:Tab
        XX(F, FiberIdFormatItem),           //F:协程id
        XX(N, ThreadNameFormatItem),        //N:线程名称
#undef XX
    };

    for(auto& i : vec)
    {
        if(std::get<2>(i) == 0) {
            m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
        } else {
            auto it = s_format_items.find(std::get<0>(i));
            if(it == s_format_items.end()) {
                m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
                m_error = true;
            } else {
                m_items.push_back(it->second(std::get<1>(i)));
            }
        }

    }
 }


/*--------------- -基类Appender------------------*/

void LogAppender::setFormatter(LogFormatter::ptr val)
{
    m_formatter = val;
    if(!m_formatter)
    {
        m_hasFormatter = true;
    }else{
        m_hasFormatter = false;
    }
}

LogFormatter::ptr LogAppender::getFormatter()
{
    return m_formatter;
}



/*----------------Appender的子类-----------------*/
FileLogAppender::FileLogAppender(const std::string& filename) :m_filename(filename)
{reopen();}

bool FileLogAppender::reopen()
{
	if(m_filestream)
	{
		m_filestream.close();
	}
	m_filestream.open(m_filename);
	
	return !!m_filestream;
}
void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
{
   if(level >= m_level) {
        uint64_t now = event->getTime();
        if(now >= (m_lastTime + 3)) {
            reopen();
            m_lastTime = now;
        }
        if(!m_formatter->format(m_filestream, logger, level, event))
        {
            std::cout << "write file logs error" << std::endl;
        }
    }
}
std::string FileLogAppender::toYamlString()
{
    YAML::Node node;
    node["type"] = "FileLogAppender";
    node["file"] = m_filename;
    if(m_level != LogLevel::UNKNOW)
    {
        node["level"] = LogLevel::ToString(m_level);
    }
    if(m_hasFormatter && m_formatter)
    {
        node["formatter"] = m_formatter->getPattern();
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}


void StdoutLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event)
{
	if(level >= m_level)
	{
        m_formatter->format(std::cout, logger, level, event);
    }
}


std::string StdoutLogAppender::toYamlString()
{
    YAML::Node node;
    node["type"] = "StdoutLogAppender";
    if(m_level != LogLevel::UNKNOW)
    {
        node["level"] = LogLevel::ToString(m_level);
    }
    if(m_hasFormatter && m_formatter)
    {
        node["formatter"] = m_formatter->getPattern();
    }
    std::stringstream ss;
    ss << node;
    return ss.str();
}

/*--------------------LoggerManager------------------*/
LoggerManager::LoggerManager() 
{
    m_root.reset(new Logger);
    m_root->addAppender(LogAppender::ptr(new StdoutLogAppender));

    m_loggers[m_root->m_name] = m_root;

    init();
}
void LoggerManager::init() {}

Logger::ptr LoggerManager::getLogger(const std::string& name) 
{
    auto it = m_loggers.find(name);
    if(it != m_loggers.end()) {
        return it->second;
    }

    Logger::ptr logger(new Logger(name));
    logger->m_root = m_root;
    m_loggers[name] = logger;
    return logger;
}

std::string LoggerManager::toYamlString()
{
    YAML::Node node;
    for(auto& it : m_loggers)
    {
        node.push_back(YAML::Load(it.second->toYamlString()));
    }
    std::stringstream ss;
    ss << node;
    return ss.str();

}



/*-----------------配置系统整合日志系统----------------*/
struct LogAppenderDefine 
{
    int type = 0; //1 File, 2 Stdout
    LogLevel::Level level = LogLevel::UNKNOW;
    std::string formatter;
    std::string file;

    bool operator==(const LogAppenderDefine& oth) const 
    {
        return type == oth.type
            && level == oth.level
            && formatter == oth.formatter
            && file == oth.file;
    }
};

struct LogDefine
{
    std::string name;
    LogLevel::Level level = LogLevel::UNKNOW;
    std::string formatter;
    std::vector<LogAppenderDefine> appenders;

    bool operator==(const LogDefine& other) const
    {
        return name == other.name
            && level == other.level
            && formatter == other.formatter
            && appenders == other.appenders;
    }

    bool operator<(const LogDefine& other) const
    {
        return name < other.name;
    }
};

angel::ConfigVar<std::set<LogDefine> >::ptr g_log_defines = angel::Config::Lookup("logs", std::set<LogDefine>(), "logs config");

template<>
class LexicalCast<std::string, LogDefine>
{
public:
    LogDefine operator()(const std::string& v)
    {
        YAML::Node node = YAML::Load(v);
        LogDefine ldf;
        if(!node["name"].IsDefined())
        {
            std::cout << "log config error: name is null" << node << std::endl;
            throw std::logic_error("log config is null");
        }
        ldf.name = node["name"].as<std::string>();
        ldf.level = LogLevel::FromString(node["level"].IsDefined() ? node["level"].as<std::string>() : "");
        if(node["formatter"].IsDefined())
        {
            ldf.formatter = node["formatter"].as<std::string>(); 
        }
        if(node["appenders"].IsDefined())
        {
            for(size_t x = 0; x < node["appenders"].size(); ++x)
            {
                auto a = node["appenders"][x];
                if(!a["type"].IsDefined())
                {
                    std::cout << "log config error: appender type is null" << a << std::endl;
                    continue;
                }
                std::string type = a["type"].as<std::string>();
                LogAppenderDefine lad;
                if(type == "FileLogAppender")
                {
                    lad.type = 1;
                    if(!a["file"].IsDefined())
                    {
                        std::cout << "log config error: fileappender iis null" << a <<std::endl;
                        continue;
                    }
                    lad.file = a["file"].as<std::string>();
                    if(a["formatter"].IsDefined())
                    {
                        lad.formatter = a["formatter"].as<std::string>();
                        std::cout << lad.formatter;
                    }
                }else if(type == "StdoutLogAppender"){
                    lad.type = 2;
                    if(a["formatter"].IsDefined())
                    {
                        lad.formatter = a["formatter"].as<std::string>();
                    }
                }else{
                    std::cout << "log config error appender type is invalid." << a << std::endl;
                    continue;
                }
                ldf.appenders.push_back(lad);
            }
        }
        return ldf;
    }
};

template<>
class LexicalCast<LogDefine, std::string>
{
public:
    std::string operator()(const LogDefine& i)
    {
        YAML::Node nodes;
        nodes["name"] = i.name;
        if(i.level != LogLevel::UNKNOW)
        {
            nodes["level"] = LogLevel::ToString(i.level);
        }
        if(!i.formatter.empty())
        {
            nodes["formatter"] = i.formatter;
        }

        for(auto& a : i.appenders)
        {
            YAML::Node inode;
            if(a.type == 1)
            {
                inode["type"] = "FileLogAppender";
                inode["file"] = a.file;
            }else if(a.type == 2){
                inode["type"] = "StdoutLogAppender";
            }
            if(a.level != LogLevel::UNKNOW)
            {
                inode["level"] = LogLevel::ToString(a.level);
                std::cout << "####" << inode["level"] << std::endl;
            }
            if(!a.formatter.empty())
            {
                inode["formatter"] = a.formatter;
            }
            nodes["appenders"].push_back(inode);
        }
        std::stringstream ss;
        ss << nodes;
        return ss.str();
    }
};

struct LogIniter
{
    LogIniter() 
    {
        g_log_defines->addListener([](const std::set<LogDefine>& old_value,
                    const std::set<LogDefine>& new_value){
            ANGEL_LOG_INFO(ANGEL_LOG_ROOT()) << "on_logger_conf_changed";
            for(auto& i : new_value) 
            {
                auto it = old_value.find(i);
                angel::Logger::ptr logger;
                if(it == old_value.end()) 
                {
                    //新增logger
                    logger = ANGEL_LOG_NAME(i.name);
                } else {
                    if(!(i == *it)) 
                    {
                        //修改的logger
                        logger = ANGEL_LOG_NAME(i.name);
                    } else {
                        continue;
                    }
                }
                logger->setLevel(i.level);
                if(!i.formatter.empty()) 
                {
                    logger->setFormatter(i.formatter);
                }

                logger->clearAppender();
                for(auto& a : i.appenders) 
                {
                    angel::LogAppender::ptr ap;
                    if(a.type == 1) 
                    {
                        ap.reset(new FileLogAppender(a.file));
                    } else if(a.type == 2) {
                        ap.reset(new StdoutLogAppender);
                    }
                    ap->setLevel(a.level);
                    if(!a.formatter.empty()) 
                    {
                        LogFormatter::ptr fmt(new LogFormatter(a.formatter));
                        if(!fmt->isError()) {
                            ap->setFormatter(fmt);
                        } else {
                            std::cout << "log.name=" << i.name << " appender type=" << a.type
                                      << " formatter=" << a.formatter << " is invalid" << std::endl;
                        }
                    }
                    logger->addAppender(ap);
                }
            }

            for(auto& i : old_value) 
            {
                auto it = new_value.find(i);
                if(it == new_value.end()) 
                {
                    //删除logger
                    auto logger = ANGEL_LOG_NAME(i.name);
                    logger->setLevel((LogLevel::Level)0);
                    logger->clearAppender();
                }
            }
        });
    }
};

static LogIniter __log_init;
}

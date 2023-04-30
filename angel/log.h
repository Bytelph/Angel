#ifndef __ANGEL_LOG_H__
#define __ANGEL_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdarg.h>
#include <map>
#include "singleton.h"

/* 名称：常用接口宏定义
 * 日期:2023.03.22
 * 目的：定义外部可调用的接口，实现外部软件对所需产生的日志按照对应的级别，格式进行解析
 */

//日志器管理回调root用户权限作为用户名
#define ANGEL_LOG_ROOT() angel::LoggerMgr::GetInstance()->getRoot()
//通过日志器name在日志器管理中查找对应名称用户的日志器
#define ANGEL_LOG_NAME(name) angel::LoggerMgr::GetInstance()->getLogger(name);

//日志解析接口，通过已知的日志器和日志级别，对所产生的日志进行解析
#define ANGEL_LOG_LEVEL(logger, level) \
    if(logger->getLevel() <= level) \
        angel::LogEventWarp(angel::LogEvent::ptr(new angel::LogEvent(logger, level, __FILE__, __LINE__, 0, angel::GetThreadId(), angel::GetFiberId(), time(0), "tname" ))).getSS()

#define ANGEL_LOG_DEBUG(logger) ANGEL_LOG_LEVEL(logger, angel::LogLevel::DEBUG)
#define ANGEL_LOG_INFO(logger) ANGEL_LOG_LEVEL(logger, angel::LogLevel::INFO)
#define ANGEL_LOG_WARN(logger) ANGEL_LOG_LEVEL(logger, angel::LogLevel::WARN)
#define ANGEL_LOG_ERROR(logger) ANGEL_LOG_LEVEL(logger, angel::LogLevel::ERROR)
#define ANGEL_LOG_FATAL(logger) ANGEL_LOG_LEVEL(logger, angel::LogLevel::FATAL)

//日志解析接口，通过已知的日志器，日志级别，日志格式对所产生的日志进行解析
#define ANGEL_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if(logger->getLevel() <= level) \
        angel::LogEventWarp(angel::LogEvent::ptr(new angel::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, angel::GetThreadId(),\
                angel::GetFiberId(), time(0), "tname"))).getEvent()->format(fmt, __VA_ARGS__)

#define ANGEL_LOG_FMT_DEBUG(logger, fmt, ...) ANGEL_LOG_FMT_LEVEL(logger, angel::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define ANGEL_LOG_FMT_INFO(logger, fmt, ...) ANGEL_LOG_FMT_LEVEL(logger, angel::LogLevel::INFO, fmt, __VA_ARGS__)
#define ANGEL_LOG_FMT_WARN(logger, fmt, ...) ANGEL_LOG_FMT_LEVEL(logger, angel::LogLevel::WARN, fmt, __VA_ARGS__)
#define ANGEL_LOG_FMT_ERROR(logger, fmt, ...) ANGEL_LOG_FMT_LEVEL(logger, angel::LogLevel::ERROR, fmt, __VA_ARGS__)
#define ANGEL_LOG_FMT_FATAL(logger, fmt, ...) ANGEL_LOG_FMT_LEVEL(logger, angel::LogLevel::FATAL, fmt, __VA_ARGS__)

namespace angel
{

class Logger;

/* 类名：LogLevel
 * 功能：作为日志级别信息，日志的优先级只有大于Logger的日志级别才会被响应和解析，优先级越大表明日志内容的重要性月高
 * 0：UNKNOW：未知级别；
 * 1：DEBUG：调试级别
 * 2：INFO：通知级别
 * 3：WARN：警告级别
 * 4：ERROR：错误级别
 * 5：FATAL：致命级别
 */
 
class LogLevel
{
public:
	enum Level
	{
        UNKNOW = 0,
		DEBUG = 1,
		INFO =2,
		WARN =3,
		ERROR = 4,
		FATAL = 5
	};
    
    //日志级别转化为文本输出
    static const char* ToString(LogLevel::Level level);
    //文本转化为日志级别
    static LogLevel::Level FromString(const std::string& str);
};

/* 类名：LogEvent
 * 功能：日志事件类，一个事件对应一个事件类对象，包含了一个日志的基本信息
 * m_file：文件名
 * m_line：行号
 * m_elapse：进程启动起的毫秒数 
 * m_threadId：线程id
 * m_fiberId：携程id
 * m_time：时间戳
 * m_threadName：线程名称  
 * m_level：日志等级
 */

class LogEvent
{
public:
	typedef std::shared_ptr<LogEvent> ptr;
    LogEvent(std::shared_ptr<Logger> logger,LogLevel::Level level
            ,const char* file, int32_t line, uint32_t elapse
            ,uint32_t thread_id, uint32_t fiber_id, uint64_t time
            ,const std::string& thread_name);

    const char* getFile() const {return m_file;}
    int32_t getLine() const {return m_line;}
    uint32_t getElapse() const {return m_elapse;}
    uint32_t getThreadId() const {return m_threadId;}
    uint32_t getFiberId() const {return m_fiberId;}
    uint64_t getTime() const {return m_time;}
    const std::string& getThreadName() {return m_threadName;}
    std::stringstream& getSS() {return m_ss;}
    std::string getContent() const {return m_ss.str();}
    std::shared_ptr<Logger> getLogger() const {return m_logger;}
    LogLevel::Level getLevel() const {return m_level;}
    bool pushMysql();

    //格式化写入日志
    void format(const char* fmt, ...);
    void format(const char* fmt, va_list al);
  
private:
	const char* m_file = nullptr;      //文件名
	int32_t m_line = 0;                //行号
	uint32_t m_elapse = 0;		    //进程启动起的毫秒数 
	uint32_t m_threadId = 0;           //线程id
	uint32_t m_fiberId =0;       	    //携程id
	uint64_t m_time = 0;		    //时间戳
	std::string m_threadName;       //线程名称
    std::stringstream m_ss;         //日志内容流
    std::shared_ptr<Logger> m_logger;   //日志器
    LogLevel::Level m_level;        //日志等级

};

/* 类名：LogEventWarp
 * 功能：日志包装类，对日志事件类进行包装，
 */

class LogEventWarp
{
public:
    LogEventWarp(LogEvent::ptr e);
    ~LogEventWarp();

    std::stringstream& getSS();

    LogEvent::ptr getEvent() const {return m_event;}

private:
    LogEvent::ptr m_event;
};



/* 类名：LogFormatter
 * 功能：日志格式器，在日志解析过程中提供多样化的日志格式，包括默认类型，自定义类型日志格式
 * 默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
 * %m 输出代码中指定的消息；
 * %M 输出打印该条日志的方法名；
 * %p 输出优先级，即DEBUG，INFO，WARN，ERROR，FATAL；
 * %r 输出自应用启动到输出该log信息耗费的毫秒数；
 * %c 输出所属的类目，通常就是所在类的全名；
 * %t 输出产生该日志事件的线程名；
 * %n 输出一个回车换行符，Windows平台为"rn”，Unix平台为"n”；
 * %d 输出日志时间点的日期或时间，默认格式为ISO8601，也可以在其后指定格式，比如：%d{yyyy-MM-dd HH:mm:ss,SSS}，输出类似：2002-10-18 22:10:28,921；
 * %l 输出日志事件的发生位置，及在代码中的行数；
 */
class LogFormatter
{
public:
	typedef std::shared_ptr<LogFormatter> ptr;

	LogFormatter(const std::string& pattern);

    std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
    std::ostream& format(std::ostream& ofs, std::shared_ptr<Logger> logger,LogLevel::Level level, LogEvent::ptr event);

public:	
	class FormatItem
	{
	public:
		typedef std::shared_ptr<FormatItem> ptr;

		virtual ~FormatItem(){}
        //格式化日志流
		virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
	};
	
    //初始化解析日志模板
    void init();
    //返回日志格式是否正确
    bool isError() const {return m_error;}
    //返回日志格式模板
    const std::string getPattern() const {return m_pattern;}
	
private:
    //日志模板
    std::string m_pattern;
    //日志格式解析
	std::vector<FormatItem::ptr> m_items;
    bool m_error = false;
};

/* 类名：LogAppender
 * 功能：日志输出方式类，为抽象类，提供日志输出的方式，子类有StdoutLogAppender和FileLogAppender
 * StdoutLogAppender：将日志输出到调试控制台上
 * FileLogAppender：将日志输出到日志文件中
 */
class LogAppender
{
    friend class Logger;
public:
	typedef std::shared_ptr<LogAppender> ptr;
	virtual ~LogAppender(){}
	
    //写入日志
	virtual void log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) = 0;
    
    //更改日志格式器
	void setFormatter(LogFormatter::ptr val);
    //获取日志格式器
	LogFormatter::ptr getFormatter();
    //获取日志级别
    LogLevel::Level getLevel() const {return m_level;}
	//设置日志级别
    void setLevel(LogLevel::Level val) {m_level = val;}

    virtual std::string toYamlString() = 0;
protected:
    //日志级别
	LogLevel::Level m_level = LogLevel::DEBUG;
    //是否有日志格式器
    bool m_hasFormatter = false;
    //日志格式器
	LogFormatter::ptr m_formatter;
	
};

/* 类名：Logger
 * 功能：日志器，包含日志器的基本信息和日志解析函数，日志器可以根据不同的日志级别解析输出不同日志信息
 */
class Logger : public std::enable_shared_from_this<Logger>
{
    friend class LoggerManager;
public:
	typedef std::shared_ptr<Logger> ptr;

	Logger(const std::string& name = "root");
    //写日志
	void log(LogLevel::Level level, LogEvent::ptr event);
	//写不同级别的日志
	void debug(LogEvent::ptr event);
	void info(LogEvent::ptr event);
	void warn(LogEvent::ptr event);
	void error(LogEvent::ptr event);
	void fatal(LogEvent::ptr event);
	
	void addAppender(LogAppender::ptr appender);
	void delAppender(LogAppender::ptr appender);
    void clearAppender();
	LogLevel::Level getLevel() const {return m_level;}
	void setLevel(LogLevel::Level val){m_level = val;}
  
    const std::string& getName() const {return m_name;}
    //设置日志格式器
    void setFormatter(LogFormatter::ptr val);
    //设置日志格式模板
    void setFormatter(const std::string& val);
    //获取日志格式器
    LogFormatter::ptr getFormatter();
    std::string toYamlString();

private:
	std::string m_name;		                        //日志名称
	LogLevel::Level m_level;	                    //日志级别
	std::list<LogAppender::ptr> m_appenders;		//Appender集合
    LogFormatter::ptr m_formatter;                  //日志格式器
    Logger::ptr m_root;                             //主日志器
};

/* 类名：StdoutLogAppender
 * 功能：将日志输出到调试控制台上
 */
class StdoutLogAppender: public LogAppender
{
public:
	typedef std::shared_ptr<StdoutLogAppender> ptr;
   
    void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event);
    std::string toYamlString() override;
protected:

private:
};


/* 类名：FileLogAppender
 * 将日志输出到日志文件中
 */
class FileLogAppender: public LogAppender
{
public:
	typedef std::shared_ptr<FileLogAppender> ptr;
	
    FileLogAppender(const std::string& filename);
	void log(Logger::ptr logger, LogLevel::Level level,LogEvent::ptr event);

	bool reopen();
    std::string toYamlString() override;
protected:
private:
    //文件路径
	std::string m_filename;
    //文件流
	std::ofstream m_filestream;
    //上次重新打开的时间
    uint64_t m_lastTime = 0; 
};

/* 类名：LoggerManger
 * 功能：对所有日志器进行管理
 */
class LoggerManager
{
public:
    LoggerManager();
    
    //获取日志器
    Logger::ptr getLogger(const std::string& name);
    
    void init();
    Logger::ptr getRoot() const {return m_root;}
    std::string toYamlString();

private:
    //日志器容器
    std::map<std::string, Logger::ptr> m_loggers;
    //主日志器
    Logger::ptr m_root;
};

//日志器管理类单例模式
typedef angel::Singleton<LoggerManager> LoggerMgr;

}
#endif

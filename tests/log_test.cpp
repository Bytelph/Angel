#include <iostream>
//#include <thread>
//#include <vector>
#include "../angel/log.h"
#include "../angel/util.h"

/*
void parseLog(angel::Logger::ptr logger)
{
    ANGEL_LOG_ERROR(logger) << "error message";
}

int main(int argc, char** argv)
{    
    angel::Logger::ptr g_logger = ANGEL_LOG_ROOT();
    angel::Logger::ptr logger( new angel::Logger);
    logger->addAppender(angel::LogAppender::ptr(new angel::StdoutLogAppender));
    std::vector<std::thread> threads;
    for(int i = 0; i< 5; ++i)
    {
        threads.emplace_back(std::thread(parseLog,logger));
    }
    for(auto& t : threads)
    {
        t.join();
    }

    return 0;
}*/

int main(int argc, char** argv)
{    
    //angel::Logger::ptr g_logger = ANGEL_LOG_ROOT();

    angel::Logger::ptr logger( new angel::Logger);
    logger->addAppender(angel::LogAppender::ptr(new angel::StdoutLogAppender));

    //angel::LogAppender::ptr f_appender(new angel::FileLogAppender("./log/log.log"));
    //f_appender->setFormatter(fmt);
    //f_appender ->setLevel(angel::LogLevel::ERROR);
    //logger->addAppender(f_appender);

    logger->setLevel(angel::LogLevel::FATAL);
    //logger->setFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T%m%n");
    
   // angel::LogEvent::ptr event(new angel::LogEvent(g_logger ,angel::LogLevel::DEBUG,  __FILE__, __LINE__, 0, angel::GetThreadId(), angel::GetFiberId(), time(0), "tname"));
   // std::cout << event->getTime()<<std::endl<<event->getFile() <<std::endl;
   // event->getSS() << "hello angel log" << std::endl;
   // std::cout << event->getContent() << std::endl;

   // logger->log(angel::LogLevel::DEBUG, event);
    
    std::cout <<std::endl;
    std::cout <<std::endl;
    ANGEL_LOG_DEBUG(logger) << "debug message";
    ANGEL_LOG_INFO(logger) << "info message";
    ANGEL_LOG_WARN(logger) << "warn message";
    ANGEL_LOG_ERROR(logger) << "error message";
    ANGEL_LOG_FATAL(logger) << "fatal message";

    //ANGEL_LOG_FMT_ERROR(logger, "error %s", "aa");

    //auto l = angel::LoggerMgr::GetInstance()->getLogger("xx");
    //ANGEL_LOG_INFO(l) << "eror";
    std::cout <<std::endl;
    std::cout <<std::endl;
    return 0;
}

/*
#include <cstdlib>
#include <chrono>

using namespace std;

// 定义解析日志的函数
int parseLog() {
    // 在此处添加解析日志的代码
    angel::Logger::ptr logger( new angel::Logger);
    logger->addAppender(angel::LogAppender::ptr(new angel::StdoutLogAppender));
    ANGEL_LOG_INFO(logger) << "info message";
    return 0;// 假设解析出来的结果为日志行的长度
}

int main(int argc, char* argv[]) {
    // 计时器开始
    auto start_time = chrono::steady_clock::now();

    // 按行读取测试数据，并解析
    int parsed_lines = 0;
    for(int i = 0; i < 10000; i++) {
        // 调用解析日志的函数
        parseLog();
        parsed_lines++;
    }

    // 计时器结束
    auto end_time = chrono::steady_clock::now();
    auto time_diff = end_time - start_time;

    // 输出结果
    cout << "Parsed " << parsed_lines << " log lines in "
         << chrono::duration_cast<chrono::milliseconds>(time_diff).count()
         << " milliseconds" << endl;

    return 0;
}
*/

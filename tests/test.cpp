#include <iostream>
#include "../angel/log.h"
#include "../angel/util.h"

int main(int argc, char** argv)
{    
    angel::Logger::ptr g_logger = ANGEL_LOG_ROOT();

    angel::Logger::ptr logger( new angel::Logger);
    logger->addAppender(angel::LogAppender::ptr(new angel::StdoutLogAppender));

    angel::LogAppender::ptr f_appender(new angel::FileLogAppender("./log/log.log"));
    //f_appender->setFormatter(fmt);
    //f_appender ->setLevel(angel::LogLevel::ERROR);
    logger->addAppender(f_appender);
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

    /*auto l = angel::LoggerMgr::GetInstance()->getLogger("xx");
    ANGEL_LOG_INFO(l) << "eror";*/
    std::cout <<std::endl;
    std::cout <<std::endl;
    return 0;
}


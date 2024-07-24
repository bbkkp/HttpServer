#include <iostream>

#include "Logger.h"
#include "Timestamp.h"
Logger &Logger::instance()
{
    static Logger logger;
    return logger;
}
void Logger::setLogLevel(int level)
{
    logLevel_ = level;
}
void Logger::log(std::string msg,std::string func_name,std::string file_name, int line)
{
    switch (logLevel_)
    {
    case INFO:
        std::cout << "[INFO]";
        break;
    case ERROR:
        std::cout << "[ERROR]";
        break;
    case FATAL:
        std::cout << "FATAL]";
        break;
    case DEBUG:
        std::cout << "[DEBUG]";
        break;
    default:
        break;
    }
    std::cout <<"["<< Timestamp::now().to_string() <<"]----"<< msg <<"--["<<func_name<<"]"<<"["<<file_name<<" "<<line<<"]"<<std::endl;

}
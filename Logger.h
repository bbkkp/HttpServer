#pragma once
#include <string>

#include "noncopyable.h"

#define logInfo(LogmsgFormat, ...) \
    do \
    { \
        Logger &logger = Logger::instance(); \
        logger.setLogLevel(INFO); \
        char buf[1024] = {0}; \
        snprintf(buf, 1024,  LogmsgFormat ,##__VA_ARGS__); \
        logger.log(buf,  __func__, __FILE__, __LINE__); \
    }while (0)
#define logError(LogmsgFormat, ...) \
    do \
    { \
        Logger &logger = Logger::instance(); \
        logger.setLogLevel(INFO); \
        char buf[1024] = {0}; \
        snprintf(buf, 1024,  LogmsgFormat ,##__VA_ARGS__); \
       logger.log(buf,  __func__, __FILE__, __LINE__); \
    }while (0)

#define logFatal(LogmsgFormat, ...) \
    do \
    { \
        Logger &logger = Logger::instance(); \
        logger.setLogLevel(INFO); \
        char buf[1024] = {0}; \
      snprintf(buf, 1024,  LogmsgFormat ,##__VA_ARGS__); \
        logger.log(buf,  __func__, __FILE__, __LINE__); \
    }while (0)
#ifdef MUDEBUG
#define logDebug(LogmsgFormat, ...) \
    do \
    { \
        Logger &logger = Logger::instance(); \
        logger.setLogLevel(INFO); \
        char buf[1024] = {0}; \
       snprintf(buf, 1024,  LogmsgFormat ,##__VA_ARGS__); \
        logger.log(buf,  __func__, __FILE__, __LINE__); \
    }while (0)
#else
    #define logDebug(LogmsgFormat,...)
#endif
enum logLevel
{
    INFO,
    ERROR,
    FATAL,
    DEBUG,
};

class Logger : noncopyable
{
public:
    static Logger &instance();
    void setLogLevel(int level);
    void log(std::string msg,std::string func_name,std::string file_name, int line);

private:
    int logLevel_;
    Logger() = default;
};
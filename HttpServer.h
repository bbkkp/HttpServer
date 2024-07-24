#pragma once

#include <signal.h>

#define PORT 8081

// HTTP服务器
class HttpServer
{
private:
    int _port; // 端口号
public:
    HttpServer(int port)
        : _port(port)
    {
    }

    // 初始化服务器
    void InitServer()
    {
        signal(SIGPIPE, SIG_IGN); // 忽略SIGPIPE信号，防止写入时崩溃
    }
    // 启动服务器
    void Loop();

    ~HttpServer()
    {
    }
};
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "Logger.h"
#include "TcpServer.h"

// 获取单例对象
TcpServer *TcpServer::GetInstance(int port)
{
    static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; // 定义静态的互斥锁
    if (_svr == nullptr)
    {
        pthread_mutex_lock(&mtx); // 加锁
        if (_svr == nullptr)
        {
            // 创建单例TCP服务器对象并初始化
            _svr = new TcpServer(port);
            _svr->InitServer();
        }
        pthread_mutex_unlock(&mtx); // 解锁
    }
    return _svr; // 返回单例对象
}

// 初始化服务器
void TcpServer::InitServer()
{
    Socket(); // 创建套接字
    Bind();   // 绑定
    Listen(); // 监听
    logInfo("tcp_server init ... success");
}

// 创建套接字
void TcpServer::Socket()
{
    _listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (_listen_sock < 0)
    { // 创建套接字失败
        logInfo("socket error!");
        return;
    }
    // 设置端口复用
    int opt = 1;
    setsockopt(_listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    logInfo("create socket ... success");
}

// 绑定
void TcpServer::Bind()
{
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(_port);
    local.sin_addr.s_addr = INADDR_ANY;

    if (bind(_listen_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
    { // 绑定失败
        logInfo("bind error!");
        return;
    }
    logInfo("bind socket ... success");
}

// 监听
void TcpServer::Listen()
{
    if (listen(_listen_sock, BACKLOG) < 0)
    { // 监听失败
        logInfo("listen error!");
        return;
    }
    logInfo("listen socket ... success");
}

// 获取监听套接字
int TcpServer::Sock()
{
    return _listen_sock;
}
// 单例对象指针初始化为nullptr
TcpServer *TcpServer::_svr = nullptr;
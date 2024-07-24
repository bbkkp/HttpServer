#pragma once

#include "noncopyable.h"
#define BACKLOG 5

// TCP服务器
class TcpServer : noncopyable
{
private:
    int _port;              // 端口号
    int _listen_sock;       // 监听套接字
    static TcpServer *_svr; // 指向单例对象的static指针
private:
    // 构造函数私有
    TcpServer(int port)
        : _port(port), _listen_sock(-1)
    {
    }

public:
    // 获取单例对象
    static TcpServer *GetInstance(int port);
    // 初始化服务器
    void InitServer();
    // 创建套接字
    void Socket();
    // 绑定
    void Bind();
    // 监听
    void Listen();
    // 获取监听套接字
    int Sock();

    ~TcpServer()
    {
        if (_listen_sock >= 0)
        { // 关闭监听套接字
            close(_listen_sock);
        }
    }
};

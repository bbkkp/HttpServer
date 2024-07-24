#include <iostream>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "TcpServer.h"
#include "Logger.h"
#include "HttpServer.h"
#include "threadPool11.h"
#include "Protocol.h"
// void *print(void *)
// {
//     std::cout << "somrthing coming" << std::endl;
//     return((void *)0);
// }
// 启动服务器
void HttpServer::Loop()
{
    logInfo("loop begin");
    TcpServer *tsvr = TcpServer::GetInstance(_port); // 获取TCP服务器单例对象
    int listen_sock = tsvr->Sock();                  // 获取监听套接字
    ThreadPool threadPool;                             //创s建c++11线程池
    while (true)
    {
        struct sockaddr_in peer;
        memset(&peer, 0, sizeof(peer));
        socklen_t len = sizeof(peer);
        int sock = accept(listen_sock, (struct sockaddr *)&peer, &len); // 获取新连接
        if (sock < 0)
        {
            continue; // 获取失败，继续获取
        }

        // 打印客户端相关信息
        std::string client_ip = inet_ntoa(peer.sin_addr);
        int client_port = ntohs(peer.sin_port);

        logInfo("get a new link: [%s:%d]",client_ip.c_str(),client_port);
        
        //构建任务并放入任务队列中
        // Task task(sock);
        // ThreadPool::GetInstance()->PushTask(task);


         
         auto ret = threadPool.commit(CallBack::HandlerRequest,sock);
        // 创建新线程处理新连接发起的HTTP请求
        // int *p = new int(sock);
        // pthread_t tid;
        // pthread_create(&tid, nullptr, print, nullptr);
        // pthread_detach(tid); // 线程分离
    }
}

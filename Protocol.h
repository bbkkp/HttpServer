#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "Util.h"
#include "Logger.h"

#define SEP ": "
#define WEB_ROOT "wwwroot"
#define HOME_PAGE "index.html"
#define HTTP_VERSION "HTTP/1.0"
#define LINE_END "\r\n"

#define PAGE_400 "400.html"
#define PAGE_404 "404.html"
#define PAGE_500 "500.html"

#define OK 200
#define BAD_REQUEST 400
#define NOT_FOUND 404
#define INTERNAL_SERVER_ERROR 500

// HTTP请求
class HttpRequest
{
public:
    // HTTP请求内容
    std::string _request_line;                // 请求行
    std::vector<std::string> _request_header; // 请求报头
    std::string _blank;                       // 空行
    std::string _request_body;                // 请求正文

    // 解析结果
    std::string _method;                                     // 请求方法
    std::string _uri;                                        // URI
    std::string _version;                                    // 版本号
    std::unordered_map<std::string, std::string> _header_kv; // 请求报头中的键值对
    int _content_length;                                     // 正文长度
    std::string _path;                                       // 请求资源的路径
    std::string _query_string;                               // uri中携带的参数

    // CGI相关
    bool _cgi; // 是否需要使用CGI模式
public:
    HttpRequest()
        : _content_length(0) // 默认请求正文长度为0
          ,
          _cgi(false) // 默认不使用CGI模式
    {
    }
    ~HttpRequest()
    {
    }
};

// HTTP响应
class HttpResponse
{
public:
    // HTTP响应内容
    std::string _status_line;                  // 状态行
    std::vector<std::string> _response_header; // 响应报头
    std::string _blank;                        // 空行
    std::string _response_body;                // 响应正文（CGI相关）

    // 所需数据
    int _status_code;    // 状态码
    int _fd;             // 响应文件的fd  （非CGI相关）
    int _size;           // 响应文件的大小（非CGI相关）
    std::string _suffix; // 响应文件的后缀（非CGI相关）
public:
    HttpResponse()
        : _blank(LINE_END) // 设置空行
          ,
          _status_code(OK) // 状态码默认为200
          ,
          _fd(-1) // 响应文件的fd初始化为-1
          ,
          _size(0) // 响应文件的大小默认为0
    {
    }
    ~HttpResponse()
    {
    }
};

// 读取请求、分析请求、构建响应
// IO通信

// 服务端EndPoint
class EndPoint
{
private:
    int _sock;                   // 通信的套接字
    HttpRequest _http_request;   // HTTP请求
    HttpResponse _http_response; // HTTP响应
    bool _stop;                  // 是否停止本次处理
private:
    // 读取请求行
    bool RecvHttpRequestLine();

    // 读取请求报头和空行
    bool RecvHttpRequestHeader();

    // 解析请求行
    void ParseHttpRequestLine();

    // 解析请求报头
    void ParseHttpRequestHeader();

    // 判断是否需要读取请求正文
    bool IsNeedRecvHttpRequestBody();

    // 读取请求正文
    bool RecvHttpRequestBody();

    // CGI处理
    int ProcessCgi();

    // 非CGI处理
    int ProcessNonCgi();

    void BuildOkResponse();

    void HandlerError(std::string page);

public:
    EndPoint(int sock)
        : _sock(sock), _stop(false)
    {
    }
    ~EndPoint()
    {
    }
    // 本次处理是否停止
    bool IsStop();

    // 读取请求
    void RecvHttpRequest();

    // 处理请求
    void HandlerHttpRequest();

    // 构建响应
    void BuildHttpResponse();

    // 发送响应
    bool SendHttpResponse();
};

class CallBack
{
public:
    CallBack()
    {
    }
    void operator()(int sock)
    {
        HandlerRequest(sock);
    }
     static void HandlerRequest(int sock);
    ~CallBack()
    {
    }
};
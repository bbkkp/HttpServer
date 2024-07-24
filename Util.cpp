#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unordered_map>

#include "Util.h"

// 读取一行
int Util::ReadLine(int sock, std::string &out)
{
    char ch = 'X'; // ch只要不初始化为\n即可（保证能够进入while循环）
    while (ch != '\n')
    {
        ssize_t size = recv(sock, &ch, 1, 0);
        if (size > 0)
        {
            if (ch == '\r')
            {
                // 窥探下一个字符是否为\n
                recv(sock, &ch, 1, MSG_PEEK);

                if (ch == '\n')
                { // 下一个字符是\n
                    //\r\n->\n
                    recv(sock, &ch, 1, 0); // 将这个\n读走
                }
                else
                { // 下一个字符不是\n
                    //\r->\n
                    ch = '\n'; // 将ch设置为\n
                }
            }
            // 普通字符或\n
            out.push_back(ch);
        }
        else if (size == 0)
        { // 对方关闭连接
            return 0;
        }
        else
        { // 读取失败
            return -1;
        }
    }
    return out.size(); // 返回读取到的字符个数
}
// 切割字符串
bool Util::CutString(std::string &target, std::string &sub1_out, std::string &sub2_out, std::string sep)
{
    size_t pos = target.find(sep, 0);
    if (pos != std::string::npos)
    {
        sub1_out = target.substr(0, pos);
        sub2_out = target.substr(pos + sep.size());
        return true;
    }
    return false;
}
//根据状态码获取状态码描述
std::string Util::CodeToDesc(int code)
{
    std::string desc;
    switch(code){
        case 200:
            desc = "OK";
            break;
        case 400:
            desc = "Bad Request";
            break;
        case 404:
            desc = "Not Found";
            break;
        case 500:
            desc = "Internal Server Error";
            break;
        default:
            break;
    }
    return desc;
}
//根据后缀获取资源类型
std::string Util::SuffixToDesc(const std::string& suffix)
{
    static std::unordered_map<std::string, std::string> suffix_to_desc = {
        {".html", "text/html"},
        {".css", "text/css"},
        {".js", "application/x-javascript"},
        {".jpg", "application/x-jpg"},
        {".xml", "text/xml"}
    };
    auto iter = suffix_to_desc.find(suffix);
    if(iter != suffix_to_desc.end()){
        return iter->second;
    }
    return "text/html"; //所给后缀未找到则默认该资源为html文件
}

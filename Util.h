#pragma once
#include <iostream>
//工具类
class Util{
    public:
        //读取一行
        static int ReadLine(int sock, std::string& out);
        //切割字符串
        static bool CutString(std::string& target, std::string& sub1_out, std::string& sub2_out, std::string sep);
        
        //根据状态码获取状态码描述
        static std::string CodeToDesc(int code);
        //根据后缀获取资源类型
        static std::string SuffixToDesc(const std::string& suffix);
};
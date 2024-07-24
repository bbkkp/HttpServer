#pragma once
#include <string>
class Timestamp
{
public:
    Timestamp();
    explicit Timestamp(int64_t microSecondsSinceEpoch_);
    static Timestamp now();
    std::string to_string();
private:
    int64_t microSecondsSinceEpoch_;
};
#pragma once

#include <queue>
#include <functional>
#include <mutex>
#include <future>
#include <iostream>

class TaskQueue
{

public:
    using Task = std::function<void()>; // 任务类
    template <typename F, typename... Args>
    auto addTask(F &f, Args &&...args) -> std::future<decltype(f(args...))>; // 添加任务
    Task takeTask();                                                         // 取任务
    bool empty() { return taskQueue.empty(); }

private:
    std::mutex taskQueueMutex;  // 任务队列互斥锁
    std::queue<Task> taskQueue; // 任务队列
};

template <typename F, typename... Args> // 可变参数模板，模板必须在头文件定义
auto TaskQueue::addTask(F &f, Args &&...args) -> std::future<decltype(f(args...))>
{
    using RetType = decltype(f(args...)); // 获取函数返回值类型
                                          // 将函数封装为无形参的类型 std::bind(f, std::forward<Args>(args)...)：将参数与函数名绑定
                                          // packaged_task<RetType()>(std::bind(f, std::forward<Args>(args)...)); 将绑定参数后的函数封装为只有返回值没有形参的任务对象，这样就能使用get_future得到future对象，然后future对象可以通过get方法获取返回值了
                                          // std::make_shared<std::packaged_task<RetType()>>(std::bind(f, std::forward<Args>(args)...)); 生成智能指针，离开作用域自动析构
    auto task = std::make_shared<std::packaged_task<RetType()>>(std::bind(f, std::forward<Args>(args)...));
    std::lock_guard<std::mutex> lockGuard(taskQueueMutex); // 插入时上锁，防止多个线程同时插入
    // 将函数封装为无返回无形参类型，通过lamdba表达式，调用封装后的函数，注意，此时返回一个无形参无返回值的函数对象
    taskQueue.emplace([task]
                      { (*task)(); });
    return task->get_future();
}

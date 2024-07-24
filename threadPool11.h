#pragma once
#include <atomic>
#include <thread>
#include <condition_variable>
#include "taskQueue.h"

class ThreadPool
{
    std::atomic<int> threadNum{};                        // 最小线程数
    std::atomic<int> busyThreadNum;                      // 忙线程数
    std::condition_variable notEmptyCondVar;             // 判断任务队列是否非空
    std::mutex threadPoolMutex;                          // 线程池互斥锁
    bool shutdown;                                       // 线程池是否启动
    std::unique_ptr<TaskQueue> taskQueue;                // 任务队列
    std::vector<std::shared_ptr<std::thread>> threadVec; // 线程池
public:
    explicit ThreadPool(int threadNum = 5); // 创建线程池
    ~ThreadPool();                          // 销毁线程池

    template <typename F, typename... Args>
    auto commit(F &f, Args &&...args) -> decltype(taskQueue->addTask(f, std::forward<Args>(args)...)); // 提交一个任务
    void worker();
};

template <typename F, typename... Args> // 可变参数模板
auto ThreadPool::commit(F &f, Args &&...args) -> decltype(taskQueue->addTask(f, std::forward<Args>(args)...))
{
    // 这个目的就是把接收的参数直接转发给我们上面写的addTask方法，这样，就可以对使用者隐藏TaskQueue的细节，只向用户暴露ThreadPool就行
    auto ret = taskQueue->addTask(f, std::forward<Args>(args)...);
    notEmptyCondVar.notify_one();
    return ret;
}

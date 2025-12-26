// ThreadPool.h
#pragma once

#include <vector>      // std::vector
#include <functional>  // std::function
#include <string_view> // std::string_view
#include <thread>      // std::thread
#include <memory>      // std::unique_ptr

#include "ThreadOxPool/TaskScheduler.h"

class Worker;    // forward declaration
struct TaskBase; // forward declaration

constexpr size_t kMinWorkerCount = 4;

using LogFn = std::function<void(std::string_view)>;

class ThreadPool {
private:
    size_t m_workerCount{};
    std::vector<std::unique_ptr<Worker>> m_workers; 

    TaskScheduler m_taskScheduler;

    LogFn m_log;

private:
    static size_t computeWorkerCount(size_t requested) noexcept;

    void initWorkers(size_t requested);

    void log(std::string_view msg) const noexcept;

public:
    ThreadPool(size_t workerCount = std::thread::hardware_concurrency(),
             QueueConfig queueConfig = {}, LogFn logger = {});

    ~ThreadPool();

    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool &operator=(ThreadPool &&) = delete;

    bool tryEnqueue(std::unique_ptr<TaskBase> task,
                  Priority priority = Priority::Low);
    
    // Getters
    size_t getWorkerCount();
    const QueueConfig& getQueueConfig();
};
// Worker.h
#pragma once

#include <thread>  // std::thread
#include <cstddef> // size_t
#include <atomic>

#include "ThreadOxPool/TaskScheduler.h"

class ThreadPool; // forward declaration

class Worker {
private:
    std::thread m_thread;
    ThreadPool& m_pool;
    TaskScheduler& m_taskSheduler;

    size_t m_highTaskStreak = 0;
    std::atomic<bool> m_running{true};

public:
    explicit Worker(ThreadPool& pool, TaskScheduler& taskScheduler)noexcept
    : m_pool(pool),
      m_taskSheduler(taskScheduler)
    {
        m_thread = std::thread(&Worker::run, this);
    }

    ~Worker() noexcept {
        m_running.store(false, std::memory_order_relaxed);
        if (m_thread.joinable())
            m_thread.join();
    }

    Worker(const Worker&)            = delete;
    Worker& operator=(const Worker&) = delete;
    Worker(Worker&&)                 = delete;
    Worker& operator=(Worker&&)      = delete;

    void run();
    void stop() noexcept;
};
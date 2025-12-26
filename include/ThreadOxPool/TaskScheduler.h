#pragma once

#include <memory> // std::unique_ptr 

#include "ThreadOxPool/Tasks/TaskBase.h"
#include "ThreadOxPool/SchedulerTypes.h"
#include "../rigtorp/MPMCQueue.h"

constexpr size_t kMediumPriorityPollInterval = 8;
constexpr size_t kLowPriorityPollInterval    = 32;

class TaskScheduler {
private:
    rigtorp::MPMCQueue<std::unique_ptr<TaskBase>> m_highPriorityQueue;
    rigtorp::MPMCQueue<std::unique_ptr<TaskBase>> m_mediumPriorityQueue;
    rigtorp::MPMCQueue<std::unique_ptr<TaskBase>> m_lowPriorityQueue;

    QueueConfig m_config;

public:
    TaskScheduler(const QueueConfig& config) 
        : m_highPriorityQueue(config.highQueueSize),
          m_mediumPriorityQueue(config.mediumQueueSize),
          m_lowPriorityQueue(config.lowQueueSize),
          m_config(config)
    {

    }

    ~TaskScheduler() noexcept = default;

    TaskScheduler (const TaskScheduler&) = delete;
    TaskScheduler operator=(const TaskScheduler&) = delete;

    TaskScheduler(TaskScheduler&&) = delete;
    TaskScheduler& operator=(TaskScheduler&&) = delete;

    std::unique_ptr<TaskBase> getTask(size_t& highStreak) {
        std::unique_ptr<TaskBase> task;

        if (m_highPriorityQueue.try_pop(task)) {
            ++highStreak;

            if (highStreak >= kLowPriorityPollInterval) {
                highStreak = 0;
                if (m_lowPriorityQueue.try_pop(task)) return task;
            }

            if (highStreak >= kMediumPriorityPollInterval) {
                if (m_mediumPriorityQueue.try_pop(task)) return task;
            }

            return task;
        }

        highStreak = 0;

        if (m_mediumPriorityQueue.try_pop(task)) return task;
        if (m_lowPriorityQueue.try_pop(task))    return task;

        return nullptr;
    }

    bool tryEnqueue(std::unique_ptr<TaskBase> task, Priority priority) {
        switch (priority) {
            case Priority::High:
                return m_highPriorityQueue.try_push(std::move(task));
            case Priority::Medium:
                return m_mediumPriorityQueue.try_push(std::move(task));
            case Priority::Low:
                return m_lowPriorityQueue.try_push(std::move(task));
        }
        return false;
    }

    const QueueConfig& getQueueConfig() {
        return m_config;
    }

    bool empty() const noexcept {
        return m_highPriorityQueue.empty() &&
            m_mediumPriorityQueue.empty() &&
            m_lowPriorityQueue.empty();
    }
};
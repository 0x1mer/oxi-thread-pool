#pragma once

#include "ThreadOxPool/Tasks/TaskStatus.h"

#include <atomic>             // std::atomic
#include <mutex>              // std::mutex
#include <condition_variable> // std::condition_variable

struct TaskState {
    std::atomic<TaskStatus> status{TaskStatus::Pending};
    std::mutex mutex;
    std::condition_variable cv;
};
// ThreadPool.cpp
#include "ThreadOxPool/ThreadPool.h"
#include "ThreadOxPool/Worker.h"
#include "ThreadOxPool/Tasks/TaskBase.h"

#include <algorithm>   // std::min
#include <memory>
#include <utility>

ThreadPool::ThreadPool(
    size_t workerCount,
    QueueConfig queueConfig,
    LogFn logger
)
    : m_taskScheduler(queueConfig)
    , m_log(std::move(logger))
{
    initWorkers(workerCount);

    log("ThreadPool created with " +
        std::to_string(m_workerCount));
}

ThreadPool::~ThreadPool() = default;

void ThreadPool::initWorkers(size_t requested) {
    m_workerCount = computeWorkerCount(requested);

    m_workers.reserve(m_workerCount);

    for (size_t i = 0; i < m_workerCount; ++i) {
            m_workers.emplace_back(std::make_unique<Worker>(*this, m_taskScheduler));
    }
}

void ThreadPool::log(std::string_view msg) const noexcept {
    if (m_log)
        m_log(msg);
}

[[nodiscard]]
bool ThreadPool::tryEnqueue(std::unique_ptr<TaskBase> task, Priority priority) {
    return m_taskScheduler.tryEnqueue(std::move(task), priority);
}

size_t ThreadPool::computeWorkerCount(size_t requested) noexcept {
    if (requested == 0) { requested = kMinWorkerCount; }

    size_t hw = std::thread::hardware_concurrency();

    if (hw == 0) { hw = kMinWorkerCount; }

    return std::min(requested, hw);
}

size_t ThreadPool::getWorkerCount() {
    return m_workers.size();
}

const QueueConfig& ThreadPool::getQueueConfig() {
    return m_taskScheduler.getQueueConfig();
}
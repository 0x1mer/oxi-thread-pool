// Worker.cpp
#include "ThreadOxPool/Worker.h"

void Worker::run() {
    size_t highStreak = 0;

    while (m_running.load(std::memory_order_relaxed)) {
        auto task = m_taskSheduler.getTask(highStreak);
        if (task) {
            try {
                task->execute();
            } catch (const std::exception& ex) {
                task->onError(ex);
            }

            task->futureResult();

        } else {
            std::this_thread::yield();
        }
    }
}

void Worker::stop() noexcept {
    m_running.store(false, std::memory_order_relaxed);
}
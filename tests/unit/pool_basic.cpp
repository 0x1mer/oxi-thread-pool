#include "ThreadOxPool/ThreadPool.h"
#include <cstddef>
#include <string_view>
#include <cassert>

// 1️⃣ Basic creation
void test_default_creation() {
    ThreadPool pool{};
    size_t wc = pool.getWorkerCount();

    assert(wc >= 1);
    assert(wc <= std::thread::hardware_concurrency() || 
           std::thread::hardware_concurrency() == 0);
}

// 2️⃣ The apparent number of workers
void test_custom_worker_count() {
    ThreadPool pool{4};

    size_t wc = pool.getWorkerCount();

    assert(wc == 4);
}

// 3️⃣ 0 workers (edge case)
void test_zero_workers() {
    ThreadPool pool{0};

    size_t wc = pool.getWorkerCount();

    assert(wc == kMinWorkerCount);
}

void test_default_queue_config() {
    ThreadPool pool{};
    const QueueConfig& cfg = pool.getQueueConfig();

    assert(cfg.highQueueSize   == kMinHighQueueSize);
    assert(cfg.mediumQueueSize == kMinMediumQueueSize);
    assert(cfg.lowQueueSize    == kMinLowQueueSize);
}

void test_custom_queue_config() {
    QueueConfig cfg{32, 64, 128};
    ThreadPool pool{4, cfg};

    const QueueConfig& actual = pool.getQueueConfig();

    assert(actual.highQueueSize   == 32);
    assert(actual.mediumQueueSize == 64);
    assert(actual.lowQueueSize    == 128);
}

void test_zero_queue_config_fallback() {
    QueueConfig cfg{0, 0, 0};
    ThreadPool pool{4, cfg};

    const QueueConfig& actual = pool.getQueueConfig();

    assert(actual.highQueueSize   == kMinHighQueueSize);
    assert(actual.mediumQueueSize == kMinMediumQueueSize);
    assert(actual.lowQueueSize    == kMinLowQueueSize);
}

// 5️⃣ Custom logger
void test_custom_logger() {
    LogFn logger = [](std::string_view msg) {};

    ThreadPool pool{4, {}, logger};
}

// 6️⃣ All at once
void test_full_custom_creation() {
    QueueConfig cfg{};

    LogFn logger = [](std::string_view msg) {};

    ThreadPool pool{8, cfg, logger};
}

int main() {
    test_default_creation();
    test_custom_worker_count();
    test_zero_workers();
    test_custom_queue_config();
    test_custom_logger();
    test_full_custom_creation();

    return 0;
}
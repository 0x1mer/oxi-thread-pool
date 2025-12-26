#include "ThreadOxPool/ThreadPool.h"

#include <cassert>
#include <cstddef>
#include <string_view>
#include <atomic>
#include <thread>

namespace tests {

// ─────────────────────────────────────────────
// Construction
// ─────────────────────────────────────────────

void Should_CreateThreadPool_When_DefaultConstructed()
{
    ThreadPool pool{};

    const size_t workerCount = pool.getWorkerCount();

    assert(workerCount >= 1);
    assert(
        workerCount <= std::thread::hardware_concurrency() ||
        std::thread::hardware_concurrency() == 0
    );
}

void Should_UseExactWorkerCount_When_CustomWorkerCountProvided()
{
    ThreadPool pool{4};

    const size_t workerCount = pool.getWorkerCount();

    assert(workerCount == 4);
}

void Should_FallbackToMinimum_When_ZeroWorkersProvided()
{
    ThreadPool pool{0};

    const size_t workerCount = pool.getWorkerCount();

    assert(workerCount == kMinWorkerCount);
}

// ─────────────────────────────────────────────
// Queue configuration
// ─────────────────────────────────────────────

void Should_UseDefaultQueueConfig_When_NotProvided()
{
    ThreadPool pool{};

    const QueueConfig& cfg = pool.getQueueConfig();

    assert(cfg.highQueueSize   == kMinHighQueueSize);
    assert(cfg.mediumQueueSize == kMinMediumQueueSize);
    assert(cfg.lowQueueSize    == kMinLowQueueSize);
}

void Should_UseCustomQueueConfig_When_Provided()
{
    const QueueConfig cfg{32, 64, 128};
    ThreadPool pool{4, cfg};

    const QueueConfig& actual = pool.getQueueConfig();

    assert(actual.highQueueSize   == 32);
    assert(actual.mediumQueueSize == 64);
    assert(actual.lowQueueSize    == 128);
}

void Should_FallbackQueueConfig_When_ZeroSizesProvided()
{
    const QueueConfig cfg{0, 0, 0};
    ThreadPool pool{4, cfg};

    const QueueConfig& actual = pool.getQueueConfig();

    assert(actual.highQueueSize   == kMinHighQueueSize);
    assert(actual.mediumQueueSize == kMinMediumQueueSize);
    assert(actual.lowQueueSize    == kMinLowQueueSize);
}

// ─────────────────────────────────────────────
// Logging
// ─────────────────────────────────────────────

void Should_InvokeLogger_When_ThreadPoolDestroyed()
{
    std::atomic<size_t> callCount{0};

    LogFn logger = [&](std::string_view)
    {
        ++callCount;
    };

    {
        ThreadPool pool{4, {}, logger};
    } // destruction / shutdown

    assert(callCount > 0);
}

// ─────────────────────────────────────────────
// Combined configuration
// ─────────────────────────────────────────────

void Should_CreateThreadPool_When_AllCustomParametersProvided()
{
    const QueueConfig cfg{};
    LogFn logger = [](std::string_view) {};

    [[maybe_unused]]
    ThreadPool pool{8, cfg, logger};
}

} // namespace tests

int main()
{
    using namespace tests;

    Should_CreateThreadPool_When_DefaultConstructed();
    Should_UseExactWorkerCount_When_CustomWorkerCountProvided();
    Should_FallbackToMinimum_When_ZeroWorkersProvided();

    Should_UseDefaultQueueConfig_When_NotProvided();
    Should_UseCustomQueueConfig_When_Provided();
    Should_FallbackQueueConfig_When_ZeroSizesProvided();

    Should_InvokeLogger_When_ThreadPoolDestroyed();
    Should_CreateThreadPool_When_AllCustomParametersProvided();

    return 0;
}
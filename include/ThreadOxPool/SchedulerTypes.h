#pragma once

#include <cstddef> // size_t

constexpr size_t kMinHighQueueSize   = 256;
constexpr size_t kMinMediumQueueSize = 256;
constexpr size_t kMinLowQueueSize    = 256;

struct QueueConfig {
    size_t highQueueSize;
    size_t mediumQueueSize;
    size_t lowQueueSize;

    QueueConfig(
        size_t high,
        size_t medium,
        size_t low
    ) noexcept
        : highQueueSize(high ? high : kMinHighQueueSize)
        , mediumQueueSize(medium ? medium : kMinMediumQueueSize)
        , lowQueueSize(low ? low : kMinLowQueueSize)
    {}

    QueueConfig() noexcept
        : highQueueSize(kMinHighQueueSize)
        , mediumQueueSize(kMinMediumQueueSize)
        , lowQueueSize(kMinLowQueueSize)
    {}
};

enum class Priority {
    Low,
    Medium,
    High
};
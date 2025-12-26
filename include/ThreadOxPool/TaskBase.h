#pragma once

#include <exception> // std::exception

struct TaskBase {
    virtual ~TaskBase() = default;
    virtual void execute() = 0;
    virtual void onError(const std::exception& ex) noexcept = 0;
    virtual void futureResult() noexcept = 0;
};
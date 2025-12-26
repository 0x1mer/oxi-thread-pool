#include "ThreadOxPool/SchedulerTypes.h"
#include "ThreadOxPool/Tasks/TaskBase.h"
#include "ThreadOxPool/ThreadPool.h"

#include <iostream>
#include <memory>

struct LowTask : TaskBase {
    std::atomic<size_t>* counter;

    explicit LowTask(std::atomic<size_t>& c) noexcept
        : counter(&c) {}

    void execute() override {
        ++(*counter);
    }

    void onError(const std::exception&) noexcept override {}

    void futureResult() noexcept override {}
};

void test_low_task_execution() {
    ThreadPool pool{4};

    std::atomic<size_t> executed{0};

    pool.tryEnqueue(
        std::make_unique<LowTask>(executed),
        Priority::Low
    );

    //pool.waitIdle();

    assert(executed == 1);
}

int main() {
    ThreadPool pool {
        4,
        {5, 6, 7}, 
        [](std::string_view msg){std::cout << msg << "\n";}
    };


}
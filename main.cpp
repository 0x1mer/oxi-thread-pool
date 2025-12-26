#include <iostream>

#define LOGGING
#include "ThreadOxPool/ThreadPool.h"

int main(int, char**){
    std::cout << "Hello, from OxiThreadPool!\n";
    ThreadPool* pool = new ThreadPool();
}

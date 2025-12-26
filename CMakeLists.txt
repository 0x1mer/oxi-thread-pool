cmake_minimum_required(VERSION 3.16)
project(OxiThreadPool VERSION 0.1.0 LANGUAGES CXX)

add_library(OxiThreadPool
    src/ThreadPool.cpp
    src/Worker.cpp
)

target_include_directories(OxiThreadPool
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(OxiThreadPool
    PUBLIC cxx_std_20
)

include(CTest)
enable_testing()

# -------- pool_basic --------
add_executable(pool_basic_tests
    tests/unit/pool_basic.cpp
)

target_link_libraries(pool_basic_tests
    PRIVATE OxiThreadPool
)

add_test(
    NAME Pool.Basic
    COMMAND pool_basic_tests
)
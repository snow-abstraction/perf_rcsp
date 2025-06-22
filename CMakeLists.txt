cmake_minimum_required(VERSION 3.28)
project(perf_rcsp)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 23)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/debug_bin)
elseif (CMAKE_BUILD_TYPE STREQUAL "O1")
    add_compile_options(-O1 -DNDEBUG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/O1)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -mavx -DNDEBUG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/release_bin)
elseif (CMAKE_BUILD_TYPE STREQUAL "Sanitize")
    set(CMAKE_CXX_FLAGS "-fsanitize=address")
    add_compile_options(-g -O0 -fno-omit-frame-pointer)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/sanitize_bin)
else ()
    message(FATAL_ERROR "Unknown type of build specified by CMAKE_BUILD_TYPE.")
endif ()

enable_testing()
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

find_package(spdlog REQUIRED)

find_package(Boost REQUIRED COMPONENTS
        graph
        numeric_conversion
)

find_package(benchmark REQUIRED)

add_executable(perf_rcsp
        code/src/main.cpp
        code/src/example_graphs.cpp
        code/src/rcsp_boost_graph.cpp
)
target_link_libraries(perf_rcsp PRIVATE
        spdlog::spdlog
        Boost::graph
)

add_executable(benchmark2
        code/src/benchmark.cpp
        code/src/example_graphs.cpp
        code/src/rcsp_boost_graph.cpp
)
target_link_libraries(benchmark2 PRIVATE
        spdlog::spdlog
        Boost::graph
        benchmark::benchmark
        benchmark::benchmark_main
)

add_executable(run_tests
        code/test/state_operators_test.cpp
)
target_link_libraries(run_tests PRIVATE
        spdlog::spdlog
        GTest::gtest_main
)
gtest_discover_tests(run_tests)

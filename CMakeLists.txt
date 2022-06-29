cmake_minimum_required(VERSION 3.22)
project(Concurrency)

set(CMAKE_CXX_STANDARD 17)

add_executable(thread_test_1 thread_test_1.cpp)
add_executable(thread_test_2 thread_test_2.cpp)
add_executable(thread_test_3 thread_test_3.cpp)
add_executable(thread_transfer_owner thread_transfer_owner.cpp)

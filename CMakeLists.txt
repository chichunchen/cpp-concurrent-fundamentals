cmake_minimum_required(VERSION 3.10)
project(concurrent_toolkits_cpp)

set(CMAKE_CXX_STANDARD 11)

include_directories(centralized_locks)
include_directories(utilities)

add_executable(concurrent_toolkits_cpp
        centralized_locks/lamport_bakery.h
        centralized_locks/tas_lock.h
        centralized_locks/tatas_lock.h
        utilities/Timer.h
        tests/concurrent_vector_with_lock.cpp centralized_locks/ticket_lock.h)

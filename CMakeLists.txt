cmake_minimum_required(VERSION 3.10)
project(concurrent_toolkits_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)

include_directories(centralized_locks)
include_directories(utilities)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(concurrent_toolkits_cpp
        centralized_locks/lamport_bakery.h
        centralized_locks/tas_lock.h
        centralized_locks/tatas_lock.h
        centralized_locks/ticket_lock.h
        centralized_locks/lamport_fast.h
        scalable_locks/mcs_lock.h
        utilities/PreciseTimer.h
        tests/tests.cpp
        tests/lamport_test.cpp
        tests/lamport_test.h
        tests/mcs_test.cpp
        tests/mcs_test.h tests/mutex_tas_tatas_ticket.cpp tests/mutex_tas_tatas_ticket.h tests/config.h tests/sssp_barrier.cpp tests/sssp_barrier.h tests/simplegraph_atomic.h barriers/centralized_barrier.hpp)

TARGET_LINK_LIBRARIES(concurrent_toolkits_cpp
        pthread
        )

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)
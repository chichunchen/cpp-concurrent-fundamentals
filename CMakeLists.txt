cmake_minimum_required(VERSION 3.10)
project(concurrent_toolkits_cpp)

set(CMAKE_CXX_STANDARD 11)

include_directories(centralized_locks)
include_directories(utilities)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(concurrent_toolkits_cpp
        centralized_locks/lamport_bakery.hpp
        centralized_locks/tas_lock.hpp
        centralized_locks/tatas_lock.hpp
        centralized_locks/ticket_lock.hpp
        centralized_locks/lamport_fast.hpp
        scalable_locks/mcs_lock.h
        utilities/PreciseTimer.hpp
        tests/tests.cpp
        tests/lamport_test.cpp
        tests/lamport_test.hpp
        tests/mcs_test.cpp
        tests/mcs_test.hpp tests/mutex_tas_tatas_ticket.cpp tests/mutex_tas_tatas_ticket.hpp tests/config.h scalable_locks/tas_exp_lock.hpp scalable_locks/ticket_exp_lock.hpp scalable_locks/tatas_exp_lock.hpp utilities/Misc.hpp)

TARGET_LINK_LIBRARIES(concurrent_toolkits_cpp
        pthread
        )

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)
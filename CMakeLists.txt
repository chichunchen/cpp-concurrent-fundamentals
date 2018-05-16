cmake_minimum_required(VERSION 3.10)
project(concurrent_toolkits_cpp)

set(CMAKE_CXX_STANDARD 11)

include_directories(centralized_locks)
include_directories(utilities)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(concurrent_toolkits_cpp
        centralized_locks/lamport_bakery.h
        centralized_locks/tas_lock.h
        centralized_locks/tatas_lock.h
        utilities/Timer.h
        tests/concurrent_vector_with_lock.cpp centralized_locks/ticket_lock.h tests/lamport_test.cpp tests/lamport_test.h centralized_locks/lamport_fast.h)

TARGET_LINK_LIBRARIES( concurrent_toolkits_cpp
		pthread
		)

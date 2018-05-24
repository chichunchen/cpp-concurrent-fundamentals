cmake_minimum_required(VERSION 3.10)
project(concurrent_toolkits_cpp)

set(CMAKE_CXX_STANDARD 17)

include_directories(centralized_mutex)
include_directories(scalable_mutex)
include_directories(rwlock)
include_directories(ds)
include_directories(utilities)

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads REQUIRED)

add_executable(concurrent_toolkits_cpp
        centralized_mutex/lamport_bakery.hpp
        centralized_mutex/tas.hpp
        centralized_mutex/tatas.hpp
        centralized_mutex/ticket.hpp
        centralized_mutex/lamport_fast.hpp
        scalable_mutex/mcs.hpp
        scalable_mutex/tas_exp.hpp
        scalable_mutex/ticket_exp.hpp
        scalable_mutex/tatas_exp.hpp
        rwlock/rw_reader_pref.hpp
        ds/hazard_pointer.hpp
        ds/msqueue.hpp
        ds/stack_no_free.hpp
        ds/stack_no_free.tpp
        ds/stack_with_hp.hpp
        ds/stack_with_hp.tpp
        utilities/PreciseTimer.hpp
        utilities/Misc.hpp
        tests/config.h
        tests/lamport_test.cpp
        tests/lamport_test.hpp
        tests/mcs_test.cpp
        tests/mcs_test.hpp
        tests/mutex_tas_tatas_ticket.cpp
        tests/mutex_tas_tatas_ticket.hpp
        tests/ds_test.cpp
        tests/ds_test.hpp
        tests/tests.cpp
        )

TARGET_LINK_LIBRARIES(concurrent_toolkits_cpp
        pthread
        )

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

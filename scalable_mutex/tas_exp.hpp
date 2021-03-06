//
// Created by 陳其駿 on 5/20/18.
//

#ifndef CONCURRENT_TOOLKITS_CPP_TAS_EXP_HPP
#define CONCURRENT_TOOLKITS_CPP_TAS_EXP_HPP

#include <atomic>
#include <zconf.h>
#include <ctime>

#include "Misc.hpp"

namespace scalable_locks {

    class Test_and_set_lock {
    private:
        std::atomic<bool> f{};
        const int base = 1;
        const int limit = 100000;
        const int multiplier = 2;

    public:
        Test_and_set_lock() : f(false) {}

        void lock() {
            int delay = base;
            while (f.exchange(true, std::memory_order_acquire)) {
                timespec ts = {0, delay};
                nanosleep(&ts, nullptr);
                delay = MIN(delay * multiplier, limit);
            }
        }

        void unlock() {
            f.store(false, std::memory_order_release);
            // prevents all preceding writes from moving past all subsequent stores.
        }
    };

}

#endif //CONCURRENT_TOOLKITS_CPP_TAS_EXP_HPP

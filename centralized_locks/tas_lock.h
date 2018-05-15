//
// Created by 陳其駿 on 5/15/18.
//

#ifndef CONCURRENT_TOOLKITS_CPP_TEST_AND_SET_LOCK_H
#define CONCURRENT_TOOLKITS_CPP_TEST_AND_SET_LOCK_H

#include <atomic>

namespace centralized_locks {
    class Test_and_set_lock {
    private:
        std::atomic<bool> f{};

    public:
        Test_and_set_lock () : f(false) {}

        void lock() {
            while (f.exchange(true)) ;
            std::atomic_thread_fence(std::memory_order_release);
        }

        void unlock() {
            f = false;
            // prevents all preceding writes from moving past all subsequent stores.
            std::atomic_thread_fence(std::memory_order_release);
        }
    };
}

#endif

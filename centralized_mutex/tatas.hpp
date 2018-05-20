//
// Created by 陳其駿 on 5/15/18.
//

#ifndef CONCURRENT_TOOLKITS_CPP_TEST_AND_TEST_AND_SET_LOCK_H
#define CONCURRENT_TOOLKITS_CPP_TEST_AND_TEST_AND_SET_LOCK_H

#include <atomic>

namespace centralized_locks {

    class Test_and_test_and_set_lock {
    private:
        std::atomic<bool> f{};

    public:
        Test_and_test_and_set_lock() : f(false) {}

        void lock() {
            while (f.exchange(true, std::memory_order_acquire)) {
                while(f.load(std::memory_order_relaxed));
            }
        }

        void unlock() {
            f.store(false, std::memory_order_release);
            // prevents all preceding writes from moving past all subsequent stores.
        }
    };

}

#endif

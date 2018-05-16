//
// Created by 陳其駿 on 5/15/18.
//

#ifndef CONCURRENT_TOOLKITS_CPP_LAMPORT_FAST_H
#define CONCURRENT_TOOLKITS_CPP_LAMPORT_FAST_H

#include <atomic>

#define LOWER (-1)

namespace centralized_locks {

    class lamport_fast_lock {
    private:
        std::atomic<bool> *trying;
        int thread_num;
        std::atomic<int> x;
        std::atomic<int> y;

    public:
        explicit lamport_fast_lock(int thread_num) : thread_num(thread_num), x({0}), y({LOWER}) {
            trying = new std::atomic<bool>[thread_num];
            for (int i = 0; i < thread_num; i++) {
                trying[i] = false;
            }
        }

        ~lamport_fast_lock() {
            delete[] trying;
        }

        void lock(int self) {
            while (true) {
                trying[self].store(true);
                x.store(self);

                // after write x, immediately check if y has been modified
                if (y.load() != LOWER) {
                    trying[self].store(false);
                    while (y.load() != LOWER);              // spin
                    continue;                               // go back to top of loop
                }
                y.store(self);

                // after write y, immediately check if x has been modified
                if (x.load() != self) {
                    trying[self].store(false);
                    for (int i = 0; i < thread_num; i++) {
                        while (trying[i].load());           // spin
                    }
                    if (y.load() != self) {
                        while (y.load() != LOWER);          // spin
                        continue;                           // go back to top of loop
                    }
                }

                break;
            }
            std::atomic_thread_fence(std::memory_order_release);
        }

        void unlock(int self) {
            std::atomic_thread_fence(std::memory_order_acquire);
            y.store(LOWER);
            trying[self].store(false);
            std::atomic_thread_fence(std::memory_order_release);
        }
    };

}

#endif //CONCURRENT_TOOLKITS_CPP_LAMPORT_FAST_H

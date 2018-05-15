//
// Created by 陳其駿 on 5/15/18.
//

#ifndef CONCURRENT_TOOLKITS_CPP_LAMPORT_BAKERY_H
#define CONCURRENT_TOOLKITS_CPP_LAMPORT_BAKERY_H

#include <cmath>
#include <atomic>
#include <vector>
#include <iostream>

namespace centralized_locks {
    class lamport_bakery_lock {
    private:
        std::atomic<bool> *choosing;
        std::atomic<int> *number;
        int thread_num;

        int max_number() {
            int max = 0;
            for (int i = 0; i < thread_num; i++) {
                if (number[i] > max) {
                    max = number[i];
                }
            }
            return max;
        }

    public:
        explicit lamport_bakery_lock(int thread_num) : thread_num(thread_num) {
            choosing = new std::atomic<bool>[thread_num];
            number = new std::atomic<int>[thread_num];
            for (int i = 0; i < thread_num; i++) {
                choosing[i] = false;
                number[i] = 0;
            }
        }

        void lock(int self) {
            choosing[self].store(true);
            int max = 1 + max_number();
//            std::cout << "max: " << max << std::endl;
            number[self].store(max);
            choosing[self].store(false);
            for (int i = 0; i < thread_num; i++) {
                while (choosing[i].load());     // spin till other thread picks a ticket
                int t;
                while (true) {
                    t = number[i].load();
                    // (t, i) >= (m, self)
                    if (t == 0 || (t > max || (t == max && i >= self)))
                        break;
                }
            }
            std::atomic_thread_fence(std::memory_order_release);
        }

        void unlock(int self) {
            number[self].store(0);
            std::atomic_thread_fence(std::memory_order_release);
        }
    };
}

#endif
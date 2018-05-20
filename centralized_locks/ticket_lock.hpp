//
// Created by 陳其駿 on 5/15/18.
//

#ifndef CONCURRENT_TOOLKITS_CPP_TICKET_LOCK_H
#define CONCURRENT_TOOLKITS_CPP_TICKET_LOCK_H

#include <atomic>

namespace centralized_locks {

    struct Ticket_lock {
        std::atomic<int> next_ticket;
        std::atomic<int> now_serving;
        const int base = 5;

        Ticket_lock() : next_ticket(0), now_serving(0) {}

        void lock() {
            int my_ticket = next_ticket.fetch_add(1, std::memory_order_relaxed);
            while (true) {
                int ns = now_serving.load(std::memory_order_relaxed);
                if (ns == my_ticket) {
                    break;
                }

                int pause = base * (my_ticket - ns);
                for (int i = 0; i < pause; i++);
            }
            std::atomic_thread_fence(std::memory_order_release);
        }

        void unlock() {
            int t = now_serving + 1;
            now_serving.store(t, std::memory_order_release);
        }
    };

}

#endif
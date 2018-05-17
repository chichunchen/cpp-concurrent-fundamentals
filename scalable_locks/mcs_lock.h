//
// Created by 陳其駿 on 5/16/18.
//

#ifndef CONCURRENT_TOOLKITS_CPP_MCS_LOCK_H
#define CONCURRENT_TOOLKITS_CPP_MCS_LOCK_H

#include <atomic>
#include <mutex>

namespace scalable_locks {

    struct _qnode {
        std::atomic<_qnode*> next;
        std::atomic<bool> waiting;

        _qnode() : next(nullptr), waiting(false) {};
    };

    class mcs_lock {
    private:
        std::atomic<_qnode*> tail;

    public:
        mcs_lock() : tail(nullptr) {}

        void lock(_qnode *p) {
//            std::cout << ", address: " << &p << std::endl;

            p->next = nullptr;
            p->waiting = true;

            _qnode *prev = tail.exchange(p);

            if (prev) {
                // if the list was not previously empty, it sets the predecessor's next
                // field to refer to its own local node
                prev->next.store(p);
                while (p->waiting.load());      // spin
            }

            std::atomic_thread_fence(std::memory_order_acquire);
        }

        void unlock(_qnode *p) {
            _qnode *succ = p->next.load();
            _qnode *old_p = p;

            // check whether this thread's local node's next field is null
            if (!succ) {
                // if no other thread is contending for the lock, then return
                if (tail.compare_exchange_strong(p, nullptr)) return;

                // if race condition happens then it means other threads are also trying to
                // acquire the lock, so spin until it finishes
                while (!succ)
                    succ = old_p->next.load();
            }

            succ->waiting.store(false);
            std::atomic_thread_fence(std::memory_order_release);
        }
    };

}

#endif //CONCURRENT_TOOLKITS_CPP_MCS_LOCK_H

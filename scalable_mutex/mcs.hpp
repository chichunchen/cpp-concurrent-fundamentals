//
// Created by 陳其駿 on 5/16/18.
//

#ifndef CONCURRENT_TOOLKITS_CPP_MCS_LOCK_H
#define CONCURRENT_TOOLKITS_CPP_MCS_LOCK_H

#include <atomic>
#include <mutex>

namespace scalable_locks {

    struct _qnode {
        std::atomic<_qnode *> next;
        std::atomic<bool> waiting;

        _qnode() : next(nullptr), waiting(false) {};
    };

    class mcs {
    private:
        std::atomic<_qnode *> tail;

    public:
        mcs() : tail(nullptr) {}

        void lock(_qnode *p) {
            p->next = nullptr;
            p->waiting = true;

            _qnode *prev = tail.exchange(p);

            if (prev) {
                // if the list was not previously empty, it sets the predecessor's next
                // field to refer to its own local node
                prev->next.store(p, std::memory_order_release);
                while (p->waiting.load(std::memory_order_acquire));      // spin
            }
        }

        void unlock(_qnode *p) {
            _qnode *succ = p->next.load();
            _qnode *old_p = p;                  // don't know why the CAS in the while loop below change p even if the
                                                // execution fails

            // check whether this thread's local node's next field is null
            if (!succ) {
                // if no other thread is contending for the lock, then return
                if (tail.compare_exchange_strong(p, nullptr)) return;

                // if race condition happens then it means other threads are also trying to
                // acquire the lock, so spin until it finishes
                while (!succ)
                    succ = old_p->next.load(std::memory_order_acquire);
            }

            succ->waiting.store(false, std::memory_order_release);
        }
    };

}

#endif //CONCURRENT_TOOLKITS_CPP_MCS_LOCK_H

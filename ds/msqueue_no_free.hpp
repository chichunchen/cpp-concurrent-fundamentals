//
// Created by 陳其駿 on 2018/5/25.
//

#ifndef CONCURRENT_TOOLKITS_CPP_MSQUEUE_NO_FREE_HPP
#define CONCURRENT_TOOLKITS_CPP_MSQUEUE_NO_FREE_HPP


#include <iostream>
#include <atomic>

namespace lockfree_ds {

    template<typename T>
    class msqueue_no_free {
    private:
        struct node;

        struct ptr {
            node *p;
            unsigned int count;

            ptr() noexcept : p(nullptr), count(0) {}

            ptr(node *ptr) : p(ptr), count(0) {}

            ptr(node *ptr, unsigned int count) : p(ptr), count(count) {}

            bool operator==(const ptr &other) const {
                return p == other.p && count == other.count;
            }
        };

        struct node {
            T data;
            std::atomic<ptr> next;

            // dummy node
            node() : next(ptr()) {}

            // normal node
            explicit node(T const &value) : data(value), next(ptr()) {}
        };

        std::atomic<ptr> head;
        std::atomic<ptr> tail;
        std::atomic<int> counter;
    public:
        msqueue_no_free() : head(new node()), tail(head.load()), counter(0) {}

        /**
         * Push an element onto the queue
         * @param data
         */
        void push(T const &data) {
            auto *w = new node(data);
            ptr t, n;
            while (true) {
                t = tail.load();
                n = t.p->next.load();
                if (t == tail.load()) {
                    if (!n.p) {
                        if (t.p->next.compare_exchange_weak(n, ptr(w, n.count + 1))) {
                            break;
                        }
                    } else {
                        tail.compare_exchange_weak(t, ptr(n.p, t.count + 1));
                    }
                }
            }
            tail.compare_exchange_weak(t, ptr(w, t.count + 1));
            counter.fetch_add(1);
        }

        /**
         * Pop the element in the front from the queue, panic when pop from empty msqueue
         * @return a shared pointer that points to the removed element
         */
        T pop() {
            T rtn;
            ptr h, t, n;

            while (true) {
                h = head.load();
                t = tail.load();
                n = h.p->next.load();
                if (h == head.load()) {
                    if (h.p == t.p) {
                        if (!n.p) {
                            std::runtime_error("[ERROR]: Pop from empty msqueue");
                        }
                        tail.compare_exchange_weak(t, ptr(n.p, t.count + 1));
                    } else {
                        // read value before CAS; otherwise another pop might free n
                        rtn = n.p->data;
                        if (head.compare_exchange_weak(h, ptr(n.p, h.count + 1))) {
                            break;
                        }
                    }
                }
            }

            counter.fetch_sub(1);
            return rtn;
        }

        int size() {
            return counter;
        }

        bool empty() {
            return counter == 0;
        }

        // non-concurrent call
        void dump(int cnt) {
            ptr curr = head;
            for (int i = 0; i <= cnt; i++) {
                std::cout << curr.p->val << "\n";
                curr = curr.p->next;
            }
        }
    };

}

#endif //CONCURRENT_TOOLKITS_CPP_MSQUEUE_NO_FREE_HPP

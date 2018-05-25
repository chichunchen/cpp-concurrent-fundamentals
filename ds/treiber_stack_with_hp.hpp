//
// Created by 陳其駿 on 2018/5/21.
//

#ifndef CONCURRENT_TOOLKITS_CPP_LOCKFREE_HP_STACK_HPP
#define CONCURRENT_TOOLKITS_CPP_LOCKFREE_HP_STACK_HPP

#include <atomic>
#include <memory>

namespace lockfree_ds {

    template<typename T>
    class treiber_stack_with_hp {
    private:
        struct node {
            std::shared_ptr<T> data;
            node *next;

            explicit node(T const &data_) : data(std::make_shared<T>(data_)) {}
        };

        std::atomic<node *> head;
        std::atomic<size_t> counter;
    public:
        void push(T const &data);

        std::shared_ptr<T> pop();

        size_t size() {
            return counter;
        }

        bool empty() {
            return counter == 0;
        }
    };
}

#include "treiber_stack_with_hp.tpp"

#endif //CONCURRENT_TOOLKITS_CPP_LOCKFREE_STACK_HPP

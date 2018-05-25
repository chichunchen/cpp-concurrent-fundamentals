//
// Created by 陳其駿 on 2018/5/24.
//

#ifndef CONCURRENT_TOOLKITS_CPP_STACK_NO_FREE_HPP
#define CONCURRENT_TOOLKITS_CPP_STACK_NO_FREE_HPP

#include <atomic>
#include <memory>

namespace lockfree_ds {

    template<typename T>
    class treiber_stack_no_free {
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

#include "stack_no_free.tpp"

#endif //CONCURRENT_TOOLKITS_CPP_STACK_NO_FREE_HPP
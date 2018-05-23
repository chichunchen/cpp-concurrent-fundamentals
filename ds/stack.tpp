//
// Created by 陳其駿 on 2018/5/23.
//

#include "stack.hpp"

namespace lockfree_ds {

    template<typename T>
    void stack<T>::push(T const &data) {
        node *const new_node = new node(data);
        new_node->next = head.load();
        while (!head.compare_exchange_weak(new_node->next, new_node));
        counter++;
    }

    template<typename T>
    std::shared_ptr<T> stack<T>::pop() {
        node *old_head = head.load();
        while (old_head &&
               !head.compare_exchange_weak(old_head, old_head->next));
        counter--;
        return old_head ? old_head->data : std::shared_ptr<T>();
    }
}

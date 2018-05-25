//
// Created by 陳其駿 on 2018/5/23.
//

#ifndef CONCURRENT_TOOLKITS_CPP_LOCKFREE_STACK_TPP
#define CONCURRENT_TOOLKITS_CPP_LOCKFREE_STACK_TPP

#include "treiber_stack_with_hp.hpp"
#include "hazard_pointer.hpp"

namespace lockfree_ds {

    template<typename T>
    void treiber_stack_with_hp<T>::push(T const &data) {
        node *const new_node = new node(data);
        new_node->next = head.load();
        while (!head.compare_exchange_weak(new_node->next, new_node));
        counter++;
    }

    template<typename T>
    std::shared_ptr<T> treiber_stack_with_hp<T>::pop() {
        std::atomic<void *> &hp = get_hazard_pointer_for_current_thread();
        node *old_head = head.load();
        node *temp;
        do {
            // this loop ensures the node hasn't been deleted between the
            // reading of the old_head and the setting of the hazard pointer
            do {
                temp = old_head;
                // update hp for the new head just loaded
                hp.store(old_head);
                old_head = head.load();
            } while (old_head != temp);
        } while (old_head && !head.compare_exchange_strong(old_head, old_head->next));

        // successfully update the header, so cleanup hazard pointer
        hp.store(nullptr);
        if (old_head) {
            counter--;
            std::shared_ptr<T> res;
            res.swap(old_head->data);
            // check for hazard pointers referencing a node before free it
            if (outstanding_hazard_pointers_for(old_head)) {
                reclaim_later(old_head);
            } else {
                delete(old_head);
            }
            delete_nodes_with_no_hazards();
            return res;
        } else {
            return nullptr;
        }
    }

}

#endif
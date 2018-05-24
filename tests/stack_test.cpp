//
// Created by 陳其駿 on 2018/5/21.
//

#include "stack_test.hpp"
#include "stack_no_free.hpp"
#include "stack_with_hp.hpp"

using namespace std;

template<typename S>
static void test_push_stack(S *v, int tid) {
    for (int i = 0; i < NODE_NUM; i++) {
        v->push(i);
    }
}

template<typename S>
static void test_pop_stack(S *v, int tid) {
    for (int i = 0; i < NODE_NUM; i++) {
        v->pop();
    }
}

template<typename S>
void lockfree_stack_test() {
    thread thread_arr[THREAD_NUM];
    auto timer = PreciseTimer("lockfree_stack_test");

    timer.start();
    auto *stack = new S();
    {
        for (int i = 0; i < THREAD_NUM; i++) {
            thread_arr[i] = thread(test_push_stack<S>, stack, i);
        }
        for (auto &i : thread_arr) {
            i.join();
        }
    }

    assert(stack->size() == THREAD_NUM * NODE_NUM);
    timer.stop();
    timer.print();

    timer.reset();
    timer.start();
    {
        for (int i = 0; i < THREAD_NUM; i++) {
            thread_arr[i] = thread(test_pop_stack<S>, stack, i);
        }
        for (auto &i : thread_arr) {
            i.join();
        }
    }
    timer.stop();
    timer.print();

    assert(stack->empty());

    delete stack;
}

void lockfree_stack_test_driver() {
    lockfree_stack_test<lockfree_ds::stack_no_free<int>>();
    lockfree_stack_test<lockfree_ds::stack_with_hp<int>>();
}
//
// Created by 陳其駿 on 2018/5/21.
//

#include "stack_test.hpp"

using namespace std;

static void test_push_stack(lockfree_ds::stack<int> *v, int tid) {
    for (int i = 0; i < NODE_NUM; i++) {
        v->push(i);
    }
}

static void test_pop_stack(lockfree_ds::stack<int> *v, int tid) {
    for (int i = 0; i < NODE_NUM; i++) {
        v->pop();
    }
}

void lockfree_stack_test() {
    thread thread_arr[THREAD_NUM];
    auto timer = PreciseTimer("lockfree_stack_test");

    timer.start();
    auto * stack = new lockfree_ds::stack<int>();
    {
        for (int i = 0; i < THREAD_NUM; i++) {
            thread_arr[i] = thread(test_push_stack, stack, i);
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
            thread_arr[i] = thread(test_pop_stack, stack, i);
        }
        for (auto &i : thread_arr) {
            i.join();
        }
    }
    timer.stop();
    timer.print();

    assert(stack->empty());
}
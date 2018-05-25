//
// Created by 陳其駿 on 2018/5/21.
//

#include "ds_test.hpp"
#include "treiber_stack_no_free.hpp"
#include "treiber_stack_with_hp.hpp"
#include "msqueue_no_free.hpp"
#include "msqueue_with_hp.hpp"

using namespace std;

template<typename DS>
static void test_push(DS *v, int tid) {
    for (int i = 0; i < NODE_NUM; i++) {
        v->push(i);
    }
}

template<typename DS>
static void test_pop(DS *v, int tid) {
    for (int i = 0; i < NODE_NUM; i++) {
        v->pop();
    }
}

template<typename DS>
void lockfree_ds_test(const string &name) {
    thread thread_arr[THREAD_NUM];
    auto timer = PreciseTimer(name);

    timer.start();
    auto *container = new DS();
    {
        for (int i = 0; i < THREAD_NUM; i++) {
            thread_arr[i] = thread(test_push<DS>, container, i);
        }
        for (auto &i : thread_arr) {
            i.join();
        }
    }

    assert(container->size() == THREAD_NUM * NODE_NUM);
    timer.stop();
    timer.print();

    timer.reset();
    timer.start();
    {
        for (int i = 0; i < THREAD_NUM; i++) {
            thread_arr[i] = thread(test_pop<DS>, container, i);
        }
        for (auto &i : thread_arr) {
            i.join();
        }
    }
    timer.stop();
    timer.print();

    assert(container->empty());

    delete container;
}

void lockfree_stack_test_driver() {
//    lockfree_ds_test<lockfree_ds::treiber_stack_no_free<int>>("lockfree_ds::treiber_stack_no_free");
//    lockfree_ds_test<lockfree_ds::treiber_stack_with_hp<int>>("lockfree_ds::treiber_stack_with_hp");
//    lockfree_ds_test<lockfree_ds::msqueue_no_free<int>>("lockfree_ds::msqueue_no_free");
    lockfree_ds_test<lockfree_ds::msqueue_with_hp<int>>("lockfree_ds::msqueue_with_hp");
}
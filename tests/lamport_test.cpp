//
// Created by 陳其駿 on 5/15/18.
//

#include "lamport_test.hpp"
#include <algorithm>
#include <functional>

using namespace std;

template<class lock>
static void test_push(vector<int> *v, int tid, lock *lamport_lock) {
    for (int i = 0; i < NODE_NUM; i++) {
        lamport_lock->lock(tid);
        v->push_back(i);
        lamport_lock->unlock(tid);
    }
}

template<class lock>
static void test_pop(vector<int> *v, int tid, lock *lamport_lock) {
    for (int i = 0; i < NODE_NUM; i++) {
        lamport_lock->lock(tid);
        v->pop_back();
        lamport_lock->unlock(tid);
    }
}

template<class lock>
static void lamport_test(const string &name) {
    thread thread_arr[THREAD_NUM];

    auto *vec = new vector<int>();
    auto *lamport_vec_lock = new lock(THREAD_NUM);

    auto timer = PreciseTimer(name);
    timer.start();
    for (int i = 0; i < THREAD_NUM; i++) {
        thread_arr[i] = thread(test_push<lock>, vec, i, lamport_vec_lock);
    }
    for (auto &i : thread_arr) {
        i.join();
    }
    timer.stop();
    timer.print();
    assert(vec->size() == THREAD_NUM * NODE_NUM);

    timer.reset();
    timer.start();
    for (int i = 0; i < THREAD_NUM; i++) {
        thread_arr[i] = thread(test_pop<lock>, vec, i, lamport_vec_lock);
    }
    for (auto &i : thread_arr) {
        i.join();
    }
    timer.stop();
    timer.print();

    assert(vec->empty());
}

void lamport_test_driver() {
    lamport_test<centralized_locks::lamport_bakery_lock>("lamport_bakery_lock");
    lamport_test<centralized_locks::lamport_fast_lock>("lamport_fast_lock");
}
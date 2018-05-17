//
// Created by 陳其駿 on 5/16/18.
//

#include "mcs_test.h"

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include "Timer.h"
#include "../scalable_locks/mcs_lock.h"

#define THREAD_NUM      20
#define NODE_NUM        1000

using namespace std;

scalable_locks::mcs_lock *mcs_lock;

static void test_push_mcs(vector<int> *v, int tid) {
    scalable_locks::_qnode q;

    for (int i = 0; i < NODE_NUM; i++) {
        mcs_lock->lock(&q);
        v->push_back(i);
        mcs_lock->unlock(&q);
    }
}

static void test_pop_mcs(vector<int> *v, int tid) {
    scalable_locks::_qnode q;

    for (int i = 0; i < NODE_NUM; i++) {
//        cout << "tid: " << tid << ", address: " << &q << endl;
        mcs_lock->lock(&q);
        v->pop_back();
        mcs_lock->unlock(&q);
    }
}

void mcs_test() {
    thread thread_arr[THREAD_NUM];

    auto *vec = new vector<int>();
    mcs_lock = new scalable_locks::mcs_lock();

    auto timer = Timer("mcs_test");
    timer.start();
    for (int i = 0; i < THREAD_NUM; i++) {
        thread_arr[i] = thread(test_push_mcs, vec, i);
    }
    for (auto &i : thread_arr) {
        i.join();
    }
    timer.stop();
    timer.print();

//    cout << "total elements: " << vec->size() << endl;
    assert(vec->size() == THREAD_NUM * NODE_NUM);

    timer.reset();
    timer.start();
    for (int i = 0; i < THREAD_NUM; i++) {
        thread_arr[i] = thread(test_pop_mcs, vec, i);
    }
    for (auto &i : thread_arr) {
        i.join();
    }
    timer.stop();
    timer.print();

    assert(vec->empty());
}
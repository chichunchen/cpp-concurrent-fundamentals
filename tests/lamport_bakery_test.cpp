//
// Created by 陳其駿 on 5/15/18.
//

#include "lamport_bakery_test.h"

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <lamport_bakery.h>
#include <ticket_lock.h>
#include "tas_lock.h"
#include "tatas_lock.h"
#include "Timer.h"

#define THREAD_NUM      5
#define NODE_NUM        3000

using namespace std;

centralized_locks::lamport_bakery_lock *vec_lock;

void test_push_lamport(vector<int> *v, int tid) {
    for (int i = 0; i < NODE_NUM; i++) {
        vec_lock->lock(tid);
        v->push_back(i);
        vec_lock->unlock(tid);
    }
}

void test_pop_lamport(vector<int> *v, int tid) {
    for (int i = 0; i < NODE_NUM; i++) {
        vec_lock->lock(tid);
        v->pop_back();
        vec_lock->unlock(tid);
    }
}

void lamport_bakery_test() {
    thread thread_arr[THREAD_NUM];

    auto *vec = new vector<int>();
    vec_lock = new centralized_locks::lamport_bakery_lock(THREAD_NUM);

    auto timer = Timer("vec push");
    timer.start();
    for (int i = 0; i < THREAD_NUM; i++) {
        thread_arr[i] = thread(test_push_lamport, vec, i);
    }
    for (int i = 0; i < THREAD_NUM; ++i) {
        thread_arr[i].join();
    }
    timer.stop();
    timer.print();

    cout << "total elements: " << vec->size() << endl;
    timer.reset();
    timer.start();
    for (int i = 0; i < THREAD_NUM; i++) {
        thread_arr[i] = thread(test_pop_lamport, vec, i);
    }
    for (auto &i : thread_arr) {
        i.join();
    }
    timer.stop();
    timer.print();

    assert(vec->empty());
}
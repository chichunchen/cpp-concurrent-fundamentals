//
// Created by 陳其駿 on 5/15/18.
//

#include "lamport_test.h"

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include "lamport_bakery.h"
#include "lamport_fast.h"
#include "Timer.h"

#define THREAD_NUM      20
#define NODE_NUM        1000

using namespace std;

centralized_locks::lamport_bakery_lock *bakery_vec_lock;
centralized_locks::lamport_fast_lock *fast_vec_lock;

static void test_push_bakery(vector<int> *v, int tid) {
    for (int i = 0; i < NODE_NUM; i++) {
        bakery_vec_lock->lock(tid);
        v->push_back(i);
        bakery_vec_lock->unlock(tid);
    }
}

static void test_pop_bakery(vector<int> *v, int tid) {
    for (int i = 0; i < NODE_NUM; i++) {
        bakery_vec_lock->lock(tid);
        v->pop_back();
        bakery_vec_lock->unlock(tid);
    }
}

static void test_push_fast(vector<int> *v, int tid) {
    for (int i = 0; i < NODE_NUM; i++) {
        fast_vec_lock->lock(tid);
        v->push_back(i);
        fast_vec_lock->unlock(tid);
    }
}

static void test_pop_fast(vector<int> *v, int tid) {
    for (int i = 0; i < NODE_NUM; i++) {
        fast_vec_lock->lock(tid);
        v->pop_back();
        fast_vec_lock->unlock(tid);
    }
}

void lamport_bakery_test() {
    thread thread_arr[THREAD_NUM];

    auto *vec = new vector<int>();
    bakery_vec_lock = new centralized_locks::lamport_bakery_lock(THREAD_NUM);

    auto timer = Timer("lamport_bakery_test");
    timer.start();
    for (int i = 0; i < THREAD_NUM; i++) {
        thread_arr[i] = thread(test_push_bakery, vec, i);
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
        thread_arr[i] = thread(test_pop_bakery, vec, i);
    }
    for (auto &i : thread_arr) {
        i.join();
    }
    timer.stop();
    timer.print();

    assert(vec->empty());
}

void lamport_fast_test() {
    thread thread_arr[THREAD_NUM];

    auto *vec = new vector<int>();
    fast_vec_lock = new centralized_locks::lamport_fast_lock(THREAD_NUM);

    auto timer = Timer("lamport_fast_test");
    timer.start();
    for (int i = 0; i < THREAD_NUM; i++) {
        thread_arr[i] = thread(test_push_fast, vec, i);
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
        thread_arr[i] = thread(test_pop_fast, vec, i);
    }
    for (auto &i : thread_arr) {
        i.join();
    }
    timer.stop();
    timer.print();

    assert(vec->empty());
}
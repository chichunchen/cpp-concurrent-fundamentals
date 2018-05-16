#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <lamport_bakery.h>
#include <ticket_lock.h>
#include "tas_lock.h"
#include "tatas_lock.h"
#include "Timer.h"
#include "lamport_test.h"

#define THREAD_NUM      10
#define NODE_NUM        10000

using namespace std;

//std::mutex vec_mutex_lock;
//centralized_locks::Test_and_set_lock vec_mutex_lock;
//centralized_locks::Test_and_test_and_set_lock vec_mutex_lock;
centralized_locks::Ticket_lock vec_mutex_lock;

void test_push(vector<int> *v, int tid) {
    for (int i = 0; i < NODE_NUM; i++) {
        vec_mutex_lock.lock();
        v->push_back(i);
        vec_mutex_lock.unlock();
    }
}

void test_pop(vector<int> *v, int tid) {
    for (int i = 0; i < NODE_NUM; i++) {
        vec_mutex_lock.lock();
        v->pop_back();
        vec_mutex_lock.unlock();
    }
}

void test_normal_api_lock() {
    thread thread_arr[THREAD_NUM];

    auto *vec = new vector<int>();

    auto timer = Timer("vec push");
    timer.start();
    for (int i = 0; i < THREAD_NUM; i++) {
        thread_arr[i] = thread(test_push, vec, i);
    }
    for (int i = 0; i < THREAD_NUM; ++i) {
        thread_arr[i].join();
    }
    timer.stop();
    timer.print();

    cout << "total elements: " << vec->size() << endl;
    assert(vec->size() == THREAD_NUM * NODE_NUM);

    timer.reset();
    timer.start();
    for (int i = 0; i < THREAD_NUM; i++) {
        thread_arr[i] = thread(test_pop, vec, i);
    }
    for (auto &i : thread_arr) {
        i.join();
    }
    timer.stop();
    timer.print();

    assert(vec->empty());
}

int main() {
    lamport_bakery_test();
//    test_normal_api_lock();
    return 0;
}

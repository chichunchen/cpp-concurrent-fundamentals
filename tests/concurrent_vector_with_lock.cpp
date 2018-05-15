#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <lamport_bakery.h>
#include <ticket_lock.h>
#include "tas_lock.h"
#include "tatas_lock.h"
#include "Timer.h"

#define THREAD_NUM      10
#define NODE_NUM        10000

using namespace std;

//std::mutex vec_mutex;
//centralized_locks::Test_and_set_lock vec_mutex;
centralized_locks::Test_and_test_and_set_lock vec_mutex;
//centralized_locks::lamport_bakery_lock *vec_mutex;
//centralized_locks::Ticket_lock vec_mutex;

void test_push(vector<int> *v, int tid) {
    for (int i = 0; i < NODE_NUM; i++) {
//        vec_mutex->lock(tid);
        vec_mutex.lock();
        v->push_back(i);
        vec_mutex.unlock();
//        vec_mutex->unlock(tid);
    }
}

void test_pop(vector<int> *v, int tid) {
    for (int i = 0; i < NODE_NUM; i++) {
        vec_mutex.lock();
        v->pop_back();
        vec_mutex.unlock();
    }
}

int main() {
    thread thread_arr[THREAD_NUM];

    auto *vec = new vector<int>();
//    vec_mutex = new centralized_locks::lamport_bakery_lock(THREAD_NUM);

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
    timer.reset();
    timer.start();
    for (int i = 0; i < THREAD_NUM; i++) {
        thread_arr[i] = thread(test_pop, vec, i);
    }
    for (int i = 0; i < THREAD_NUM; ++i) {
        thread_arr[i].join();
    }
    timer.stop();
    timer.print();

    assert(vec->empty());

    return 0;
}
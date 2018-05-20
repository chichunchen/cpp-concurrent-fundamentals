//
// Created by 陳其駿 on 5/19/18.
//

#include "mutex_tas_tatas_ticket.hpp"

template <class lock>
static void test_push(std::vector<int> *v, lock *vec_mutex_lock) {
    for (int i = 0; i < NODE_NUM; i++) {
        vec_mutex_lock->lock();
        v->push_back(i);
        vec_mutex_lock->unlock();
    }
}

template <class lock>
static void test_pop(std::vector<int> *v, lock *vec_mutex_lock) {
    for (int i = 0; i < NODE_NUM; i++) {
        vec_mutex_lock->lock();
        v->pop_back();
        vec_mutex_lock->unlock();
    }
}

template <class lock>
static void test_mutex_tas_tatas_ticket(const std::string &name) {
    std::thread thread_arr[THREAD_NUM];

    auto *vec = new std::vector<int>();
    auto *vec_mutex_lock = new lock;
    auto timer = PreciseTimer(name);

    timer.start();
    for (auto &i : thread_arr) {
        i = std::thread(test_push<lock>, vec, vec_mutex_lock);
    }
    for (auto &i : thread_arr) {
        i.join();
    }
    timer.stop();
    timer.print();

    assert(vec->size() == THREAD_NUM * NODE_NUM);

    timer.reset();
    timer.start();
    for (auto &i : thread_arr) {
        i = std::thread(test_pop<lock>, vec, vec_mutex_lock);
    }
    for (auto &i : thread_arr) {
        i.join();
    }
    timer.stop();
    timer.print();

    assert(vec->empty());
}

void test_mutex_tas_tatas_ticket_driver() {
    test_mutex_tas_tatas_ticket<std::mutex>("mutex");
    test_mutex_tas_tatas_ticket<centralized_locks::Test_and_set_lock>("test and set lock");
    test_mutex_tas_tatas_ticket<centralized_locks::Test_and_test_and_set_lock>("test and test and set lock");
    test_mutex_tas_tatas_ticket<centralized_locks::Ticket_lock>("ticket lock");
}

void test_scalable_tas_tatas_ticket_driver() {
    test_mutex_tas_tatas_ticket<scalable_locks::Test_and_set_lock>("tas lock with exponential backoff");
    test_mutex_tas_tatas_ticket<scalable_locks::Test_and_test_and_set_lock>("tatas lock with exponential backoff");
    test_mutex_tas_tatas_ticket<scalable_locks::Ticket_lock>("ticket lock with exponential backoff");
}
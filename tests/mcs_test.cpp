//
// Created by 陳其駿 on 5/16/18.
//

#include "mcs_test.hpp"

using namespace std;

scalable_locks::mcs *mcs_lock;

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
    auto timer = PreciseTimer("mcs_test");

    timer.start();
    auto *vec = new vector<int>();
    {
        mcs_lock = new scalable_locks::mcs();
        for (int i = 0; i < THREAD_NUM; i++) {
            thread_arr[i] = thread(test_push_mcs, vec, i);
        }
        for (auto &i : thread_arr) {
            i.join();
        }
    }

    assert(vec->size() == THREAD_NUM * NODE_NUM);
    timer.stop();
    timer.print();

    timer.reset();
    timer.start();
    {
        mcs_lock = new scalable_locks::mcs();
        for (int i = 0; i < THREAD_NUM; i++) {
            thread_arr[i] = thread(test_pop_mcs, vec, i);
        }
        for (auto &i : thread_arr) {
            i.join();
        }
    }
    timer.stop();
    timer.print();

    assert(vec->empty());
}
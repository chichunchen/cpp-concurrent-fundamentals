//
// Created by 陳其駿 on 5/19/18.
//

#ifndef CONCURRENT_TOOLKITS_CPP_CENTRALIZED_BARRIER_HPP
#define CONCURRENT_TOOLKITS_CPP_CENTRALIZED_BARRIER_HPP

#include <atomic>
#include <vector>
#include <iostream>
#include <mutex>

// TODO: There is fatal bug that leak threads even if the threads amount is not equal to n
class centralized_barrier {
private:
    std::atomic<int> count;
    const int n;
    std::atomic<bool> sense;
    std::vector<bool> local_sense;
    std::atomic<int> phase;
    std::mutex barrier_mutex;

public:
    centralized_barrier(int threadNum) : count(0), n(threadNum - 1), sense(true), phase(0) {
        for (int i = 0; i < threadNum; ++i) {
            local_sense.push_back(true);
        }
    }

    void wait_scs(int self) {
        bool s = !local_sense[self];
        local_sense[self] = s;                  // each thread toggles its own sense

        if (count.fetch_add(1) == n) {          // note release ordering
            std::cout << "wait_scs tid: " << self << std::endl;
            count.store(0);
            sense.store(s);                     // last thread toggles global sense
            phase.fetch_add(1);
            std::cout << std::endl << "---- phase " << phase << " ----" << std::endl;
        } else {
            std::cout << "wait_scs tid: " << self << std::endl;
            while (sense.load() != s);          // spin
        }
    }

    void wait_458(int self) {
        bool s = !local_sense[self];
        local_sense[self] = s;

        int prev = count.fetch_add(1);
        if (prev == n) {
            count.store(0, std::memory_order_relaxed);
            phase.fetch_add(1, std::memory_order_relaxed);

            barrier_mutex.lock();
            std::cout << std::endl << "---- phase " << phase << " ----" << std::endl;
            barrier_mutex.unlock();

            // make sure all writes are visible before write to sense.
            // all to write fence();
            std::atomic_thread_fence(std::memory_order_release);

            sense.store(s);
        }

        while (sense.load(std::memory_order_relaxed) != s);

        // do not allow operations after barrier to happen
        // before this fence
        // read_to_all_fence()
        std::atomic_thread_fence(std::memory_order_acquire);
    }
};

#endif //CONCURRENT_TOOLKITS_CPP_CENTRALIZED_BARRIER_HPP

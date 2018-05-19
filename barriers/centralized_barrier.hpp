//
// Created by 陳其駿 on 5/19/18.
//

#ifndef CONCURRENT_TOOLKITS_CPP_CENTRALIZED_BARRIER_HPP
#define CONCURRENT_TOOLKITS_CPP_CENTRALIZED_BARRIER_HPP

#include <atomic>
#include <vector>
#include <iostream>

class centralized_barrier {
private:
    std::atomic<int> count;
    const int n;
    std::atomic<bool> sense;
    std::vector<bool> local_sense;
    std::atomic<int> phase;
public:
    centralized_barrier(int threadNum) : count(0), n(threadNum - 1), sense(true), phase(0) {
        for (int i = 0; i < threadNum; ++i) {
            local_sense.push_back(true);
        }
    }

    void wait(int self) {
        bool s = !local_sense[self];
        local_sense[self] = s;                  // each thread toggles its own sense

        if (count.fetch_add(1) == n) {      // note release ordering
            std::cout << "wait tid: " << self << std::endl;
            count.store(0);
            sense.store(s);                     // last thread toggles global sense
            phase.fetch_add(1);
            std::cout << std::endl << "---- phase " << phase << " ----" << std::endl;
        } else {
            std::cout << "wait tid: " << self << std::endl;
            while (sense.load() != s);          // spin
        }
    }
};

#endif //CONCURRENT_TOOLKITS_CPP_CENTRALIZED_BARRIER_HPP

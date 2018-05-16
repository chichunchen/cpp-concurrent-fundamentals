//
// Created by 陳其駿 on 5/15/18.
//

#ifndef CONCURRENT_TOOLKITS_CPP_TIMER_H
#define CONCURRENT_TOOLKITS_CPP_TIMER_H

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <utility>
#include <cassert>

using std::chrono::system_clock;

class Timer {
private:
    std::string name;
    system_clock::time_point begin;
    system_clock::time_point end;
    std::chrono::duration<double> total;
    std::chrono::duration<double> last;
    bool active;
    bool valid;

public:
    explicit Timer(std::string name) : name(std::move(name)), active(false), valid(false), total(0), last(0) {}

    // TODO should reset total and last
    void reset() {
        assert(!active);
    }

    void start() {
        assert(!active);
        begin = std::chrono::system_clock::now();
        active = true;
        valid = false;
    }

    void stop() {
        assert(active);
        end = std::chrono::system_clock::now();
        last = end - begin;
        total += last;
        active = false;
        valid = true;
    }

    void print() {
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        std::cout << "finished computation at " << std::ctime(&end_time)
                  << "elapsed time: " << last.count() << "s\n";
    }
};


#endif //CONCURRENT_TOOLKITS_CPP_TIMER_H

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

class PreciseTimer {
private:
    std::string name;
    std::chrono::time_point<std::chrono::high_resolution_clock> begin;
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    std::chrono::duration<double> total;
    std::chrono::duration<double> last;
    bool active;
    bool valid;

public:
    explicit PreciseTimer(std::string name) : name(std::move(name)), active(false), valid(false), total(0), last(0) {}

    void reset() {
        assert(!active);
        total = std::chrono::milliseconds::zero();
        last = std::chrono::milliseconds::zero();
    }

    void start() {
        assert(!active);
        begin = std::chrono::high_resolution_clock::now();
        active = true;
        valid = false;
    }

    void stop() {
        assert(active);
        end = std::chrono::high_resolution_clock::now();
        last = end - begin;
        total += last;
        active = false;
        valid = true;
    }

    void print() {
        std::cout << "[" << name << "] elapsed " << last.count() << "s\n";
    }
};


#endif //CONCURRENT_TOOLKITS_CPP_TIMER_H

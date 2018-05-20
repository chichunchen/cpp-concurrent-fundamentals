//
// Created by 陳其駿 on 5/20/18.
//

#ifndef CONCURRENT_TOOLKITS_CPP_RW_READER_PREF_HPP
#define CONCURRENT_TOOLKITS_CPP_RW_READER_PREF_HPP

#include <atomic>
#include <zconf.h>
#include <ctime>

#include "Misc.hpp"

namespace rwlocks {

    class ReaderPreferenceLock {
    private:
        std::atomic<int> n;     // low-order bit indicates whether a writer is
                                // active; remaining bits are a count of active
                                // or waiting readers
        const int WA_flag = 1;
        const int RC_int = 2;
        const int base = 5;
        const int limit = 100;
        const int multiplier = 100000;

    public:
        ReaderPreferenceLock() : n(0) {}

        void writer_acquire() {
            int delay = base;
            int a = 0;
            while (!n.compare_exchange_strong(a, WA_flag)) {
                timespec ts = {0, delay};
                nanosleep(&ts, nullptr);
                delay = MIN(delay * multiplier, limit);
            }
            // fence(R||RW)
        }

        void writer_release() {
            n.fetch_sub(WA_flag);
        }

        void reader_acquire() {
            n.fetch_add(RC_int);
            while (1 == (n.load() & WA_flag));
            // fence(R||R)
        }

        void reader_release() {
            n.fetch_add(-RC_int);
        }
    };

}

#endif //CONCURRENT_TOOLKITS_CPP_RW_READER_PREF_HPP

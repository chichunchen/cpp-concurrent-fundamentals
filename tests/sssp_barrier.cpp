//
// Created by 陳其駿 on 5/19/18.
//

#include "sssp_barrier.h"

#include <cstdio>
#include <cassert>
#include <climits>
#include <thread>
#include <string>
#include <pthread.h>

#include "simplegraph_atomic.h"
#include "PreciseTimer.h"
#include "config.h"
#include "../barriers/centralized_barrier.hpp"

using namespace std;

const int INF = INT_MAX;
int threadNum = 1;
bool changed = false;

centralized_barrier *mybarrier;

void sssp(SimpleCSRGraphUII g, int tid, int* rounds_ptr) {
    size_t total_nodes = g.num_nodes;
    size_t slice = total_nodes / threadNum;
    size_t start = tid * slice;
    size_t end = start + slice;
    if (tid+1 == threadNum) end = total_nodes;

    int rounds;
    int src = 0;

    // sssp_init
    for (size_t i = start; i < end; i++) {
        g.node_wt[i] = (i == src) ? 0 : INF;
    }

    mybarrier->wait_scs(tid);

    // sssp_round
    for(rounds = 0; rounds < total_nodes - 1; rounds++) {
        for(size_t node = start; node < end; node++) {
            if(g.node_wt[node] == INF) continue;

            for(unsigned int e = g.row_start[node]; e < g.row_start[node + 1]; e++) {
                unsigned int dest = g.edge_dst[e];

                while (true) {
                    int distance = g.node_wt[node].load() + g.edge_wt[e].load();
                    int prev_distance = g.node_wt[dest].load();

                    bool pred_1 = distance == (g.node_wt[node].load() + g.edge_wt[e]);
                    bool pred_2 = prev_distance == g.node_wt[dest].load();

                    if (pred_1 && pred_2) {
                        if(prev_distance > distance) {
                            g.node_wt[dest].exchange(distance);
                            // check if atomic swap success
                            // if not success, then we continue to next loop
                            if (g.node_wt[dest].load() == distance) {
                                changed = true;
                                break;
                            }
                            else
                                continue;
                        }
                        break;
                    }
                }
            }
        }

        mybarrier->wait_scs(tid);
        if(!changed) break;
        mybarrier->wait_scs(tid);
        changed = false;
        mybarrier->wait_scs(tid);
    }

    if (tid == 0) *rounds_ptr = rounds;
}

void write_output(SimpleCSRGraphUII &g, const char *out) {
    FILE *fp;

    fp = fopen(out, "w");
    if(!fp) {
        fprintf(stderr, "ERROR: Unable to open output file '%s'\n", out);
        exit(1);
    }

    for(int i = 0; i < g.num_nodes; i++) {
        int r;
        if(g.node_wt[i] == INF) {
            r = fprintf(fp, "%d INF\n", i);
        } else {
            r = fprintf(fp, "%d %d\n", i, g.node_wt[i].load());
        }

        if(r < 0) {
            fprintf(stderr, "ERROR: Unable to write output\n");
            exit(1);
        }
    }
}

void sssp_barrier_driver() {
    char const *argv[4] = {
            "sssp_barrier_driver", "NY.txt", "gg.txt", "THREAD_NUM"
    };

    SimpleCSRGraphUII input;

    if(!input.load_file(argv[1])) {
        fprintf(stderr, "ERROR: failed to load graph\n");
        exit(1);
    }

    printf("Loaded '%s', %zu nodes, %zu edges\n", argv[1], input.num_nodes, input.num_edges);

    PreciseTimer t("sssp");

    int src = 0;
    threadNum = THREAD_NUM;

    thread thread_arr[threadNum];

#ifdef __APPLE__
    mybarrier = new centralized_barrier(threadNum);
#elif __linux__
    pthread_barrier_init(&mybarrier, NULL, threadNum);
#endif

    t.start();

    // start of parallel sssp
    int rounds = 0;
    int* rounds_ptr = &rounds;
    for (int i = 0; i < threadNum; ++i) {
        thread_arr[i] = thread(sssp, input, i, rounds_ptr);
    }
    for (int i = 0; i < threadNum; ++i) {
        thread_arr[i].join();
    }
    // end of parallel sssp_sssp

    t.stop();

#ifdef __linux__
    pthread_barrier_destroy(&mybarrier);
#endif

    printf("%d rounds\n", rounds); /* parallel versions may have a different number of rounds */
    t.print();

    write_output(input, argv[2]);

    printf("Wrote output '%s'\n", argv[2]);
}

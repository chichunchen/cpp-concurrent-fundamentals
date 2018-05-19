#include <iostream>
#include <thread>
#include <vector>

#include "lamport_test.h"
#include "mcs_test.h"
#include "mutex_tas_tatas_ticket.h"
#include "sssp_barrier.h"

using namespace std;

void lock() {
    test_mutex_tas_tatas_ticket_driver();
    lamport_test_driver();
    mcs_test();
}

void barrier() {
	sssp_barrier_driver();
}

int main() {
	barrier();
    return 0;
}

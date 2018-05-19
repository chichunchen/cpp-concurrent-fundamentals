#include <iostream>
#include <thread>
#include <vector>

#include "lamport_test.h"
#include "mcs_test.h"
#include "mutex_tas_tatas_ticket.h"

using namespace std;


int main() {
    test_mutex_tas_tatas_ticket_driver();
    lamport_test_driver();
    mcs_test();
    return 0;
}

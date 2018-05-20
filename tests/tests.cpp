#include <iostream>
#include <thread>
#include <vector>

#include "lamport_test.hpp"
#include "mcs_test.hpp"
#include "mutex_tas_tatas_ticket.hpp"

using namespace std;


int main() {
    test_mutex_tas_tatas_ticket_driver();
    lamport_test_driver();
    mcs_test();
    return 0;
}

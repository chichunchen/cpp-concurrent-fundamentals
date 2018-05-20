//
// Created by 陳其駿 on 5/19/18.
//

#ifndef CONCURRENT_TOOLKITS_CPP_MUTEX_TAS_TATAS_TICKET_H
#define CONCURRENT_TOOLKITS_CPP_MUTEX_TAS_TATAS_TICKET_H

#include <vector>
#include <thread>
#include <mutex>

#include "config.h"
#include "PreciseTimer.hpp"
#include "tas_lock.hpp"
#include "tatas_lock.hpp"
#include "ticket_lock.hpp"

#include "../scalable_locks/tas_exp_lock.hpp"
#include "../scalable_locks/tatas_exp_lock.hpp"
#include "../scalable_locks/ticket_exp_lock.hpp"


void test_mutex_tas_tatas_ticket_driver();

void test_scalable_tas_tatas_ticket_driver();


#endif //CONCURRENT_TOOLKITS_CPP_MUTEX_TAS_TATAS_TICKET_H

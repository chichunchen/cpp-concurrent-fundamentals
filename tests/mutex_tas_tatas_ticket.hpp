//
// Created by 陳其駿 on 5/19/18.
//

#ifndef CONCURRENT_TOOLKITS_CPP_MUTEX_TAS_TATAS_TICKET_H
#define CONCURRENT_TOOLKITS_CPP_MUTEX_TAS_TATAS_TICKET_H

#include <vector>
#include <thread>
#include "config.h"
#include "PreciseTimer.hpp"
#include <mutex>
#include "tas_lock.hpp"
#include "tatas_lock.hpp"
#include "ticket_lock.hpp"


void test_mutex_tas_tatas_ticket_driver();


#endif //CONCURRENT_TOOLKITS_CPP_MUTEX_TAS_TATAS_TICKET_H

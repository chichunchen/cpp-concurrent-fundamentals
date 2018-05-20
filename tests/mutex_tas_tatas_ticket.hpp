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
#include "tas.hpp"
#include "tatas.hpp"
#include "ticket.hpp"

#include "tas_exp.hpp"
#include "tatas_exp.hpp"
#include "ticket_exp.hpp"


void test_mutex_tas_tatas_ticket_driver();

void test_scalable_tas_tatas_ticket_driver();


#endif //CONCURRENT_TOOLKITS_CPP_MUTEX_TAS_TATAS_TICKET_H

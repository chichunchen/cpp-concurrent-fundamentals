//
// Created by 陳其駿 on 5/15/18.
//

#ifndef CONCURRENT_TOOLKITS_CPP_LAMPORT_BAKERY_TEST_H
#define CONCURRENT_TOOLKITS_CPP_LAMPORT_BAKERY_TEST_H

#include <string>

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include "lamport_bakery.h"
#include "lamport_fast.h"
#include "PreciseTimer.h"
#include "config.h"

void lamport_test_driver();

#endif //CONCURRENT_TOOLKITS_CPP_LAMPORT_BAKERY_TEST_H

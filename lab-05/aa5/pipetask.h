#pragma once


#include "matr.h"

#include <vector>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <array>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <ctime>

typedef struct {
    //help
    int n;
    
    //time
    std::chrono::_V2::system_clock::time_point start_generate;
    std::chrono::_V2::system_clock::time_point stop_generate;
    std::chrono::_V2::system_clock::time_point start_mul;
    std::chrono::_V2::system_clock::time_point stop_mul;
    std::chrono::_V2::system_clock::time_point start_write;
    std::chrono::_V2::system_clock::time_point stop_write;

    // data
    matr* m1;
    matr* m2;
    matr* mrez;
} pipeTask;

typedef struct {
    vector<pipeTask*> queue;
    bool stop;
} queue;

void Enqueue(queue* qu, pipeTask* t);
pipeTask* Dequeue(queue* qu);
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

#include "matr.h"
#include "iog.h"

#include "pipeline.h"
#include "interface.h"

int main()
{
    setbuf(stdout, NULL);
    srand(time(nullptr));

    auto start = chrono::high_resolution_clock::now();

    auto q = pipeline(8);
    logging(q, 8, start);

    auto q1 = pipelineposl(8);
    logging(q1, 8, start);

    return 0;
}
#include "Profiler.h"

#include <chrono>
#include <iostream>
using namespace std;

void Profiler::start() {
    t1 = chrono::high_resolution_clock::now();
}

void Profiler::end() {
    t2 = chrono::high_resolution_clock::now();

    printf("Duration - %lu ms\n", chrono::duration_cast<chrono::milliseconds>(t2 - t1).count());
}
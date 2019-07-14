#ifndef RUSHHOURTEST_PROFILER_H
#define RUSHHOURTEST_PROFILER_H

#include <chrono>
using namespace std;

class Profiler {
private:
    chrono::high_resolution_clock::time_point t1;
    chrono::high_resolution_clock::time_point t2;

public:
    void start();
    void end();
};

#endif

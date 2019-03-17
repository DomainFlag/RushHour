#include <iostream>

#include "./../include/RushHour.h"

using namespace std;

int main(int argc, char ** argv) {
    string filepath = "./res/puzzle13.txt";

    RushHour rh(filepath);
    rh.solve_forward();

    cout << "Solved with " << rh.thread.size() << " moves" << endl;

    return 0;
};

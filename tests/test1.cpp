#include <iostream>

#include "./../include/RushHour.h"

using namespace std;

int main() {
    RushHour rh = RushHour::create("./res/puzzle-c.txt", 3, 0, 2, 1);
    rh.draw();

    int depth = rh.solve_backward(-1);
    cout << depth << endl;

    return 0;
};

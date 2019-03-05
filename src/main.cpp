#include <iostream>

#include "./../include/RushHour.h"

using namespace std;

int main() {
    RushHour rh("./res/puzzle.txt");
    rh.draw();

    int min = rh.solve();

    cout << "Shortest path is - "  << min << endl;

    return 0;
};

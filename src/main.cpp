#include <iostream>

#include "./../include/RushHour.h"

using namespace std;

int main(int argc, const char ** argv) {
    string filepath = (argc == 1) ? "./res/puzzle.txt" : "./res/" + string(argv[1]);

    RushHour rh(filepath);
    rh.draw();

    int min = rh.solve();

    cout << "Shortest path is - "  << min << endl;

    return 0;
};

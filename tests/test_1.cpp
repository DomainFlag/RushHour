#include <iostream>

#include "./../include/RushHour.h"

using namespace std;

int main(int argc, char ** argv) {
    srand(time(NULL));

    RushHour::create(3, 0, 2, 1, 8);

    return 0;
};

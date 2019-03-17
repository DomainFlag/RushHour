#include <iostream>

#include "./../include/RushHour.h"
#include "./../include/Window.h"

using namespace std;

int main(int argc, char ** argv) {
    srand(time(NULL));

    RushHour::create(3, 0, 2, 1, 15);

    return 0;
};

#include "./../include/Window.h"

#include <iostream>
#include <string>

using namespace std;

int main(int argc, const char ** argv) {
    string filepath = "./res/";
    if(argc == 1) {
        filepath += "puzzle.txt";
    } else if(argc > 1) {
        char sufix[100];

        sscanf(argv[1], "item=%s", sufix);

        filepath += sufix;
    }

    cout << "Using input file: " << filepath << endl;

    Window window;

    window.init(filepath);

    return 0;
};

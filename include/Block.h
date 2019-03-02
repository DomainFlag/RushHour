#ifndef Block_h
#define Block_h

#include <iostream>
#include <vector>

using namespace std;

class Block {
public:
    unsigned int row;
    unsigned int col;
    unsigned int length;

    int value;
    bool orientation;

    Block(unsigned int row, unsigned int col, unsigned int length, int value, bool orientation);

    Block(unsigned int row, unsigned int col, bool orientation);

    ~Block();

    void print();
};

#endif
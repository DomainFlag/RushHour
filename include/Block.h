#ifndef Block_h
#define Block_h

#include <iostream>
#include <vector>

using namespace std;

class Block {
public:
    unsigned int row, col;
    unsigned int length;

    int value;
    bool orientation, flag = false;

    Block();

    Block(unsigned int row, unsigned int col, unsigned int length, int value, bool orientation);

    Block(unsigned int row, unsigned int col, unsigned int length, bool orientation);

    Block(unsigned int row, unsigned int col, bool orientation);

    ~Block();

    void print();

    bool isMe(unsigned int row, unsigned int col);

    bool equal(Block * block1, Block * block2);
};

#endif

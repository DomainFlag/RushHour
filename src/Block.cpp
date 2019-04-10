#include <iostream>
#include <vector>

#include "./../include/Block.h"

using namespace std;

Block::Block() {};

Block::Block(unsigned int row, unsigned int col, unsigned int length, int value, bool orientation) : row(row), col(col), length(length), value(value), orientation(orientation) {};

Block::Block(unsigned int row, unsigned int col, unsigned int length, bool orientation) : Block(row, col, length, 1, orientation) {};

Block::Block(unsigned int row, unsigned int col, bool orientation) : row(row), col(col), orientation(orientation) {};

Block::~Block() {};

void Block::print() {
    cout << "row: " << this->row << "; col: " << this->col << " & len: " << this->length << endl;
};

bool Block::isMe(unsigned int row, unsigned int col) {
    return (row == this->row && this->col <= col && col < this->col + this->length) ||
        (col == this->col && this->row <= row && row < this->row + this->length);
};

bool Block::equal(Block * block1, Block * block2) {
    return block1->row == block2->row && block1->col == block2->col;
};

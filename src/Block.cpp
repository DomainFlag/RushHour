#include <iostream>
#include <vector>

#include "./../include/Block.h"

using namespace std;

Block::Block(unsigned int row, unsigned int col, unsigned int length, int value, bool orientation) : row(row), col(col), length(length), value(value), orientation(orientation) {};

Block::Block(unsigned int row, unsigned int col, bool orientation) : row(row), col(col), orientation(orientation) {};

Block::~Block() {};

void Block::print() {
    cout << "row: " << this->row << "; col: " << this->col << " & len: " << this->length << endl;
};

// bool equal(Move * move, Block * next) {
//     int row = block->row, col = block->col;
//     acc = (block->orientation) ? & col : & row;
//     if(move->block->)
// }
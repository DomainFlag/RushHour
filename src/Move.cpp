#include <vector>

#include "./../include/Move.h"
#include "./../include/Block.h"

Move::Move() {};

Move::Move(Move * parent, Block * block, int value) : parent(parent), block(block), value(value) {
    this->depth = parent->depth + 1;
};

Move::~Move() {
    for(unsigned int g = 0; g < moves.size(); g++) {
        if(moves[g] != NULL) {
            delete moves[g];
        }
    }
};

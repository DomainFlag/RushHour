#include <vector>

#include "./../include/Move.h"
#include "./../include/Block.h"

Move::Move() {};

Move::Move(Block * block, int value) : block(block), value(value) {};

Move::~Move() {
    for(unsigned int g = 0; g < moves.size(); g++) {
        if(moves[g] != NULL) {
            delete moves[g];
        }
    }
};

int Move::depth() {
    if(this->parent == NULL)
        return 0;

    return 1 + this->parent->depth();
};
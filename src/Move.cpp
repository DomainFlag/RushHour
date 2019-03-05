#include <vector>

#include "./../include/Move.h"
#include "./../include/Block.h"

Move::Move() {};

Move::Move(Move * parent, Block * block, int value) : parent(parent), block(block), value(value) {};

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

void Move::link(Move * move1, Move * move2) {
    move1->linked.insert(move2);
    move2->linked.insert(move1);
};

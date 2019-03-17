#include <vector>
#include <unordered_set>
#include <queue>

#include "./../include/Move.h"
#include "./../include/Block.h"

Move::Move() {};

Move::Move(Move * parent, Block * block, int value) : parent(parent), block(block), value(value) {
    this->depth = parent->depth + 1;
};

int Move::getDepth() {
    Move * cycle = this;
    int depth = 0;

    while(cycle->parent != NULL) {
        cycle = cycle->parent;

        depth++;
    };

    return depth;
};
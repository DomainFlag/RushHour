#include "./../include/LinkedMove.h"
#include "./../include/Block.h"

LinkedMove::LinkedMove() : Move() {};

LinkedMove::LinkedMove(Move * move, Block * block, int value) : Move(move, block, value) {};

void LinkedMove::link(LinkedMove * move1, LinkedMove * move2) {
    move1->linked.push_back(move2);
    move2->linked.push_back(move1);
};

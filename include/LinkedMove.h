#ifndef LinkedMove_h
#define LinkedMove_h

#include <vector>

#include "./Move.h"
#include "./Block.h"

class LinkedMove : public Move {
public:
    vector<Move *> linked;

    LinkedMove();

    LinkedMove(Move * move, Block * block, int value);

    static void link(LinkedMove * move1, LinkedMove * move2);
};

#endif

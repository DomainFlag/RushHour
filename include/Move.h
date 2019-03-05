#ifndef Move_h
#define Move_h

#include <vector>
#include <unordered_set>

#include "./Block.h"

class Move {
public:
    vector<Move *> moves;
    unordered_set<Move *> linked;

    Move * parent = NULL;
    Block * block;

    int value;
    bool resolved = false;

    Move();

    Move(Move * move, Block * block, int value);

    ~Move();

    int depth();

    static void link(Move * move1, Move * move2);
};

#endif

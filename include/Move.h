#ifndef Move_h
#define Move_h

#include <vector>
#include <unordered_set>

#include "./Block.h"

class Move {
public:
    unordered_set<Move *> parents;
    unordered_set<Move *> children;

    Move * parent = NULL;
    Move * link = NULL;
    Block * block;

    int depth = -1;
    int mark = -1;
    int value;
    bool resolved;

    Move();

    Move(Move * move, Block * block, int value);

    int getDepth();
};

#endif

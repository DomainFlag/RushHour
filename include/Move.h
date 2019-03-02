#ifndef Move_h
#define Move_h

#include <vector>

#include "./Block.h"

class Move {
public:
    vector<Move *> moves;

    Move * parent = NULL;
    Block * block;
    
    int value;

    Move();

    Move(Block * block, int value);

    ~Move();

    int depth();
};

#endif
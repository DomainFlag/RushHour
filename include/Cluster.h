#ifndef Cluster_h
#define Cluster_h

#include <vector>
#include <unordered_set>

#include "Block.h"
#include "Move.h"

using namespace std;

class Cluster {
public:

    Move * head = NULL;
    Move * gap =  NULL;

    Block target;
    Block destination;
    vector<Block> blocks;

    unordered_set<Move *> graph;

    int distance = -1;

    Cluster(Block * & target, Block * & destination, vector<Block *> & blocks);

    ~Cluster();

    void setHead(Move * head);

    Move * getHead();
};

#endif
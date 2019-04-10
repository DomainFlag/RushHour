#include <vector>

#include "./../include/Cluster.h"
#include "./../include/Block.h"
#include "./../include/Move.h"

using namespace std;

Cluster::Cluster(Block * & target, Block * & destination, vector<Block *> & blocks) {
    this->target = * target;
    this->destination = * destination;

    for(Block * & block : blocks) {
        this->blocks.push_back(* block);
    }
};

Cluster::~Cluster() {
    if(this->head != NULL)
        delete this->head;
};

void Cluster::setHead(Move * head) {
    this->head = head;
};

Move * Cluster::getHead() {
    return this->head;
};
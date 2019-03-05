#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <queue>
#include <time.h>

#include "./../include/Block.h"
#include "./../include/Move.h"
#include "./../include/RushHour.h"

using namespace std;

RushHour::RushHour() {};

RushHour::RushHour(string filepath) {
    fstream file(filepath);

    if(file.is_open()) {
        // Reading destination
        unsigned int row, col, length;
        bool orientation;

        file >> row >> col >> orientation;

        this->destination = new Block(row, col, orientation);

        // Reading target
        file >> row >> col >> length >> orientation;
        this->target = new Block(row, col, length, 2, orientation);

        this->blocks.push_back(this->target);

        // Reading blocks
        while(file >> row >> col >> length >> orientation) {
            Block * block = new Block(row, col, length, 1, orientation);

            this->blocks.push_back(block);
        }

        file.close();
    } else {
        cout << "File " << filepath << " can't be opened" << endl;

        exit(1);
    }

    this->init();
};

RushHour::RushHour(Block * target, Block * destination) {
    this->target = target;
    this->destination = destination;

    this->init();
};

RushHour::~RushHour() {
    if(this->destination != NULL)
        delete this->destination;

    for(Block * & block : this->blocks) {
        delete block;
    }
};

void RushHour::sample(RushHour & rushHour, unsigned int count) {
    rushHour.blocks.reserve(count + 1);

    srand(time(NULL));

    unsigned int row, col, length;
    bool orientation, resolved;
    for(unsigned int g = 0; g < count; g++) {
        resolved = false;

        while(!resolved) {
            row = rand() % RushHour::size;
            col = rand() % RushHour::size;
            length = rand() % 2 + 2;
            orientation = rand() % 2 == 1;

            Block * block = new Block(row, col, length, 1, orientation);

            if(rushHour.resolve(block)) {
                rushHour.blocks.push_back(block);
                rushHour.insert(block);

                resolved = true;
            } else {
                delete block;
            }
        }
    }
};

RushHour RushHour::create(string filepath, unsigned int row, unsigned int col, unsigned int length, bool orientation) {
    Block * target = new Block(row, col, length, 2, orientation);
    Block * destination = new Block(row, col, orientation);

    RushHour rushHour(target, destination);
    rushHour.blocks.push_back(target);
    rushHour.insert(target);

    RushHour::sample(rushHour, 12);

    fstream file(filepath, fstream::out | fstream::trunc);

    if(file.is_open()) {
        file << destination->row << " " << destination->col << " " << destination->orientation << endl;

        for(unsigned int g = 0; g < rushHour.blocks.size(); g++) {
            Block * & block = rushHour.blocks[g];

            file << block->row << " " << block->col << " " << block->length << " " << block->orientation << endl;
        }

        file.close();
    }

    return rushHour;
};

void RushHour::insert(Block * block) {
    int row = block->row, col = block->col;
    int * acc = (block->orientation) ? & col : & row;

    for(unsigned int g = 0; g < block->length; g++) {
        this->domain[row][col] = block->value;

        (* acc)++;
    }
};

Move * RushHour::encode(Move * move) {
    stringstream ss;

    for(unsigned int g = 0; g < this->blocks.size(); g++) {
        Block * & block = this->blocks[g];

        ss << block->row << block->col << g;
    }

    string state = ss.str();

    auto it = this->state.find(state);
    if(it == this->state.end()) {
        this->state.insert({state, move});
    } else {
        return it->second;
    }

    return NULL;
};

void RushHour::init() {
    for(Block * & block : this->blocks) {
        insert(block);
    }
};

void RushHour::draw() {
    stringstream ss;

    for(unsigned int g = 0; g < this->size + 2; g++) {
        ss << "-";
    }

    ss << endl;

    for(unsigned int g = 0; g < this->size; g++) {
        ss << "|";

        for(unsigned int h = 0; h < this->size; h++) {
            if(!this->domain[g][h]) {
                ss << " ";
            } else if(this->domain[g][h] == 1) {
                ss << "x";
            } else {
                ss << "o";
            }
        }

        ss << "|" << endl;
    }

    for(unsigned int g = 0; g < this->size + 2; g++) {
        ss << "-";
    }

    ss << endl;

    cout << ss.str() << endl;
};

bool RushHour::resolve() {
    int row = this->target->row, col = this->target->col;
    int * acc = (this->target->orientation) ? & col : & row;

    int sgn = 1, target;
    if((this->destination->orientation && this->destination->col == 0) || (!this->destination && this->destination->row == 0)) {
        sgn = -1;

        (* acc) -= 1;
    } else {
        (* acc) += this->target->length;
    }

    if(this->target->row == this->destination->row) {
        target = this->destination->col;
    } else {
        target = this->destination->row;
    }

    if(!this->resolve(* acc))
        return true;

    while((* acc) != target) {
        if(this->domain[row][col] > 0)
            return false;

        (* acc) += sgn;
    }

    if((* acc) == target) {
        if(this->domain[row][col] > 0)
            return false;
    }

    return true;
};

bool RushHour::resolve(Block * & block) {
    int row = block->row, col = block->col;
    int * acc = (block->orientation) ? & col : & row;

    for(unsigned int g = 0; g < block->length; g++) {
        if(!this->resolve(* acc) || this->domain[row][col]) {
            return false;
        }

        (* acc) += 1;
    }

    return true;
};

void RushHour::resolve(Move * & move, bool revert) {
    int row = move->block->row, col = move->block->col;
    int * acc = (move->block->orientation) ? & col : & row;
    unsigned int * cur = (move->block->orientation) ? & move->block->col : & move->block->row;

    int dir = !revert ? move->value : - move->value;

    if(dir == 1) {
        this->domain[row][col] = 0;

        (* acc) += move->block->length;

        this->domain[row][col] = move->block->value;
    } else {
        (* acc) -= 1;

        this->domain[row][col] = move->block->value;

        (* acc) += move->block->length;

        this->domain[row][col] = 0;
    }

    (* cur) += dir;
};

bool RushHour::resolve(const unsigned int & position) {
    return position >= 0 && position < RushHour::size;
};

bool RushHour::resolve(Block * & block, int & value) {
    int row = block->row, col = block->col;
    int * current = (block->orientation) ? & col : & row;

    if(value == 1) {
        (* current) += (block->length - 1);
    }

    (* current) += value;

    if(resolve(* current)) {
        return !this->domain[row][col];
    }

    return false;
};

void RushHour::forward(Move * move) {
    if(move->parent != NULL) {
        this->forward(move->parent);

        this->resolve(move, false);
    }
};

void RushHour::backward(Move * move) {
    if(move->parent != NULL) {
        this->resolve(move, true);

        this->backward(move->parent);
    }
};

int RushHour::solve() {
    Move root;

    queue<Move *> ariadne;
    ariadne.push(& root);

    while(!ariadne.empty()) {
        Move * move = ariadne.front();
        ariadne.pop();

        forward(move);

        if(this->encode(move) == NULL) {
            if(this->resolve()) {
                this->draw();

                return move->depth();
            }

            for(unsigned int g = 0; g < this->blocks.size(); g++) {
                for(int h = -1; h <= 1; h += 2) {
                    if(this->resolve(this->blocks[g], h)) {
                        Move * child = new Move(move, this->blocks[g], h);

                        move->moves.push_back(child);
                        ariadne.push(child);
                    }
                }
            }
        }

        backward(move);
    }

    return -1;
};

int RushHour::solve_backward(int depth) {
    Move root;

    vector<Move *> graph;

    queue<Move *> ariadne;
    ariadne.push(& root);

    bool resolved;
    while(!ariadne.empty()) {
        Move * move = ariadne.front();
        ariadne.pop();

        forward(move);

        Move * linkedMove = this->encode(move);
        resolved = this->resolve();

        if(!resolved) {
            move->resolved = true;

            if(linkedMove == NULL) {
                graph.push_back(move);
            }

            if(move->parent != NULL && move->parent->resolved) {
                Move::link(move, move->parent);
            }
        }

        if(linkedMove != NULL) {
            if(!resolved && linkedMove->resolved) {
                Move::link(move, linkedMove);
            }
        } else {
            for(unsigned int g = 0; g < this->blocks.size(); g++) {
                for(int h = -1; h <= 1; h += 2) {
                    if(this->resolve(this->blocks[g], h)) {
                        Move * child = new Move(move, this->blocks[g], h);
                        move->moves.push_back(child);

                        ariadne.push(child);
                    }
                }
            }
        }

        backward(move);
    }

    return -1;
};

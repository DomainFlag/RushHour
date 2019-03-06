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

void RushHour::create(string filepath, unsigned int row, unsigned int col, unsigned int length, bool orientation, int complexity) {
    bool resolved = false;

    while(!resolved) {
        Block * target = new Block(row, col, length, 2, orientation);
        Block * destination = new Block(row, col, orientation);

        RushHour rushHour(target, destination);
        rushHour.blocks.push_back(target);
        rushHour.insert(target);

        RushHour::sample(rushHour, 10);

        int current = rushHour.solve_backward();
        if(current > complexity) {
            resolved = true;

            cout << "Updated state:" << endl;
            rushHour.draw();

            cout << current << " steps needed to be solved" << endl;

            RushHour::saveToFile(rushHour, filepath);
        }
    }
};

void RushHour::saveToFile(RushHour & rushHour, string filepath) {
    fstream file(filepath, fstream::out | fstream::trunc);

    if(file.is_open()) {
        file << rushHour.destination->row << " " << rushHour.destination->col << " " << rushHour.destination->orientation << endl;

        for(unsigned int g = 0; g < rushHour.blocks.size(); g++) {
            Block * & block = rushHour.blocks[g];

            file << block->row << " " << block->col << " " << block->length << " " << block->orientation << endl;
        }

        file.close();
    }

    cout << "Successfully saved" << endl;
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

                return move->depth;
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

int RushHour::solve_backward() {
    Move root;

    vector<Move *> graph;

    queue<Move *> ariadne;
    ariadne.push(& root);

    while(!ariadne.empty()) {
        Move * move = ariadne.front();
        ariadne.pop();

        forward(move);

        Move * link = this->encode(move);
        move->resolved = this->resolve();

        if(!move->resolved) {
            graph.push_back(move);
        }

        if(link != NULL) {
            move->link = link;
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

    return RushHour::solve_graph(graph);
};

int RushHour::solve_graph(vector<Move *> & graph) {
    unordered_set<Move *> state;

    Move * gap = NULL;
    int mark = -1;

    bool red;

    cout << "Non-resolved states: " << graph.size() << endl;

    for(int g = 0; g < graph.size(); g++) {
        Move * & move = graph[g];
        if(state.find(move) != state.end()) {
            continue;
        }

        queue<Move *> moves;
        moves.push(move);

        red = true;
        while(!moves.empty() && red) {
            move = moves.front();
            moves.pop();

            state.insert(move);

            if(!move->resolved) {
                if(move->parent->resolved) {
                    move->mark = 1;
                }

                for(int g = 0; g < move->moves.size(); g++) {
                    Move * & child = move->moves[g];

                    if(child->resolved) {
                        red = false;
                    } else {
                        if(move->moves[g]->mark == -1 || (move->moves[g]->mark > (move->mark + 1))) {
                            move->moves[g]->mark = (move->mark + 1);
                        }

                        if(state.find(move->moves[g]) == state.end()) {
                            moves.push(move->moves[g]);
                        }
                    }
                }

                if(move->moves.empty()) {
                    red = false;
                }
            } else {
                red = false;
            }
        }
    }

    for(int g = 0; g < graph.size(); g++) {
        if(!graph[g]->resolved) {
            if(graph[g]->mark > mark) {
                mark = graph[g]->mark;

                gap = graph[g];
            }
        }
    }

    if(gap != NULL && gap->mark != -1) {
        this->forward(gap);

        return gap->mark;
    }

    return -1;
};

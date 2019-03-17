#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <queue>
#include <time.h>
#include <ncurses.h>

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
    if(this->destination != NULL) {
        delete this->destination;
    }

    for(Block * & block : this->blocks) {
        delete block;
    }

    this->clear();
};

void RushHour::clear() {
    unordered_set<Move *> state;

    this->clear(state, this->root);
};

void RushHour::clear(unordered_set<Move *> & state, Move * move) {
    if(move != NULL) {
        for(auto it = move->children.begin(); it != move->children.end(); it++) {
            if((* it) != NULL && state.find(* it) == state.end()) {
                state.insert(* it);

                delete (* it);

                this->clear(state, (* it));
            }
        }
    }
};

int RushHour::parseInt(string message, int min, int max) {
    cout << message;

    int value;
    cin >> value;

    while(value < min || value > max) {
        cout << "The min value is " << min << " and max is " << max << endl << message;

        cin >> value;
    }

    return value;
};

bool RushHour::parseBool(string message) {
    cout << message << endl;
    cout << "Press [y|n]: ";

    string value;
    cin >> value;

    while(true) {
        if(value.size() != 1) {
            cout << "Press a single key [y|n]: ";
        } else {
            if(value[0] == 'y' || value[0] == 'Y')
                return true;

            if(value[0] == 'n' || value[0] == 'N')
                return false;

            cout << "Press a valid key [y|n]: ";
        }
    }
};

void RushHour::sample(RushHour & rushHour, unsigned int count) {
    rushHour.blocks.reserve(count + 1);

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

void RushHour::create() {
    unsigned int row = RushHour::parseInt("Destination row: ", 0, RushHour::size);
    unsigned int col = RushHour::parseInt("Destination col: ", 0, RushHour::size);
    unsigned int length = RushHour::parseInt("Truck length: ", 2, 3);
    bool orientation = RushHour::parseBool("Truck is horizontal: ");
    int complexity = RushHour::parseInt("Desired min complexity board: ", 0, RushHour::complexity_max);

    RushHour::create(row, col, length, orientation, complexity);
};

void RushHour::create(unsigned int row, unsigned int col, unsigned int length, bool orientation, int complexity) {
    for(unsigned int g = 0; g < RushHour::trials; g++) {
        cout << "Trial #" << g << endl;

        Block * target = new Block(row, col, length, 2, orientation);
        Block * destination = new Block(row, col, orientation);

        RushHour rushHour(target, destination);
        rushHour.blocks.push_back(target);
        rushHour.insert(target);

        RushHour::sample(rushHour, 10);

        int current = rushHour.solve_backward();
        if(current > complexity) {
            cout << current << " steps needed to be solved" << endl;

            bool saveToFile = RushHour::parseBool("Save current board: ");
            if(saveToFile) {
                RushHour::saveToFile(rushHour);
            }

            return;
        }
    }
};

void RushHour::saveToFile(RushHour & rushHour) {
    string filepath;
    cout << "Enter board name: ";

    bool resolved = false;
    do {
        cin >> filepath;

        filepath = "./res/" + filepath;

        fstream file(filepath, fstream::out | fstream::trunc);

        if(file.is_open()) {
            file << rushHour.destination->row << " " << rushHour.destination->col << " " << rushHour.destination->orientation << endl;

            for(unsigned int g = 0; g < rushHour.blocks.size(); g++) {
                Block * & block = rushHour.blocks[g];

                file << block->row << " " << block->col << " " << block->length << " " << block->orientation << endl;
            }

            cout << "Successfully saved" << endl;

            file.close();

            resolved = true;
        } else {
            cout << "Please enter a valid name: " ;
        }
    } while(!resolved);
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

bool RushHour::resolve() {
    return this->target->isMe(this->destination->row, this->destination->col);
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

    int dir = revert ? - move->value : move->value;

    for(unsigned int g = 0; g < move->block->length; g++) {
        this->domain[row][col] = 0;

        (* acc)++;
    }

    (* cur) += dir;

    row = move->block->row, col = move->block->col;

    for(unsigned int g = 0; g < move->block->length; g++) {
        this->domain[row][col] = move->block->value;

        (* acc)++;
    }
};

bool RushHour::resolve(const unsigned int position) {
    return position >= 0 && position < RushHour::size;
};

bool RushHour::resolve(Block * & block, int value) {
    int row = block->row, col = block->col;
    int * current = (block->orientation) ? & col : & row;

    (* current) += value;

    for(unsigned int g = 0; g < block->length; g++) {
        if(!block->isMe(row, col)) {
            if(!resolve(* current) || this->domain[row][col] != 0)
                return false;
        }

        (* current)++;
    }

    return true;
};

void RushHour::forward(Move * move) {
    if(move->parent != NULL) {
        this->forward(move->parent);

        this->resolve(move, false);
    }
};

void RushHour::backward(Move * move, bool save) {
    if(move->parent != NULL) {
        this->resolve(move, true);

        this->backward(move->parent, save);

        if(save) {
            this->thread.push_back(move);
        }
    }
};

void RushHour::cycle(int value) {
    unsigned int i = this->index + value;

    if(i >= 0 && i < this->thread.size()) {
        if(value == -1) {
            this->resolve(this->thread[this->index - 1], true);
        } else {
            this->resolve(this->thread[this->index], false);
        }

        this->index = i;
    };
};

void RushHour::solve_forward() {
    this->clear();

    this->root = new Move();

    queue<Move *> ariadne;
    ariadne.push(this->root);

    while(!ariadne.empty()) {
        Move * move = ariadne.front();
        ariadne.pop();

        forward(move);

        if(this->encode(move) == NULL) {
            if(this->resolve()) {
                backward(move, true);

                return;
            }

            for(unsigned int g = 0; g < this->blocks.size(); g++) {
                for(int h = -1; h <= 1; h += 2) {
                    bool resolved = true;

                    for(unsigned int i = 1; i < this->size && resolved; i++) {
                        resolved = this->resolve(this->blocks[g], h * i);

                        if(resolved) {
                            Move * child = new Move(move, this->blocks[g], h * i);

                            move->children.insert(child);
                            ariadne.push(child);
                        }
                    }
                }
            }
        }

        backward(move, false);
    }
};

int RushHour::depth() {
    return thread.size();
};

int RushHour::solve_backward() {
    this->clear();

    this->root = new Move();

    unordered_set<Move *> graph;

    queue<Move *> ariadne;
    ariadne.push(this->root);

    while(!ariadne.empty()) {
        Move * move = ariadne.front();
        ariadne.pop();

        forward(move);

        Move * link = this->encode(move);
        move->resolved = this->resolve();

        if(link != NULL) {
            link->parents.insert(move->parent);

            move->parent->children.erase(move);
            move->parent->children.insert(link);
        } else {
            if(!move->resolved && graph.find(move) == graph.end()) {
                graph.insert(move);
            }

            for(unsigned int g = 0; g < this->blocks.size(); g++) {
                for(int h = -1; h <= 1; h += 2) {
                    bool resolved = true;

                    for(unsigned int i = 1; i < this->size && resolved; i++) {
                        resolved = this->resolve(this->blocks[g], h * i);

                        if(resolved) {
                            Move * child = new Move(move, this->blocks[g], h * i);

                            move->children.insert(child);
                            child->parents.insert(move);

                            ariadne.push(child);
                        }
                    }
                }
            }
        }

        backward(move, false);

        if(link != NULL) {
            delete move;
        }
    }

    return RushHour::solve_graph(graph);
};

int RushHour::solve_graph(unordered_set<Move *> & graph) {
    Move * move, * root;
    int mark, depth;

    for(auto it = graph.begin(); it != graph.end(); it++) {
        unordered_set<Move *> state;
        queue<Move *> moves;

        root = (* it);
        root->mark = -1;

        state.insert(root);

        root->depth = 0;
        moves.push(root);

        mark = 1000000, depth = 0;

        while(!moves.empty()) {
            move = moves.front();
            moves.pop();

            if(depth != move->depth) {
                depth = move->depth;
            }

            if(mark <= depth) {
                break;
            };

            if(move->resolved) {
                if(mark > move->depth) {
                    mark = move->depth;
                }

                break;
            } else {
                if(move->mark != -1) {
                    if(move->depth + move->mark < mark) {
                        mark = move->depth + move->mark;
                    }
                } else {
                    // case where there is nearby move where the state is resolved
                    for(auto it1 = move->parents.begin(); it1 != move->parents.end(); it1++) {
                        Move * item = (* it1);

                        if(state.find(item) == state.end()) {
                            item->depth = move->depth + 1;

                            moves.push(item);
                            state.insert(item);
                        }
                    }

                    for(auto it1 = move->children.begin(); it1 != move->children.end(); it1++) {
                        Move * item = (* it1);

                        if(state.find(item) == state.end()) {
                            item->depth = move->depth + 1;

                            moves.push(item);
                            state.insert(item);
                        }
                    }
                }
            }
        }

        if(mark != 1000000) {
            root->mark = mark;
        } else {
            root->mark = -1;
        }
    }

    mark = -1;

    Move * gap = NULL;
    for(Move * move : graph) {
        if(!move->resolved) {
            if(move->mark > mark) {
                mark = move->mark;

                gap = move;
            }
        }
    }

    cout << mark << endl;

    if(gap != NULL && gap->mark != -1) {
        this->forward(gap);

        return gap->mark;
    }

    return -1;
};

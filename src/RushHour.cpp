#include <iostream>
#include <string>
#include <unordered_set>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <queue>
#include <time.h>
#include <limits>
#include <chrono>
#include <thread>
#include <mutex>
#include <algorithm>

#include "./../include/Block.h"
#include "./../include/Move.h"
#include "./../include/Cluster.h"
#include "./../include/RushHour.h"
#include "./../include/Profiler.h"

using namespace std;

static const int MAX_INT = numeric_limits<int>::max();

RushHour::RushHour() {
    this->size = size_max;
};

RushHour::RushHour(unsigned int size) {
    if(size > size_max) {
        printf("Illegal size, exceeded max size: %d", size);

        exit(-1);
    }

    this->size = size;
};

RushHour::RushHour(string filepath) {
    fstream file("./../" + filepath);
    if(file.is_open()) {
        // Reading size of board
        unsigned int size;

        file >> size;

        this->size = size;

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

RushHour::RushHour(unsigned int size, Block * target, Block * destination) {
    this->size = size;
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
    this->clear(this->root);
};

void RushHour::clear(Move * root) {
    unordered_set<Move *> state;

    this->clear(state, root);
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

    if(value.size() != 1) {
        cout << "Press a single key [y|n]: ";
    } else {
        if(value[0] == 'y' || value[0] == 'Y')
            return true;

        if(value[0] == 'n' || value[0] == 'N')
            return false;

        cout << "Press a valid key [y|n]: ";
    }

    return RushHour::parseBool(message);
};

void RushHour::sample(RushHour & rushHour, unsigned int count) {
    rushHour.blocks.reserve(count + 1);

    unsigned int row, col, length;
    bool orientation, resolved;
    for(unsigned int g = 0; g < count; g++) {
        resolved = false;

        while(!resolved) {
            row = rand() % rushHour.size;
            col = rand() % rushHour.size;
            length = 2;
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
    unsigned int size = RushHour::parseInt("Board size: ", size_max - 1, size_max);
    unsigned int row = RushHour::parseInt("Destination row: ", 0, size);
    unsigned int col = RushHour::parseInt("Destination col: ", 0, size);
    unsigned int length = RushHour::parseInt("Truck length: ", 2, 3);
    bool orientation = RushHour::parseBool("Truck is horizontal: ");
    int complexity = RushHour::parseInt("Desired min complexity board: ", 0, RushHour::complexity_max);

    RushHour::create(size, row, col, length, orientation, complexity);
};

void RushHour::create(unsigned int size, unsigned int row, unsigned int col, unsigned int length, bool orientation, int complexity) {
    for(unsigned int g = 0; g < RushHour::trials; g++) {
        Block * target = new Block(row, col, length, 2, orientation);
        Block * destination = new Block(row, col, orientation);

        RushHour rushHour(size, target, destination);
        rushHour.blocks.push_back(target);
        rushHour.insert(target);

        RushHour::sample(rushHour, 7);

        unordered_set<Move *> graph;

        int current = rushHour.solve_cluster(graph)->mark;

        printf("Trial #%u with complexity: %d\n", g, current);
        fflush(stdout);

        if(current > complexity) {
            cout << current << " steps needed to be solved" << endl;

            bool saveToFile = RushHour::parseBool("Save current board: ");
            if(saveToFile) {
                RushHour::saveToFile(rushHour);
            }

            if(!RushHour::parseBool("Continue search: "))
                return;
        }
    }
};

void RushHour::saveToFile(RushHour & rushHour, Cluster & cluster) {
    for(unsigned int g = 0; g < rushHour.blocks.size(); g++) {
         * rushHour.blocks[g] = cluster.blocks[g];
    }

    saveToFile(rushHour);
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
            file << rushHour.size << endl;
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

void RushHour::print() {
    cout << endl;

    for(unsigned int g = 0; g < this->size; g++) {
        for(unsigned int h = 0; h < this->size; h++) {
            cout << this->domain[g][h];
        }

        cout << endl;
    }
};

void RushHour::createGraph(unsigned int size, unsigned int count) {
    // initialize rushHour instance
    RushHour rushHour(size);

    // reserve blocks memory
    rushHour.blocks.reserve(count + 1);

    // by default horizontal as all RushHour configurations are symmetrical to respect of target's orientation
    auto * target = new Block(rushHour.size / 2, rushHour.size - 2, 2, 2, 1);
    rushHour.target = target;
    rushHour.blocks.push_back(target);
    rushHour.insert(target);

    // by default
    rushHour.destination = new Block(rushHour.size / 2, rushHour.size - 1, 1);

    // initialize blocks
    for(unsigned int g = 0; g < count; g++) {
        auto * block = new Block(0, 0, 2, 1);

        rushHour.blocks.push_back(block);
    }

    // length 3 blocks
    int blocks_large_count = RushHour::parseInt("Number of blocks len 3: ", 0, count);

    // length 3 blocks vertical
    int blocks_large_vertical = RushHour::parseInt("Number of blocks len 3 vertical: ", 0, blocks_large_count);

    // length 2 blocks vertical
    int blocks_small_vertical = RushHour::parseInt("Number of blocks len 2 vertical: ", 0, count - blocks_large_count);

    // iterator
    unsigned int it = 1;
    while(it < rushHour.blocks.size()) {
        if(blocks_large_count > 0) {
            rushHour.blocks[it]->length = 3;

            blocks_large_count--;
        }

        if(blocks_large_vertical > 0 && rushHour.blocks[it]->length == 3) {
            rushHour.blocks[it]->orientation = 0;

            blocks_large_vertical--;
        }

        if(blocks_small_vertical > 0 && rushHour.blocks[it]->length == 2) {
            rushHour.blocks[it]->orientation = 0;

            blocks_small_vertical--;
        }

        it++;
    }

    // cluster container for freeing up all the clusters heads
    vector<Cluster *> clusters;

    // every unique available states
    unordered_set<string> state;

    // single pass for now
    for(unsigned int g = 0; g < rushHour.blocks.size(); g++) {
        if(rushHour.blocks[g] != rushHour.target) {
            rushHour.blocks[g]->orientation = 0;
        }

        Profiler profiler;
        profiler.start();

        rushHour.backtrackGraph(clusters, state);

        profiler.end();

        break;
    }

    printf("Clusters size: %lu\n", clusters.size());

    Profiler profiler;
    profiler.start();

    // retrieve the most dense cluster
    Cluster * cluster = rushHour.solve_parallel_clusters(clusters, 4);

    profiler.end();

    printf("For %dd with only 2 length blocks and all possible orientations - %d\n", rushHour.size, cluster->gap->mark);

    // save board
    bool saveToFile = RushHour::parseBool("Save current board: ");
    if(saveToFile) {
        // init rushHour with the chosen cluster
        rushHour.init(cluster);

        // forward the board from resolved position to starting position
        rushHour.forward(cluster->gap);

        RushHour::saveToFile(rushHour);
    }

    // free up memory
    for(Cluster * & cluster : clusters) {
        delete cluster;
    }
};

void RushHour::backtrackGraph(vector<Cluster *> & clusters, unordered_set<string> & state, unsigned int depth) {
    if(depth == this->blocks.size() - 1) {
        auto * cluster = new Cluster(this->target, this->destination, this->blocks);

        this->root = new Move();

        solve_backward(cluster->graph, &state);

        cluster->head = this->root;

        clusters.push_back(cluster);

        return;
    }

    for(auto & block : this->blocks) {
        if(block == this->target)
            continue;

        if(!block->flag) {
            for(unsigned int h = 0; h < this->size; h++) {
                for(unsigned int i = 0; i < this->size; i++) {
                    block->row = h, block->col = i;

                    if(block->orientation == 1 && block->row == target->row) {
                        continue;
                    }

                    if(this->resolve(block)) {
                        string encoding = this->encode();
                        if(state.find(encoding) == state.end()) {
                            state.insert(encoding);

                            // forward change
                            this->insert(block);
                            block->flag = true;

                            this->backtrackGraph(clusters, state, depth + 1);

                            // backtrack back to previous state
                            this->insert(block, true);
                            block->flag = false;
                        }
                    }
                }
            }
        }
    }
};

void RushHour::insert(Block * block, bool revert) {
    int row = block->row, col = block->col;
    int * acc = (block->orientation) ? & col : & row;

    int value = (revert) ? 0 : block->value;

    for(unsigned int g = 0; g < block->length; g++) {
        this->domain[row][col] = value;

        (* acc)++;
    }
};

string RushHour::encode() {
    stringstream ss;

    map<int, Block *> ordered_blocks;
    for(auto & block : this->blocks) {
        ordered_blocks.insert({block->row * this->size + block->col, block});
    }

    for(auto & block : ordered_blocks) {
        ss << block.second->row << block.second->col << block.second->length << block.second->orientation << block.second->value;
    }

    return ss.str();
};

Move * RushHour::record(Move * move, string & encoding) {
    auto it = this->state.find(encoding);
    if(it == this->state.end()) {
        this->state.insert({encoding, move});
    } else {
        return it->second;
    }

    return nullptr;
};

void RushHour::zeros() {
    for(unsigned int g = 0; g < this->size; g++) {
        for(unsigned int h = 0; h < this->size; h++) {
            this->domain[g][h] = 0;
        }
    }
};

void RushHour::init() {
    this->zeros();

    for(auto & block : this->blocks) {
         insert(block);
    }
};

void RushHour::init(Cluster * & cluster) {
    this->zeros();

    for(unsigned int g = 0; g < cluster->blocks.size(); g++) {
         * this->blocks[g] = cluster->blocks[g];

         insert(& cluster->blocks[g]);
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
    return position >= 0 && position < this->size;
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
    if(move->parent != nullptr) {
        this->resolve(move, true);

        this->backward(move->parent, save);

        if(save) {
            this->path.push_back(move);
        }
    }
};

void RushHour::cycle(int value) {
    unsigned int i = this->index + value;

    if(i >= 0 && i < this->path.size()) {
        if(value == -1) {
            this->resolve(this->path[this->index - 1], true);
        } else {
            this->resolve(this->path[this->index], false);
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

        string encoding = this->encode();
        if(this->record(move, encoding) == nullptr) {
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
    return path.size();
};

Move * RushHour::solve_cluster(unordered_set<Move *> & graph, Move * root) {
    if(root == nullptr) {
        this->clear();

        this->root = new Move();
    }

    solve_backward(graph);

    this->root->mark = RushHour::solve_graph(graph)->mark;

    return this->root;
};

void RushHour::solve_backward(unordered_set<Move *> & graph, unordered_set<string> * state) {
    queue<Move *> ariadne;
    ariadne.push(this->root);

    while(!ariadne.empty()) {
        Move * move = ariadne.front();
        ariadne.pop();

        forward(move);

        string encoding = this->encode();
        if(state != nullptr && state->find(encoding) != state->end()) {
            state->insert(encoding);
        }

        Move * link = this->record(move, encoding);
        move->resolved = this->resolve();

        bool beenThere = link != nullptr && link->parent != move->parent;

        if(beenThere) {
            link->parents.insert(move->parent);

            if(move->parent != nullptr) {
                move->parent->children.erase(move);
                move->parent->children.insert(link);
            } else {
                // nothing to do the root is minimal
                return;
            }
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

        if(beenThere) {
            delete move;
        }
    }
};

Cluster * RushHour::solve_clusters(vector<Cluster *> & clusters) {
    Cluster * prov = nullptr;
    int distance = 0;

    for(Cluster * & cluster : clusters) {
        this->init(cluster);

        cluster->gap = solve_graph(cluster->graph);
        if(cluster->gap != nullptr && cluster->gap->mark > distance) {
            prov = cluster;

            distance = cluster->gap->mark;
        }
    }

    return prov;
};

Cluster * RushHour::solve_parallel_clusters(vector<Cluster *> & clusters, int threadCount) {
    Cluster * prov = nullptr;
    int distance = 0;

    vector<RushHour> boards(threadCount, RushHour(size));
    vector<thread> threads;

    mutex locker;
    int counter = 0;
    for(int g = 0; g < threadCount; g++) {
        thread th(RushHour::solve_parallel_cluster, ref(boards.at(g)), ref(clusters), ref(locker), ref(counter));

        threads.push_back(move(th));
    }

    for(thread & th : threads) {
        th.join();
    }

    for(Cluster * & cluster : clusters) {
        if(cluster->gap != nullptr && cluster->gap->mark > distance) {
            prov = cluster;

            distance = cluster->gap->mark;
        }
    }

    return prov;
};

void RushHour::solve_parallel_cluster(RushHour & rushHour, vector<Cluster *> & clusters, mutex & locker, int & counter) {
    bool flag = true;
    while(flag) {
        locker.lock();
        int index = counter++;
        locker.unlock();

        if(index < clusters.size()) {
            Cluster * cluster = clusters.at(index);
            rushHour.init(cluster);

            cluster->gap = rushHour.solve_graph(cluster->graph);
        } else {
            flag = false;
        }
    }
}

Move * RushHour::solve_graph(unordered_set<Move *> & graph) {
    Move * move, * root;
    int mark, depth;

    for(auto it : graph) {
        unordered_set<Move *> state;
        queue<Move *> moves;

        root = it;
        root->mark = -1;

        state.insert(root);

        root->depth = 0;
        moves.push(root);

        mark = MAX_INT, depth = 0;
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

        if(mark != MAX_INT) {
            root->mark = mark;
        } else {
            root->mark = -1;
        }
    }

    mark = -1;

    Move * gap = nullptr;
    for(Move * move : graph) {
        if(!move->resolved) {
            if(move->mark > mark) {
                mark = move->mark;

                gap = move;
            }
        }
    }

    if(gap != nullptr && gap->mark != -1) {
        this->forward(gap);

        return gap;
    }

    return nullptr;
};
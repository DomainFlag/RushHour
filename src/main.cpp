#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <fstream>
#include <sstream>
using namespace std;

class Block {
public:
    int row;
    int col;
    int length;
    int value = 0;

    // true = horizontal; false = vertical
    bool orientation;

    Block(int row, int col, int length, bool orientation, int value) : row(row), col(col), length(length), orientation(orientation), value(value) {};
    Block(int row, int col, bool orientation) : row(row), col(col), orientation(orientation) {};
    ~Block() {}

    void print() {
        cout << "row: " << this->row << "; col: " << this->col << " & len: " << this->length << endl;
    }
};

class Move {
public:
    vector<Move *> moves;
    Move * parent = NULL;
    Block * block;
    int value;

    Move() {}
    Move(Block * block, int value) : block(block), value(value) {}
    ~Move() {
        for(int g = 0; g < moves.size(); g++) {
            if(moves[g] != NULL) {
                delete moves[g];
            }
        }
    }
};

class RushHour {
public:
    static int const size = 6;

    vector<Block *> blocks;
    unordered_set<string> state;
    unordered_set<Block *> jam;

    Block * target;
    Block * destination;
    int domain[RushHour::size][RushHour::size] = {0};

    RushHour(vector<Block *> & blocks, Block * target, Block * destination) {
        this->blocks = blocks;
        this->target = target;
        this->destination = destination;

        this->init();
    }

    void insert(Block * & block, int & row, int & col, int * & acc) {
        row = block->row, col = block->col;
        acc = (block->orientation) ? & col : & row;

        for(int g = 0; g < block->length; g++) {
            this->domain[row][col] = block->value;

            (* acc)++;
        }
    }

    bool encode() {
        stringstream ss;

        for(int g = 0; g < this->blocks.size(); g++) {
            Block * & block = this->blocks[g];

            ss << block->row << block->col << g;
        }

        string state = ss.str();

        if(this->state.find(state) == this->state.end()) {
            this->state.insert(state);
        } else {
            // Go back

            return false;
        }

        return true;
    }

    // void generate(int count, ) {
    //
    // }

    void init() {
        int row, col;
        int * acc;

        for(Block * & block : this->blocks) {
            insert(block, row, col, acc);
        }
    }

    void draw() {
        stringstream ss;

        for(int g = 0; g < this->size + 2; g++) {
            ss << "-";
        }

        ss << endl;

        for(int g = 0; g < this->size; g++) {
            ss << "|";

            for(int h = 0; h < this->size; h++) {
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

        for(int g = 0; g < this->size + 2; g++) {
            ss << "-";
        }

        ss << endl;

        cout << ss.str() << endl;
    }

    bool resolve() {
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
    }

    void resolve(Move * & move, bool revert = false) {
        int row = move->block->row, col = move->block->col;
        int * acc = (move->block->orientation) ? & col : & row;
        int * cur = (move->block->orientation) ? & move->block->col : & move->block->row;

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
    }

    bool resolve(const int & position) {
        return position >= 0 && position < RushHour::size;
    }

    bool resolve(Block * & block, int & value) {
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
    }
};

void forward(RushHour & rh, Move * move) {
    if(move->parent != NULL) {
        forward(rh, move->parent);

        rh.resolve(move, false);
    }
}

void backward(RushHour & rh, Move * move) {
    if(move->parent != NULL) {
        rh.resolve(move, true);

        backward(rh, move->parent);
    }
}

int count(Move * move) {
    if(move->parent == NULL)
        return 0;

    return 1 + count(move->parent);
}

int solve(RushHour & rh) {
    Move * root = new Move;
    queue<Move *> ariadne;

    for(int g = 0; g < rh.blocks.size(); g++) {
        for(int h = -1; h <= 1; h += 2) {
            if(rh.resolve(rh.blocks[g], h)) {
                Move * move = new Move(rh.blocks[g], h);
                move->parent = root;

                root->moves.push_back(move);
                ariadne.push(move);
            }
        }
    }

    while(!ariadne.empty()) {
        Move * move = ariadne.front();
        forward(rh, move);

        ariadne.pop();

        if(rh.encode()) {
            if(rh.resolve()) {
                rh.draw();

                int min = count(move);

                delete root;

                return min;
            }

            for(int g = 0; g < rh.blocks.size(); g++) {
                for(int h = -1; h <= 1; h += 2) {
                    if(rh.resolve(rh.blocks[g], h)) {
                        Move * child = new Move(rh.blocks[g], h);
                        child->parent = move;

                        move->moves.push_back(child);
                        ariadne.push(child);
                    }
                }
            }
        }

        backward(rh, move);
    }

    delete root;

    return 0;
}

int main() {
    fstream file("./res/puzzle.txt");

    // Reading destination
    int row, col, length, orientation;
    file >> row >> col >> orientation;

    Block * destination = new Block(row, col, orientation);

    vector<Block *> blocks;

    // Reading target
    file >> row >> col >> length >> orientation;
    Block * target = new Block(row, col, length, orientation, 2);

    blocks.push_back(target);

    // Reading blocks
    while(file >> row >> col >> length >> orientation) {
        Block * block = new Block(row, col, length, orientation, 1);

        blocks.push_back(block);
    }

    file.close();

    RushHour rh(blocks, target, destination);
    rh.draw();

    int min = solve(rh);

    cout << "Shortest path is - "  << min << endl;

    delete destination;
    for(Block * & block : blocks) {
        delete block;
    }

    return 0;
}

#ifndef RushHour_h
#define RushHour_h

#include <string>
#include <vector>
#include <unordered_map>

#include "./../include/Block.h"
#include "./../include/Move.h"

using namespace std;

class RushHour {
public:
    static unsigned int const size = 6;

    vector<Block *> blocks;
    unordered_map<string, Move *> state;

    Block * target = NULL;
    Block * destination = NULL;

    int domain[RushHour::size][RushHour::size] = {0};

    RushHour();

    RushHour(string filepath);

    RushHour(Block * target, Block * destination);

    ~RushHour();

    static void sample(RushHour & rushHour, unsigned int count);

    static void create(string filepath, unsigned int row, unsigned int col, unsigned int length, bool orientation, int complexity = -1);

    static void saveToFile(RushHour & rushHour, string filepath);

    void insert(Block * block);

    bool validate(Block * block);

    vector<Block *> sampleTargets();

    vector<Block *> sampleDestinations();

    Move * encode(Move * move);

    void init();

    void draw();

    bool resolve();

    bool resolve(Block * & block);

    void resolve(Move * & move, bool revert = false);

    bool resolve(const unsigned int & position);

    bool resolve(Block * & block, int & value);

    void forward(Move * move);

    void backward(Move * move);

    int solve();

    int solve_backward();

    int solve_graph(vector<Move *> & graph);
};

#endif

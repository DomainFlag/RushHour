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
    static unsigned int const trials = 20;
    static unsigned int const complexity_max = 100;

    vector<Block *> blocks;
    unordered_map<string, Move *> state;

    Move * root = NULL;
    int index = 0;
    vector<Move *> thread;

    Block * target = NULL;
    Block * destination = NULL;

    int domain[RushHour::size][RushHour::size] = {0};

    RushHour();

    RushHour(string filepath);

    RushHour(Block * target, Block * destination);

    ~RushHour();

    static int parseInt(string message, int min, int max);

    static bool parseBool(string message);

    static void sample(RushHour & rushHour, unsigned int count);

    static void create(unsigned int row, unsigned int col, unsigned int length, bool orientation, int complexity = -1);

    static void create();

    static void saveToFile(RushHour & rushHour);

    void clear();

    void clear(unordered_set<Move *> & state, Move * move);

    void insert(Block * block);

    bool validate(Block * block);

    vector<Block *> sampleTargets();

    vector<Block *> sampleDestinations();

    Move * encode(Move * move);

    void init();

    static void square(Block & block, char ** drawing);

    bool resolve();

    bool resolve(Block * & block);

    void resolve(Move * & move, bool revert = false);

    bool resolve(const unsigned int position);

    bool resolve(Block * & block, int value);

    void forward(Move * move);

    void backward(Move * move, bool save);

    void cycle(int value);

    void solve_forward();

    int depth();

    int solve_backward();

    int solve_graph(unordered_set<Move *> & graph);
};

#endif

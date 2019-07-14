#ifndef RushHour_h
#define RushHour_h

#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <unordered_set>

#include "./../include/Block.h"
#include "./../include/Move.h"
#include "./../include/Cluster.h"

using namespace std;

class RushHour {
public:
    static unsigned int const size_max = 6;
    static unsigned int const trials = 20;
    static unsigned int const complexity_max = 100;

    vector<Block *> blocks;
    unordered_map<string, Move *> state;

    Move * root = NULL;

    int index = 0;
    unsigned int size = 6;

    vector<Move *> path;

    Block * target = NULL;
    Block * destination = NULL;

    int domain[size_max][size_max] = {0};

    RushHour();

    RushHour(unsigned int size);

    RushHour(string filepath);

    RushHour(unsigned int size, Block * target, Block * destination);

    ~RushHour();

    // parse an integer within [min, max] range
    static int parseInt(string message, int min, int max);

    // parse a bool
    static bool parseBool(string message);

    // sample a random board
    static void sample(RushHour & rushHour, unsigned int count);

    // create a random board based on user's input
    static void create();

    // create a random board
    static void create(unsigned int size, unsigned int row, unsigned int col, unsigned int length, bool orientation, int complexity = -1);

    // save board to local storage
    static void saveToFile(RushHour & rushHour);

    // save board based on cluster to local storage
    static void saveToFile(RushHour & rushHour, Cluster & cluster);

    // create all clusters given board dimension
    static void createGraph(unsigned int size, unsigned int count);

    // print board in a rudimentary way(stdout)
    void print();

    // free up memory
    void clear();

    // free up memory
    void clear(Move * move);

    // free up memory
    void clear(unordered_set<Move *> & state, Move * move);

    // insert a block into domain(board), with ability to revert insertion
    void insert(Block * block, bool revert = false);

    // encode the current board to an identifier
    string encode();

    // record the current board to not eat your own tail
    Move * record(Move * move, string & encoding);

    // make board empty
    void zeros();

    // initialize board
    void init();

    // initialize board based on cluster
    void init(Cluster * & cluster);

    // check if the board is resolved, winnable position
    bool resolve();

    // check if block/car can be placed into the board
    bool resolve(Block * & block);

    // given direction and distance move block or revert its movement into the board
    void resolve(Move * & move, bool revert = false);

    // resolve position index if valid based on board constraints
    bool resolve(const unsigned int position);

    // resolve block position based on position with value offset
    bool resolve(Block * & block, int value);

    // forward a move to initial state
    void forward(Move * move);

    // backward a move from its initial state
    void backward(Move * move, bool save);

    // cycle a connected move(path) based on value
    void cycle(int value);

    // get path size, distance to a resolved position
    int depth();

    // solve forward a graph by breadth first search
    void solve_forward();

    // retrieve all unique states and create their graph
    void backtrackGraph(vector<Cluster *> & clusters, unordered_set<string> & state, unsigned int depth = 0);

    // solve a single cluster
    Move * solve_cluster(unordered_set<Move *> & graph, Move * move = NULL);

    // solve clusters by returning the one with maximal distance to resolved state
    Cluster * solve_clusters(vector<Cluster *> & clusters);

    // solve clusters in parallel by returning the one with maximal distance to resolved state
    Cluster * solve_parallel_clusters(vector<Cluster *> & clusters, int threadCount = 1);

    // solve a clusters in parallel by returning the cluster with the distance to resolved state
    static void solve_parallel_cluster(RushHour & rushHour, vector<Cluster *> & clusters, mutex & locker, int & counter);

    // solve backward and construct a full graph given a starting position
    void solve_backward(unordered_set<Move *> & graph, unordered_set<string> * state = nullptr);

    // solve a graph by breadth first search and marking to get maximal distance within a graph
    Move * solve_graph(unordered_set<Move *> & graph);
};

#endif

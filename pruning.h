#include "domino.h"

//adjust this if there are problems
#define HASH_TABLE_SIZE 10000000ULL

namespace Pruning{
    extern int nodes;
    extern int collisions;

    typedef struct {
        int proximity;
        U64 key;
    } HashEntry;

    const int PRUNING_DEPTH = 8;
    extern HashEntry hashTable[HASH_TABLE_SIZE];
    //call this after calling init_hash()
    void init_pruning();
    
    //internal functions dont worry abt these
    extern int ply;
    void search(int depth, Domino* domino);

    //returns how far the domino is away from being solved
    //only works if it is relatively close to being solved (within "PRUNING_DEPTH" amount of moves)
    //otherwise returns -1
    int proximity_to_solved(Domino* dom);
}
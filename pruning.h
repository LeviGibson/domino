#pragma once
#include "domino.h"

//The size of the hash table
//adjust this if there are problems
#define HASH_TABLE_SIZE (200000000ULL)
//500,000,000

//Only do he mod operation with this value, not HASH_TABLE_SIZE
//add a bit of padding at the end of the array
//because otherwise me might accidently store / search for values in indecies that are larger than the array
//learned that the hard way
#define HASH_DIVISOR (HASH_TABLE_SIZE-10)

namespace Pruning{
    extern int nodes;
    extern int collisions;

    #pragma pack (1)

    typedef struct {
        int8_t proximity;
        int8_t proximityNoExtraQt;
        U64 key;
    } HashEntry;

    #pragma pack (0)


    const int PRUNING_DEPTH = 11;
    // extern HashEntry hashTable[HASH_TABLE_SIZE];
    //call this after calling init_hash()
    void init_pruning();
    void free_pruning();
    
    //internal functions dont worry abt these
    extern int ply;
    void search(int depth, Domino* domino);

    //returns how far the domino is away from being solved
    //only works if it is relatively close to being solved (within "PRUNING_DEPTH" amount of moves)
    //otherwise returns -1
    int proximity_to_solved(Domino* dom, int noExtraQt = 0);
}
#include "pruning.h"
#include <cstring>
#include <fstream>
#include <cassert>

Pruning::HashEntry Pruning::hashTable[HASH_TABLE_SIZE];
int Pruning::ply;
int Pruning::nodes;
int Pruning::collisions;

//recursion function that fills the hashTable
//called by generate_table()
void Pruning::search(int depth, Domino* domino){
    nodes++;
    U64 dominoHash = domino->domino_hash();
    HashEntry* entry = &hashTable[dominoHash % HASH_TABLE_SIZE];

    // I think this is what chaining is
    // Or something of the sort
    // getting rid of collisions
    if (entry->proximity != -1 && dominoHash != entry->key){
        int numCollisions = 1;
        while (1){
            collisions++;
            entry = &hashTable[(dominoHash % HASH_TABLE_SIZE) + numCollisions];
            if (entry->proximity != -1 && dominoHash != entry->key){
                numCollisions++;
                // printf("Everything just broke wheee try again plz\n");
            } else {
                break;
            }
        }
    }

    entry->proximity = ply;
    entry->key = dominoHash;
    
    if (depth == 0)
        return;

    for (int move = 0; move < 7; move++) {
        if (domino->is_repetition(move))
            continue;

        domino->make_move(move);
        ply++;
        search(depth-1, domino);
        ply--;
        domino->undo_move();
    }
    
}

int Pruning::proximity_to_solved(Domino *dom) {
    U64 key = dom->domino_hash();
    HashEntry* entry = &hashTable[key % HASH_TABLE_SIZE];

    if (entry->proximity == -1)
        return -1;

    if (key == entry->key)
        return entry->proximity;
    else {
        int numCollisions = 1;
        while (1){
            entry = &hashTable[(key % HASH_TABLE_SIZE)+numCollisions];

            if (entry->proximity == -1)
                return -1;

            if (entry->key == key){
                return entry->proximity;
            } else {
                numCollisions++;
            }
        }
    }
        
}

void reset_pruning_table(){
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Pruning::hashTable[i].proximity = -1;
        Pruning::hashTable[i].key = -1ULL;
    }
}

void generate_table(){
    reset_pruning_table();
    Pruning::ply = 0;
    Pruning::nodes = 0;
    Domino dom = Domino();
    Pruning::search(Pruning::PRUNING_DEPTH, &dom);
    printf("Generated pruning table with %d nodes and %d collisions (%f%% capacity)\n", Pruning::nodes, Pruning::collisions, (float)Pruning::nodes*100 / (float)HASH_TABLE_SIZE);
}

void save_table(){
    std::fstream file;
    file.open("pruningTable.bin", std::ios_base::out|std::ios_base::binary);
    if(!file.is_open()) {
        std::cout<<"Unable to open the file\n";
        assert(0);
    }

    file.write((const char*)Pruning::hashTable, sizeof(Pruning::hashTable));
    file.close();
}

void load_table(){
    FILE *fin = fopen("pruningTable.bin", "rb");
    int _tmp = fread(Pruning::hashTable, sizeof(Pruning::HashEntry), HASH_TABLE_SIZE, fin);
    fclose(fin);
}

void Pruning::init_pruning(){
    if (binary_file_exists("pruningTable.bin")){
        load_table();
    } else {
        generate_table();
        save_table();
    }
}

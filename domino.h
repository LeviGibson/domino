#pragma once

#include <iostream>
#include <vector>

#define CORNER_INDEX_COUNT 40320

#define U64 unsigned long long

//ordinal values of each move (for make_move function)
//this program does not do D moves because they're dumb
//you can solve a domino optimally with just these moves, and D moves only widen the search tree
const int U = 0;
const int UP = 1;
const int U2 = 2;
const int R2 = 3;
const int F2 = 4;
const int B2 = 5;
const int L2 = 6;

void init_hash();

//yes or no, does this binary file exist?
//no particular reason why it's in the Domino file, it's just convenient and used a couple of times in the program
int binary_file_exists(std::string path);

extern std::string MOVECHARS[7];

class Domino{
    public:
        Domino();

        //returns 1 if domino is solved
        //returns 0 otherwise
        //includes states like U D'
        int is_domino_solved();

        //returns weather or not corners are solved
        //includes states like U D'
        int are_corners_solved();

        //are corners solved relative to HTR?
        //includes states like U D'
        int are_corners_solved_htr();

        void parse_alg(std::string alg);
        void set_random_state();
        //reset to solved state
        void reset();
        
        //deletes cube history before a search
        void clear_history();

        //prints a graphical depiction of the domino
        void print_domino();

        //prints all the pieces, in integer form
        void print_pieces();

        //prints all the moves that have been done to the domino
        void print_moves();

        //do a move from a move ordinal
        void make_move(int move);

        //low-level function
        //each corner case has an index, and this returns the index of the corner case
        //there are 40320 corner cases
        int corner_index();

        //low-level function
        //set the corners to a state, with the same indexing system used in the corner_index function
        void set_corners_from_index(int index);

        //undos the last move
        //can stack, so you can call it multiple times in a row
        void undo_move();
        //is a move stupid (like R2 R2 for example)
        int is_repetition(int move);

        //is a repetition when solving corners
        //no R2 L2
        //no more than 3 half turns in a row
        int is_repetition_corners(int move);

        //How many quarter turns does it take to solve the corners
        int qt_count();

        //Generates a 64-bit hash key for the domino
        //THIS GENERATES THE SAME HASH FOR MIRRORS
        U64 domino_hash();

        std::vector<int> history;

        //the state of the domino
        int corners[8];
        int edges[8];

    private:
        //very low-level dont use these
        //they don't update the history
        //use make_move() instead
        void move_U();
        void move_UP();
        void move_U2();
        void move_R2();
        void move_L2();
        void move_B2();
        void move_F2();

        int savedCornerIndex;

        

        // very low level, cycles four corners/edges to make a move
        void corner_cycle(int c1, int c2, int c3, int c4);
        void edge_cycle(int e1, int e2, int e3, int e4);
        //swaps two corners/edges (to make R2, L2, F2, B2 and U2 work)
        void edge_swap(int e1, int e2);
        void corner_swap(int c1, int c2);

};



#pragma once

#include <iostream>
#include <vector>

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
        std::vector<int> history;

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

        //the state of the domino
        int corners[8];
        int edges[8];

        // very low level, cycles four corners/edges to make a move
        void corner_cycle(int c1, int c2, int c3, int c4);
        void edge_cycle(int e1, int e2, int e3, int e4);
        //swaps two corners/edges (to make R2, L2, F2, B2 and U2 work)
        void edge_swap(int e1, int e2);
        void corner_swap(int c1, int c2);

};



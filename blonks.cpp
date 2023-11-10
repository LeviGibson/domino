#include "blonks.h"
#include <cassert>

int blonkPly = 0;
#define UNSOLVED -1

int blonkSearch(int depth, int index, Domino* dom){
    if (dom->is_2x2x1_solved(index)){
        return blonkPly;
    }

    if (depth <= 0)
        return UNSOLVED;

    for (int move = 0; move < 7; move++) {
        if (dom->is_repetition(move))
            continue;
        
        blonkPly++;
        dom->make_move(move);

        int res = blonkSearch(depth-1, index, dom);

        blonkPly--;
        dom->undo_move();

        if (res != UNSOLVED)
            return res;
    }
    
    return UNSOLVED;
}

int Blonks::solution_length_2x2x1(Domino *domino, int index) {
    Domino dom = *domino;
    assert(index >= 0 && index <= 7);

    for (int i = 0; i < 20; i++) {
        blonkPly = 0;
        dom.clear_history();

        int res = blonkSearch(i, index, &dom);
        if (res != UNSOLVED)
            return res;
    }
    
    printf("ERROR COULD NOT FIND 2x2x2 WITH INDEX %d\n", index);
    return -1;
}

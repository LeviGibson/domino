#include "htr.h"

int htrPly = 0;
#define UNSOLVED -1

int blonkSearch(int depth, Domino* dom){
    if (dom->get_htr_subset() == 0){
        return htrPly;
    }

    if (depth <= 0)
        return UNSOLVED;

    for (int move = 0; move < 7; move++) {
        if (dom->is_repetition(move))
            continue;
        
        htrPly++;
        dom->make_move(move);

        int res = blonkSearch(depth-1, dom);

        htrPly--;
        dom->undo_move();

        if (res != UNSOLVED)
            return res;
    }
    
    return UNSOLVED;
}

int HTR::htr_length(Domino *domino, int inverse) {
    Domino dom = *domino;
    if (inverse)
        dom.invert();

    for (int i = 0; i < 20; i++) {
        htrPly = 0;
        dom.clear_history();

        int res = blonkSearch(i, &dom);
        if (res != UNSOLVED)
            return res;
    }
    
    printf("ERROR COULD NOT FIND DOM WITH INDEX\n");
    return -1;
}

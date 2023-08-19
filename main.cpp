#include "domino.h"
#include "search.h"
#include "corners.h"
#include "pruning.h"
#include <iostream>

const int SS1[8] = {0, 1, 2, 3, 4, 5, 6, 7};
const int SS2[8] = {3, 0, 1, 2, 5, 6, 7, 4};
const int SS3[8] = {2, 3, 0, 1, 6, 7, 4, 5};
const int SS4[8] = {1, 2, 3, 0, 7, 4, 5, 6};

int startTime;

int main(){
    srand ( time(NULL) );
    Corners::init_corners();
    init_hash();
    Pruning::init_pruning();

    Domino dom = Domino();
    for (int i = 0; i < 1000; i++) {
        dom.set_random_state();
        Search::find_optimal(dom);
    }
};

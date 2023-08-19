#include "domino.h"
#include "search.h"
#include "corners.h"
#include "pruning.h"
#include <iostream>
#include <cstring>

const int SS1[8] = {0, 1, 2, 3, 4, 5, 6, 7};
const int SS2[8] = {3, 0, 1, 2, 5, 6, 7, 4};
const int SS3[8] = {2, 3, 0, 1, 6, 7, 4, 5};
const int SS4[8] = {1, 2, 3, 0, 7, 4, 5, 6};

int startTime;

int main(){
    // srand ( time(NULL) );
    Corners::init_corners();
    init_hash();
    Pruning::init_pruning();

    Domino dom = Domino();
    dom.parse_alg("B2 U' R2 U' L2 F2 U L2 U' F2 U2 L2");

    Search::find_optimal(dom, 1, 1);
};

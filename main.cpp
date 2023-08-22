#include "domino.h"
#include "search.h"
#include "corners.h"
#include "pruning.h"
#include <iostream>
#include <cstring>

int main(){
    srand ( time(NULL) );
    Corners::init_corners();
    init_hash();
    Pruning::init_pruning();

    Domino dom = Domino();
    for (int i = 0; i < 10000; i++)
    {
        dom.set_random_state();
        if (dom.qt_count() == 0) {
            Search::find_optimal(dom, 1, 1);
            std::cout << dom.corner_index() <<std::endl; 
        }
    }
    
};

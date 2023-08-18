#include "domino.h"
#include "search.h"
#include "corners.h"
#include <iostream>

int main(){
    srand((unsigned) time(NULL));
    Corners::init_corners();
    init_hash();

    Domino dom = Domino();
    dom.set_random_state();
    std::cout << dom.qt_count() << std::endl;
    Search::find_optimal(dom);
};

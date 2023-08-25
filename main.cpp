#include "domino.h"
#include "search.h"
#include "corners.h"
#include "pruning.h"
#include "htrSubsetStats.h"
#include "features.h"
#include <iostream>
#include <cstring>



int main(){
    
    // srand ( time(NULL) );
    Corners::init_corners();
    init_hash();
    Pruning::init_pruning();

    htrSubsetStats::calculate_stats(50000, 0);
    
    // Features::generate_features(1000000);
    // htrSubsetStats::calculate_stats(100000, 1);
    // Domino dom = Domino();
    // int tmp[8] = {1, 6, 7, 0, 2, 3, 4, 5};
    // memcpy(dom.corners, tmp, sizeof(tmp));
    // int tmp2[8] = {3, 6, 1, 0, 2, 5, 4, 7};
    // memcpy(dom.edges, tmp2, sizeof(tmp));

    // int prox = Pruning::proximity_to_solved(&dom);
    // std::cout << prox << std::endl;

    // dom.print_domino();
    // Search::find_optimal(dom);

};

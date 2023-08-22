#include "domino.h"
#include "search.h"
#include "corners.h"
#include "pruning.h"
#include "htrSubsetStats.h"
#include <iostream>
#include <cstring>



int main(){
    
    // srand ( time(NULL) );
    Corners::init_corners();
    init_hash();
    Pruning::init_pruning();

    htrSubsetStats::calculate_stats(10000);
    
};

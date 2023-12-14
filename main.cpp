#include "domino.h"
#include "search.h"
#include "corners.h"
#include "pruning.h"
#include "blonks.h"
#include "htr.h"
#include "htrSubsetStats.h"
#include "scramblePrinter.h"
#include "features.h"
#include "allcases.h"
#include <iostream>
#include <cstring>

int main(){

    // printf("%s\n", argv[1]);
    // printf("%d\n", );
    // exit(0);
    
    Corners::init_corners();
    init_hash();
    Pruning::init_pruning();

    Cases::generate();

    // for (int i = 0; i < 100000; i++) {
    //     dom.set_random_state();
    //     if (dom.get_htr_subset() != 35)
    //         continue;

    //     Search::find_optimal(dom);
    //     if (Search::solution.qt_count() == 1){
    //         Search::solution.print();
    //     }
    // }
    

    // for (int i = 0; i < 104; i++) {
    //     if (Features::VALID_HTR_SUBSETS[i]){
    //         Features::generate_features(5000, i, dom.htr_subset_name(i) + ".csv");
    //     }
    // }

    // dom.set_random_state();
    // Search::find_optimal(dom, 1);

    // int bl = HTR::htr_length(&dom);
    // printf("%d\n", bl);

    // Domino dom = Domino();
    // dom.parse_alg("R2 U R2");
    // dom.calculate_block_score();

    // for (int i = 0; i < 8; i++) {
    //     printf("%d ", dom.block_score[i]);
    // }
    // printf("\n");

    // dom.invert();
    // dom.print_domino();

    // dom.calculate_block_score();
    // for (int i = 0; i < 8; i++) {
    //     printf("%d ", dom.block_score[i]);
    // }
    // printf("\n");

    // Domino dom = Domino();
    // for (int i = 0; i < 1000; i++) {
    //     dom.set_random_state();
    //     dom.calculate_block_score();
        // for (int i = 0; i < 8; i++) {
        //     // if (i == 4)
        //     //     printf("\n");
        //     printf("%d ", dom.block_score[i]);
        // }
        // printf("\n");

        // dom.invert();


        // dom.calculate_block_score();
        // for (int i = 0; i < 8; i++) {
        //     // if (i == 4)
        //     //     printf("\n");
        //     printf("%d ", dom.block_score[i]);
        // }
        // printf("\n");
        
        // dom.print_domino();
    //     Search::find_optimal(dom, 1);
    //     printf("\n");
    // }
    

    // htrSubsetStats::calculate_subset_probabilities(1000000);
    // Domino dom = Domino();
    // for (size_t i = 0; i < 10000; i++) {
    //     dom.set_random_state();
    //     Search::find_optimal(dom);
    //     printf("%d\n", i);
    // }

    // Features::generate_features(1000000);
    
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

    Pruning::free_pruning();

};

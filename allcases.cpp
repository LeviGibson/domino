#include "allcases.h"
#include <fstream>
#include <iostream>
#include <thread>

#define BATCH_SIZE 100000
#define CORE_COUNT 4
#define TOTAL_BATCH_SIZE (BATCH_SIZE/CORE_COUNT)

Algorithm solutions[CORE_COUNT][BATCH_SIZE];


void Cases::generate(){
    Domino dom = Domino();
    // Domino dom = Domino();
    // Features::generate_features(100000, 23);

    // for (int i = 0; i < 1000; i++)
    // {
    //     dom.set_random_state();
    //     if (dom.get_htr_subset() == 23)
    //         printf("%u\n", dom.domino_index());
    // }
    // dom.set_domino_from_index(12376893);
    // printf("%d\n", dom.get_htr_subset());

    int num = 0;
    //1,625,702,399
    // for (unsigned int i = 0; i < 1625702399; i++) {

    std::ofstream myfile;
    myfile.open("example.csv");

    // for (unsigned int i = (startingValue*1000000); i < startingValue+1000000; i++) {
    for (unsigned int i = 0; i < 1625702399; i++) {
    // for (unsigned int i = 0; i < 1000000; i++) {
        dom.set_domino_from_index(i);

        if (dom.get_htr_subset() == 23){
            num++;
            
            int opt = Search::find_optimal(dom, 0);
            myfile << opt << "," << i << "," << dom.htr_subset_name(dom.get_htr_subset()) << "," << Search::solution.to_string() << std::endl;

            if (num % 1000 == 0)
            printf("%d\t%d\t%f%\n", i, num, (float)i/16257023.99);
        
        }
    }
    
    printf("%d\n", num);
}

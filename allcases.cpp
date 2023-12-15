#include "allcases.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <thread>

#define BATCH_SIZE 100000
#define CORE_COUNT 8
#define TOTAL_BATCH_SIZE (BATCH_SIZE*CORE_COUNT)

std::ofstream files[CORE_COUNT];
int progress[CORE_COUNT];
std::thread threads[CORE_COUNT];

//threaded function


void open_files(){
    memset(progress, 0, sizeof(progress));
    for (int i = 0; i < CORE_COUNT; i++) {
        files[i].open(std::to_string(i) + ".csv");
    }
}

void close_files(){
    for (int i = 0; i < CORE_COUNT; i++) {
        files[i].close();
    }
}

void generate_subset(int coreIndex){
    Domino dom = Domino();
    Search search = Search();

    for (unsigned int i = 0; i < 1625702399; i++) {
        if (i % CORE_COUNT == coreIndex){
            dom.set_domino_from_index(i);

            int opt = search.find_optimal(dom, 0);
            files[coreIndex] << opt << "," << i << "," << dom.htr_subset_name(dom.get_htr_subset()) << "," << search.solution.to_string() << std::endl;
            
            progress[coreIndex]++;
        }

        if (coreIndex == 0 && i % 100000 == 0){
            for (int j = 0; j < CORE_COUNT; j++)
                printf("thread %d\t%f%\n", j, ((float)progress[j]/16257023.99)*CORE_COUNT);

            unsigned int total = 0;
            for (int j = 0; j < CORE_COUNT; j++) {
                total += progress[j];
            }
            printf("Total: %u", total);

            printf("\n\n");
        }
    }

}

//generate all cases
void Cases::generate(){
    open_files();

    unsigned int finalValue = 1625702399-(1625702399%TOTAL_BATCH_SIZE);
    // printf("%u\n", finalValue);

    for (int i = 0; i < CORE_COUNT; i++) {
        threads[i] = std::thread(generate_subset, i);
    }

    for (int i = 0; i < CORE_COUNT; i++) {
        threads[i].join();
    }

    close_files();
}

// void Cases::generate(){
//     Domino dom = Domino();
//     // Domino dom = Domino();
//     // Features::generate_features(100000, 23);

//     // for (int i = 0; i < 1000; i++)
//     // {
//     //     dom.set_random_state();
//     //     if (dom.get_htr_subset() == 23)
//     //         printf("%u\n", dom.domino_index());
//     // }
//     // dom.set_domino_from_index(12376893);
//     // printf("%d\n", dom.get_htr_subset());

//     int num = 0;
//     //1,625,702,399
//     // for (unsigned int i = 0; i < 1625702399; i++) {

//     std::ofstream myfile;
//     myfile.open("example.csv");
//     Search search = Search();

//     // for (unsigned int i = (startingValue*1000000); i < startingValue+1000000; i++) {
//     for (unsigned int i = 0; i < 1625702399; i++) {
//     // for (unsigned int i = 0; i < 1000000; i++) {
//         dom.set_domino_from_index(i);

//         if (dom.get_htr_subset() == 23){
//             num++;
            
//             int opt = search.find_optimal(dom, 0);
//             myfile << opt << "," << i << "," << dom.htr_subset_name(dom.get_htr_subset()) << "," << search.solution.to_string() << std::endl;

//             if (num % 1000 == 0)
//             printf("%d\t%d\t%f%\n", i, num, (float)i/16257023.99);
        
//         }
//     }
    
//     printf("%d\n", num);
// }

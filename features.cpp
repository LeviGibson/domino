#include "features.h"
#include "search.h"
#include "htr.h"
#include "blonks.h"
#include <fstream>
#include <cstring>
#include <cassert>

int Features::VALID_HTR_SUBSETS[104] = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 
    0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1};


void write_feature(Features::Row* row, std::ofstream* file){
    *file << row->label << ",";

    row->domino.calculate_block_score();
    
    // *file << HTR::htr_length(&row->domino, 0) << ",";
    // *file << HTR::htr_length(&row->domino, 1) << ",";
    

    // int htrSubset = row->domino.get_htr_subset();
    // for (size_t i = 0; i < 104; i++) {
    //     if (!Features::VALID_HTR_SUBSETS[i])
    //         continue;
    //     *file << (i == htrSubset) << ",";
    // }

    row->domino.get_misoriented();
    for (size_t j = 0; j < 16; j++) {
        *file << row->domino.misoriented_normal[j] << ",";
    }

    for (size_t j = 0; j < 16; j++) {
        *file << row->domino.misoriented_normal[j] << ",";
    }

    row->domino.calculate_block_score();
    for (size_t j = 0; j < 16; j++) {
        *file << row->domino.block_score[j] << ",";
    }
    
    // for (int i = 0; i < 8; i++){
    //     *file << Blonks::solution_length_2x2x1(&row->domino, i, 0) << ",";
    // }
    // for (int i = 0; i < 8; i++){
    //     *file << Blonks::solution_length_2x2x1(&row->domino, i, 1) << ",";
    // }

    // for (size_t i = 0; i < 8; i++) {
    //     for (size_t j = 0; j < 8; j++) {
    //         *file << row->edges[i][j] << ",";
    //     }
    // }

    // for (size_t i = 0; i < 8; i++) {
    //     for (size_t j = 0; j < 8; j++) {
    //         *file << row->icorners[i][j] << ",";
    //     }
    // }

    // for (size_t i = 0; i < 8; i++) {
    //     for (size_t j = 0; j < 8; j++) {
    //         *file << row->iedges[i][j] << ",";
    //     }
    // }

    *file << "\n";
    
}

void write_header(std::ofstream* file){
    Domino d = Domino();

    *file << "label,";
    
    for (int i = 0; i < 16; i++) {
        *file << "nMis" << i << ",";
    }
    for (int i = 0; i < 16; i++) {
        *file << "iMis" << i << ",";
    }
    for (int i = 0; i < 16; i++) {
        *file << "nblonk" << i << ",";
    }

    // for (int i = 0; i < 8; i++) {
    //     *file << "n2x2x1" << i << ",";
    // }
    // for (int i = 0; i < 8; i++) {
    //     *file << "i2x2x1" << i << ",";
    // }
    

    // for (int i = 0; i < 8; i++) {
    //     for (int j = 0; j < 8; j++) {
    //         *file << "c" << j << "->" << i << ",";
    //     }
    // }

    // for (int i = 0; i < 8; i++) {
    //     for (int j = 0; j < 8; j++) {
    //         *file << "e" << j << "->" << i << ",";
    //     }
    // }

    // for (int i = 0; i < 8; i++) {
    //     for (int j = 0; j < 8; j++) {
    //         *file << "ic" << j << "->" << i << ",";
    //     }
    // }

    // for (int i = 0; i < 8; i++) {
    //     for (int j = 0; j < 8; j++) {
    //         *file << "ie" << j << "->" << i << ",";
    //     }
    // }

    // for (int i = 0; i < 104; i++) {
    //     if (!Features::VALID_HTR_SUBSETS[i])
    //         continue;
    //     *file << d.htr_subset_name(i) << ",";
    // }
    *file << "\n";
    
}

void Features::generate_features(int numRows, int subset, std::string name) {
    std::ofstream myfile;
    myfile.open(name);

    Row dataset = Row();

    write_header(&myfile);

    if (subset != -1)
        assert(VALID_HTR_SUBSETS[subset]);

    for (int i = 0; i < numRows; i++) {
        printf("%d\n", i);
        dataset.domino = Domino();
        dataset.domino.set_random_state();

        //get spesific subset case
        if (subset != -1){
            while (dataset.domino.get_htr_subset() != subset){
                dataset.domino.set_random_state();
            }
        }

        dataset.htrSubset = dataset.domino.get_htr_subset();
        dataset.label = Search::find_optimal(dataset.domino, 0, 1);

        memset(dataset.edges, 0, sizeof(dataset.edges));
        memset(dataset.corners, 0, sizeof(dataset.corners));
        memset(dataset.iedges, 0, sizeof(dataset.iedges));
        memset(dataset.icorners, 0, sizeof(dataset.icorners));

        for (int i = 0; i < 8; i++) {
            dataset.edges[i][dataset.domino.edges[i]] = 1;
            dataset.corners[i][dataset.domino.corners[i]] = 1;
        }
        // dataset.domino.invert();
        // for (int i = 0; i < 8; i++) {
        //     dataset.iedges[i][dataset.domino.edges[i]] = 1;
        //     dataset.icorners[i][dataset.domino.corners[i]] = 1;
        // }
        // dataset.domino.invert();
        
        write_feature(&dataset, &myfile);
    }

    myfile.close();
}

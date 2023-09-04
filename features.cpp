#include "features.h"
#include "search.h"
#include <fstream>
#include <cstring>

int VALID_HTR_SUBSETS[104] = {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 
    0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1};


void write_feature(Features::Row* row, std::ofstream* file){
    *file << row->label << ",";

    row->domino.calculate_block_score();
    for (int i = 0; i < 8; i++) {
        *file << row->domino.block_score[i] << ",";
    }
    

    int htrSubset = row->domino.get_htr_subset();
    for (size_t i = 0; i < 104; i++) {
        if (!VALID_HTR_SUBSETS[i])
            continue;
        *file << (i == htrSubset) << ",";
    }

    // for (size_t i = 0; i < 8; i++) {
    //     for (size_t j = 0; j < 8; j++) {
    //         *file << row->corners[i][j] << ",";
    //     }
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
    
    for (int i = 0; i < 8; i++) {
        *file << "pairtype" << i << ",";
    }
    

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

    for (int i = 0; i < 104; i++) {
        if (!VALID_HTR_SUBSETS[i])
            continue;
        *file << d.htr_subset_name(i) << ",";
    }
    *file << "\n";
    
}

void Features::generate_features(int numRows) {
    std::ofstream myfile;
    myfile.open("dataset.csv");

    Row dataset = Row();

    write_header(&myfile);

    for (int i = 0; i < numRows; i++) {
        printf("%d\n", i);
        dataset.domino = Domino();
        dataset.domino.set_random_state();

        //get 1qt 4c4e cases
        // while (dataset.domino.get_htr_subset() != 23){
        //     dataset.domino.set_random_state();
        // }

        dataset.htrSubset = dataset.domino.get_htr_subset();
        dataset.label = Search::find_optimal(dataset.domino, 0, 0);

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

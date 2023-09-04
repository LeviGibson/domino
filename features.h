#pragma once
#include "domino.h"
#include "corners.h"
#include "search.h"

namespace Features{
    typedef struct {
        Domino domino;
        int htrSubset;
        int corners[8][8];
        int edges[8][8];
        int icorners[8][8];
        int iedges[8][8];
        int label;
    } Row;

    extern int VALID_HTR_SUBSETS[104];

    void generate_features(int numRows, int subset = -1, std::string name = "dataset.csv");
}

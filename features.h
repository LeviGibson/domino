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
        int label;
    } Row;

    void generate_features(int numRows);
}

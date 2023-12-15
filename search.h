#pragma once
#include "domino.h"

class Search{
public:
    Search();
    int find_optimal(Domino domino, int print = 0, int noExtraQts = 0);
    Algorithm solution;
private:
    int search(int depth, int extended, Domino* domino);
    int ply = 0;
    int NO_EXTRA_QUARTER_TURNS = 0;
};

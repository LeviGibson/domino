#pragma once
#include "domino.h"

namespace htrSubsetStats{
    void calculate_stats(int numSamples, int noExtraQt = 0);
    void calculate_subset_probabilities(int numSamples);
}

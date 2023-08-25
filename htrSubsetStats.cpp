#include "htrSubsetStats.h"
#include "search.h"
#include <cstring>

float lengths[104];
float samples[104];

void htrSubsetStats::calculate_stats(int numSamples, int noExtraQt) {
    memset(lengths, 0, sizeof(lengths));
    memset(samples, 0, sizeof(lengths));

    Domino dom = Domino();
    for (int i = 0; i < numSamples; i++) {
        dom.set_random_state();
        int subset = dom.get_htr_subset();
        if (samples[subset] / i > .03)
        {
            // printf("rejected\n");
            i--;
            continue;
        }

        int isSolvableWithoutExtraQts = dom.qt_count() > 1;
        
        int optimal = Search::find_optimal(dom, 0, std::min(noExtraQt, isSolvableWithoutExtraQts));
        lengths[subset] += (float)optimal;
        samples[subset]++;
        printf("%d\n", i);
    }

    for (int i = 0; i < 104; i++) {
        if (samples[i] == 0)
            continue;
        printf("%s\t%f\t(%d)\n", dom.htr_subset_name(i).c_str(), lengths[i]/samples[i], (int)samples[i]);
    }
}

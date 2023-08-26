#include "htrSubsetStats.h"
#include "search.h"
#include <cstring>

float lengths[104];
float samples[104];
float qturns[104];
// float hasExtraQts[104];

void htrSubsetStats::calculate_stats(int numSamples, int noExtraQt) {
    memset(lengths, 0, sizeof(lengths));
    memset(samples, 0, sizeof(lengths));

    Domino dom = Domino();
    for (int i = 0; i < numSamples; i++) {
        dom.set_random_state();
        int subset = dom.get_htr_subset();
        if (samples[subset] / i > .03)
        {
            i--;
            continue;
        }

        int isSolvableWithoutExtraQts = dom.qt_count() > 1 || dom.get_htr_subset() == 23 || dom.get_htr_subset() == 0;
        
        int optimal = Search::find_optimal(dom, 1, std::min(noExtraQt, isSolvableWithoutExtraQts));
        // int noEQToptimal = 10000;
        // if (isSolvableWithoutExtraQts){
        //     noEQToptimal = Search::find_optimal(dom, 0, 1);
        // }
        
        qturns[subset] += Search::solution.qt_count();
        lengths[subset] += (float)optimal;
        samples[subset]++;

        // if (optimal < noEQToptimal){
        //     hasExtraQts[subset]++;
        // } 

        printf("%d\n", i);
    }

    for (int i = 0; i < 104; i++) {
        if (samples[i] == 0)
            continue;
        printf("%s\t%f\t%f\t%d\n", dom.htr_subset_name(i).c_str(), lengths[i]/samples[i], qturns[i] / samples[i], (int)samples[i]);
    }
}

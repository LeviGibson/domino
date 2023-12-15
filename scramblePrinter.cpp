#include "scramblePrinter.h"
#include "domino.h"
#include "search.h"

void ScramblePrinter::print_scrambles(int numSamples) {
    Domino dom = Domino();
    Search search = Search();

    for (int i = 0; i < numSamples; i++) {
        dom.set_random_state();
        search.find_optimal(dom, 1, 1);
    }
}

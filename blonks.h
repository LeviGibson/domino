#pragma once
#include "domino.h"

namespace Blonks{
    //gets the optional solution length for solving a 2x2x1 around corner "index"
    int solution_length_2x2x1(Domino *domino, int index, int invert=0);
}
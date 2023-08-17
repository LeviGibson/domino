#include "domino.h"
#include "search.h"
#include "corners.h"
#include <iostream>



// "U2 R2 U' R2 U F2 U' L2 U L2 U' B2 D2 B2"
int main(){
    srand((unsigned) time(NULL));
    Corners::init_corners();

    // std::string setup = "U2 R2 U' R2 U F2 U' L2 U L2 U' B2 U2 F2";

    Domino d = Domino();
    
};

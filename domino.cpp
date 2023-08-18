#include <cstdio>
#include <iostream>
#include <cstring>
#include <sstream>
#include <cassert>
#include <stdlib.h>
#include "domino.h"
#include "corners.h"

// the position of each corner in the "corners" array
// follows the standard bld scheme (i think idk or something close)
#define ULB 0
#define URB 1
#define URF 2
#define ULF 3
#define DLF 4
#define DRF 5
#define DRB 6
#define DLB 7

// the position of each edge in the "edges" array
// follows the standard bld scheme (i think idk or something close)
#define UB 0
#define UR 1
#define UF 2
#define UL 3
#define DF 4
#define DR 5
#define DB 6
#define DL 7

std::string MOVECHARS[7] = {"U", "U'", "U2", "R2", "F2", "B2", "L2"};

const int DEFAULT_CORNERS[8] = {0, 1, 2, 3, 4, 5, 6, 7};
const int DEFAULT_EDGES[8] = {0, 1, 2, 3, 4, 5, 6, 7};


//The 4 solved states without E slice (Solved, Solved+ U D', solved + U2 D2, solved + U' D)
const int SOLVED_STATE_1[8] = {0, 1, 2, 3, 4, 5, 6, 7};
const int SOLVED_STATE_2[8] = {3, 0, 1, 2, 5, 6, 7, 4};
const int SOLVED_STATE_3[8] = {2, 3, 0, 1, 6, 7, 4, 5};
const int SOLVED_STATE_4[8] = {1, 2, 3, 0, 7, 4, 5, 6};

Domino::Domino(){
    //set domino to default solved state
    reset();
};

void Domino::reset(){
    memcpy(edges, DEFAULT_EDGES, sizeof(edges));
    memcpy(corners, DEFAULT_CORNERS, sizeof(corners));
    clear_history();

    //changes are being made to the cube, so the corner index is no longer valid
    savedCornerIndex = -1;
}

//returns 1 if domino is solved
//returns 0 otherwise
//checks for the 4 solved states without E slice (Solved, Solved+ U D', solved + U2 D2, solved + U' D)
int Domino::is_domino_solved(){
    if (memcmp(corners, SOLVED_STATE_1, sizeof(corners)) == 0){
        if (memcmp(edges, SOLVED_STATE_1, sizeof(edges)) == 0){
            return 1;
        };
    };

    if (memcmp(corners, SOLVED_STATE_2, sizeof(corners)) == 0){
        if (memcmp(edges, SOLVED_STATE_2, sizeof(edges)) == 0){
            return 1;
        };
    };

    if (memcmp(corners, SOLVED_STATE_3, sizeof(corners)) == 0){
        if (memcmp(edges, SOLVED_STATE_3, sizeof(edges)) == 0){
            return 1;
        };
    };

    if (memcmp(corners, SOLVED_STATE_4, sizeof(corners)) == 0){
        if (memcmp(edges, SOLVED_STATE_4, sizeof(edges)) == 0){
            return 1;
        };
    };

    return 0;
};

int Domino::are_corners_solved(){

    //slow version
    
    /*
    if (memcmp(corners, SOLVED_STATE_1, sizeof(corners)) == 0)
        return 1;
    if (memcmp(corners, SOLVED_STATE_2, sizeof(corners)) == 0)
        return 1;
    if (memcmp(corners, SOLVED_STATE_3, sizeof(corners)) == 0)
        return 1;
    if (memcmp(corners, SOLVED_STATE_4, sizeof(corners)) == 0)
        return 1;
    return 0;
    */
    
    //speedier and simpler version
    int index = corner_index();

    //these are the indecies of the 4 solved states (U D', U2 D2, U' D)
    if (index == 0 || index == 5898 || index == 11536 || index == 15129)
        return 1;
    return 0;
}

int Domino::qt_count(){
    return Corners::quarter_turn_counts[corner_index()];
}

//every single state that has corners solved relative to HTR
//includes states that are off by U D'
int CORNER_SOLVED_STATES_HTR[192] = {0,16,288,304,576,592,1565,1574,1783,1799,1998,2009,3109,3117,3245,3254,3419,3420,4527,4531,
    4737,4746,4925,4934,5167,5183,5455,5471,5743,5759,5889,5898,6199,6215,6414,6425,7525,7533,7569,7578,
    7835,7836,8943,8947,9153,9162,9249,9258,10805,10814,11137,11158,11352,11368,11520,11536,11808,11824,12096,12112,
    13071,13075,13373,13382,13469,13478,14485,14493,14787,14804,15053,15062,15129,15138,15553,15574,15768,15784,16687,16703,
    16975,16991,17263,17279,17487,17491,17697,17706,17885,17894,18901,18909,19203,19220,19377,19386,20933,20942,21099,21116,
    21410,21418,22425,22434,22613,22622,22828,22832,23040,23056,23328,23344,23616,23632,24535,24551,24745,24766,25181,25190,
    25257,25266,25515,25532,25826,25834,26841,26850,26937,26946,27244,27248,28207,28223,28495,28511,28783,28799,28951,28967,
    29161,29182,29505,29514,31061,31070,31157,31166,31372,31376,32483,32484,32741,32750,32786,32794,33894,33905,34104,34120,
    34421,34430,34560,34576,34848,34864,35136,35152,35385,35394,35573,35582,35788,35792,36899,36900,37065,37074,37202,37210,
    38310,38321,38520,38536,38745,38754,39727,39743,40015,40031,40303,40319};


//This is the search algorithm that generated the array above
//i searched 
/*
        int csearch(int depth, Domino* d){
            if (d->are_corners_solved())
                return 1;
            if (depth == 0)
                return 0;
            
            for (int i = U2; i <= L2; i++){
                d->make_move(i);
                int res = csearch(depth-1, d);
                d->undo_move();

                if (res)
                    return res;
            }

            return 0;
        }
*/

int binarySearch(int* arr, int l, int r, int x)
{
    while (l <= r) {
        int m = l + (r - l) / 2;
 
        // Check if x is present at mid
        if (arr[m] == x)
            return m;
 
        // If x greater, ignore left half
        if (arr[m] < x)
            l = m + 1;
 
        // If x is smaller, ignore right half
        else
            r = m - 1;
    }
 
    // If we reach here, then element was not present
    return -1;
}

//this can be sped up with a binary search
//but im lazy so no
int Domino::are_corners_solved_htr(){
    int index = corner_index();
    //perform a binary search for the corner index in the CORNER_SOLVED_STATES_HTR array
    int result = binarySearch(CORNER_SOLVED_STATES_HTR, 0, 191, index);
    if (result == -1)
        return 0;
    else
        return 1;
}

U64 corner_hash_keys[8][8];
U64 edge_hash_keys[8][8];

void init_hash(){
    if (binary_file_exists("hashKeys.bin")){
        // read_binary_file();
    } else {
        // generate_corner_tables();
        // save_corner_table();
    }
}

U64 Domino::domino_hash(){
    
}

int binary_file_exists(std::string path){
    FILE *codefile;
   if(codefile=fopen(path.c_str(),"r")) {
       fclose(codefile);
       return 1;
   } else {
       return 0;
   }

   return 0;
}

//i am very proud of this
//please read it and be WOWED
//creates a unique index for each possible corner case, and it's the most compact it can be!
//kinda based off this https://stackoverflow.com/questions/7918806/finding-n-th-permutation-without-computing-others

//turns the list of corners into a path, where the width of the tree diminishes by 1 each time you go down a node
//multiplies each step taken in the tree by the number of leaf nodes in the subtree, and then you have a unique index! look at that!
int Domino::corner_index(){
    if (savedCornerIndex != -1){
        return savedCornerIndex;
    }

    int path[8] = {-1, -1, -1, -1, -1, -1, -1};
    int indecies[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    for (int c = 0; c < 8; c++){
        path[c] = indecies[corners[c]];
        for (int i = corners[c]; i < 8; i++){
            indecies[i]--;
        }
    }

    int ret = 5040*path[0] + 720*path[1] + 120*path[2] + 24*path[3] + 6*path[4] + 2*path[5] + 1*path[6];
    savedCornerIndex = ret;

    return ret;
}

//low-level function
//set the corners to a state, with the same indexing system used in the corner_index function
void Domino::set_corners_from_index(int index){
    clear_history();

    //make sure index is within range
    assert(index >= 0);
    assert(index < 40320);

    //generate the path from the index
    //see Domino::corner_index()
    int path[8] = {-1, -1, -1, -1, -1, -1, 0};
    int treesizes[8] = {5040, 720, 120, 24, 6, 2, 1, 0};
    for (int i = 0; i < 7; i++){
        int node = index / treesizes[i];
        path[i] = node;
        index -= node * treesizes[i];
    }

    int indecies[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    for (int i = 0; i < 8; i++){
        corners[i] = indecies[path[i]];
        for (int j = path[i]; j < 8; j++){
            indecies[j] = indecies[j+1];
        }
    }
}

//Random const arrays needed by the print_domino function
const char UD_COLORS[8] = {'w', 'w', 'w', 'w', 'y', 'y', 'y', 'y'};
// the first index is weather or not the corner is in HTR
const char CORNER_RL_COLORS[2][8] = {{'b', 'b', 'g', 'g', 'g', 'g', 'b', 'b'},
                                    {'o', 'r', 'r', 'o', 'o', 'r', 'r', 'o'}
                                    };
// the first index is weather or not the corner is in HTR
const char CORNER_FB_COLORS[2][8] = {{'o', 'r', 'r', 'o', 'o', 'r', 'r', 'o'},
                                    {'b', 'b', 'g', 'g', 'g', 'g', 'b', 'b'}
                                    };

const char EDGE_SIDE_COLORS[8] = {'b', 'r', 'g', 'o', 'g', 'r', 'b', 'o'};

//random function needed by print_domino. Used in no other places.
int is_corner_in_htr(int placement, int corner){
    if (placement % 2 == corner % 2)
        return 1;
    return 0;
};

//prints a graphical depiction of the domino
//please excuse the horror
//i promise it works and you'll never need to touch it
void Domino::print_domino(){
    std::cout << " " << CORNER_FB_COLORS[is_corner_in_htr(0, corners[0])][corners[0]] << EDGE_SIDE_COLORS[edges[0]] << CORNER_FB_COLORS[is_corner_in_htr(1, corners[1])][corners[1]] << std::endl;

    std::cout << CORNER_RL_COLORS[is_corner_in_htr(0, corners[0])][corners[0]] << UD_COLORS[corners[0]] << UD_COLORS[edges[0]] << UD_COLORS[corners[1]] << CORNER_RL_COLORS[is_corner_in_htr(1, corners[1])][corners[1]] << std::endl;
    std::cout << EDGE_SIDE_COLORS[edges[3]] << UD_COLORS[edges[3]] << 'w' << UD_COLORS[edges[1]] << EDGE_SIDE_COLORS[edges[1]] << std::endl;
    std::cout << CORNER_RL_COLORS[is_corner_in_htr(3, corners[3])][corners[3]] << UD_COLORS[corners[3]] << UD_COLORS[edges[2]] << UD_COLORS[corners[2]] << CORNER_RL_COLORS[is_corner_in_htr(2, corners[2])][corners[2]] << std::endl;
    
    std::cout << " " << CORNER_FB_COLORS[is_corner_in_htr(3, corners[3])][corners[3]] << EDGE_SIDE_COLORS[edges[2]] << CORNER_FB_COLORS[is_corner_in_htr(2, corners[2])][corners[2]] << std::endl;
    std::cout << " " << CORNER_FB_COLORS[is_corner_in_htr(4, corners[4])][corners[4]] << EDGE_SIDE_COLORS[edges[4]] << CORNER_FB_COLORS[is_corner_in_htr(5, corners[5])][corners[5]] << std::endl;

    std::cout << CORNER_RL_COLORS[is_corner_in_htr(4, corners[4])][corners[4]] << UD_COLORS[corners[4]] << UD_COLORS[edges[4]] << UD_COLORS[corners[5]] << CORNER_RL_COLORS[is_corner_in_htr(5, corners[5])][corners[5]] << std::endl;
    std::cout << EDGE_SIDE_COLORS[edges[7]] << UD_COLORS[edges[7]] << 'y' << UD_COLORS[edges[5]] << EDGE_SIDE_COLORS[edges[5]] << std::endl;
    std::cout << CORNER_RL_COLORS[is_corner_in_htr(7, corners[7])][corners[7]] << UD_COLORS[corners[7]] << UD_COLORS[edges[6]] << UD_COLORS[corners[6]] << CORNER_RL_COLORS[is_corner_in_htr(6, corners[6])][corners[6]] << std::endl;

    std::cout << " " << CORNER_FB_COLORS[is_corner_in_htr(7, corners[7])][corners[7]] << EDGE_SIDE_COLORS[edges[6]] << CORNER_FB_COLORS[is_corner_in_htr(6, corners[6])][corners[6]] << std::endl;
};

void Domino::move_U(){
    corner_cycle(ULB, URB, URF, ULF);
    edge_cycle(UB, UR, UF, UL);
};

void Domino::move_UP(){
    corner_cycle(ULF, URF, URB, ULB);
    edge_cycle(UL, UF, UR, UB);
};

void Domino::move_U2(){
    move_U();
    move_U();
};

void Domino::move_R2(){
    corner_swap(URB, DRF);
    corner_swap(URF, DRB);
    edge_swap(UR, DR);
};

void Domino::move_L2(){
    corner_swap(ULB, DLF);
    corner_swap(ULF, DLB);
    edge_swap(UL, DL);
};

void Domino::move_F2(){
    corner_swap(ULF, DRF);
    corner_swap(URF, DLF);
    edge_swap(UF, DF);
}

void Domino::move_B2(){
    corner_swap(ULB, DRB);
    corner_swap(URB, DLB);
    edge_swap(UB, DB);
};

void Domino::make_move(int move){
    //changes are being made to the cube, so the corner index is no longer valid
    savedCornerIndex = -1;

    if (move == U)
        move_U();
    else if (move == UP)
        move_UP();
    else if (move == U2)
        move_U2();
    else if (move == R2)
        move_R2();
    else if (move == F2)
        move_F2();
    else if (move == L2)
        move_L2();
    else if (move == B2)
        move_B2();
    else
        std::cout << "Invalid move in make_move function " << move << std::endl;

    history.push_back(move);
}

void Domino::undo_move(){
    //changes are being made to the cube, so the corner index is no longer valid
    savedCornerIndex = -1;

    assert(history.size() > 0);
    int move = history[history.size()-1];

    if (move == U)
        move_UP();
    else if (move == UP)
        move_U();
    else if (move == U2)
        move_U2();
    else if (move == R2)
        move_R2();
    else if (move == F2)
        move_F2();
    else if (move == L2)
        move_L2();
    else if (move == B2)
        move_B2();
    else
        std::cout << "Invalid move in undo_move function " << move << std::endl;

    history.pop_back();
}

void Domino::clear_history(){
    history.clear();
    //changes are being made to the cube, so the corner index is no longer valid
    savedCornerIndex = -1;
}

int Domino::is_repetition(int move){
    if (history.size() == 0)
        return 0;

    int lastmove = history[history.size()-1];
    int lastlastmove = history[history.size()-2];

    //if it's a move repetition, it's a repetition
    if (move == lastmove)
        return 1;

    //U, U', and U2 all cancel with each other
    if (move == U || move == UP || move == U2){
        if (lastmove == U || lastmove == UP || lastmove == U2)
            return 1;
    }

    //Avoid R2 L2 R2, etc
    if (move == R2 && lastmove == L2 && lastlastmove == R2){
        return 1;
    }
    if (move == L2 && lastmove == R2 && lastlastmove == L2){
        return 1;
    }
    if (move == F2 && lastmove == B2 && lastlastmove == F2){
        return 1;
    }
    if (move == B2 && lastmove == F2 && lastlastmove == B2){
        return 1;
    }

    return 0;
}

int is_move_qt(int move){
    if (move == U || move == UP)
        return 1;
    return 0;
}

int Domino::is_repetition_corners(int move){
    if (is_repetition(move))
        return 1;

    if (history.size() == 0)
        return 0;
    
    int lastmove = history[history.size()-1];

    if (move == R2 && lastmove == L2)
        return 1;
    if (move == L2 && lastmove == R2)
        return 1;
    if (move == F2 && lastmove == B2)
        return 1;
    if (move == B2 && lastmove == F2)
        return 1;
    
    // // no more than 3 half turns in a row
    if (history.size() >= 3){
        int lastlastmove = history[history.size()-2];
        int lastlastlastmove = history[history.size()-3];
        if (!is_move_qt(move) && !is_move_qt(lastmove) && !is_move_qt(lastlastmove) && !is_move_qt(lastlastlastmove)){
            return 1;
        }
    }

    return 0;
}

void Domino::parse_alg(std::string alg){
    std::stringstream ss(alg);

    while (std::getline(ss, alg, ' ')){
        if (alg == "U"){
            make_move(U);
        } else if (alg == "U'"){
            make_move(UP);
        } else if (alg == "U2"){
            make_move(U2);
        } else if (alg == "R2"){
            make_move(R2);
        } else if (alg == "F2"){
            make_move(F2);
        } else if (alg == "L2"){
            make_move(L2);
        } else if (alg == "B2"){
            make_move(B2);
        } else {
            std::cout << "unrecognized move " << alg << std::endl;
            exit(1);
        }
    }
};

//TODO
// set the domino to a completely random solvable state
void Domino::set_random_state(){
        //changes are being made to the cube, so the corner index is no longer valid
    savedCornerIndex = -1;
    reset();
    int parity = (rand() % 2);

    //50% chance of swapping parity
    if (parity){
        corner_swap(URB, ULB);
        edge_swap(UF, UB);
    };

    //swap each piece with a random counterpart (including itself)
    for (int piece = 0; piece < 8; piece++){
        edge_swap(piece, rand()%8);
    }

    for (int piece = 0; piece < 8; piece++){
        corner_swap(piece, rand()%8);
    }
}

// very low level, cycles four corners to make a move
void Domino::corner_cycle(int c1, int c2, int c3, int c4){
    int tmp = corners[c4];
    corners[c4] = corners[c3];
    corners[c3] = corners[c2];
    corners[c2] = corners[c1];
    corners[c1] = tmp;
};

// very low level, cycles four edges to make a move
void Domino::edge_cycle(int e1, int e2, int e3, int e4){
    int tmp = edges[e4];
    edges[e4] = edges[e3];
    edges[e3] = edges[e2];
    edges[e2] = edges[e1];
    edges[e1] = tmp;
};

void Domino::edge_swap(int e1, int e2){
    int tmp = edges[e1];
    edges[e1] = edges[e2];
    edges[e2] = tmp;
};

void Domino::corner_swap(int c1, int c2){
    int tmp = corners[c1];
    corners[c1] = corners[c2];
    corners[c2] = tmp;
};

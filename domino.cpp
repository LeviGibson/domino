#include <cstdio>
#include <iostream>
#include <cstring>
#include <sstream>
#include <cassert>
#include <stdlib.h>
#include <fstream>
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


void print_hash(U64 hash){

    int32_t rank = 8;
    for (int32_t square = 0; square < 64; square++){
        if (square % 8 == 0) {
            printf("\n");
            rank--;
        }
        if ((hash >> square)&1ULL){
            printf("1  ");
        } else{
            printf(".  ");
        }
    }
    printf("\n");
}

U64 corner_hash_keys[8][8];
U64 edge_hash_keys[8][8];

void generate_hash_keys(){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            corner_hash_keys[i][j] = ((U64)std::rand()) | (((U64)std::rand()) << 32);
            edge_hash_keys[i][j] = ((U64)std::rand()) | (((U64)std::rand()) << 32);
        }
    }
}

//write hash keys to binary file
void write_hash_keys(){
    std::fstream file;
    file.open("hashKeys.bin", std::ios_base::out|std::ios_base::binary);
    if(!file.is_open()) {
        std::cout<<"Unable to open the file\n";
        assert(0);
    }

    file.write((const char*)corner_hash_keys, sizeof(corner_hash_keys));
    file.write((const char*)edge_hash_keys, sizeof(edge_hash_keys));

    file.close();
}

//load hash keys from "hashKeys.bin"
void read_hash_keys(){
    FILE *fin = fopen("hashKeys.bin", "rb");
    int _tmp = fread(corner_hash_keys, sizeof(U64), 8*8, fin);
    _tmp = fread(edge_hash_keys, sizeof(U64), 8*8, fin);
    fclose(fin);
}

//if hash keys are already made, load them, otherwise, generate and save them
void init_hash(){
    if (binary_file_exists("hashKeys.bin")){
        read_hash_keys();
    } else {
        generate_hash_keys();
        write_hash_keys();
    }
}

void normalize_piece_set(int* c, int* e){
    const int* transformer = NULL;
    if (e[0] == UB || e[0] == DB)
        transformer = &SOLVED_STATE_1[0];
    if (e[0] == UR || e[0] == DR)
        transformer = &SOLVED_STATE_2[0];
    if (e[0] == UF || e[0] == DF)
        transformer = &SOLVED_STATE_3[0];
    if (e[0] == UL || e[0] == DL)
        transformer = &SOLVED_STATE_4[0];
    
    for (int i = 0; i < 8; i++){
        c[i] = transformer[c[i]];
        e[i] = transformer[e[i]];
    }
}

U64 Domino::domino_hash(){
    int tmpCorners[8];
    int tmpEdges[8];
    memcpy(tmpCorners, corners, sizeof(corners));
    memcpy(tmpEdges, edges, sizeof(edges));

    normalize_piece_set(tmpCorners, tmpEdges);

    U64 hash = 0ULL;
    for (int i = 0; i < 8; i++){
        hash ^= corner_hash_keys[i][tmpCorners[i]];
        hash ^= edge_hash_keys[i][tmpEdges[i]];
    }
    return hash;
}

void Domino::set_state(int c0, int c1, int c2, int c3, int c4, int c5, int c6, int c7, int e0, int e1, int e2, int e3, int e4, int e5, int e6, int e7) {
    corners[0] = c0;
    corners[1] = c1;
    corners[2] = c2;
    corners[3] = c3;
    corners[4] = c4;
    corners[5] = c5;
    corners[6] = c6;
    corners[7] = c7;

    edges[0] = e0;
    edges[1] = e1;
    edges[2] = e2;
    edges[3] = e3;
    edges[4] = e4;
    edges[5] = e5;
    edges[6] = e6;
    edges[7] = e7;

    clear_history();
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
    std::cout << std::endl;
};

void Domino::print_pieces(){
    for (int i = 0; i < 8; i++){
        printf("%d, ", corners[i]);
    }

    // printf("\n");

    for (int i = 0; i < 8; i++){
        printf("%d, ", edges[i]);
    }

    printf("\n");
    printf("\n");
}

void Domino::print_moves() {
    for (int i = 0; i < history.size(); i++) {
        std::cout << MOVECHARS[history[i]] << " ";
    }
    std::cout << std::endl;
    
}

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

    history.add(move);
}

int QUARTER_TURN_HTR_SUBSET_INDICIES[6] = {0, 1, 2, 4, 5, 6};
int ORIENTED = 0;
int MISORIENTED = 1;
//semi-oriented is for 4c cases where you can't take the orientation relative to the corners
int SEMIORIENTED = 2;


std::string CORNERTYPE_NAMES[7] = {"0qt ", "1qt ", "2qt(a) ", "2qt(b) ", "3qt ", "4qt ", "5qt "};
std::string MISCORNER_NAMES[3] = {"0c", "2c", "4c"};
std::string MISEDGE_NAMES[5] = {"0e", "2e", "4e", "6e", "8e"};

std::string Domino::htr_subset_name(int subset){
    std::string name;
    int cornerType = subset / 15;
    subset -= cornerType * 15;

    int misorientedEdges = subset / 3;
    subset -= 3*misorientedEdges;

    int misorientedCorners = subset;

    return CORNERTYPE_NAMES[cornerType] + MISCORNER_NAMES[misorientedCorners] + MISEDGE_NAMES[misorientedEdges];
}

int Domino::get_htr_subset() {
    int orientation = -1;

    int misorientedCorners = 0;
    for (int i = 0; i < 8; i++) {
        if (i%2 != corners[i] % 2){
            misorientedCorners++;
        }
    }

    if (misorientedCorners > 4)
        orientation = MISORIENTED;
    else if (misorientedCorners < 4)
        orientation = ORIENTED;
    else if (misorientedCorners == 4)
        orientation = SEMIORIENTED;

    if (orientation == MISORIENTED){
        misorientedCorners = 8-misorientedCorners;
    }
    
    int misorientedEdges = 0;

    for (int i = 0; i < 8; i++) {
        if (i%2 != edges[i] % 2){
            misorientedEdges++;
        }
    }

    if (orientation == MISORIENTED)
        misorientedEdges = 8-misorientedEdges;
    if (orientation == SEMIORIENTED && misorientedEdges > 4)
        misorientedEdges = 8-misorientedEdges;

    int cornerType = QUARTER_TURN_HTR_SUBSET_INDICIES[qt_count()];
    //add extra index for bad 2qt
    if (cornerType == 2 && two_quarterturn_type() == BAD_2QT)
        cornerType++;

    //save space because neither of these can be odd
    misorientedEdges /= 2;
    misorientedCorners /= 2;
    //cornerType 0-6
    //corners 0-2
    //edges 0-4
    return (misorientedCorners) + (misorientedEdges * 3) + (cornerType * 3 * 5);
}

//why is this function so hard to write it should be really easy
//there's probably a way easier implementation of this but idc
int Domino::two_quarterturn_type() {
    assert(qt_count() == 2);

    int f1 = 0;
    int f2 = 0;
    for (int i = 0; i < 8; i++) {
        if (i % 2 != corners[i] % 2){
            if (i <= 3)
                f1++;
            else
                f2++;
        }
    }

    if (f1 == 4 || f2 == 4)
        return BAD_2QT;

    if (f1 == 1 || f2 == 1)
        return GOOD_2QT;

    f1 = 0;
    f2 = 0;

    for (int i = 0; i < 8; i++) {
        if (i % 2 != corners[i] % 2){
            if (i <= 3)
                f1+=i;
            else
                f2+=i-4;
        }
    }

    if (f1 == 1 || f1 == 5)
        f1 = 0;
    else
        f1 = 1;

    if (f2 == 1 || f2 == 5)
        f2 = 0;
    else
        f2 = 1;

    if (f1 == f2)
        return BAD_2QT;
    else
        return GOOD_2QT;
}

void Domino::undo_move()
{
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

    history.pop();
}

void Domino::clear_history(){
    history.clear();
    //changes are being made to the cube, so the corner index is no longer valid
    savedCornerIndex = -1;
}

int Domino::is_repetition(int move){
    if (history.size() == 0){
        return 0;
    }

    int lastlastmove = -1;

    if (history.size() != 1){
        lastlastmove = history[history.size()-2];
    }

    int lastmove = history[history.size()-1];

    //if it's a move repetition, it's a repetition
    if (move == lastmove){
        return 1;
    }

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
    int parity = (std::rand() % 2);

    //50% chance of swapping parity
    if (parity){
        corner_swap(URB, ULB);
        edge_swap(UF, UB);
    };

    //swap each piece with a random counterpart (including itself)
    for (int piece = 0; piece < 8; piece++){
        edge_swap(piece, std::rand()%8);
    }

    for (int piece = 0; piece < 8; piece++){
        corner_swap(piece, std::rand()%8);
    }
}

void Domino::set_random_htr_state() {
    savedCornerIndex = -1;
    reset();

    int parity = (std::rand() % 2);

    for (int i = 0; i < 100 + parity; i++) {
        make_move((std::rand() % 5) + 2);
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


//swap two corners
//low level
void Domino::edge_swap(int e1, int e2){
    int tmp = edges[e1];
    edges[e1] = edges[e2];
    edges[e2] = tmp;
};

//swap two edges
//low level
void Domino::corner_swap(int c1, int c2){
    int tmp = corners[c1];
    corners[c1] = corners[c2];
    corners[c2] = tmp;
};

//Definitions for Algorithm class --------------------------------------------------------------------

Algorithm::Algorithm(){

}



void Algorithm::add(int move) {
    moves.push_back(move);
}

void Algorithm::pop() {
    moves.pop_back();
}

void Algorithm::print(int verbose) {
    for (int i = 0; i < size(); i++) {
        std::cout << MOVECHARS[moves[i]] << " ";
    }
    std::cout << "(" << size() << ")";
    std::cout << std::endl;
}

int Algorithm::size() {
    return moves.size();
}

void Algorithm::clear() {
    moves.clear();
}

int Algorithm::qt_count() {
    int count = 0;
    for (int i = 0; i < moves.size(); i++){
        if (moves[i] == U || moves[i] == UP)
            count++;
    }
    return count;
}

int &Algorithm::operator[](size_t i) {
    return moves[i];
}

// void Algorithm::operator=(Algorithm a) {
//     moves = a.moves;
// }

//End definitions for Algorithm class --------------------------------------------------------------------

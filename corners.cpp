#include "corners.h"
#include <cassert>
#include <fstream>

#define UNSOLVED 10000

int Corners::quarter_turn_counts[CORNER_INDEX_COUNT];
std::vector<int> cornerSolution;

int qtsDone = 0;
int cornersearch(int depth, Domino* domino){
    int cornerIndex = domino->corner_index();
    if (domino->are_corners_solved_htr()){
        cornerSolution = domino->history;
        return qtsDone;
    }
    if (depth == 0)
        return UNSOLVED;

    //remove unneccisary searching of sub-optimal solutions
    if (Corners::quarter_turn_counts[cornerIndex] != -1){
        if (qtsDone + Corners::quarter_turn_counts[cornerIndex] > 5){
            return UNSOLVED;
        }
    }
    if (qtsDone > 5)
        return UNSOLVED;

    //if no quarter turns have been made, and we know the number of quarter turns the case requires, then yeah!
    if (qtsDone == 0 && Corners::quarter_turn_counts[cornerIndex] != -1)
        return Corners::quarter_turn_counts[cornerIndex];

    int mineval = UNSOLVED;
    for (int i = 0; i <= L2; i++){
        if (domino->is_repetition_corners(i)){
            continue;
        }

        int isqt = 0;
        if (i == U || i == UP)
            isqt = 1;

        domino->make_move(i);
        qtsDone += isqt;
        mineval = std::min(cornersearch(depth-1, domino), mineval);
        qtsDone -= isqt;
        domino->undo_move();

        if (mineval != UNSOLVED)
            break;
    }
    
    return mineval;
}

int find_qt_count(Domino* domino){

    //search for a solution
    for (int depth = 0; depth < 13; depth++){
        domino->clear_history();
        qtsDone = 0;
        int result = cornersearch(depth, domino);
        if (result != UNSOLVED){
            return result;
        }
    }

    assert(0);
}

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void printProgress(double percentage, int step) {
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s] %d/%d", val, lpad, PBSTR, rpad, "", step, CORNER_INDEX_COUNT);
    fflush(stdout);
};

void generate_corner_tables(){
    std::cout << "Generating corner quarter-turn table\n";
    std::cout << "This may take a few minutes\n";
 
    Domino domino = Domino();

    //set all qt counts to -1
    for (int index = 0; index < CORNER_INDEX_COUNT; index++){
        Corners::quarter_turn_counts[index] = -1;
    }

    //fill in table
    for (int index = 0; index < CORNER_INDEX_COUNT; index++){
        domino.set_corners_from_index(index);
        int qtcount = find_qt_count(&domino);

        //some 4qt drs have optimal solutions with 6 quarter turns in them
        if (qtcount == 6){
            qtcount = 4;
        }

        Corners::quarter_turn_counts[index] = qtcount;
        printProgress((double)index / (double)CORNER_INDEX_COUNT, index);
    }

    std::cout << "\n";
}

void save_corner_table(){
    std::fstream file;
    file.open("cornerQuarterTurns.bin", std::ios_base::out|std::ios_base::binary);
    if(!file.is_open()) {
        std::cout<<"Unable to open the file\n";
        assert(0);
    }

    file.write((const char*)Corners::quarter_turn_counts, sizeof(Corners::quarter_turn_counts));
    file.close();
}

int binary_file_exists(){
    FILE *codefile;
   if(codefile=fopen("cornerQuarterTurns.bin","r")) {
       fclose(codefile);
       return 1;
   } else {
       return 0;
   }

   return 0;
}

void read_binary_file(){
    FILE *fin = fopen("cornerQuarterTurns.bin", "rb");
    int _tmp = fread(Corners::quarter_turn_counts, sizeof(int), CORNER_INDEX_COUNT, fin);
    fclose(fin);
}

void Corners::init_corners(){
    if (binary_file_exists()){
        read_binary_file();
    } else {
        generate_corner_tables();
        save_corner_table();
    }
}

#include "search.h"

#define UNSOLVED 1000
int ply = 0;

std::vector<int> solution;

int search(int depth, Domino* domino){
    if (domino->is_domino_solved()){
        solution = domino->history;
        return ply;
    }

    if (depth == 0){
        return UNSOLVED;
    }

    int optimal = UNSOLVED;

    for (int move = 0; move < 7; move++){
        if (domino->is_repetition(move))
            continue;

        domino->make_move(move);
        ply++;
        int branchval = search(depth-1, domino);
        ply--;
        domino->undo_move();

        optimal = std::min(branchval, optimal);
    }

    return optimal;
    
}

void print_solution(){
    unsigned int vecSize = solution.size();

    for(unsigned int i = 0; i < vecSize; i++){
        std::cout << MOVECHARS[solution[i]] << " ";
    }
    std::cout << std::endl;
}

int Search::find_optimal(Domino domino){
    ply = 0;
    int result;
    domino.clear_history();

    for (int depth = 0; depth < 20; depth++){
        std::cout << "searching depth " << depth << std::endl;
        result = search(depth, &domino);
        if (result != UNSOLVED)
            break;
    }

    std::cout << result << std::endl;
    print_solution();

    return 0;
}

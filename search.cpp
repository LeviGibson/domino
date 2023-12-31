#include "search.h"
#include "pruning.h"
#include <cassert>

#define UNSOLVED 1000
int ply = 0;
int NO_EXTRA_QUARTER_TURNS = 0;

// Algorithm Search::solution;

Search::Search(){
    
}

int Search::search(int depth, int extended, Domino* domino){
    if (domino->is_domino_solved()){
        Search::solution = domino->history;
        return ply;
    }

    int pruningTableRes = Pruning::proximity_to_solved(domino, NO_EXTRA_QUARTER_TURNS);
    if (!extended && pruningTableRes != -1){
        // assert(depth == 0);
        depth = pruningTableRes;
        extended = 1;
    }

    if (extended && pruningTableRes == -1){
        return UNSOLVED;
    }

    if (depth == 0){
        return UNSOLVED;
    }

    int optimal = UNSOLVED;

    int quarterTurnCount = domino->qt_count();

    for (int move = 0; move < 7; move++){
        if (domino->is_repetition(move))
            continue;

        domino->make_move(move);
        ply++;

        // int newPruningTableRes = Pruning::proximity_to_solved(domino);
        // // && (newPruningTableRes >= pruningTableRes || newPruningTableRes == -1)
        // if (extended && (newPruningTableRes == -1 || newPruningTableRes >= pruningTableRes)){
        //     domino->undo_move();
        //     ply--;
        //     continue;
        //     // if (std::abs(pruningTableRes - newPruningTableRes) >= 2 && newPruningTableRes != -1){
        //     //     printf("%d, %d\n", pruningTableRes, newPruningTableRes);
        //     // }
        // }

        //Faster than above
        if (pruningTableRes != -1){
            int newPruningTableRes = Pruning::proximity_to_solved(domino, NO_EXTRA_QUARTER_TURNS);
            if (extended && (newPruningTableRes >= pruningTableRes)){
                domino->undo_move();
                ply--;
                continue;
            }
        }

        if (NO_EXTRA_QUARTER_TURNS && (move == U || move == UP)){
            if (domino->qt_count() > quarterTurnCount){
                domino->undo_move();
                ply--;
                continue;
            }
        }

        int branchval = search(depth-1, extended, domino);
        ply--;
        domino->undo_move();

        optimal = std::min(branchval, optimal);
    }

    return optimal;
    
}

int Search::find_optimal(Domino domino, int print, int noExtraQts){
    solution = Algorithm();

    NO_EXTRA_QUARTER_TURNS = noExtraQts;

    ply = 0;
    int result;
    domino.clear_history();

    for (int depth = 0; depth < 1000; depth++){
        result = search(depth, 0, &domino);
        if (result != UNSOLVED)
            break;
    }

    if (print)
        solution.print();

    return solution.size();
}

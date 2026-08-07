#ifndef AI_INTERFACE_HPP
#define AI_INTERFACE_HPP

#include "Board.hpp"
#include <vector>

struct ScoredMove {
    int row;
    int col;
    int score;
};

std::vector<ScoredMove> getRankedMoves(const Board& board, Player player);

#endif

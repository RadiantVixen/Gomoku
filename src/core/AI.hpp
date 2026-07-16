#include "Board.hpp"
#include <algorithm>
#include <list>


std::list<std::tuple<int, int>> generateLegalMoves(const Board& board);
int heuristicEvaluation(const Board& board);
std::list<std::tuple<int, int>> CandidateOrdering(const Board& board);

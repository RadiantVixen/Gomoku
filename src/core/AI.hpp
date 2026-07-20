#include "Board.hpp"
#include <algorithm>
#include <list>
#include <tuple>
#include <climits>

std::list<std::tuple<int, int>> generateLegalMoves(const Board& board);
int heuristicEvaluation(const Board& board);
std::list<std::tuple<int, int>> CandidateOrdering(const Board& board);
std::tuple<int, int> FindBestMove(const Board& board, int maxDepth);
int MinMax(const Board& board, int depth, int ownedScore = INT_MIN);
int heuristicWin(const Board& board, Player currentPlayer, int i, int j);
int checkCapture(const Board& board, Player currentPlayer, int i, int j);
int CapturableFive(const Board& board, Player currentPlayer, int i, int j);
int ItWorth(const Board& board, Player p, int i, int j);
int Capturable(const Board& board, Player p, int i, int j);





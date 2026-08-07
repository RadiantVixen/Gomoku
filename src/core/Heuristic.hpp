// Heuristic helpers for evaluator
#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include "Board.hpp"
#include "AIInterface.hpp"
#include <vector>
#include <utility>

struct LineInfo {
    int count;
    int openEnds;
};

struct MoveScoreBreakdown {
    int offensiveScore;
    int centerBonus;
    int captureBonus;
    int capturePenalty;
    int opponentThreatPenalty;
    bool immediateWin;
    int finalScore;
};

void setEvaluatorDebugEnabled(bool enabled);
bool isEvaluatorDebugEnabled();

int countStonesInDirection(const Board& board, int row, int col, int dRow, int dCol, Player player);
int countOpenEnds(const Board& board, int row, int col, int dRow, int dCol, Player player);
LineInfo getLineInfo(const Board& board, int row, int col, int dRow, int dCol, Player player);
int getPatternScore(int count, int openEnds);
int scoreDirection(const Board& board, int row, int col, int dRow, int dCol, Player player);

int calculateOffensiveScore(const Board& board, int row, int col, Player player);
int calculateCenterBonus(int row, int col);
int checkCaptureVulnerability(const Board& board, int row, int col, Player player, int offensiveScore);
int calculateCaptureBonus(const Board& board, int row, int col, Player player);
int calculateOpponentThreatPenalty(const Board& board, int row, int col, Player player, int offensiveScore);

std::vector<std::pair<int, int>> getLegalMoves(const Board& board);
std::vector<ScoredMove> getRankedMoves(const Board& board, Player player);

MoveScoreBreakdown evaluateMoveDetailed(const Board& board, int row, int col, Player player);
void debugRankedMoves(const Board& board, Player player, int limit = 10);

#endif // HEURISTIC_HPP

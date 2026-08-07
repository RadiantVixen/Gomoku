#include "Heuristic.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>

namespace {
    bool g_evaluatorDebugEnabled = false;

    inline bool inBounds(int r, int c) {
        return r >= 0 && r < Board::SIZE && c >= 0 && c < Board::SIZE;
    }

    const char* playerName(Player player) {
        if (player == Player::BLACK) return "BLACK";
        if (player == Player::WHITE) return "WHITE";
        return "NONE";
    }

    Player opponentOf(Player player) {
        return (player == Player::BLACK) ? Player::WHITE : Player::BLACK;
    }

    bool isDirectionStart(const Board& board, int row, int col, int dRow, int dCol, Player player) {
        if (board.getCell(row, col) == Player::NONE) {
            return true;
        }
        return board.getCell(row - dRow, col - dCol) != player;
    }

    bool hasNearbyStone(const Board& board, int row, int col, int radius) {
        for (int dr = -radius; dr <= radius; ++dr) {
            for (int dc = -radius; dc <= radius; ++dc) {
                if (dr == 0 && dc == 0) {
                    continue;
                }
                int r = row + dr;
                int c = col + dc;
                if (!inBounds(r, c)) {
                    continue;
                }
                if (board.getCell(r, c) != Player::NONE) {
                    return true;
                }
            }
        }
        return false;
    }

    bool boardHasAnyStone(const Board& board) {
        for (int row = 0; row < Board::SIZE; ++row) {
            for (int col = 0; col < Board::SIZE; ++col) {
                if (board.getCell(row, col) != Player::NONE) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isMoveLegalForPlayer(const Board& board, int row, int col, Player player) {
        if (!board.isValidMove(row, col)) {
            return false;
        }
        Board copy(board);
        copy.currentPlayer = player;
        return copy.makeMove(row, col);
    }

    int bestOffensiveScoreForPlayer(const Board& board, Player player) {
        int best = 0;
        for (int row = 0; row < Board::SIZE; ++row) {
            for (int col = 0; col < Board::SIZE; ++col) {
                if (!isMoveLegalForPlayer(board, row, col, player)) {
                    continue;
                }
                int score = calculateOffensiveScore(board, row, col, player) + calculateCenterBonus(row, col);
                if (score > best) {
                    best = score;
                }
            }
        }
        return best;
    }
}

void setEvaluatorDebugEnabled(bool enabled) {
    g_evaluatorDebugEnabled = enabled;
}

bool isEvaluatorDebugEnabled() {
    return g_evaluatorDebugEnabled;
}

int countStonesInDirection(const Board& board, int row, int col, int dRow, int dCol, Player player) {
    int count = 1;

    int r = row + dRow;
    int c = col + dCol;
    while (inBounds(r, c) && board.getCell(r, c) == player) {
        ++count;
        r += dRow; c += dCol;
    }

    r = row - dRow;
    c = col - dCol;
    while (inBounds(r, c) && board.getCell(r, c) == player) {
        ++count;
        r -= dRow; c -= dCol;
    }

    return count;
}

int countOpenEnds(const Board& board, int row, int col, int dRow, int dCol, Player player) {
    int openEnds = 0;

    int r = row + dRow;
    int c = col + dCol;
    while (inBounds(r, c) && board.getCell(r, c) == player) {
        r += dRow; c += dCol;
    }
    if (inBounds(r, c) && board.getCell(r, c) == Player::NONE) ++openEnds;

    r = row - dRow;
    c = col - dCol;
    while (inBounds(r, c) && board.getCell(r, c) == player) {
        r -= dRow; c -= dCol;
    }
    if (inBounds(r, c) && board.getCell(r, c) == Player::NONE) ++openEnds;

    return openEnds;
}

LineInfo getLineInfo(const Board& board, int row, int col, int dRow, int dCol, Player player) {
    LineInfo info;
    info.count = countStonesInDirection(board, row, col, dRow, dCol, player);
    info.openEnds = countOpenEnds(board, row, col, dRow, dCol, player);
    return info;
}

int getPatternScore(int count, int openEnds) {
    if (count >= 5) return 1000000;
    if (count == 4 && openEnds == 2) return 100000;
    if (count == 4 && openEnds == 1) return 10000;
    if (count == 3 && openEnds == 2) return 5000;
    if (count == 3 && openEnds == 1) return 500;
    if (count == 2 && openEnds == 2) return 100;
    if (count == 2 && openEnds == 1) return 10;
    return 0;
}

int scoreDirection(const Board& board, int row, int col, int dRow, int dCol, Player player) {
    if (!isDirectionStart(board, row, col, dRow, dCol, player)) {
        return 0;
    }

    LineInfo info = getLineInfo(board, row, col, dRow, dCol, player);
    int score = getPatternScore(info.count, info.openEnds);
    return score;
}

int calculateOffensiveScore(const Board& board, int row, int col, Player player) {
    if (!inBounds(row, col) || board.getCell(row, col) != Player::NONE) {
        return 0;
    }

    const int directions[4][2] = {{0,1},{1,0},{1,1},{1,-1}};
    int total = 0;
    for (const auto& dir : directions) {
        int dirScore = scoreDirection(board, row, col, dir[0], dir[1], player);
        total += dirScore;
    }

    total += calculateCenterBonus(row, col);
    return total;
}

int calculateCenterBonus(int row, int col) {
    const int centerRow = Board::SIZE / 2;
    const int centerCol = Board::SIZE / 2;
    int dist = std::abs(row - centerRow) + std::abs(col - centerCol);
    if (dist <= 4) {
        return 5 * (5 - dist);
    }
    return 0;
}

int checkCaptureVulnerability(const Board& board, int row, int col, Player player, int offensiveScore) {
    if (!isMoveLegalForPlayer(board, row, col, player)) {
        return 0;
    }

    Board copy(board);
    copy.currentPlayer = player;
    copy.makeMove(row, col);

    bool immediateWin = copy.getWinner() == player;
    if (immediateWin || offensiveScore >= 1000000) {
        return 0;
    }

    for (int r = 0; r < Board::SIZE; ++r) {
        for (int c = 0; c < Board::SIZE; ++c) {
            if (copy.getCell(r, c) == player && copy.isCapturable(player, r, c) != 0) {
                return 50000;
            }
        }
    }
    return 0;
}

int calculateCaptureBonus(const Board& board, int row, int col, Player player) {
    if (!isMoveLegalForPlayer(board, row, col, player)) {
        return 0;
    }

    Board copy(board);
    copy.currentPlayer = player;

    int before = (player == Player::BLACK) ? copy.blackStonesCaptured : copy.whiteStonesCaptured;
    copy.makeMove(row, col);
    int after = (player == Player::BLACK) ? copy.blackStonesCaptured : copy.whiteStonesCaptured;

    int capturedStones = after - before;
    if (capturedStones <= 0) {
        return 0;
    }
    return (capturedStones / 2) * 30000;
}

int calculateOpponentThreatPenalty(const Board& board, int row, int col, Player player, int offensiveScore) {
    if (!isMoveLegalForPlayer(board, row, col, player)) {
        return 0;
    }

    Board copy(board);
    copy.currentPlayer = player;
    copy.makeMove(row, col);

    if (copy.getWinner() == player || offensiveScore >= 1000000) {
        return 0;
    }

    Player opponent = opponentOf(player);
    int opponentBest = bestOffensiveScoreForPlayer(copy, opponent);
    if (opponentBest >= 100000) {
        return 80000;
    }
    return 0;
}

std::vector<std::pair<int, int>> getLegalMoves(const Board& board) {
    std::vector<std::pair<int, int>> moves;
    for (int row = 0; row < Board::SIZE; ++row) {
        for (int col = 0; col < Board::SIZE; ++col) {
            if (board.isValidMove(row, col)) {
                moves.push_back({row, col});
            }
        }
    }
    return moves;
}

MoveScoreBreakdown evaluateMoveDetailed(const Board& board, int row, int col, Player player) {
    MoveScoreBreakdown details{};
    details.offensiveScore = calculateOffensiveScore(board, row, col, player);
    details.centerBonus = calculateCenterBonus(row, col);
    details.captureBonus = calculateCaptureBonus(board, row, col, player);
    details.capturePenalty = checkCaptureVulnerability(board, row, col, player, details.offensiveScore);
    details.opponentThreatPenalty = calculateOpponentThreatPenalty(board, row, col, player, details.offensiveScore);

    Board copy(board);
    copy.currentPlayer = player;
    details.immediateWin = copy.makeMove(row, col) && copy.getWinner() == player;

    details.finalScore = details.offensiveScore + details.captureBonus
        - details.capturePenalty - details.opponentThreatPenalty;

    return details;
}

std::vector<ScoredMove> getRankedMoves(const Board& board, Player player) {
    std::vector<ScoredMove> ranked;
    std::vector<std::pair<int, int>> candidates = getLegalMoves(board);
    int preFilterCount = candidates.size();

    bool hasStone = boardHasAnyStone(board);
    int postFilterCount = 0;

    for (const auto& move : candidates) {
        int row = move.first;
        int col = move.second;

        if (hasStone && !hasNearbyStone(board, row, col, 2)) {
            continue;
        }

        if (!isMoveLegalForPlayer(board, row, col, player)) {
            continue;
        }

        postFilterCount++;
        MoveScoreBreakdown details = evaluateMoveDetailed(board, row, col, player);
        ranked.push_back({row, col, details.finalScore});
    }

    if (!hasStone && ranked.empty()) {
        int center = Board::SIZE / 2;
        ranked.push_back({center, center, evaluateMoveDetailed(board, center, center, player).finalScore});
    }

    std::sort(ranked.begin(), ranked.end(), [](const ScoredMove& left, const ScoredMove& right) {
        if (left.score != right.score) {
            return left.score > right.score;
        }
        if (left.row != right.row) {
            return left.row < right.row;
        }
        return left.col < right.col;
    });

    if (g_evaluatorDebugEnabled) {
        std::cout << "[EVAL] filtered " << preFilterCount << " legal → " << postFilterCount << " after near-stone check\n";
    }

    return ranked;
}

void debugRankedMoves(const Board& board, Player player, int limit) {
    std::vector<ScoredMove> ranked = getRankedMoves(board, player);
    int shown = std::min(static_cast<int>(ranked.size()), limit);
    
    std::cout << "\n========== RANKED MOVES FOR " << playerName(player) << " ==========\n";
    std::cout << "Rank#  Pos        Score\n";
    std::cout << "-----  ---------  -----------\n";
    
    for (int i = 0; i < shown; ++i) {
        const ScoredMove& move = ranked[i];
        std::cout << "  " << (i + 1) << ".   (" << move.row << "," << move.col << ")    " << move.score << "\n";
    }
    
    std::cout << "==========================================\n";
}



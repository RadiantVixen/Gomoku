#include "AI.hpp"

#include <algorithm>

std::list<std::tuple<int, int>> generateLegalMoves(const Board& board) {
    std::list<std::tuple<int, int>> legalMoves;

    for (int row = 0; row < Board::SIZE; ++row) {
        for (int col = 0; col < Board::SIZE; ++col) {
            if (!board.isValidMove(row, col)) {
                continue;
            }

            bool hasNeighbor = false;
            for (int dRow = -1; dRow <= 1 && !hasNeighbor; ++dRow) {
                for (int dCol = -1; dCol <= 1 && !hasNeighbor; ++dCol) {
                    if (dRow == 0 && dCol == 0) {
                        continue;
                    }

                    int neighborRow = row + dRow;
                    int neighborCol = col + dCol;
                    if (neighborRow >= 0 && neighborRow < Board::SIZE &&
                        neighborCol >= 0 && neighborCol < Board::SIZE &&
                        board.getCell(neighborRow, neighborCol) != Player::NONE) {
                        hasNeighbor = true;
                    }
                }
            }

            if (hasNeighbor) {
                legalMoves.push_back(std::make_tuple(row, col));
            }
        }
    }

    if (legalMoves.empty()) {
        legalMoves.push_back(std::make_tuple(Board::SIZE / 2, Board::SIZE / 2));
    }

    return legalMoves;
}

int heuristicEvaluation(const Board& board) {
    int score = 0;

    for (int row = 0; row < Board::SIZE; ++row) {
        for (int col = 0; col < Board::SIZE; ++col) {
            Player cell = board.getCell(row, col);
            if (cell == board.getCurrentPlayer()) {
                ++score;
            } else if (cell != Player::NONE) {
                --score;
            }
        }
    }

    return score;
}

std::list<std::tuple<int, int>> CandidateOrdering(const Board& board) {
    std::list<std::tuple<int, int>> candidates = generateLegalMoves(board);
    std::vector<std::pair<int, std::tuple<int, int>>> scoredCandidates;

    for (const auto& move : candidates) {
        int row = std::get<0>(move);
        int col = std::get<1>(move);
        Board copy = board;
        copy.makeMove(row, col);
        scoredCandidates.push_back({heuristicEvaluation(copy), move});
    }

    std::sort(scoredCandidates.begin(), scoredCandidates.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });

    std::list<std::tuple<int, int>> orderedCandidates;
    for (const auto& item : scoredCandidates) {
        orderedCandidates.push_back(item.second);
    }

    return orderedCandidates;
}

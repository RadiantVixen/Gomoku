#include "Board.h"

std::list<std::tuple<int, int>> generateLegalMoves(const Board& board) {
    std::list<std::tuple<int, int>> legalmoves;

    for (int i = 0; i < Board::SIZE; ++i) {
        for (int j = 0; j < Board::SIZE; ++j) {
            if (board.isValidMove(i, j) && (i,j) not in legalmoves && board.grid[i + 1][j] != Player::NONE && board.grid[i - 1][j] != Player::NONE && board.grid[i][j + 1] != Player::NONE && board.grid[i][j - 1] != Player::NONE && board.grid[i + 1][j + 1] != Player::NONE && board.grid[i - 1][j - 1] != Player::NONE && board.grid[i + 1][j - 1] != Player::NONE && board.grid[i - 1][j + 1] != Player::NONE) {
                legalmoves.push_back(std::make_tuple(i, j));
            }
        }
    }
    return legalmoves;
}


const int MAX_DEPTH = 10;
const int WINNING = 99999999999;
const int LOSING = -99999999999;
const int CAPTURABLE_FIVE = 100000;
const int CAPTURE = 10000;
const int OPEN_FOUR = 10000;
const int OPEN_THREE = 1000;
const int CLOSED_FOUR = 1000;
const int CLOSED_THREE = 100;
const int CAPTURABLE = -10;

int heuristicEvaluation(const Board& board) {
    Player currentPlayer = board.getCurrentPlayer();
    Player opponent = (currentPlayer == Player::BLACK) ? Player::WHITE : Player::BLACK;
    int score = 0;

    for (int i = 0; i < Board::SIZE; ++i) {
        for (int j = 0; j < Board::SIZE; ++j) {
            if (board.getCell(i, j) == currentPlayer) {
                score += checkwin(board, currentPlayer, i, j);
                score += CapturableFive(board, currentPlayer, i, j);
                score += OpenFour(board, currentPlayer, i, j);
                score += OpenThree(board, currentPlayer, i, j);
                score += ClosedFour(board, currentPlayer, i, j);
                score += ClosedThree(board, currentPlayer, i, j);
                score += Capturable(board, currentPlayer, i, j);
            } else if (board.getCell(i, j) == opponent) {
                score -= checkwin(board, opponent, i, j) - 9999999999;
                score -= CapturableFive(board, opponent, i, j);
                score -= OpenFour(board, opponent, i, j);
                score -= OpenThree(board, opponent, i, j);
                score -= ClosedFour(board, opponent, i, j);
                score -= ClosedThree(board, opponent, i, j);
                score -= Capturable(board, opponent, i, j);
            }
        }
    }

    return score
}


std::list<std::tuple<int, int>> CandidateOrdering(const Board& board)
{
    const std::list<std::tuple<int, int>>& candidates = generateLegalMoves(board);
    std::vector<std::pair<int, std::tuple<int, int>>> scoredCandidates;

    for (const auto& [i, j] : candidates) {
        board.makeMove(i, j);
        int score = heuristicEvaluation(board);
        board.undoMove(i, j);
        scoredCandidates.push_back({score, {i, j}});
    }

    std::sort(scoredCandidates.begin(), scoredCandidates.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });

    std::list<std::tuple<int, int>> orderedCandidates;
    for (const auto& [score, move] : scoredCandidates) {
        orderedCandidates.push_back(move);
    }

    return orderedCandidates;
}

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
    Player currentPlayer = board.getCurrentPlayer();
    Player opponent = (currentPlayer == Player::BLACK) ? Player::WHITE : Player::BLACK;
    int score = 0;
    
    const int INF_VALUE = 100000000;

    for (int i = 0; i < Board::SIZE; ++i) {
        for (int j = 0; j < Board::SIZE; ++j) {
            Player cell = board.getCell(i, j);
            
            if (cell == currentPlayer) {
                score += heuristicWin(board, currentPlayer, i, j);
                score += checkCapture(board, currentPlayer, i, j);
                score += CapturableFive(board, currentPlayer, i, j);
                score += ItWorth(board, currentPlayer, i, j);
                score -= Capturable(board, currentPlayer, i, j);
            } 
            else if (cell == opponent) {
                score -= (heuristicWin(board, opponent, i, j) + INF_VALUE);
                score -= CapturableFive(board, opponent, i, j);
                score -= ItWorth(board, opponent, i, j);
                score += Capturable(board, opponent, i, j);
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



std::tuple<int, int> FindBestMove(const Board& board, int maxDepth) {
    std::list<std::tuple<int, int>> moves = CandidateOrdering(board);
    
    if (moves.empty()) {
        return std::make_tuple(-1, -1); 
    }

    std::tuple<int, int> bestMove = moves.front();
    int maximumScore = INT_MIN;

    for (const auto& move : moves) {
        Board b = board;
        
        b.makeMove(std::get<0>(move), std::get<1>(move));
        
        int score = MinMax(b, maxDepth - 1);
        
        if (score > maximumScore) {
            maximumScore = score;
            bestMove = move;
        }
    }
    
    return bestMove;
}


int MinMax(const Board& board, int depth,int ownedScore) {
    if (depth == 0 || board.isGameOver()) {
        return heuristicEvaluation(board);
    }
    
    std::list<std::tuple<int, int>> moves = CandidateOrdering(board);
    
    if (moves.empty()) {
        return heuristicEvaluation(board);
    }
    
    int bestScore = INT_MIN;
    
    for (const auto& move : moves) {
        Board b = board;

        b.makeMove(std::get<0>(move), std::get<1>(move));
        
        int currentScore = -MinMax(b, depth - 1, bestScore * -1);
        
        bestScore = std::max(bestScore, currentScore);
        if( currentScore <= ownedScore)
            break;
    }
    
    return bestScore;
}


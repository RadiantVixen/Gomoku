
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


int heuristicEvaluation(const Board& board) {

}


std::list<std::tuple<int, int>> CandidateOrdering(const Board& board)
{
    int score[candidates.size()];
    int index = 0;
    const std::list<std::tuple<int, int>>& candidates = generateLegalMoves(board);

    for (const auto& [i, j] : candidates) {
        board.makeMove(i, j);
        score[index] = heuristicEvaluation(board);
        board.undoMove(i, j);
        index++;
    }


}

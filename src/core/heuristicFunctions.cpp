#include "AI.hpp"

int heuristicWin(const Board& board, Player currentPlayer, int i, int j) {
    if (board.checkwin(currentPlayer, i, j)) {
        return WINNING;
    }

    return 0;
}

int checkCapture(const Board& board, Player currentPlayer, int i, int j) {
    if (board.isCapturable(currentPlayer, i, j)) {
        return CAPTURE;
    }
    return 0;
}

int CapturableFive(const Board& board, Player currentPlayer, int i, int j) {
    if (board.checkFiveInRow(currentPlayer, i, j) == 2) {
        return CAPTURABLE_FIVE;
    }
    return 0;
}

int ItWorth(const Board& board, Player p, int i, int j) {
    if (p == Player::NONE) return 0;

    int directions[4][2] = {
        {0, 1},
        {1, 0},
        {1, 1},
        {1, -1}
    };

    int maxWorth = 0;

    for (int k = 0; k < 4; ++k) {
        int leftOpen = 0;
        int rightOpen = 0;

        int prev_r = i - directions[k][0];
        int prev_c = j - directions[k][1];
        if (prev_r >= 0 && prev_r < Board::SIZE && prev_c >= 0 && prev_c < Board::SIZE) {
            if (board.grid[prev_r][prev_c] == p)
                continue;
            if (board.grid[prev_r][prev_c] == Player::NONE)
                leftOpen = 1;
        }

        int r = i + directions[k][0];
        int c = j + directions[k][1];
        int count = 1;

        while (r >= 0 && r < Board::SIZE && c >= 0 && c < Board::SIZE && board.grid[r][c] == p) {
            count++;
            r += directions[k][0];
            c += directions[k][1];
        }

        if (r >= 0 && r < Board::SIZE && c >= 0 && c < Board::SIZE && board.grid[r][c] == Player::NONE)
            rightOpen = 1;

        if (!leftOpen && !rightOpen)
            continue;

        int worth = 0;
        if (count == 4 && leftOpen && rightOpen)
            worth = OPEN_FOUR;
        else if (count == 4 && (leftOpen || rightOpen))
            worth = CLOSED_FOUR;
        else if (count == 3 && leftOpen && rightOpen)
            worth = OPEN_THREE;
        else if (count == 3 && (leftOpen || rightOpen))
            worth = CLOSED_THREE;

        if (worth > maxWorth) {
            maxWorth = worth;
        }
    }

    return maxWorth;
}

int Capturable(const Board& board, Player p, int i, int j) {
    if (p == Player::NONE) return 0;
    Player opponent = (p == Player::BLACK) ? Player::WHITE : Player::BLACK;
    int directions[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    std::vector<std::tuple<int, int>> trap;
    for (int k = 0; k < 8; ++k) {
        int r = i - directions[k][0];
        int c = j - directions[k][1];
        int l = 0;
        if (r >= 0 && r < Board::SIZE && c >= 0 && c < Board::SIZE && board.grid[r][c] != opponent)
            continue;

        while (r >= 0 && r < Board::SIZE && c >= 0 && c < Board::SIZE && l < 4) {
            trap.push_back({r, c});
            l ++;
            r += directions[k][0];
            c += directions[k][1];
        }
        if (trap.size() == 4 &&
            board.grid[std::get<0>(trap[0])][std::get<1>(trap[0])] == opponent &&
            board.grid[std::get<0>(trap[1])][std::get<1>(trap[1])] == p &&
            board.grid[std::get<0>(trap[2])][std::get<1>(trap[2])] == p &&
            board.grid[std::get<0>(trap[3])][std::get<1>(trap[3])] == Player::NONE)
            return CAPTURABLE;

        trap.clear();
    }
    return 0;
}

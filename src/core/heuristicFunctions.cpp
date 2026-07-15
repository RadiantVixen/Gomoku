#include "Board.hpp"



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

    leftOpen = 0;
    rightOpen = 0;

    int directions[4][2] = {
        {0, 1},
        {1, 0},
        {1, 1},
        {1, -1}
    };
    
    for (int k = 0; k < 4; ++k) {
        if (i - directions[k][0] >= 0 && i - directions[k][0] < SIZE && j - directions[k][1] >= 0 && j - directions[k][1] < SIZE) {
            if (board.grid[i - directions[k][0]][j - directions[k][1]] == p)
                continue;
            if (board.grid[i - directions[k][0]][j - directions[k][1]] == Player::NONE)
                leftOpen = 1;
        }
    }

        int r = i + directions[k][0];
        int c = j + directions[k][1];
        int count = 1;



        while (r >= 0 && r < SIZE && c >= 0 && c < SIZE && grid[r][c] == p) {
            count++;
            if isCapturable(p, r, c):
            r += directions[k][0];
            c += directions[k][1];
        }

        if (r >= 0 && r < SIZE && c >= 0 && c < SIZE && grid[r][c] == Player::NONE)
            rightOpen = 1;
        
        if (!leftOpen && !rightOpen)
            continue;
        if (count == 4 && leftOpen && rightOpen)
            return OPEN_FOUR;
        if (count == 4 && (leftOpen || rightOpen))
            return CLOSED_FOUR;
        if (count == 3 && leftOpen && rightOpen)
            return OPEN_THREE;
        if (count == 3 && (leftOpen || rightOpen))
            return CLOSED_THREE;
    
    return 0;
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
        if (r >= 0 && r < SIZE && c >= 0 && c < SIZE && board.grid[r][c] != opponent)
            continue;

        while (r >= 0 && r < SIZE && c >= 0 && c < SIZE && l < 4) {
            trap.push_back({r, c});
            l ++;
            r += directions[k][0];
            c += directions[k][1];
        }
        if (trap.size() == 4 &&
            grid[std::get<0>(trap[0])][std::get<1>(trap[0])] == opponent &&
            grid[std::get<0>(trap[1])][std::get<1>(trap[1])] == p &&
            grid[std::get<0>(trap[2])][std::get<1>(trap[2])] == p &&
            grid[std::get<0>(trap[3])][std::get<1>(trap[3])] == Player::NONE)
            return CAPTURABLE;
        
        trap.clear();
    }
    return 0;
}

#include "Board.hpp"

Board::Board() {
    reset();
}

void Board::reset() {
    grid.assign(SIZE, std::vector<Player>(SIZE, Player::NONE));
    currentPlayer = Player::BLACK; // Black starts first in Gomoku
    winner = Player::NONE;
    gameOver = false;
}

bool Board::isValidMove(int row, int col) const {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
        return false;
    }
    return grid[row][col] == Player::NONE;
}

bool Board::makeMove(int row, int col) {
    if (gameOver || !isValidMove(row, col)) {
        return false;
    }

    grid[row][col] = currentPlayer;

    if (checkWin(row, col)) {
        winner = currentPlayer;
        gameOver = true;
    } else {
        // Switch turn
        currentPlayer = (currentPlayer == Player::BLACK) ? Player::WHITE : Player::BLACK;
    }

    return true;
}

Player Board::getCell(int row, int col) const {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
        return Player::NONE;
    }
    return grid[row][col];
}

std::vector<std::tuple<int, int>> Board::isCapturable(int row, int col){
    Player p = grid[row][col];
    if (p == Player::NONE) return NULL;
    Player opponent = (p == Player::BLACK) ? Player::WHITE : Player::BLACK;
    if (opponent == Player::NONE) return NULL;
    std::vector<std::tuple<int, int>> table[4];
    table[0].push_back({row, col});
    int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

    for (int i = 0; i < 4; ++i) {
        int dR = row + directions[i][0];
        int dC = col + directions[i][1];
        while (row + dR >= 0 && row + dR < SIZE && col + dC >= 0 && col + dC < SIZE) {
            dR += directions[i][0];
            dC += directions[i][1];
            table[i].push_back({dR, dC});
        }
        if (grid[table[0][0]][table[0][1]] == p && grid[table[1][0]][table[1][1]] == opponent && grid[table[2][0]][table[2][1]] == opponent && grid[table[3][0]][table[3][1]] == p){
            
            return table;

        }
        
    }
    return NULL;
}


bool Board::checkCapture(int row, int col) const{
    std::vector<std::tuple<int, int>> table = this->isCapturable(row,col);
    if (table){
        for (int i = 0; i < 4; ++i){
            grid[table[i][0]][table[i][1]] = Player::NONE;
        }
    }


    
}

bool Board::doubleThrees(int row, int col){

}

bool Board::isThereaNeed(int row, int col) const{

}
    





bool Board::checkWin(int row, int col) const {
    Player p = grid[row][col];
    if (p == Player::NONE) return false;

    // Check 4 directions: horizontal, vertical, diagonal, anti-diagonal
    int directions[4][2] = {
        {0, 1},   // Horizontal
        {1, 0},   // Vertical
        {1, 1},   // Diagonal (top-left to bottom-right)
        {1, -1}   // Anti-diagonal (top-right to bottom-left)
    };

    for (int i = 0; i < 4; ++i) {
        int dRow = directions[i][0];
        int dCol = directions[i][1];
        
        // Count in positive direction
        int count = 1;
        int r = row + dRow;
        int c = col + dCol;
        while (r >= 0 && r < SIZE && c >= 0 && c < SIZE && grid[r][c] == p) {
            count++;
            r += dRow;
            c += dCol;
        }

        // Count in negative direction
        r = row - dRow;
        c = col - dCol;
        while (r >= 0 && r < SIZE && c >= 0 && c < SIZE && grid[r][c] == p) {
            count++;
            r -= dRow;
            c -= dCol;
        }

        if (count >= 5) {
            return true;
        }
    }

    return false;
}

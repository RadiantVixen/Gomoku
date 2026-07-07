#include "Board.hpp"

Board::Board() {
    reset();
}

void Board::reset() {
    grid.assign(SIZE, std::vector<Player>(SIZE, Player::NONE));
    currentPlayer = Player::BLACK;
    winner = Player::NONE;
    gameOver = false;
    whiteStonesCaptured = 0;
    blackStonesCaptured = 0;
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

    if (checkWin(currentPlayer, row, col)) {
        winner = currentPlayer;
        gameOver = true;
    } else {
        currentPlayer = (currentPlayer == Player::BLACK) ? Player::WHITE : Player::BLACK;
    }
    this->checkCapture(row, col);
    this->isGameOver();
        
    return true;
}

Player Board::getCell(int row, int col) const {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
        return Player::NONE;
    }
    return grid[row][col];
}



std::vector<std::tuple<int, int>> Board::isCapturable(Player p, int row, int col){
    if (p == Player::NONE) return {};
    Player opponent = (p == Player::BLACK) ? Player::WHITE : Player::BLACK;
    int directions[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    std::vector<std::tuple<int, int>> result;
    for (int i = 0; i < 8; ++i) {
        int r = row;
        int c = col;
        int j = 0;
        while (r >= 0 && r < SIZE && c >= 0 && c < SIZE && j < 4) {
            result.push_back({r, c});
            j ++;
            r += directions[i][0];
            c += directions[i][1];
        }
        if (result.size() == 4 &&
            grid[std::get<0>(result[0])][std::get<1>(result[0])] == opponent &&
            grid[std::get<0>(result[1])][std::get<1>(result[1])] == p &&
            grid[std::get<0>(result[2])][std::get<1>(result[2])] == p &&
            grid[std::get<0>(result[3])][std::get<1>(result[3])] == opponent){
            std::cout << "Capture!\n";
            if (p == Player::BLACK){
                blackStonesCaptured += 2;
            } else {
                whiteStonesCaptured += 2;
            }
            return result;

        }
        result.clear();
    }
    return {};
}

bool Board::checkCapture(int row, int col) {
    auto table = isCapturable(currentPlayer, row, col);
    
    if (!table.empty()) {
        int r1 = std::get<0>(table[1]);
        int c1 = std::get<1>(table[1]);
        
        int r2 = std::get<0>(table[2]);
        int c2 = std::get<1>(table[2]);

        grid[r1][c1] = Player::NONE;
        grid[r2][c2] = Player::NONE;
        return true;
    }
    return false;
}

bool Board::isFreeThree(Player p, int row, int col) const{
    int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

    for (int i = 0; i < 4; ++i) {
        int count = 1;
        int count1 = 0;
        int dR = row + directions[i][0];
        int dC = col + directions[i][1];
        while (dR >= 0 && dR < SIZE && dC >= 0 && dC < SIZE){
            if (grid[dR][dC] == p){
                count ++;
            } else if (grid[dR][dC] == Player::NONE){
                count1 ++;
            } else{
                break;
            }
            dR += directions[i][0];
            dC += directions[i][1];
        }
        dR = row - directions[i][0];
        dC = col - directions[i][1];
        while (dR >= 0 && dR < SIZE && dC >= 0 && dC < SIZE){
            if (grid[dR][dC] == p){
                count ++;
            } else if (grid[dR][dC] == Player::NONE){
                count1 ++;
            } else{
                break;
            }
            dR -= directions[i][0];
            dC -= directions[i][1];
        }
        if (count >= 3 && count1 + count >= 5){
            return true;
        }
    }
    return false;
}

bool Board::doubleThree(Player p, int row, int col) const{
    int count = 0;
    for (int i = 0; i < 4; ++i) {
        if (this->isFreeThree(p, row, col)){
            count++;
        }
    }
    return count >= 2;
}


bool Board::isGameOver(){
    Player opponent = (currentPlayer == Player::BLACK) ? Player::WHITE : Player::BLACK;

    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
            if (grid[i][j] == Player::NONE){
                if (this->checkWin(currentPlayer, i, j) || this->checkWin(opponent, i, j))
                    return false;
                if (!this->isCapturable(currentPlayer, i, j).empty() || !this->isCapturable(opponent, i, j).empty())
                    return false;
            }
        }
    }
    std::cout << "jghkj\n";
    if (whiteStonesCaptured >= 2)
        winner = Player::BLACK;
    else if (blackStonesCaptured >= 2)
        winner = Player::WHITE;

    gameOver = true;
    return gameOver;
}


bool Board::checkWin(Player p,int row, int col) const {
    if (p == Player::NONE) return false;

    int directions[4][2] = {
        {0, 1},
        {1, 0},
        {1, 1},
        {1, -1}
    };

    for (int i = 0; i < 4; ++i) {
        int r = row + directions[i][0];
        int c = col + directions[i][1];
        int count = 1;
        while (r >= 0 && r < SIZE && c >= 0 && c < SIZE && grid[r][c] == p) {
            count++;
            r += directions[i][0];
            c += directions[i][1];
        }

        r = row - directions[i][0];
        c = col - directions[i][1];
        while (r >= 0 && r < SIZE && c >= 0 && c < SIZE && grid[r][c] == p) {
            count++;
            r -= directions[i][0];
            c -= directions[i][1];
        }

        if (count >= 5) {
            return true;
        }
    }

    return false;
}

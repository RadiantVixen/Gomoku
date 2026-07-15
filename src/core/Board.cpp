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

bool Board::isValidMove(int row, int col) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE ) {
        return false;
    }
    return grid[row][col] == Player::NONE;
}


void Board::undoMove(int row, int col) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
        return;
    }
    grid[row][col] = Player::NONE;
    currentPlayer = (currentPlayer == Player::BLACK) ? Player::WHITE : Player::BLACK;
    gameOver = false;
    winner = Player::NONE;
}

bool Board::makeMove(int row, int col) {
    if (gameOver || !isValidMove(row, col)) {
        return false;
    }

    grid[row][col] = currentPlayer;

    this->checkCapture(row, col);

    if (checkwin(currentPlayer, row, col)) {
        winner = currentPlayer;
        gameOver = true;
    } else {
        currentPlayer = (currentPlayer == Player::BLACK) ? Player::WHITE : Player::BLACK;
    }

    this->isGameOver();
        
    return true;
}

Player Board::getCell(int row, int col) const {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
        return Player::NONE;
    }
    return grid[row][col];
}

bool Board::checkCapture(Player p, int row, int col){
    if (p == Player::NONE) return false;
    Player opponent = (p == Player::BLACK) ? Player::WHITE : Player::BLACK;
    int directions[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    std::vector<std::tuple<int, int>> trap;
    for (int i = 0; i < 8; ++i) {
        int r = row;
        int c = col;
        int j = 0;
        while (r >= 0 && r < SIZE && c >= 0 && c < SIZE && j < 4) {
            trap.push_back({r, c});
            j ++;
            r += directions[i][0];
            c += directions[i][1];
        }
        if (trap.size() == 4 &&
            grid[std::get<0>(trap[0])][std::get<1>(trap[0])] == p &&
            grid[std::get<0>(trap[1])][std::get<1>(trap[1])] == opponent &&
            grid[std::get<0>(trap[2])][std::get<1>(trap[2])] == opponent &&
            grid[std::get<0>(trap[3])][std::get<1>(trap[3])] == p){
            std::cout << "Huh Captured!\n";
            grid[std::get<0>(trap[1])][std::get<1>(trap[1])] = Player::NONE;
            grid[std::get<0>(trap[2])][std::get<1>(trap[2])] = Player::NONE;


            if (p == Player::BLACK){
                blackStonesCaptured += 2;
            } else {
                whiteStonesCaptured += 2;
            }
            return true;
        }
        trap.clear();
    }
    return false;
}




bool Board::doubleThree(Player p, int row, int col) {
    int count = 0;
    for (int i = 0; i < 4; ++i) {
        if (this->isFreeThree(p, row, col)){
            count++;
        }
    }
    return count >= 2;
}


bool Board::isCapturable(Player p, int row, int col){
    if (p == Player::NONE) return 0;
    Player opponent = (p == Player::BLACK) ? Player::WHITE : Player::BLACK;
    int directions[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    std::vector<std::tuple<int, int>> trap;
    for (int k = 0; k < 8; ++k) {
        int r = i - directions[k][0];
        int c = j - directions[k][1];
        int l = 0;
        if (r >= 0 && r < SIZE && c >= 0 && c < SIZE && board.grid[r][c] != opponent && board.grid[r][c] != Player::NONE)
            continue;

        while (r >= 0 && r < SIZE && c >= 0 && c < SIZE && l < 4) {
            trap.push_back({r, c});
            l ++;
            r += directions[k][0];
            c += directions[k][1];
        }
        if (trap.size() == 4 &&
            (grid[std::get<0>(trap[0])][std::get<1>(trap[0])] == opponent  || grid[std::get<0>(trap[0])][std::get<1>(trap[0])] == Player::NONE) &&
            grid[std::get<0>(trap[1])][std::get<1>(trap[1])] == p &&
            grid[std::get<0>(trap[2])][std::get<1>(trap[2])] == p &&
            (grid[std::get<0>(trap[3])][std::get<1>(trap[3])] == Player::NONE || grid[std::get<0>(trap[3])][std::get<1>(trap[3])] == opponent)
        )
            return CAPTURABLE;
        
        trap.clear();
    }
    return 0;
}



int Board::checkFiveInRow(Player p,int row, int col)  {
    if (p == Player::NONE) return false;
    capturable = 0;

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
            if isCapturable(p, r, c):
            capturable++;
            r += directions[i][0];
            c += directions[i][1];
        }
        
        r = row - directions[i][0];
        c = col - directions[i][1];
        while (r >= 0 && r < SIZE && c >= 0 && c < SIZE && grid[r][c] == p) {
            count++;
            if isCapturable(p, r, c):
            capturable++;
            r -= directions[i][0];
            c -= directions[i][1];
        }
        
        if (count >= 5) {
            
            return 1 if (capturable >= 1) {
                return 2;
            }
        }
    }
    
    return 0;
}

bool Board::isGameOver(){
    Player opponent = (currentPlayer == Player::BLACK) ? Player::WHITE : Player::BLACK;
    gameOver = true;
    return gameOver;
}

bool Board::checkwin(Player p, int row, int col){
    if (if p == Player::BLACK && whiteStonesCaptured >= 10){
        winner = Player::BLACK;
        gameOver = true;
        return true;
    }
    else if (p == Player::WHITE && blackStonesCaptured >= 10){
        winner = Player::WHITE;
        gameOver = true;
        return true;
    }
    if (checkFiveInRow(p, row, col) == 1) {
        gameOver = true;
        return true;
    }
    return false;
}

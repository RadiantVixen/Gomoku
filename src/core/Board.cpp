#include "Board.hpp"
#include <algorithm>
#include <array>
#include <string>

namespace {

bool inBounds(int row, int col)
{
    return row >= 0 && row < Board::SIZE && col >= 0 && col < Board::SIZE;
}

Player opponentOf(Player player)
{
    return (player == Player::BLACK) ? Player::WHITE : Player::BLACK;
}

bool matchesCapturePattern(const Board& board, Player player, int row, int col, int dr, int dc)
{
    Player opponent = opponentOf(player);
    return inBounds(row + dr, col + dc) && inBounds(row + 2 * dr, col + 2 * dc) && inBounds(row + 3 * dr, col + 3 * dc)
        && board.getCell(row + dr, col + dc) == opponent
        && board.getCell(row + 2 * dr, col + 2 * dc) == opponent
        && board.getCell(row + 3 * dr, col + 3 * dc) == player;
}

std::string buildLineWithPlacement(const Board& board, Player player, int row, int col, int dr, int dc)
{
    std::string line;
    line.reserve(9);

    for (int offset = -4; offset <= 4; ++offset) {
        if (offset == 0) {
            line.push_back('X');
            continue;
        }

        int r = row + offset * dr;
        int c = col + offset * dc;
        if (!inBounds(r, c)) {
            line.push_back('O');
            continue;
        }

        Player cell = board.getCell(r, c);
        if (cell == Player::NONE) {
            line.push_back('.');
        } else if (cell == player) {
            line.push_back('X');
        } else {
            line.push_back('O');
        }
    }

    return line;
}

bool hasFreeThreePattern(const std::string& line)
{
    static const std::array<std::string, 3> patterns = {".XXX.", ".XX.X.", ".X.XX."};

    for (const std::string& pattern : patterns) {
        std::size_t pos = line.find(pattern);
        while (pos != std::string::npos) {
            if (pos <= 4 && 4 < pos + pattern.size()) {
                return true;
            }
            pos = line.find(pattern, pos + 1);
        }
    }

    return false;
}

std::vector<std::tuple<int, int>> collectWinningLine(const Board& board, Player player, int row, int col)
{
    const int directions[4][2] = {
        {0, 1},
        {1, 0},
        {1, 1},
        {1, -1}
    };

    for (const auto& direction : directions) {
        int dr = direction[0];
        int dc = direction[1];
        std::vector<std::tuple<int, int>> line;

        int r = row;
        int c = col;
        while (inBounds(r - dr, c - dc) && board.getCell(r - dr, c - dc) == player) {
            r -= dr;
            c -= dc;
        }

        while (inBounds(r, c) && board.getCell(r, c) == player) {
            line.push_back({r, c});
            r += dr;
            c += dc;
        }

        if (line.size() >= 5) {
            return line;
        }
    }

    return {};
}

bool pendingLineWasBroken(const Board& board, const std::vector<std::tuple<int, int>>& cells)
{
    for (const auto& cell : cells) {
        int row = std::get<0>(cell);
        int col = std::get<1>(cell);
        if (board.getCell(row, col) == Player::NONE) {
            return true;
        }
    }

    return false;
}

}

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
    pendingWin = false;
    pendingWinner = Player::NONE;
    pendingWinningCells.clear();
}

bool Board::isValidMove(int row, int col) const {
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
    pendingWin = false;
    pendingWinner = Player::NONE;
    pendingWinningCells.clear();
}

bool Board::makeMove(int row, int col) {
    if (gameOver || !isValidMove(row, col)) {
        return false;
    }

    Player movingPlayer = currentPlayer;
    grid[row][col] = currentPlayer;

    bool captured = this->checkCapture(movingPlayer, row, col);

    if (!captured && this->doubleThree(movingPlayer, row, col)) {
        grid[row][col] = Player::NONE;
        return false;
    }

    if (checkwin(movingPlayer, row, col)) {
        winner = movingPlayer;
        gameOver = true;
        pendingWin = false;
        pendingWinner = Player::NONE;
        pendingWinningCells.clear();
        return true;
    }

    if (pendingWin && movingPlayer != pendingWinner) {
        if (!pendingLineWasBroken(*this, pendingWinningCells)) {
            winner = pendingWinner;
            gameOver = true;
            pendingWin = false;
            pendingWinner = Player::NONE;
            pendingWinningCells.clear();
            return true;
        }

        pendingWin = false;
        pendingWinner = Player::NONE;
        pendingWinningCells.clear();
    }

    currentPlayer = (currentPlayer == Player::BLACK) ? Player::WHITE : Player::BLACK;

    // this->isGameOver();
        
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
    bool captured = false;
    int capturedStones = 0;
    const int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

    for (const auto& direction : directions) {
        int dr = direction[0];
        int dc = direction[1];

        if (matchesCapturePattern(*this, p, row, col, dr, dc)) {
            grid[row + dr][col + dc] = Player::NONE;
            grid[row + 2 * dr][col + 2 * dc] = Player::NONE;
            capturedStones += 2;
            captured = true;
        }

        if (matchesCapturePattern(*this, p, row, col, -dr, -dc)) {
            grid[row - dr][col - dc] = Player::NONE;
            grid[row - 2 * dr][col - 2 * dc] = Player::NONE;
            capturedStones += 2;
            captured = true;
        }
    }

    if (captured) {
        if (p == Player::BLACK) {
            blackStonesCaptured += capturedStones;
        } else {
            whiteStonesCaptured += capturedStones;
        }
    }

    return captured;
}




bool Board::doubleThree(Player p, int row, int col) {
    int count = 0;
    const int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

    for (const auto& direction : directions) {
        std::string line = buildLineWithPlacement(*this, p, row, col, direction[0], direction[1]);
        if (hasFreeThreePattern(line)) {
            count++;
        }
    }
    return count >= 2;
}


bool Board::isCapturable(Player p, int i, int j){
    if (p == Player::NONE) return 0;
    Player opponent = opponentOf(p);
    const int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

    for (const auto& direction : directions) {
        int dr = direction[0];
        int dc = direction[1];

        if (getCell(i - dr, j - dc) == opponent &&
            getCell(i + dr, j + dc) == p &&
            getCell(i + 2 * dr, j + 2 * dc) == Player::NONE) {
            return CAPTURABLE;
        }

        if (getCell(i - 2 * dr, j - 2 * dc) == Player::NONE &&
            getCell(i - dr, j - dc) == p &&
            getCell(i + dr, j + dc) == opponent) {
            return CAPTURABLE;
        }
    }
    return 0;
}



int Board::checkFiveInRow(Player p,int row, int col)  {
    if (p == Player::NONE) return false;
    int capturable = 0;

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
            if (this->isCapturable(p, r, c))
                capturable++;
            r += directions[i][0];
            c += directions[i][1];
        }
        
        r = row - directions[i][0];
        c = col - directions[i][1];
        while (r >= 0 && r < SIZE && c >= 0 && c < SIZE && grid[r][c] == p) {
            count++;
            if (this->isCapturable(p, r, c))
                capturable++;
            
            r -= directions[i][0];
            c -= directions[i][1];
        }
        
        if (count >= 5) {
            if (capturable >= 1)
                return 2;
            return 1;
        }
    }
    
    return 0;
}

bool Board::isGameOver(){
    return gameOver;
} 

bool Board::checkwin(Player p, int row, int col){
    if (p == Player::BLACK && blackStonesCaptured >= 10){
        winner = Player::BLACK;
        gameOver = true;
        pendingWin = false;
        pendingWinner = Player::NONE;
        pendingWinningCells.clear();
        return true;
    }
    else if (p == Player::WHITE && whiteStonesCaptured >= 10){
        winner = Player::WHITE;
        gameOver = true;
        pendingWin = false;
        pendingWinner = Player::NONE;
        pendingWinningCells.clear();
        return true;
    }
    int lineStatus = checkFiveInRow(p, row, col);
    if (lineStatus == 1) {
        gameOver = true;
        pendingWin = false;
        pendingWinner = Player::NONE;
        pendingWinningCells.clear();
        return true;
    }
    if (lineStatus == 2) {
        pendingWin = true;
        pendingWinner = p;
        pendingWinningCells = collectWinningLine(*this, p, row, col);
        gameOver = false;
        winner = Player::NONE;
    }
    return false;
}

bool Board::isFreeThree(Player, int, int) {
    return false;
}



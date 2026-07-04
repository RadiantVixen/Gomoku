#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <tuple>

enum class Player {
    NONE = 0,
    BLACK = 1,
    WHITE = 2
};

class Board {
public:
    static const int SIZE = 19;

    Board();
    ~Board() = default;

    void reset();
    bool makeMove(int row, int col);
    bool isValidMove(int row, int col) const;
    Player getCell(int row, int col) const;
    Player getCurrentPlayer() const { return currentPlayer; }
    
    bool checkWin(int row, int col) const;
    bool checkCapture(int row, int col) const;
    std::vector<std::tuple<int, int>> isCapturable(int row, int col);
    bool isThereaNeed(int row, int col);
    bool doubleThrees(int row, int col);
    Player getWinner() const { return winner; }
    bool isGameOver() const { return gameOver; }

private:
    std::vector<std::vector<Player>> grid;
    Player currentPlayer;
    Player winner;
    bool gameOver;

    bool checkDirection(int row, int col, int dRow, int dCol) const;
};

#endif

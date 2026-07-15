#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <tuple>
#include <iostream>

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
    Player getCurrentPlayer() const { return currentPlayer;}
    bool checkFiveInRow(Player p, int row, int col) const;
    bool checkCapture(int row, int col);
    bool checkPotentialWin(Player p,int row, int col) const;
    std::vector<std::tuple<int, int>> isCapturable(Player p, int row, int col);
    bool doubleThree(Player p, int row, int col) const;
    bool isFreeThree(Player p, int row, int col) const;
    Player getWinner() const { return winner; }
    bool isGameOver();
    
    private:
    std::vector<std::vector<Player>> grid;
    Player currentPlayer;
    Player winner;
    bool gameOver;
    int whiteStonesCaptured;
    int blackStonesCaptured;
    bool checkDirection(int row, int col, int dRow, int dCol) const;
};

#endif

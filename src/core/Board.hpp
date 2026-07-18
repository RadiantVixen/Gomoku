#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <tuple>
#include <iostream>
#include <list>

const int MAX_DEPTH = 10;
long const int WINNING = 99999999999;
long const int LOSING = -99999999999;
const int CAPTURABLE_FIVE = 100000;
const int CAPTURE = 10000;
const int OPEN_FOUR = 10000;
const int OPEN_THREE = 1000;
const int CLOSED_FOUR = 1000;
const int CLOSED_THREE = 100;
const int CAPTURABLE = -100;


enum class Player {
    NONE = 0,
    BLACK = 1,
    WHITE = 2
};

class Board {
public:
    static const int SIZE = 19;
    std::vector<std::vector<Player>> grid;
    Player currentPlayer;
    Player winner;
    bool gameOver;
    int whiteStonesCaptured;
    int blackStonesCaptured;
    bool pendingWin;
    Player pendingWinner;
    std::vector<std::tuple<int, int>> pendingWinningCells;

    Board();
    ~Board() = default;

    void reset();
    Player getCell(int row, int col) const;
    bool isValidMove(int row, int col) const;
    bool makeMove(int row, int col);
    bool isCapturable(Player p, int row, int col);
    int checkFiveInRow(Player p, int row, int col);
    bool checkCapture(Player p, int row, int col);
    bool doubleThree(Player p, int row, int col);
    bool isFreeThree(Player p, int row, int col);
    bool isGameOver();
    bool checkwin(Player p, int row, int col);
    Player getCurrentPlayer() const { return currentPlayer;}
    Player getWinner() const { return winner; }
    void undoMove(int row, int col);

    
};

#endif



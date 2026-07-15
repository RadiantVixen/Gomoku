#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <tuple>
#include <iostream>


const int MAX_DEPTH = 10;
const int WINNING = 99999999999;
const int LOSING = -99999999999;
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

    Board();
    ~Board() = default;

    void reset();
    Player getCell(int row, int col);
    bool isValidMove(int row, int col);
    bool makeMove(int row, int col);
    int checkFiveInRow(Player p, int row, int col);
    bool checkCapture(int row, int col);
    bool doubleThree(Player p, int row, int col);
    bool isFreeThree(Player p, int row, int col);
    bool isGameOver();
    bool checkwin(Player p, int row, int col);
    Player getCurrentPlayer() { return currentPlayer;}
    Player getWinner() { return winner; }

    
};

#endif

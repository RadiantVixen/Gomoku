#include <iostream>
#include <tuple>
#include "../core/Board.hpp"
#include "../core/AI.hpp"

int main() {
    Board board;
    board.reset();

    // Scenario: Black has four in a row at row 9, cols 9-12
    board.grid[9][9] = Player::BLACK;
    board.grid[9][10] = Player::BLACK;
    board.grid[9][11] = Player::BLACK;
    board.grid[9][12] = Player::BLACK;

    // Add some white stones elsewhere
    board.grid[8][10] = Player::WHITE;
    board.grid[10][10] = Player::WHITE;

    // Set current player to BLACK
    board.currentPlayer = Player::BLACK;

    // Call the AI function
    int maxDepth = 2;
    auto best = FindBestMove(board, maxDepth);
    int r = std::get<0>(best);
    int c = std::get<1>(best);

    std::cout << "Best move returned by FindBestMove: (" << r << ", " << c << ")\n";

    return 0;
}




#include "core/Board.hpp"
#include "gui/GUI.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    Board board;
    GUI gui(board);

    if (!gui.init()) {
        std::cerr << "Failed to initialize GUI." << std::endl;
        return 1;
    }

    
    std::cout << "Starting Gomoku game..." << std::endl;
    gui.run();

    return 0;
}

// #include "core/Board.hpp"
#include "core/AI.hpp"
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
    
    auto candidates = CandidateOrdering(board);
    // for (const auto& move : candidates) {
    //     std::cout << "Candidate Move: (" << std::get<0>(move) << ", "
    //               << std::get<1>(move) << ')' << std::endl;
    // }
    if (!candidates.empty()) {
        const auto& move = candidates.front();
        std::cout << "Candidate Move: (" << std::get<0>(move) << ", "
                  << std::get<1>(move) << ')' << std::endl;
        std::cout << "Heuristic Evaluation: " << heuristicEvaluation(board) << std::endl;
    }
    return 0;
}

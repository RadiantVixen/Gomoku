#include <algorithm>
#include <chrono>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../src/core/Board.hpp"
#include "../src/core/Heuristic.hpp"

using std::cout;
using std::endl;

namespace {

struct TestStats {
    int passed = 0;
    int failed = 0;
};

void setCell(Board& board, int row, int col, Player player) {
    if (row >= 0 && row < Board::SIZE && col >= 0 && col < Board::SIZE) {
        board.grid[row][col] = player;
    }
}

void clearBoard(Board& board) {
    board.reset();
}

char toSymbol(Player player) {
    if (player == Player::BLACK) {
        return 'X';
    }
    if (player == Player::WHITE) {
        return 'O';
    }
    return '.';
}

Player parsePlayer(const std::string& raw) {
    std::string s;
    for (char ch : raw) {
        s.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
    }
    if (s == "white" || s == "w") {
        return Player::WHITE;
    }
    return Player::BLACK;
}

void printHeader(const std::string& title) {
    cout << "\n============================================================\n";
    cout << title << '\n';
    cout << "============================================================\n";
}

void printCase(const std::string& id, const std::string& description) {
    cout << "\n[" << id << "] " << description << '\n';
}

void printBoardWindow(const Board& board, int minRow, int maxRow, int minCol, int maxCol) {
    cout << "     ";
    for (int col = minCol; col <= maxCol; ++col) {
        cout << (col % 10);
    }
    cout << '\n';
    for (int row = minRow; row <= maxRow; ++row) {
        cout << (row < 10 ? "  " : " ") << row << "  ";
        for (int col = minCol; col <= maxCol; ++col) {
            cout << toSymbol(board.getCell(row, col));
        }
        cout << '\n';
    }
}

void printFullBoard(const Board& board) {
    printBoardWindow(board, 0, Board::SIZE - 1, 0, Board::SIZE - 1);
}

void loadMap(Board& board, const std::vector<std::string>& rows, int startRow = 0, int startCol = 0) {
    clearBoard(board);
    for (int r = 0; r < static_cast<int>(rows.size()); ++r) {
        for (int c = 0; c < static_cast<int>(rows[r].size()); ++c) {
            int rr = startRow + r;
            int cc = startCol + c;
            if (rr < 0 || rr >= Board::SIZE || cc < 0 || cc >= Board::SIZE) {
                continue;
            }
            char ch = rows[r][c];
            if (ch == 'X' || ch == 'x') {
                setCell(board, rr, cc, Player::BLACK);
            } else if (ch == 'O' || ch == 'o') {
                setCell(board, rr, cc, Player::WHITE);
            } else {
                setCell(board, rr, cc, Player::NONE);
            }
        }
    }
}

bool loadMapFromFile(Board& board, const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }

    clearBoard(board);
    std::vector<std::string> rows;
    rows.reserve(Board::SIZE);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        std::string filtered;
        for (char ch : line) {
            if (ch == '.' || ch == 'X' || ch == 'x' || ch == 'O' || ch == 'o') {
                filtered.push_back(ch);
            }
        }
        if (!filtered.empty()) {
            rows.push_back(filtered);
        }
    }

    if (static_cast<int>(rows.size()) != Board::SIZE) {
        return false;
    }

    for (int row = 0; row < Board::SIZE; ++row) {
        if (static_cast<int>(rows[row].size()) != Board::SIZE) {
            return false;
        }
    }

    loadMap(board, rows);
    return true;
}

void expectEq(TestStats& stats, int actual, int expected, const std::string& label) {
    if (actual == expected) {
        ++stats.passed;
        cout << "  ✅ PASS  " << label << " | expected=" << expected << " got=" << actual << '\n';
    } else {
        ++stats.failed;
        cout << "  ❌ FAIL  " << label << " | expected=" << expected << " got=" << actual << '\n';
    }
}

void expectTrue(TestStats& stats, bool condition, const std::string& label) {
    if (condition) {
        ++stats.passed;
        cout << "  ✅ PASS  " << label << '\n';
    } else {
        ++stats.failed;
        cout << "  ❌ FAIL  " << label << '\n';
    }
}

bool containsMove(const std::vector<ScoredMove>& moves, int row, int col) {
    for (const ScoredMove& move : moves) {
        if (move.row == row && move.col == col) {
            return true;
        }
    }
    return false;
}

void printBreakdown(const Board& board, int row, int col, Player player) {
    MoveScoreBreakdown details = evaluateMoveDetailed(board, row, col, player);
    cout << "    move(" << row << "," << col << ")"
         << " off=" << details.offensiveScore
         << " cap+=" << details.captureBonus
         << " cap-=" << details.capturePenalty
         << " opp-=" << details.opponentThreatPenalty
         << " center=" << details.centerBonus
         << " => final=" << details.finalScore
         << (details.immediateWin ? " [WIN]" : "") << '\n';
}

void printTopMovesWithBreakdown(const Board& board, Player player, int top) {
    std::vector<ScoredMove> ranked = getRankedMoves(board, player);
    int limit = std::min(top, static_cast<int>(ranked.size()));
    cout << "Top " << limit << " ranked moves:\n";
    for (int i = 0; i < limit; ++i) {
        cout << "  #" << (i + 1) << " ";
        printBreakdown(board, ranked[i].row, ranked[i].col, player);
    }
}

void runAllTests() {
    TestStats stats;
    Board board;

    printHeader("Evaluator Test Suite (Readable Mode)");

    printCase("S1", "Direction helpers and pattern scoring");
    loadMap(board, {"XX."});
    printBoardWindow(board, 0, 0, 0, 5);
    expectEq(stats, countStonesInDirection(board, 0, 2, 0, 1, Player::BLACK), 3,
             "countStonesInDirection on XX.");

    loadMap(board, {".XXX."});
    printBoardWindow(board, 0, 0, 0, 6);
    LineInfo info = getLineInfo(board, 0, 4, 0, 1, Player::BLACK);
    expectEq(stats, info.count, 4, "getLineInfo.count");
    expectEq(stats, info.openEnds, 2, "getLineInfo.openEnds");
    expectEq(stats, getPatternScore(4, 2), 100000, "getPatternScore open four");

    printCase("S2", "Offensive scoring scenarios");
    loadMap(board, {".XXX.."});
    printBoardWindow(board, 0, 0, 0, 6);
    expectEq(stats, calculateOffensiveScore(board, 0, 4, Player::BLACK), 100000,
             "open four at (0,4)");

    loadMap(board, {"OXXX.."});
    printBoardWindow(board, 0, 0, 0, 6);
    expectEq(stats, calculateOffensiveScore(board, 0, 4, Player::BLACK), 10000,
             "broken four at (0,4)");

    printCase("S3", "Capture vulnerability and immunity on immediate win");
    loadMap(board, {"OX..."});
    printBoardWindow(board, 0, 0, 0, 5);
    int riskOff = calculateOffensiveScore(board, 0, 2, Player::BLACK);
    expectEq(stats, checkCaptureVulnerability(board, 0, 2, Player::BLACK, riskOff), 50000,
             "capture vulnerability penalty");
    printBreakdown(board, 0, 2, Player::BLACK);

    loadMap(board, {".XXXX"});
    printBoardWindow(board, 0, 0, 0, 5);
    MoveScoreBreakdown winMove = evaluateMoveDetailed(board, 0, 0, Player::BLACK);
    expectTrue(stats, winMove.immediateWin, "safe winning move detected");
    expectEq(stats, winMove.capturePenalty, 0, "winning move not penalized");

    printCase("S4", "Move ranking and near-stone filter");
    loadMap(board, {
        "...................",
        "...................",
        "...................",
        "...................",
        "...................",
        "...................",
        "...................",
        "...................",
        ".........X.........",
        "...................",
        "...................",
        "...................",
        "...................",
        "...................",
        "...................",
        "...................",
        "...................",
        "...................",
        "..................."
    });
    std::vector<ScoredMove> filtered = getRankedMoves(board, Player::BLACK);
    expectEq(stats, static_cast<int>(filtered.size()), 24,
             "near-stone filter candidate count around one center stone");
    printTopMovesWithBreakdown(board, Player::BLACK, 5);

    printCase("S5", "Additional factors: center bonus and capture bonus");
    clearBoard(board);
    MoveScoreBreakdown center = evaluateMoveDetailed(board, 9, 9, Player::BLACK);
    MoveScoreBreakdown corner = evaluateMoveDetailed(board, 0, 0, Player::BLACK);
    expectTrue(stats, center.finalScore > corner.finalScore, "center preferred over corner");

    loadMap(board, {".OOX."});
    printBoardWindow(board, 0, 0, 0, 5);
    MoveScoreBreakdown capture = evaluateMoveDetailed(board, 0, 0, Player::BLACK);
    expectEq(stats, capture.captureBonus, 30000, "capture bonus +30000 per pair");
    printBreakdown(board, 0, 0, Player::BLACK);

    printCase("S6", "Double-three move exclusion");
    clearBoard(board);
    setCell(board, 9, 8, Player::BLACK);
    setCell(board, 9, 10, Player::BLACK);
    setCell(board, 8, 9, Player::BLACK);
    setCell(board, 10, 9, Player::BLACK);
    printBoardWindow(board, 6, 12, 6, 12);
    std::vector<ScoredMove> ranked = getRankedMoves(board, Player::BLACK);
    expectTrue(stats, !containsMove(ranked, 9, 9), "(9,9) excluded due to double-three");

    printCase("S7", "Performance sanity check");
    loadMap(board, {
        "...................",
        "........X..........",
        ".......OXO.........",
        "......XXOXX........",
        ".....OOXXXO........",
        "......XOOXX........",
        ".......XOX.........",
        "........O..........",
        "..................."
    }, 4, 0);
    auto start = std::chrono::high_resolution_clock::now();
    const int iterations = 100;
    for (int i = 0; i < iterations; ++i) {
        (void)getRankedMoves(board, Player::BLACK);
    }
    auto end = std::chrono::high_resolution_clock::now();
    double totalMs = std::chrono::duration<double, std::milli>(end - start).count();
    double avgMs = totalMs / iterations;
    cout << "  timing: total=" << totalMs << " ms | avg=" << avgMs << " ms/eval\n";
    expectTrue(stats, avgMs < 100.0, "average runtime under 100 ms (debug-build threshold)");

    printHeader("Final Summary");
    cout << "passed=" << stats.passed << " failed=" << stats.failed << '\n';
}

int runMapMode(const std::string& path, Player player, int top) {
    Board board;
    if (!loadMapFromFile(board, path)) {
        std::cerr << "Error: failed to load map from '" << path << "'.\n";
        std::cerr << "Expected format: 19 lines of 19 chars using only '.', 'X', 'O'.\n";
        return 1;
    }

    printHeader("Real Map Evaluator Mode");
    cout << "map file: " << path << '\n';
    cout << "player  : " << (player == Player::BLACK ? "BLACK (X)" : "WHITE (O)") << '\n';
    cout << "top N   : " << top << '\n';

    cout << "\nLoaded board:\n";
    printFullBoard(board);

    std::vector<ScoredMove> ranked = getRankedMoves(board, player);
    cout << "\nCandidate moves after filtering: " << ranked.size() << '\n';
    printTopMovesWithBreakdown(board, player, top);

    if (!ranked.empty()) {
        cout << "\nBest move suggestion: (" << ranked[0].row << "," << ranked[0].col
             << ") score=" << ranked[0].score << '\n';
    }
    return 0;
}

}

int main(int argc, char** argv) {
    if (argc == 1) {
        runAllTests();
        return 0;
    }

    std::string mapPath;
    Player player = Player::BLACK;
    int top = 10;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--map" && i + 1 < argc) {
            mapPath = argv[++i];
        } else if (arg == "--player" && i + 1 < argc) {
            player = parsePlayer(argv[++i]);
        } else if (arg == "--top" && i + 1 < argc) {
            top = std::max(1, std::stoi(argv[++i]));
        } else {
            std::cerr << "Unknown argument: " << arg << '\n';
            std::cerr << "Usage:\n";
            std::cerr << "  ./tests/TestHeuristic\n";
            std::cerr << "  ./tests/TestHeuristic --map <path> [--player black|white] [--top N]\n";
            return 1;
        }
    }

    if (mapPath.empty()) {
        std::cerr << "Error: --map <path> is required when using CLI mode.\n";
        return 1;
    }

    return runMapMode(mapPath, player, top);
}

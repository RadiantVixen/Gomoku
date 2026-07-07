#include "GUI.hpp"
#include <cmath>
#include <iostream>

// Complete 8x8 bitmap font for ASCII 32 to 126
const unsigned char font8x8[95][8] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // ' ' (32)
    {0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x18, 0x00}, // '!' (33)
    {0x66, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00}, // '"' (34)
    {0x24, 0x7E, 0x24, 0x24, 0x7E, 0x24, 0x24, 0x00}, // '#' (35)
    {0x18, 0x3E, 0x60, 0x3C, 0x06, 0x7C, 0x18, 0x00}, // '$' (36)
    {0x00, 0x62, 0x66, 0x0C, 0x18, 0x30, 0x66, 0x46}, // '%' (37)
    {0x3C, 0x66, 0x3C, 0x38, 0x67, 0x66, 0x3F, 0x00}, // '&' (38)
    {0x06, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00}, // '\'' (39)
    {0x0C, 0x18, 0x30, 0x30, 0x30, 0x18, 0x0C, 0x00}, // '(' (40)
    {0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x18, 0x30, 0x00}, // ')' (41)
    {0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00}, // '*' (42)
    {0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00}, // '+' (43)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x08}, // ',' (44)
    {0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00}, // '-' (45)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00}, // '.' (46)
    {0x00, 0x02, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40}, // '/' (47)
    {0x3C, 0x66, 0x6E, 0x76, 0x66, 0x66, 0x3C, 0x00}, // '0' (48)
    {0x18, 0x1C, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00}, // '1' (49)
    {0x3C, 0x66, 0x06, 0x0C, 0x30, 0x60, 0x7E, 0x00}, // '2' (50)
    {0x3C, 0x66, 0x06, 0x1C, 0x06, 0x66, 0x3C, 0x00}, // '3' (51)
    {0x06, 0x0E, 0x1E, 0x36, 0x7E, 0x06, 0x06, 0x00}, // '4' (52)
    {0x7E, 0x60, 0x7C, 0x06, 0x06, 0x66, 0x3C, 0x00}, // '5' (53)
    {0x3C, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x3C, 0x00}, // '6' (54)
    {0x7E, 0x06, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x00}, // '7' (55)
    {0x3C, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0x00}, // '8' (56)
    {0x3C, 0x66, 0x66, 0x3E, 0x06, 0x0C, 0x38, 0x00}, // '9' (57)
    {0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00}, // ':' (58)
    {0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x08}, // ';' (59)
    {0x00, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x00, 0x00}, // '<' (60)
    {0x00, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00, 0x00}, // '=' (61)
    {0x00, 0x30, 0x18, 0x0C, 0x18, 0x30, 0x00, 0x00}, // '>' (62)
    {0x3C, 0x66, 0x06, 0x0C, 0x18, 0x00, 0x18, 0x00}, // '?' (63)
    {0x3C, 0x66, 0x6E, 0x6A, 0x6E, 0x60, 0x3C, 0x00}, // '@' (64)
    {0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00}, // 'A' (65)
    {0x7C, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x7C, 0x00}, // 'B' (66)
    {0x3C, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3C, 0x00}, // 'C' (67)
    {0x78, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0x78, 0x00}, // 'D' (68)
    {0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x7E, 0x00}, // 'E' (69)
    {0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x60, 0x00}, // 'F' (70)
    {0x3C, 0x66, 0x60, 0x6E, 0x66, 0x66, 0x3C, 0x00}, // 'G' (71)
    {0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00}, // 'H' (72)
    {0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00}, // 'I' (73)
    {0x1F, 0x04, 0x04, 0x04, 0x04, 0x24, 0x1C, 0x00}, // 'J' (74)
    {0x66, 0x6C, 0x78, 0x70, 0x78, 0x6C, 0x66, 0x00}, // 'K' (75)
    {0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x00}, // 'L' (76)
    {0x63, 0x77, 0x7F, 0x6B, 0x63, 0x63, 0x63, 0x00}, // 'M' (77)
    {0x66, 0x6E, 0x7E, 0x76, 0x66, 0x66, 0x66, 0x00}, // 'N' (78)
    {0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00}, // 'O' (79)
    {0x7C, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x00}, // 'P' (80)
    {0x3C, 0x66, 0x66, 0x66, 0x6E, 0x7C, 0x0F, 0x00}, // 'Q' (81)
    {0x7C, 0x66, 0x66, 0x7C, 0x78, 0x6C, 0x66, 0x00}, // 'R' (82)
    {0x3C, 0x66, 0x30, 0x1C, 0x06, 0x66, 0x3C, 0x00}, // 'S' (83)
    {0x7E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00}, // 'T' (84)
    {0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00}, // 'U' (85)
    {0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00}, // 'V' (86)
    {0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00}, // 'W' (87)
    {0x66, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x66, 0x00}, // 'X' (88)
    {0x66, 0x66, 0x66, 0x3C, 0x08, 0x08, 0x08, 0x00}, // 'Y' (89)
    {0x7E, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x7E, 0x00}, // 'Z' (90)
    {0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00}, // '[' (91)
    {0x00, 0x40, 0x30, 0x18, 0x0C, 0x06, 0x02, 0x00}, // '\\' (92)
    {0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0x00}, // ']' (93)
    {0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00}, // '^' (94)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF}, // '_' (95)
    {0x18, 0x18, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00}, // '`' (96)
    {0x00, 0x00, 0x3C, 0x06, 0x3E, 0x66, 0x3B, 0x00}, // 'a' (97)
    {0x60, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x7C, 0x00}, // 'b' (98)
    {0x00, 0x00, 0x3C, 0x66, 0x60, 0x66, 0x3C, 0x00}, // 'c' (99)
    {0x06, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x3E, 0x00}, // 'd' (100)
    {0x00, 0x00, 0x3C, 0x66, 0x7E, 0x60, 0x3C, 0x00}, // 'e' (101)
    {0x0E, 0x18, 0x3E, 0x18, 0x18, 0x18, 0x18, 0x00}, // 'f' (102)
    {0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x3C}, // 'g' (103)
    {0x60, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x00}, // 'h' (104)
    {0x08, 0x00, 0x18, 0x08, 0x08, 0x08, 0x1C, 0x00}, // 'i' (105)
    {0x0C, 0x00, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x38}, // 'j' (106)
    {0x60, 0x60, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0x00}, // 'k' (107)
    {0x1C, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00}, // 'l' (108)
    {0x00, 0x00, 0x66, 0x7F, 0x6B, 0x63, 0x63, 0x00}, // 'm' (109)
    {0x00, 0x00, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x00}, // 'n' (110)
    {0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x3C, 0x00}, // 'o' (111)
    {0x00, 0x00, 0x7C, 0x66, 0x66, 0x7C, 0x60, 0x60}, // 'p' (112)
    {0x00, 0x00, 0x3E, 0x66, 0x66, 0x3E, 0x06, 0x07}, // 'q' (113)
    {0x00, 0x00, 0x7C, 0x66, 0x60, 0x60, 0x60, 0x00}, // 'r' (114)
    {0x00, 0x00, 0x3E, 0x60, 0x3C, 0x06, 0x7C, 0x00}, // 's' (115)
    {0x18, 0x18, 0x7E, 0x18, 0x18, 0x18, 0x0E, 0x00}, // 't' (116)
    {0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3B, 0x00}, // 'u' (117)
    {0x00, 0x00, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00}, // 'v' (118)
    {0x00, 0x00, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00}, // 'w' (119)
    {0x00, 0x00, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x00}, // 'x' (120)
    {0x00, 0x00, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x3C}, // 'y' (121)
    {0x00, 0x00, 0x7E, 0x0C, 0x18, 0x30, 0x7E, 0x00}  // 'z' (122)
};

GUI::GUI(Board& b)
    : board(b), window(nullptr), renderer(nullptr), running(false),
      moveCount(0), lastMoveRow(-1), lastMoveCol(-1), startTicks(0) {}

GUI::~GUI() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

bool GUI::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    window = SDL_CreateWindow("Gomoku",
                             SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    startTicks = SDL_GetTicks();

    return true;
}

void GUI::run() {
    running = true;
    while (running) {
        handleEvents();
        render();
        SDL_Delay(8);
    }
}

void GUI::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                int x = event.button.x;
                int y = event.button.y;

                // Reset button region
                int btnX = BOARD_AREA_SIZE + 40;
                int btnW = SIDEBAR_WIDTH - 80;
                int btnY = WINDOW_HEIGHT - 80;
                int btnH = 44;
                if (x >= btnX && x <= btnX + btnW && y >= btnY && y <= btnY + btnH) {
                    board.reset();
                    moveCount = 0;
                    lastMoveRow = -1;
                    lastMoveCol = -1;
                    continue;
                }

                // Board click
                int boardX = x - BOARD_PADDING;
                int boardY = y - BOARD_PADDING;
                int col = (boardX + CELL_SIZE / 2) / CELL_SIZE;
                int row = (boardY + CELL_SIZE / 2) / CELL_SIZE;

                if (row >= 0 && row < Board::SIZE && col >= 0 && col < Board::SIZE) {
                    int intersectionX = BOARD_PADDING + col * CELL_SIZE;
                    int intersectionY = BOARD_PADDING + row * CELL_SIZE;
                    double dist = std::sqrt(std::pow(x - intersectionX, 2) + std::pow(y - intersectionY, 2));

                    if (dist <= 18.0) {
                        if (board.makeMove(row, col)) {
                            moveCount++;
                            lastMoveRow = row;
                            lastMoveCol = col;
                        }
                    }
                }
            }
        }
    }
}

void GUI::render() {
    // Dark background behind everything
    SDL_SetRenderDrawColor(renderer, 18, 18, 24, 255);
    SDL_RenderClear(renderer);

    drawBoard();
    drawStones();
    drawLastMoveMarker();
    drawHoverGhost();
    drawCoordinateLabels();
    drawSidebar();

    SDL_RenderPresent(renderer);
}

// ─── Board ───────────────────────────────────────────────────────────────────

void GUI::drawBoard() {
    // Outer shadow / frame
    SDL_Rect outerFrame = {4, 4, BOARD_AREA_SIZE - 8, BOARD_AREA_SIZE - 8};
    SDL_SetRenderDrawColor(renderer, 12, 10, 8, 255);
    SDL_RenderFillRect(renderer, &outerFrame);

    // Wood board fill
    SDL_Rect boardRect = {8, 8, BOARD_AREA_SIZE - 16, BOARD_AREA_SIZE - 16};
    SDL_SetRenderDrawColor(renderer, 222, 178, 110, 255);
    SDL_RenderFillRect(renderer, &boardRect);

    // Subtle warm overlay gradient (simulated by horizontal bands)
    for (int y = 8; y < BOARD_AREA_SIZE - 8; y += 6) {
        int phase = (y * 3) % 40;
        int mod = (phase < 20) ? phase : 40 - phase;
        Uint8 r = (Uint8)std::min(255, 222 + mod / 4);
        Uint8 g = (Uint8)std::min(255, 178 - mod / 3);
        Uint8 b = (Uint8)std::max(0, 110 - mod / 2);
        SDL_SetRenderDrawColor(renderer, r, g, b, 18);
        SDL_Rect band = {8, y, BOARD_AREA_SIZE - 16, 6};
        SDL_RenderFillRect(renderer, &band);
    }

    // Grid lines — thin elegant lines
    SDL_SetRenderDrawColor(renderer, 50, 35, 18, 180);
    for (int i = 0; i < Board::SIZE; ++i) {
        int pos = BOARD_PADDING + i * CELL_SIZE;
        SDL_RenderDrawLine(renderer, pos, BOARD_PADDING, pos, BOARD_PADDING + (Board::SIZE - 1) * CELL_SIZE);
        SDL_RenderDrawLine(renderer, BOARD_PADDING, pos, BOARD_PADDING + (Board::SIZE - 1) * CELL_SIZE, pos);
    }

    // Star points (hoshi)
    int starCoords[3] = {3, 9, 15};
    for (int r : starCoords) {
        for (int c : starCoords) {
            int cx = BOARD_PADDING + c * CELL_SIZE;
            int cy = BOARD_PADDING + r * CELL_SIZE;
            drawDisk(cx, cy, 4, SDL_Color{50, 35, 18, 220});
        }
    }
}

// ─── Stones ──────────────────────────────────────────────────────────────────

void GUI::drawStones() {
    for (int r = 0; r < Board::SIZE; ++r) {
        for (int c = 0; c < Board::SIZE; ++c) {
            Player p = board.getCell(r, c);
            if (p != Player::NONE) {
                int cx = BOARD_PADDING + c * CELL_SIZE;
                int cy = BOARD_PADDING + r * CELL_SIZE;
                drawStone(cx, cy, 17, p);
            }
        }
    }
}

// ─── Last move marker ────────────────────────────────────────────────────────

void GUI::drawLastMoveMarker() {
    if (lastMoveRow < 0 || lastMoveCol < 0) return;
    Player p = board.getCell(lastMoveRow, lastMoveCol);
    if (p == Player::NONE) return;

    int cx = BOARD_PADDING + lastMoveCol * CELL_SIZE;
    int cy = BOARD_PADDING + lastMoveRow * CELL_SIZE;

    // Small contrasting square marker
    SDL_Color markerColor = (p == Player::BLACK) ? SDL_Color{255, 80, 80, 220} : SDL_Color{220, 50, 50, 220};
    int s = 5;
    SDL_Rect marker = {cx - s, cy - s, s * 2 + 1, s * 2 + 1};
    SDL_SetRenderDrawColor(renderer, markerColor.r, markerColor.g, markerColor.b, markerColor.a);
    SDL_RenderFillRect(renderer, &marker);
}

// ─── Hover ghost ─────────────────────────────────────────────────────────────

void GUI::drawHoverGhost() {
    if (board.getWinner() != Player::NONE) return;

    int mx, my;
    SDL_GetMouseState(&mx, &my);

    int boardX = mx - BOARD_PADDING;
    int boardY = my - BOARD_PADDING;
    int col = (boardX + CELL_SIZE / 2) / CELL_SIZE;
    int row = (boardY + CELL_SIZE / 2) / CELL_SIZE;

    if (row < 0 || row >= Board::SIZE || col < 0 || col >= Board::SIZE) return;
    if (board.getCell(row, col) != Player::NONE) return;
    if (mx >= BOARD_AREA_SIZE) return;

    int cx = BOARD_PADDING + col * CELL_SIZE;
    int cy = BOARD_PADDING + row * CELL_SIZE;

    // Semi-transparent ghost stone
    Uint8 alpha = 80;
    if (board.getCurrentPlayer() == Player::BLACK) {
        drawDisk(cx, cy, 16, SDL_Color{30, 30, 35, alpha});
    } else {
        drawDisk(cx, cy, 16, SDL_Color{220, 220, 225, alpha});
    }
    // Subtle ring outline
    drawRing(cx, cy, 16, 2, SDL_Color{255, 255, 255, 50});
}

// ─── Coordinate labels ──────────────────────────────────────────────────────

void GUI::drawCoordinateLabels() {
    std::string goLetters = "ABCDEFGHJKLMNOPQRST";
    SDL_Color labelColor = {120, 95, 55, 200};

    for (int i = 0; i < Board::SIZE; ++i) {
        // Column letters (top)
        int cx = BOARD_PADDING + i * CELL_SIZE;
        std::string letter(1, goLetters[i]);
        drawTextCentered(letter, cx, BOARD_PADDING - 26, 1, labelColor);

        // Row numbers (left)
        int cy = BOARD_PADDING + i * CELL_SIZE;
        int rowNum = Board::SIZE - i;
        std::string numStr = std::to_string(rowNum);
        int textW = (int)numStr.length() * 10;
        drawText(numStr, BOARD_PADDING - 10 - textW, cy - 4, 1, labelColor);
    }
}

// ─── Sidebar ─────────────────────────────────────────────────────────────────

void GUI::drawSidebar() {
    int sx = BOARD_AREA_SIZE;

    // Sidebar background — dark panel
    SDL_Rect sidebarRect = {sx, 0, SIDEBAR_WIDTH, WINDOW_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 22, 24, 32, 255);
    SDL_RenderFillRect(renderer, &sidebarRect);

    // Subtle left border accent
    SDL_SetRenderDrawColor(renderer, 80, 65, 40, 180);
    SDL_RenderDrawLine(renderer, sx, 0, sx, WINDOW_HEIGHT);
    SDL_RenderDrawLine(renderer, sx + 1, 0, sx + 1, WINDOW_HEIGHT);

    int cx = sx + SIDEBAR_WIDTH / 2;  // center x for sidebar content
    int py = 40;  // current y cursor

    // ── Title ──
    drawTextCentered("GOMOKU", cx, py, 3, SDL_Color{240, 235, 220, 255});
    py += 34;
    drawTextCentered("19 x 19", cx, py, 1, SDL_Color{110, 108, 125, 255});
    py += 30;

    // Separator
    SDL_SetRenderDrawColor(renderer, 50, 48, 60, 255);
    SDL_RenderDrawLine(renderer, sx + 30, py, sx + SIDEBAR_WIDTH - 30, py);
    py += 25;

    // ── Game state ──
    if (board.getWinner() != Player::NONE) {
        // Victory banner
        std::string winStr = (board.getWinner() == Player::BLACK) ? "BLACK WINS!" : "WHITE WINS!";
        SDL_Color accent = (board.getWinner() == Player::BLACK)
            ? SDL_Color{255, 185, 80, 255}   // gold for black win
            : SDL_Color{130, 200, 255, 255};  // sky blue for white win

        // Pulsating glow behind banner
        Uint32 t = SDL_GetTicks() - startTicks;
        float pulse = 0.5f + 0.5f * std::sin(t / 400.0f);
        Uint8 glowAlpha = (Uint8)(30 + 40 * pulse);

        int bannerY = py;
        int bannerH = 70;
        SDL_Rect glow = {sx + 20, bannerY - 4, SIDEBAR_WIDTH - 40, bannerH + 8};
        SDL_SetRenderDrawColor(renderer, accent.r, accent.g, accent.b, glowAlpha);
        SDL_RenderFillRect(renderer, &glow);

        // Banner bg
        drawRoundedRect(sx + 24, bannerY, SIDEBAR_WIDTH - 48, bannerH, 6, SDL_Color{35, 30, 42, 240});
        // Border
        SDL_Rect border = {sx + 24, bannerY, SIDEBAR_WIDTH - 48, bannerH};
        SDL_SetRenderDrawColor(renderer, accent.r, accent.g, accent.b, 140);
        SDL_RenderDrawRect(renderer, &border);

        // Trophy icon (small diamond shape)
        int trophyCx = cx;
        int trophyCy = bannerY + 22;
        drawDisk(trophyCx, trophyCy, 6, accent);

        drawTextCentered(winStr, cx, bannerY + 42, 2, accent);

        py += bannerH + 20;
    } else {
        // Current turn section
        drawTextCentered("CURRENT TURN", cx, py, 1, SDL_Color{130, 128, 150, 255});
        py += 22;

        // Animated pulsing ring around stone
        Uint32 t = SDL_GetTicks() - startTicks;
        float pulse = 0.5f + 0.5f * std::sin(t / 500.0f);
        Uint8 ringAlpha = (Uint8)(40 + 60 * pulse);
        int ringRadius = 22 + (int)(3.0f * pulse);

        int stoneCy = py + 24;
        drawRing(cx, stoneCy, ringRadius, 2,
                 (board.getCurrentPlayer() == Player::BLACK)
                     ? SDL_Color{255, 200, 100, ringAlpha}
                     : SDL_Color{180, 200, 255, ringAlpha});
        drawStone(cx, stoneCy, 16, board.getCurrentPlayer());

        py += 60;

        std::string turnLabel = (board.getCurrentPlayer() == Player::BLACK) ? "Black" : "White";
        drawTextCentered(turnLabel, cx, py, 2, SDL_Color{210, 210, 225, 255});
        py += 35;
    }

    // Separator
    SDL_SetRenderDrawColor(renderer, 50, 48, 60, 255);
    SDL_RenderDrawLine(renderer, sx + 30, py, sx + SIDEBAR_WIDTH - 30, py);
    py += 20;

    // ── Move counter ──
    drawTextCentered("MOVES", cx, py, 1, SDL_Color{110, 108, 125, 255});
    py += 20;
    drawTextCentered(std::to_string(moveCount), cx, py, 3, SDL_Color{230, 225, 210, 255});
    py += 45;

    // ── Hovered coordinate ──
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    int boardX = mx - BOARD_PADDING;
    int boardY = my - BOARD_PADDING;
    int col = (boardX + CELL_SIZE / 2) / CELL_SIZE;
    int row = (boardY + CELL_SIZE / 2) / CELL_SIZE;

    drawTextCentered("POSITION", cx, py, 1, SDL_Color{110, 108, 125, 255});
    py += 20;

    if (row >= 0 && row < Board::SIZE && col >= 0 && col < Board::SIZE && mx < BOARD_AREA_SIZE) {
        std::string goLetters = "ABCDEFGHJKLMNOPQRST";
        char colLetter = goLetters[col];
        int rowNumber = Board::SIZE - row;
        std::string coordStr = "";
        coordStr += colLetter;
        coordStr += std::to_string(rowNumber);
        drawTextCentered(coordStr, cx, py, 2, SDL_Color{200, 198, 215, 255});
    } else {
        drawTextCentered("--", cx, py, 2, SDL_Color{70, 68, 85, 255});
    }
    py += 40;

    // ── Last move ──
    if (lastMoveRow >= 0 && lastMoveCol >= 0) {
        drawTextCentered("LAST MOVE", cx, py, 1, SDL_Color{110, 108, 125, 255});
        py += 20;
        std::string goLetters = "ABCDEFGHJKLMNOPQRST";
        char lc = goLetters[lastMoveCol];
        int lr = Board::SIZE - lastMoveRow;
        std::string lastStr = "";
        lastStr += lc;
        lastStr += std::to_string(lr);
        drawTextCentered(lastStr, cx, py, 2, SDL_Color{255, 140, 100, 255});
        py += 35;
    }

    // ── Restart button (bottom of sidebar) ──
    int btnX = sx + 40;
    int btnW = SIDEBAR_WIDTH - 80;
    int btnY = WINDOW_HEIGHT - 80;
    int btnH = 44;

    bool hovered = (mx >= btnX && mx <= btnX + btnW && my >= btnY && my <= btnY + btnH);

    SDL_Color btnBase  = {40, 42, 55, 255};
    SDL_Color btnHover = {60, 62, 80, 255};
    drawButton("RESTART GAME", btnX, btnY, btnW, btnH, btnBase, btnHover, hovered);
}

// ─── Button ──────────────────────────────────────────────────────────────────

void GUI::drawButton(const std::string& label, int x, int y, int w, int h,
                     SDL_Color baseColor, SDL_Color hoverColor, bool hovered) {
    // Background
    drawRoundedRect(x, y, w, h, 5, hovered ? hoverColor : baseColor);

    // Border
    SDL_Color borderCol = hovered ? SDL_Color{140, 130, 100, 200} : SDL_Color{80, 78, 95, 200};
    SDL_Rect border = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, borderCol.r, borderCol.g, borderCol.b, borderCol.a);
    SDL_RenderDrawRect(renderer, &border);

    // Top highlight line
    if (hovered) {
        SDL_SetRenderDrawColor(renderer, 180, 170, 140, 80);
        SDL_RenderDrawLine(renderer, x + 4, y + 1, x + w - 4, y + 1);
    }

    // Label centered
    int textLen = (int)label.length();
    int charW = 8 + 2; // char width + spacing at scale 1
    int textW = textLen * charW - 2;
    int tx = x + (w - textW) / 2;
    int ty = y + (h - 8) / 2;

    SDL_Color textCol = hovered ? SDL_Color{255, 250, 230, 255} : SDL_Color{190, 188, 205, 255};
    drawText(label, tx, ty, 1, textCol);
}

// ─── Low-level drawing helpers ───────────────────────────────────────────────

void GUI::drawDisk(int centerX, int centerY, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int dy = -radius; dy <= radius; dy++) {
        int dx = (int)std::sqrt(radius * radius - dy * dy);
        SDL_RenderDrawLine(renderer, centerX - dx, centerY + dy, centerX + dx, centerY + dy);
    }
}

void GUI::drawRing(int centerX, int centerY, int radius, int thickness, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int t = 0; t < thickness; ++t) {
        int r = radius - t;
        for (int angle = 0; angle < 360; ++angle) {
            double rad = angle * M_PI / 180.0;
            int px = centerX + (int)(r * std::cos(rad));
            int py = centerY + (int)(r * std::sin(rad));
            SDL_RenderDrawPoint(renderer, px, py);
        }
    }
}

void GUI::drawRoundedRect(int x, int y, int w, int h, int r, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Main body (without corners)
    SDL_Rect center = {x + r, y, w - 2 * r, h};
    SDL_RenderFillRect(renderer, &center);
    SDL_Rect left = {x, y + r, r, h - 2 * r};
    SDL_RenderFillRect(renderer, &left);
    SDL_Rect right = {x + w - r, y + r, r, h - 2 * r};
    SDL_RenderFillRect(renderer, &right);

    // Corner circles
    auto fillCorner = [&](int cx, int cy) {
        for (int dy = -r; dy <= r; dy++) {
            int dx = (int)std::sqrt(r * r - dy * dy);
            SDL_RenderDrawLine(renderer, cx - dx, cy + dy, cx + dx, cy + dy);
        }
    };
    fillCorner(x + r, y + r);
    fillCorner(x + w - r - 1, y + r);
    fillCorner(x + r, y + h - r - 1);
    fillCorner(x + w - r - 1, y + h - r - 1);
}

void GUI::drawStone(int centerX, int centerY, int radius, Player player) {
    if (player == Player::BLACK) {
        // Shadow
        drawDisk(centerX + 2, centerY + 2, radius, SDL_Color{15, 15, 18, 90});

        // Body — dark gradient layers
        for (int r = radius; r > 0; --r) {
            float pct = (float)(radius - r) / radius;
            int shiftX = (int)(3.5f * pct);
            int shiftY = (int)(3.5f * pct);
            Uint8 c = 15 + (Uint8)(35.0f * pct);
            drawDisk(centerX - shiftX, centerY - shiftY, r, SDL_Color{c, c, (Uint8)(c + 4), 255});
        }
        // Specular highlight
        drawDisk(centerX - 4, centerY - 5, 3, SDL_Color{80, 80, 95, 200});
        drawDisk(centerX - 3, centerY - 4, 1, SDL_Color{140, 140, 155, 160});

    } else if (player == Player::WHITE) {
        // Shadow
        drawDisk(centerX + 2, centerY + 2, radius, SDL_Color{25, 25, 30, 70});

        // Body — light gradient layers
        for (int r = radius; r > 0; --r) {
            float pct = (float)(radius - r) / radius;
            int shiftX = (int)(4.0f * pct);
            int shiftY = (int)(4.0f * pct);
            Uint8 c = 180 + (Uint8)(70.0f * pct);
            drawDisk(centerX - shiftX, centerY - shiftY, r, SDL_Color{c, c, (Uint8)std::min(255, c + 3), 255});
        }
        // Specular highlights
        drawDisk(centerX - 5, centerY - 5, 3, SDL_Color{255, 255, 255, 220});
        drawDisk(centerX - 4, centerY - 4, 1, SDL_Color{255, 255, 255, 255});
    }
}

// ─── Text rendering ─────────────────────────────────────────────────────────

void GUI::drawChar(char c, int x, int y, int scale, SDL_Color color) {
    if (c < 32 || c > 126) return;
    int idx = c - 32;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    for (int row = 0; row < 8; ++row) {
        unsigned char rowByte = font8x8[idx][row];
        for (int col = 0; col < 8; ++col) {
            if (rowByte & (1 << (7 - col))) {
                SDL_Rect pixelRect = { x + col * scale, y + row * scale, scale, scale };
                SDL_RenderFillRect(renderer, &pixelRect);
            }
        }
    }
}

void GUI::drawText(const std::string& text, int x, int y, int scale, SDL_Color color) {
    int curX = x;
    int charWidth = 8 * scale;
    int spacing = 2 * scale;

    for (char c : text) {
        drawChar(c, curX, y, scale, color);
        curX += charWidth + spacing;
    }
}

void GUI::drawTextCentered(const std::string& text, int cx, int y, int scale, SDL_Color color) {
    int charWidth = 8 * scale;
    int spacing = 2 * scale;
    int totalW = (int)text.length() * (charWidth + spacing) - spacing;
    int startX = cx - totalW / 2;
    drawText(text, startX, y, scale, color);
}

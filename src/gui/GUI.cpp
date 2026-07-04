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
    : board(b), window(nullptr), renderer(nullptr), running(false) {}

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

    // Enable high-quality scaling (optional but nice)
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    window = SDL_CreateWindow("Gomoku - Advanced Go board",
                             SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                             WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create a hardware accelerated renderer with VSync
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Enable transparent rendering
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    return true;
}

void GUI::run() {
    running = true;
    while (running) {
        handleEvents();
        render();
        SDL_Delay(8); // limit loop rate (~120 FPS max)
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

                // Check reset button click
                // Reset button: X: 840, Y: 500, W: 170, H: 45
                if (x >= 840 && x <= 840 + 170 && y >= 500 && y <= 500 + 45) {
                    board.reset();
                    continue;
                }

                // Check click on board
                // Spacing starts at BOARD_PADDING (40) and ends at BOARD_PADDING + 18 * CELL_SIZE (760)
                int boardX = x - BOARD_PADDING;
                int boardY = y - BOARD_PADDING;
                
                // Find nearest row and col
                int col = (boardX + CELL_SIZE / 2) / CELL_SIZE;
                int row = (boardY + CELL_SIZE / 2) / CELL_SIZE;

                // Check if click was close enough to the intersection (within tolerance)
                if (row >= 0 && row < Board::SIZE && col >= 0 && col < Board::SIZE) {
                    int intersectionX = BOARD_PADDING + col * CELL_SIZE;
                    int intersectionY = BOARD_PADDING + row * CELL_SIZE;
                    double dist = std::sqrt(std::pow(x - intersectionX, 2) + std::pow(y - intersectionY, 2));

                    if (dist <= 18.0) { // click radius tolerance
                        board.makeMove(row, col);
                    }
                }
            }
        }
    }
}

void GUI::render() {
    // Clear screen with a very dark background (slate)
    SDL_SetRenderDrawColor(renderer, 24, 25, 30, 255);
    SDL_RenderClear(renderer);

    drawBoard();
    drawStones();
    drawSidebar();

    SDL_RenderPresent(renderer);
}

void GUI::drawBoard() {
    // 1. Draw Goban Wood Board Background
    // Warm rich wood tone
    SDL_Rect boardRect = {0, 0, BOARD_AREA_SIZE, BOARD_AREA_SIZE};
    SDL_SetRenderDrawColor(renderer, 219, 172, 102, 255);
    SDL_RenderFillRect(renderer, &boardRect);

    // Subtle wood grain border/shading
    SDL_SetRenderDrawColor(renderer, 194, 148, 80, 255);
    SDL_RenderDrawRect(renderer, &boardRect);

    // 2. Draw Grid Lines
    // Charcoal lines for board grid
    SDL_SetRenderDrawColor(renderer, 40, 30, 20, 200);

    for (int i = 0; i < Board::SIZE; ++i) {
        int pos = BOARD_PADDING + i * CELL_SIZE;
        // Vertical lines
        SDL_RenderDrawLine(renderer, pos, BOARD_PADDING, pos, BOARD_PADDING + (Board::SIZE - 1) * CELL_SIZE);
        // Horizontal lines
        SDL_RenderDrawLine(renderer, BOARD_PADDING, pos, BOARD_PADDING + (Board::SIZE - 1) * CELL_SIZE, pos);
    }

    // 3. Draw Star Points (Hoshi)
    // 19x19 board has 9 star points: intersections of rows/cols 3, 9, 15 (0-indexed)
    int starCoords[3] = {3, 9, 15};
    for (int r : starCoords) {
        for (int c : starCoords) {
            int cx = BOARD_PADDING + c * CELL_SIZE;
            int cy = BOARD_PADDING + r * CELL_SIZE;
            drawDisk(cx, cy, 4, SDL_Color{40, 30, 20, 255}); // Small filled circle
        }
    }
}

void GUI::drawStones() {
    for (int r = 0; r < Board::SIZE; ++r) {
        for (int c = 0; c < Board::SIZE; ++c) {
            Player p = board.getCell(r, c);
            if (p != Player::NONE) {
                int cx = BOARD_PADDING + c * CELL_SIZE;
                int cy = BOARD_PADDING + r * CELL_SIZE;
                drawStone(cx, cy, 18, p);
            }
        }
    }
}

void GUI::drawSidebar() {
    // Slate panel background on the right
    SDL_Rect sidebarRect = {BOARD_AREA_SIZE, 0, WINDOW_WIDTH - BOARD_AREA_SIZE, WINDOW_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 30, 32, 40, 255);
    SDL_RenderFillRect(renderer, &sidebarRect);

    // Thin vertical partition line
    SDL_SetRenderDrawColor(renderer, 50, 52, 65, 255);
    SDL_RenderDrawLine(renderer, BOARD_AREA_SIZE, 0, BOARD_AREA_SIZE, WINDOW_HEIGHT);

    // Get current mouse state to check button hover
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    bool resetHover = (mx >= 840 && mx <= 840 + 170 && my >= 500 && my <= 500 + 45);

    // Write title
    drawText("GOMOKU", 850, 50, 4, SDL_Color{230, 230, 250, 255});
    drawText("19x19 Goban", 860, 100, 1, SDL_Color{140, 142, 160, 255});

    // Separator line
    SDL_SetRenderDrawColor(renderer, 50, 52, 65, 255);
    SDL_RenderDrawLine(renderer, 830, 140, 1020, 140);

    // Draw game state or winner
    if (board.isGameOver()) {
        std::string winMsg = (board.getWinner() == Player::BLACK) ? "BLACK WINS!" : "WHITE WINS!";
        SDL_Color winColor = (board.getWinner() == Player::BLACK) ? SDL_Color{255, 100, 100, 255} : SDL_Color{100, 255, 100, 255};
        
        // Background banner for victory
        SDL_Rect banner = {825, 200, 200, 80};
        SDL_SetRenderDrawColor(renderer, 45, 25, 30, 255);
        SDL_RenderFillRect(renderer, &banner);
        SDL_SetRenderDrawColor(renderer, winColor.r, winColor.g, winColor.b, 100);
        SDL_RenderDrawRect(renderer, &banner);

        drawText(winMsg, 840, 230, 2, winColor);
    } else {
        // Draw turn indicator
        drawText("CURRENT TURN:", 840, 200, 1, SDL_Color{160, 162, 180, 255});
        if (board.getCurrentPlayer() == Player::BLACK) {
            drawStone(870, 260, 16, Player::BLACK);
            drawText("BLACK", 900, 252, 2, SDL_Color{255, 255, 255, 255});
        } else {
            drawStone(870, 260, 16, Player::WHITE);
            drawText("WHITE", 900, 252, 2, SDL_Color{220, 220, 220, 255});
        }
    }

    // Hovered board coordinate display
    int boardX = mx - BOARD_PADDING;
    int boardY = my - BOARD_PADDING;
    int col = (boardX + CELL_SIZE / 2) / CELL_SIZE;
    int row = (boardY + CELL_SIZE / 2) / CELL_SIZE;
    if (row >= 0 && row < Board::SIZE && col >= 0 && col < Board::SIZE && mx < BOARD_AREA_SIZE) {
        // Standard Go coordinates: Column A-T (skipping I)
        std::string goLetters = "ABCDEFGHJKLMNOPQRST";
        char colLetter = goLetters[col];
        int rowNumber = Board::SIZE - row;
        
        std::string coordStr = "Cursor: ";
        coordStr += colLetter;
        coordStr += std::to_string(rowNumber);
        drawText(coordStr, 840, 360, 1, SDL_Color{180, 182, 200, 255});
    } else {
        drawText("Cursor: --", 840, 360, 1, SDL_Color{110, 112, 130, 255});
    }

    // Draw Reset Button
    SDL_Color btnBase = SDL_Color{45, 48, 60, 255};
    SDL_Color btnHover = SDL_Color{60, 64, 85, 255};
    drawButton("RESTART", 840, 500, 170, 45, btnBase, btnHover, resetHover, false);
}

void GUI::drawButton(const std::string& label, int x, int y, int w, int h, 
                     SDL_Color baseColor, SDL_Color hoverColor, bool hovered, bool clicked) {
    (void)clicked;
    SDL_Rect rect = {x, y, w, h};
    if (hovered) {
        SDL_SetRenderDrawColor(renderer, hoverColor.r, hoverColor.g, hoverColor.b, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, baseColor.r, baseColor.g, baseColor.b, 255);
    }
    SDL_RenderFillRect(renderer, &rect);

    // Draw border
    SDL_SetRenderDrawColor(renderer, 90, 95, 120, 255);
    SDL_RenderDrawRect(renderer, &rect);

    // Label centering (approximate font size 8x8 scaled by 1.5)
    int textLen = label.length();
    int textW = textLen * 8 * 1.5; // Scale is 1.5
    int textH = 8 * 1.5;
    int tx = x + (w - textW) / 2;
    int ty = y + (h - textH) / 2;

    drawText(label, tx, ty + 1, 1, SDL_Color{230, 230, 250, 255}); // scale=1 is standard, but let's use it directly
}

void GUI::drawDisk(int centerX, int centerY, int radius, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int dy = -radius; dy <= radius; dy++) {
        int dx = (int)std::sqrt(radius * radius - dy * dy);
        SDL_RenderDrawLine(renderer, centerX - dx, centerY + dy, centerX + dx, centerY + dy);
    }
}

void GUI::drawStone(int centerX, int centerY, int radius, Player player) {
    if (player == Player::BLACK) {
        // Draw shadow first
        drawDisk(centerX + 2, centerY + 2, radius, SDL_Color{30, 30, 35, 100}); // Semi-transparent shadow
        
        // Draw body with 3D gradient
        for (int r = radius; r > 0; --r) {
            float pct = (float)(radius - r) / radius;
            int shiftX = (int)(3.0f * pct);
            int shiftY = (int)(3.0f * pct);
            
            // Slate black to dark charcoal
            Uint8 c = 12 + (Uint8)(32.0f * pct); 
            drawDisk(centerX - shiftX, centerY - shiftY, r, SDL_Color{c, c, (Uint8)(c + 3), 255});
        }
        // Small shiny specular highlight
        drawDisk(centerX - 4, centerY - 4, 3, SDL_Color{70, 70, 80, 255});
    } else if (player == Player::WHITE) {
        // Draw shadow
        drawDisk(centerX + 2, centerY + 2, radius, SDL_Color{30, 30, 35, 80});
        
        // Draw body with 3D gradient
        for (int r = radius; r > 0; --r) {
            float pct = (float)(radius - r) / radius;
            int shiftX = (int)(4.0f * pct);
            int shiftY = (int)(4.0f * pct);
            
            // Pearlescent light gray/white
            Uint8 c = 175 + (Uint8)(80.0f * pct); 
            drawDisk(centerX - shiftX, centerY - shiftY, r, SDL_Color{c, c, (Uint8)(c + 2), 255});
        }
        // Bright specular highlight
        drawDisk(centerX - 5, centerY - 5, 2, SDL_Color{255, 255, 255, 255});
    }
}

void GUI::drawChar(char c, int x, int y, int scale, SDL_Color color) {
    if (c < 32 || c > 126) return;
    int idx = c - 32;

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    for (int row = 0; row < 8; ++row) {
        unsigned char rowByte = font8x8[idx][row];
        for (int col = 0; col < 8; ++col) {
            if (rowByte & (1 << col)) {
                SDL_Rect pixelRect = { x + col * scale, y + row * scale, scale, scale };
                SDL_RenderFillRect(renderer, &pixelRect);
            }
        }
    }
}

void GUI::drawText(const std::string& text, int x, int y, int scale, SDL_Color color) {
    int curX = x;
    int charWidth = 8 * scale;
    int spacing = 2 * scale; // 2 pixel spacing between characters

    for (char c : text) {
        drawChar(c, curX, y, scale, color);
        curX += charWidth + spacing;
    }
}

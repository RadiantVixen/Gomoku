#ifndef GUI_HPP
#define GUI_HPP

#include <SDL2/SDL.h>
#include <string>
#include "../core/Board.hpp"

class GUI {
public:
    GUI(Board& board);
    ~GUI();

    bool init();
    void run();

private:
    Board& board;
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;

    // Track game state for UI
    int moveCount;
    int lastMoveRow;
    int lastMoveCol;
    Uint32 startTicks;

    // Window dimensions
    static const int SIDEBAR_WIDTH = 320;
    static const int BOARD_AREA_SIZE = 800;
    static const int WINDOW_WIDTH = BOARD_AREA_SIZE + SIDEBAR_WIDTH;
    static const int WINDOW_HEIGHT = 800;
    static const int CELL_SIZE = 40;
    static const int BOARD_PADDING = 40;

    void handleEvents();
    void render();

    // Drawing helpers
    void drawBoard();
    void drawStones();
    void drawHoverGhost();
    void drawLastMoveMarker();
    void drawCoordinateLabels();
    void drawSidebar();
    void drawButton(const std::string& label, int x, int y, int w, int h,
                    SDL_Color baseColor, SDL_Color hoverColor, bool hovered);

    // Low-level graphics helpers
    void drawDisk(int centerX, int centerY, int radius, SDL_Color color);
    void drawRing(int centerX, int centerY, int radius, int thickness, SDL_Color color);
    void drawStone(int centerX, int centerY, int radius, Player player);
    void drawRoundedRect(int x, int y, int w, int h, int r, SDL_Color color);

    // Bitmap font helpers (no external font files needed)
    void drawChar(char c, int x, int y, int scale, SDL_Color color);
    void drawText(const std::string& text, int x, int y, int scale, SDL_Color color);
    void drawTextCentered(const std::string& text, int cx, int y, int scale, SDL_Color color);
};

#endif // GUI_HPP

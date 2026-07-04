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

    // Window dimensions
    static const int WINDOW_WIDTH = 1050;
    static const int WINDOW_HEIGHT = 800;
    static const int BOARD_AREA_SIZE = 800;
    static const int CELL_SIZE = 40;
    static const int BOARD_PADDING = 40;

    void handleEvents();
    void render();

    // Drawing helpers
    void drawBoard();
    void drawStones();
    void drawSidebar();
    void drawButton(const std::string& label, int x, int y, int w, int h, SDL_Color baseColor, SDL_Color hoverColor, bool hovered, bool clicked);
    
    // Low-level graphics helpers
    void drawDisk(int centerX, int centerY, int radius, SDL_Color color);
    void drawStone(int centerX, int centerY, int radius, Player player);
    
    // Bitmap font helpers (no external font files needed)
    void drawChar(char c, int x, int y, int scale, SDL_Color color);
    void drawText(const std::string& text, int x, int y, int scale, SDL_Color color);
};

#endif // GUI_HPP

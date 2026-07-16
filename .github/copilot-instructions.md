<!-- Copilot / AI agent instructions for the Gomoku repo -->
# Gomoku agent guide

This repo is a small C++17 Gomoku game with an SDL2 desktop UI.
- Binary: `Gomoku`
- Build: `Makefile` with `all`, `clean`, `fclean`, `re`
- Dependency: SDL2 via `pkg-config` (`libsdl2-dev` on Debian/Ubuntu)

## Read first
- `src/main.cpp` creates `Board` and `GUI`, then calls `gui.run()`.
- `src/core/Board.hpp` / `src/core/Board.cpp` hold all game state and rule logic: 19x19 grid, current player, winner, captures, move validation, win checks.
- `src/gui/GUI.hpp` / `src/gui/GUI.cpp` handle SDL setup, mouse input, rendering, and the board-to-screen coordinate mapping.
- `src/core/AI.cpp` and `src/core/heuristicFunctions.cpp` are AI-related sources, but the default `Makefile` does not compile them.

## Build and run
- Build with `make` from the repo root.
- Debug build: `make CXXFLAGS='-g -O0 -std=c++17 $(shell pkg-config --cflags sdl2)'`
- Clean with `make clean`; remove the binary with `make fclean`; rebuild from scratch with `make re`.
- Run with `./Gomoku`.

## Project-specific conventions
- `Board::SIZE` is `19`; row/col indices map directly to GUI intersections.
- `Player` is a scoped enum (`NONE`, `BLACK`, `WHITE`); files use no namespaces.
- Object files live under `obj/` mirroring `src/`.
- If you add a new `.cpp`, update `SRCS` in the `Makefile`; if you add or change public headers, update `DEPS` too.

## Editing rules and UI code
- Keep rule changes in `Board` first; the GUI should read board state and only trigger `makeMove()` / `reset()`.
- If you change a `Board` or `GUI` signature, update all callers in `main.cpp`, `GUI.cpp`, and any AI code.
- `GUI.cpp` is intentionally dense: it contains custom drawing helpers, bitmap text, rounded rectangles, and stone rendering. Prefer small, localized changes there.

## AI integration
- Implement AI as a thin layer over `Board` APIs such as `getCell()`, `isValidMove()`, `makeMove()`, `checkCapture()`, and `checkwin()`.
- Keep AI non-blocking so the SDL event loop stays responsive.
- If you wire in AI sources, remember to add them to the build; they are present in the tree but not linked by default.

## Verification habits
- Re-run `make` after header or signature changes; this repo currently has some source/header drift, so include and declaration mismatches are common failure points.
- Use a small harness or direct GUI run to validate rule changes; for rendering issues, launch `./Gomoku` and test interactively.

## Examples
- Add a helper: put rule logic in `src/core/Board.cpp`, expose it in `Board.hpp`, then rebuild.
- Add a source file: create it under `src/`, add it to `SRCS`, and update `DEPS` if it has a public header.
- Add AI: expose a move-selection function in `src/core/AI.cpp` and call it from the game flow without blocking `GUI::run()`.

<!-- Copilot / AI agent instructions for the Gomoku repo -->
# Quick orientation

This repository is a small C++17 Gomoku implementation with a native SDL2 GUI.
Key facts:
- Binary produced: `Gomoku` (root of repo)
- Build system: `Makefile` (targets: `all`, `clean`, `fclean`, `re`)
- Windowing/graphics: SDL2 (`libsdl2-dev` / `pkg-config` required)

**Main components** (read these files first):
- `src/main.cpp` — program entry: constructs `Board` and `GUI`, then calls `gui.run()`.
- `src/core/Board.hpp` / `src/core/Board.cpp` — game rules and state (board size, move validation, win detection, captures, game-over logic).
- `src/gui/GUI.hpp` / `src/gui/GUI.cpp` — SDL2 rendering, event handling and user interaction. Heavy rendering code and input-to-board translation live here.
- `src/core/AI.cpp` — empty placeholder where AI logic should go. Integrate AI by calling `Board` methods.

Build & run (developer workflows)
- Install deps: `sudo apt install libsdl2-dev pkg-config` (or equivalent for your OS).
- Build: `make` (uses `pkg-config --cflags/--libs sdl2` inside the Makefile).
- Clean: `make clean` (removes `obj/`), `make fclean` removes `Gomoku` binary too.
- Quick debug build (no optimizations, include debug symbols):
  `make CXXFLAGS='-g -O0 -std=c++17 $(shell pkg-config --cflags sdl2)'`
- Run: `./Gomoku` from repo root.

Important repository conventions & patterns
- Object files placed under `obj/` mirroring `src/` structure. When adding `.cpp` files, update the `SRCS` list in the `Makefile`.
- `DEPS` in the `Makefile` lists headers used to trigger recompilation. If you add public headers, add them to `DEPS` or run `make re` for a full rebuild.
- No namespaces are used; files use plain global enum `Player` and classes `Board`, `GUI`.
- Avoid modifying rendering code in `GUI.cpp` unless necessary; it contains many low-level drawing helpers (fonts, rounded rects, disks). Small UI tweaks are fine, but large changes to rendering logic can be noisy.

Guidance for editing game logic
- Primary rule logic lives in `src/core/Board.*`:
  - `makeMove`, `isValidMove`, `checkWin`, `isCapturable`, `checkCapture`, `isGameOver`, `checkPotentialWin`.
  - `Board::SIZE` is `19`. Coordinate system: `row` and `col` indexes map directly to GUI grid.
- If you change function signatures in headers, update callers in `GUI.cpp` and `main.cpp`.
- For rule experiments, add small, isolated helpers in `Board` and keep deterministic behavior for easier testing.

AI-specific notes
- `src/core/AI.cpp` exists but is empty — intended integration point for computer opponent logic.
- Recommended approach: implement AI functions that accept `const Board&` or use `Board` APIs such as `getCell`, `getCurrentPlayer`, `isCapturable`, `checkPotentialWin` and return a row/col move. Avoid making AI directly mutate GUI state.
- When adding an AI, consider exposing a thin interface (e.g., `std::pair<int,int> AI::chooseMove(const Board&)`) and call it from `GUI` or from a new controller layer.

Testing & debugging tips
- To enable easier debugging, build with `-g -O0` as shown above and run under `gdb` or `rr`/`valgrind` for memory issues.
- Rendering bugs are easiest to reproduce by running `./Gomoku` and interacting. For logic-only tests, create a small test harness that includes `Board.hpp` and drives `makeMove`/`checkWin` without SDL.

Quick examples (common edits)
- Add a new source file: create `src/core/new.cpp`, then add it to `SRCS` in `Makefile` and `make`.
- Change Board API: update `src/core/Board.hpp`, run `make` and fix compile errors in `src/gui/GUI.cpp`.
- Implement AI: write `src/core/AI.cpp` and call `AI::chooseMove(board)` from `GUI` or a new controller; do not add heavy blocking calls on the main thread (GUI loop must remain responsive).

If something is unclear
- Tell me which area you want expanded: build, rules, AI integration, or GUI rendering. I can add targeted examples or create a minimal AI stub and instructions to wire it in.

-- End of file

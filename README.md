# Sudoku Qt6

A cross-platform Sudoku game enhanced with a modern Qt6 GUI. Based on a classic C++ terminal logic, now featuring a fully interactive graphical interface.

## Features

1. **Qt6 GUI**: Smooth interactive interface with mouse and keyboard support.
2. **Difficulty Levels**: Choose between Easy, Normal, and Hard modes.
3. **Undo Functionality**: Easily revert accidental moves.
4. **Visual Feedback**: 3x3 block coloring and selection highlighting.
5. **Cross-Platform**: Runs on Windows, Linux, and macOS.

## Dependencies

1. CMake 3.12 or higher
2. C++17 compatible compiler
3. Qt6 (Components: Core, Gui, Widgets)

## Build

Ensure Qt6 is installed and accessible to CMake (e.g., via `CMAKE_PREFIX_PATH` or system environment).

```shell
cmake -B build -S .
cmake --build build
```

## Run

The executable is generated in the `bin` directory (or build root depending on platform/generator).

```shell
./bin/sudoku
```

## Controls

### Game Interaction

- **Mouse Click**: Select a cell.
- **Keyboard 1-9**: Fill the selected cell.
- **0 / Delete / Backspace**: Clear the selected cell.
- **Arrow Keys**: Move selection cursor.

### UI Buttons

- **Difficulty Dropdown**: Select game difficulty (Easy, Normal, Hard).
- **New Game**: Start a new game with the selected difficulty.
- **Undo**: Revert the last action.
- **Check / Finish**: Check if the puzzle is solved correctly.

## Project Structure

```bash
│--.gitignore
│--CMakeLists.txt      // CMake configuration with Qt6 support
│--README.md
└--src
   │--main.cpp         // Qt Application Entry Point
   │--gui              // GUI Implementation
   │  │--mainwindow.h/cpp  // Main Window Logic
   │  │--sudokucell.h/cpp  // Custom Cell Widget
   │--scene.h/cpp      // Core Game Logic (Refactored)
   └-- ...             // Other core logic files
```

## Credits

Original core logic by [mayerui](https://github.com/mayerui/sudoku).
Ported to Qt6 GUI by Antigravity.

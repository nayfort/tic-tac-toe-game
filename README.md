# Tic-Tac-Toe

A local two-player tic-tac-toe game written in C++17 with OpenGL and GLUT.
The board and pieces are rendered in 3D, with camera rotation, zoom, round
restarts, a timer, and session scores.

## Requirements

- CMake 3.16 or newer and a C++17 compiler
- OpenGL, GLU, and GLUT development libraries
- A desktop session with an OpenGL compatibility context

The renderer uses the fixed-function OpenGL API. It does not require downloaded
assets, an account, or an internet connection to play. There is no computer opponent.

## Build and run

### macOS

Install Apple's Command Line Tools (`xcode-select --install`) and CMake
(`brew install cmake`). The system OpenGL and GLUT frameworks are used.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/tic-tac-toe
```

If your Command Line Tools installation reports `fatal error: 'vector' file not
found`, update or repair the tools. As a local workaround, build with the SDK's
standard C++ headers explicitly included:

```sh
CPLUS_INCLUDE_PATH="$(xcrun --show-sdk-path)/usr/include/c++/v1" cmake --build build --parallel
```

### Linux (Debian / Ubuntu)

```sh
sudo apt install build-essential cmake freeglut3-dev libglu1-mesa-dev
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/tic-tac-toe
```

### Windows

Install Visual Studio with the **Desktop development with C++** workload,
CMake, and FreeGLUT. For example, with an existing vcpkg installation:

```powershell
vcpkg install freeglut:x64-windows
cmake -S . -B build -A x64 -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
cmake --build build --config Release
.\build\Release\tic-tac-toe.exe
```

Set `VCPKG_ROOT` to your vcpkg installation directory. Ensure the FreeGLUT runtime
DLL is beside the executable or on `PATH` if it is not copied automatically.
CMake replaces the original machine-specific Visual Studio project files.

## Playing

Enter each player's name and press **Enter**. Names accept up to 24 printable
ASCII characters, excluding `;` and `=` (score-file separators). Empty names
are rejected; **Backspace** edits the current name. X always starts a round.

| Input | Action |
| --- | --- |
| Left click near a cell's center | Place the current player's piece |
| Right mouse button + drag | Rotate the board |
| Up / Down arrow | Zoom in / out |
| F2 (Fn + F2 on some Macs) | Start a new round, keeping names and scores |
| Esc | Save the session score and exit |

Three matching pieces in a row, column, or diagonal win. A full board without a
winner is a draw. Completed rounds reject additional moves until a restart.

## Score history

Scores are appended to `scoring.txt` in the **current working directory** when
the process exits normally, once both names have been entered. Use **Esc** for
reliable saving across GLUT implementations. Forced termination cannot save.
The directory must be writable; write failures are reported in the terminal.

```text
1;23.09.2026;Alice=2;Bob=1
```

Each record contains a session number, local date, and each player's win count.
History is printed to the terminal on startup; it does not restore past wins
into a new session. Malformed records are skipped. The file is created as needed
and excluded from Git. Concurrent game instances should use different working
directories to keep separate histories.

## Tests

```sh
ctest --test-dir build --output-on-failure
# Optional integration test: opens a real OpenGL window and requires a display.
./build/game-tests --graphics
```

For Visual Studio builds use `ctest --test-dir build -C Release` and
`.\build\Release\game-tests.exe --graphics`.

Regression checks cover all eight winning lines for both pieces, unequal board
spacing, name validation, restart state, material assignment, and malformed score
history. The graphics test additionally checks projected mouse clicks, occupied
cells, win and draw completion, restart, and OpenGL errors.

Validated locally on Apple Silicon macOS with Apple Clang. Linux CI builds and
runs the same tests; Windows instructions are provided but were not locally tested.

## Source layout

- `main.cpp`, `Score.h`: application lifecycle and score history
- `Scene.cpp`, `Scene.h`: input, camera, rendering, and round state
- `Board`, `Cross`, `Disk`, `Shape`: board and piece geometry
- `GLPlatform.h`, `utils.cpp`, `utils.h`: platform headers and rendering helpers
- `tests/game_tests.cpp`: regression and graphics integration tests

OpenGL and GLUT are legacy APIs on macOS. This project intentionally retains its
original renderer; a modern rendering backend would be a separate change.

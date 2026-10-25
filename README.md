# Data Structure Visualizer & Code Debugger

[![Language](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform](https://img.shields.io/badge/platform-Windows-0078D6.svg)](https://www.microsoft.com/windows)

A menu-driven C console application for exploring common data structures and watching their operations happen step by step. The visualizer pairs a text-based view of the current structure with a debugger panel showing the active function, call stack, local values, highlighted source line, and operation status.

> **Looking for the simpler version?** A beginner-friendly rewrite is available in [`basic/`](basic/). It uses only `stdio.h`, `stdlib.h`, `conio.h` and `windows.h`, has no ANSI colors, and keeps the same step / auto-play debugger behavior with plain text drawing. See [`basic/README.md`](basic/README.md) for build and run instructions.

## Features

- **Binary search tree:** insert, delete, search, and traversal operations with a tree view.
- **Graphs:** an undirected graph represented by an adjacency matrix, with non-recursive breadth-first search (BFS) and depth-first search (DFS). The display tracks visited vertices, the active queue or stack, and the traversal path.
- **Stack:** push, pop, and peek with the array state shown.
- **Queues:** simple and circular queue visualizations.
- **Linked lists:** singly linked list operations and a doubly linked list with insertion/deletion at positions and forward/backward traversal.
- **Interactive playback:** step through debugger frames, switch to autoplay, or continue quickly.
- **Console rendering:** ANSI-colored output highlights the current operation and data.

## Requirements

- Windows, because the application uses the Win32 console APIs, `conio.h`, and `windows.h`.
- A C compiler such as GCC via MinGW-w64, or Microsoft Visual C.
- A terminal with ANSI escape-sequence support for colored output.

## Build

With MinGW-w64 GCC, run this from the project directory:

```sh
gcc -std=c99 -Wall -Wextra visualizer.c bst_visualizer.c graph_visualizer.c linear_visualizer.c linked_list_visualizer.c -o visualizer.exe
```

The project uses Windows-specific headers and APIs, so this command is intended for a Windows GCC toolchain. For Visual Studio, create a Console App project, add the five `.c` files and `visualizer.h`, then build with the MSVC toolchain.

## Run

```powershell
.\visualizer.exe
```

Choose a data structure from the main menu and follow the prompts in its submenu. In debugger playback, use:

|        Key         |      Action      |
| ------------------ | ---------------- |
| `Enter` or `Space` | Advance one step |
| `A`                | Enable autoplay  |
| `C`                | Continue quickly |

Playback keys are used while a debugger frame is waiting. Autoplay advances automatically; pressing `Space` or `Enter` returns to step mode, and `C` switches to quick continuation.

## Project files

|            File            |                                  Purpose                                   |
| -------------------------- | -------------------------------------------------------------------------- |
| `visualizer.c`             | Program entry point, shared console helpers, debugger frame, and main menu |
| `visualizer.h`             | Shared data types, constants, globals, and function declarations           |
| `bst_visualizer.c`         | Binary search tree operations and visualization                            |
| `graph_visualizer.c`       | Graph representation, BFS/DFS, and visualization                           |
| `linear_visualizer.c`      | Stack and queue implementations and menus                                  |
| `linked_list_visualizer.c` | Singly and doubly linked list operations and menus                         |

## Notes

The graph supports up to 10 vertices. Stack and queue arrays have a capacity of 20 elements. The source uses Windows console behavior and ANSI escape sequences; output appearance may vary between terminals.

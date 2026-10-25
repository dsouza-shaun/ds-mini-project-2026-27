# Data Structure Visualizer (Basic)

A small menu-driven C program that shows common data structures on the screen
while their operations run step by step. Each animated operation shows:

- the current structure (tree, graph, stack, queue or linked list)
- a debugger panel with the function name, call stack and variable values
- the source code of the function, with the running line marked by `==>`
- a status line that explains what is happening

## Features

- **Binary search tree:** insert, search, delete, inorder / preorder /
  postorder traversal, node counts and clear. The tree is drawn with real
  branches (`-`, `|`, `/`, `\`), not just indentation.
- **Graph:** adjacency matrix, add edges, BFS and DFS with the queue or stack
  shown during the animation.
- **Stack:** push, pop, peek and clear.
- **Queues:** simple queue and circular queue with front / rear markers.
- **Linked lists:** singly linked list (insert / delete at front, end and
  position) and doubly linked list (forward and backward display).
- **Step mode:** every animated operation can be stepped through by hand,
  auto-played, or run quickly to the end.

## Requirements

- Windows (the program uses `conio.h` for `_getch()` and `windows.h` for
  `Sleep()`).
- A C compiler such as GCC from MinGW-w64 or MSYS2.

## Build

Open a terminal in the `basic` folder and run:

```sh
gcc -std=c99 -Wall -Wextra visualizer.c bst.c graph.c stack.c queue.c linked_list.c -o visualizer.exe
```

## Run

```powershell
.\visualizer.exe
```

Choose a data structure from the main menu, then use its submenu.

### Playback keys

These keys work while an animation is waiting for you:

| Key               | Action                          |
| ----------------- | ------------------------------- |
| `Enter` or `Space`| go to the next step             |
| `A`               | switch to auto-play             |
| `C`               | continue quickly to the end     |

During auto-play, pressing `Space` or `Enter` goes back to step mode.

## Files

| File               | Purpose                                                        |
| ------------------ | -------------------------------------------------------------- |
| `visualizer.h`     | shared structs, constants and function declarations            |
| `visualizer.c`     | `main()`, menu, screen clearing and the debugger frame drawing |
| `bst.c`            | binary search tree and its tree drawing                        |
| `graph.c`          | graph, BFS and DFS                                             |
| `stack.c`          | stack operations and menu                                      |
| `queue.c`          | simple queue, circular queue and menus                         |
| `linked_list.c`    | singly and doubly linked lists and menus                       |

Only `visualizer.c` contains `main()`.

## Notes and limits

- The stack, simple queue and circular queue hold at most `SIZE` (10)
  elements.
- The graph has at most `MAX_VERTICES` (10) vertices. Vertices are numbered
  from 0.
- Linked list positions start at 1.
- A simple queue cannot reuse space freed at the front until it is cleared.
  The circular queue does reuse it.
- Tree drawing uses a fixed character canvas, so very large or very deep trees
  may be clipped on screen. Inserting and deleting still works.
- The source-code panel is a short, readable version of each function, so a
  few helper lines are left out to keep the frame on one screen.

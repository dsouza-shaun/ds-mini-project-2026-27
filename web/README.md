# Web Visualizer

Same Data Structure Visualizer, but the drawing happens in the browser.
The C program stays a normal console program: you build the structures with
menus and `scanf`, and the page shows every step.

## Run it

```bash
cd web
python serve.py
```

`serve.py` does four things:

1. compiles `visualizer.c` if the executable is missing or older than the source,
2. starts a local server on `http://127.0.0.1:8000` (or the next free port),
3. opens `viewer.html` in your browser,
4. runs the C program in the same terminal.

Use the C menus normally. Every operation rewrites `trace.json`, and the page
picks it up within a second and plays the steps.

## Requirements

- Python 3 (standard library only)
- GCC on `PATH` (or build `visualizer.exe` yourself)

## Playback

| Button | What it does |
|---|---|
| Restart | go back to the first step |
| Prev / Next | move one step at a time |
| Play | play again from the first step |
| Continue | keep playing from the current step |

Keyboard: `←` and `→` step, `space` plays or pauses.

## Structures

- Binary search tree: insert, search, delete, inorder, preorder, postorder, counts
- Graph: set vertices, add edge, BFS, DFS
- Stack: push, pop, peek
- Queue: simple and circular
- Linked list: singly and doubly, insert/delete at front, end and position

## Files

| File | Purpose |
|---|---|
| `visualizer.c` | all structures, menus and the trace writer |
| `viewer.html` | page, SVG drawings and playback |
| `serve.py` | local server, build and browser launcher |
| `trace.json` | generated after every operation (ignored by git) |

`trace.json` is only a snapshot of one operation. The C program opens it with
`"w"`, writes each step, then closes it. The page ignores a half-written file
and retries on the next poll, so it never draws a broken step.

## Notes

- The C file uses only `stdio.h` and `stdlib.h`.
- No `sprintf`, no screen clearing, no `Sleep`, no ANSI escapes.
- Node types are separate: `Node` for the tree, `LNode` for the singly linked
  list, `DNode` for the doubly linked list.
- `SIZE` (10) is the capacity for the stack and both queues.

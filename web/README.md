# Web Visualizer

Same Data Structure Visualizer, but the drawing happens in the browser.
The C program stays a normal console program: you build the structures with
menus and `scanf`, and the page shows every step, the matching C function and
the exact source line that produced it.

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

Use the C menus normally. Every operation rewrites `trace.json`, and the server
pushes it to the page immediately, which plays the steps.

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

`Auto-play` is on by default, so a new operation starts playing by itself.
Untick it to step through every operation by hand. `Speed` changes playback
between 0.5x and 3x.

Keyboard: `←` / `→` step, `space` plays or pauses, `c` shows or hides the code.

## Debugger and code

Below the drawing there is a debugger panel:

- the current state: `idle`, `running`, `paused` or `done`,
- the C function that produced the step (`push`, `search`, `bfs`, ...),
- the source line number,
- the full function from `visualizer.c`, with the active line highlighted and
  scrolled into view.

The page reads `visualizer.c` from the same folder, so the code view only works
when the page is opened through `serve.py` (not with `file://`).

Each step in the trace stores `"fn"` and `"line"` alongside `"msg"` and
`"state"`. The C code fills them in with the `save_bst`, `save_stack`,
`save_queue`, `save_cq`, `save_sll`, `save_dll` and `save_graph` helpers, which
wrap `__func__` and `__LINE__`.

## Animations

- the drawing fades and lifts in on every step,
- nodes pop in when they are added or visited,
- edges draw themselves,
- the current node gets a pulsing halo,
- pushed and enqueued values pop into their slot,
- popped and dequeued values float away as a ghost,
- inserted list nodes pop in, deleted list nodes float away,
- the graph shows BFS/DFS order and the queue or stack it is using.

The page remembers the previous operation's final state and animates from it
when the new state is the old state with one item added or removed. That makes
pushes, pops and deletes animate even though each operation writes a new trace.

All motion is disabled automatically when the operating system asks for reduced
motion.

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
| `viewer.html` | page, SVG drawings, debugger and playback |
| `serve.py` | local server, build and browser launcher |
| `trace.json` | generated after every operation (ignored by git) |

`trace.json` is only a snapshot of one operation. The C program opens it with
`"w"`, writes each step, then closes it. The page receives the finished file
through `/events` (server-sent events), so updates appear without a refresh.
If that connection drops, the page falls back to polling and refreshes as soon
as the tab regains focus.

## Notes

- The C file uses only `stdio.h` and `stdlib.h`.
- No `sprintf`, no screen clearing, no `Sleep`, no ANSI escapes.
- Node types are separate: `Node` for the tree, `LNode` for the singly linked
  list, `DNode` for the doubly linked list.
- `SIZE` (10) is the capacity for the stack and both queues.
- Graph BFS/DFS reuse the normal queue and stack code, but set `tracing = 0`
  so those internal calls do not add stack/queue steps to the graph trace.

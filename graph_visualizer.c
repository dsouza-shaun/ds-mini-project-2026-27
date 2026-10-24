#include "visualizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

int graph_adj[MAX_GRAPH_VERTICES][MAX_GRAPH_VERTICES];
int graph_visited[MAX_GRAPH_VERTICES];
int graph_n = 5;
int graph_current_vertex = -1;
int graph_traversal_list[MAX_GRAPH_VERTICES];
int graph_traversal_count = 0;
Queue graph_bfs_q;
Stack graph_dfs_s;
int graph_algo_mode = 0;

void graph_visualize() {
    printf(" ADJACENCY MATRIX (n = %d):          VERTICES & EDGES:\n", graph_n);
    printf("    ");
    for (int j = 0; j < graph_n; j++) {
        printf(" %d ", j);
    }
    printf("        ");
    for (int i = 0; i < graph_n; i++) {
        if (i == graph_current_vertex) {
            printf("\033[1;33;41m*[%d]*\033[0m ", i);
        } else if (graph_visited[i]) {
            printf("\033[1;32m[%d]\033[0m ", i);
        } else {
            printf("[%d] ", i);
        }
    }
    printf("\n");

    for (int i = 0; i < graph_n; i++) {
        if (i == graph_current_vertex) {
            printf(" \033[1;33m%d |\033[0m", i);
        } else {
            printf(" %d |", i);
        }
        for (int j = 0; j < graph_n; j++) {
            if (graph_adj[i][j]) {
                printf(" \033[1;32m1\033[0m ");
            } else {
                printf(" 0 ");
            }
        }
        printf("        ");
        printf("Vertex %d connects to: ", i);
        int has = 0;
        for (int j = 0; j < graph_n; j++) {
            if (graph_adj[i][j]) {
                printf("%d ", j);
                has = 1;
            }
        }
        if (!has) printf("(none)");
        printf("\n");
    }

    printf("\n VISITED STATUS: [ ");
    for (int i = 0; i < graph_n; i++) {
        printf("%d: %s ", i, graph_visited[i] ? "\033[1;32mYES\033[0m" : "NO ");
    }
    printf("]\n");

    if (graph_algo_mode == 1) {
        printf(" BFS QUEUE     : [ ");
        if (!is_empty_queue(&graph_bfs_q)) {
            for (int i = graph_bfs_q.f; i <= graph_bfs_q.r; i++) {
                printf("%d ", graph_bfs_q.a[i]);
            }
        }
        printf("] (f: %d, r: %d)\n", graph_bfs_q.f, graph_bfs_q.r);
    } else if (graph_algo_mode == 2) {
        printf(" DFS STACK     : [ ");
        if (!is_empty_stack(&graph_dfs_s)) {
            for (int i = 0; i <= graph_dfs_s.top; i++) {
                printf("%d ", graph_dfs_s.arr[i]);
            }
        }
        printf("] (top: %d)\n", graph_dfs_s.top);
    }

    if (graph_traversal_count > 0) {
        printf(" \033[1;32mTRAVERSAL PATH:\033[0m ");
        for (int i = 0; i < graph_traversal_count; i++) {
            printf("%d%s", graph_traversal_list[i], (i == graph_traversal_count - 1) ? "" : " -> ");
        }
        printf("\n");
    }
}

void graph_add_edge(int u, int v) {
    if (u >= 0 && u < graph_n && v >= 0 && v < graph_n) {
        graph_adj[u][v] = 1;
        graph_adj[v][u] = 1;
    }
}

const char *bfs_code[] = {
    "void bfs(int start)",
    "{",
    "    Queue q; q.f = -1; q.r = -1;",
    "    int i, j;",
    "    for (i = 0; i < n; i++) visited[i] = 0;",
    "    enqueue(&q, start);",
    "    visited[start] = 1;",
    "    while (!isEmpty(&q))",
    "    {",
    "        int curr = dequeue(&q);",
    "        printf(\"%d \", curr);",
    "        for (j = 0; j < n; j++)",
    "        {",
    "            if (adj[curr][j] == 1 && visited[j] == 0)",
    "            {",
    "                enqueue(&q, j);",
    "                visited[j] = 1;",
    "            }",
    "        }",
    "    }",
    "}"
};

void bfs_debug(int start) {
    char locals[128];
    char status[128];
    int total = sizeof(bfs_code) / sizeof(bfs_code[0]);

    graph_algo_mode = 1;
    graph_bfs_q.f = -1;
    graph_bfs_q.r = -1;
    graph_traversal_count = 0;
    graph_current_vertex = -1;

    for (int i = 0; i < graph_n; i++) graph_visited[i] = 0;

    sprintf(locals, "start = %d | q.f = -1 | q.r = -1", start);
    sprintf(status, "Entering bfs(start = %d)", start);
    render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 0, status);

    for (int i = 0; i < graph_n; i++) graph_visited[i] = 0;
    sprintf(status, "Initialized visited array to 0 for all %d vertices", graph_n);
    render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 4, status);

    enqueue(&graph_bfs_q, start);
    sprintf(locals, "enqueued start = %d | q.f = %d | q.r = %d", start, graph_bfs_q.f, graph_bfs_q.r);
    sprintf(status, "Enqueued start vertex %d into queue", start);
    render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 5, status);

    graph_visited[start] = 1;
    sprintf(locals, "visited[%d] = 1", start);
    sprintf(status, "Marked vertex %d as visited", start);
    render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 6, status);

    while (!is_empty_queue(&graph_bfs_q)) {
        sprintf(status, "Queue not empty. Checking while (!isEmpty(&q))");
        render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 7, status);

        int curr = dequeue(&graph_bfs_q);
        graph_current_vertex = curr;
        sprintf(locals, "curr = %d", curr);
        sprintf(status, "Dequeued curr = %d", curr);
        render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 9, status);

        graph_traversal_list[graph_traversal_count++] = curr;
        sprintf(status, "Visited vertex %d. Output added to path.", curr);
        render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 10, status);

        for (int j = 0; j < graph_n; j++) {
            sprintf(locals, "curr = %d | j = %d | adj[%d][%d] = %d | visited[%d] = %d", curr, j, curr, j, graph_adj[curr][j], j, graph_visited[j]);
            sprintf(status, "Checking neighbor %d of %d", j, curr);
            render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 13, status);

            if (graph_adj[curr][j] == 1 && graph_visited[j] == 0) {
                enqueue(&graph_bfs_q, j);
                graph_visited[j] = 1;
                sprintf(locals, "enqueued %d | visited[%d] = 1", j, j);
                sprintf(status, "Neighbor %d is adjacent and unvisited. Enqueue and mark visited.", j);
                render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 15, status);
            }
        }
    }

    graph_current_vertex = -1;
    sprintf(status, "BFS complete! All reachable vertices visited.");
    render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 19, status);
    graph_algo_mode = 0;
}

const char *dfs_code[] = {
    "void dfs(int start)",
    "{",
    "    Stack s; s.top = -1;",
    "    int i, j;",
    "    for (i = 0; i < n; i++) visited[i] = 0;",
    "    push(&s, start);",
    "    visited[start] = 1;",
    "    printf(\"%d \", start);",
    "    while (!isEmpty(&s))",
    "    {",
    "        int curr = peek(&s);",
    "        int found = 0;",
    "        for (j = 0; j < n; j++)",
    "        {",
    "            if (adj[curr][j] == 1 && visited[j] == 0)",
    "            {",
    "                visited[j] = 1;",
    "                printf(\"%d \", j);",
    "                push(&s, j);",
    "                found = 1;",
    "                break;",
    "            }",
    "        }",
    "        if (found == 0)",
    "        {",
    "            pop(&s);",
    "        }",
    "    }",
    "}"
};

void dfs_debug(int start) {
    char locals[128];
    char status[128];
    int total = sizeof(dfs_code) / sizeof(dfs_code[0]);

    graph_algo_mode = 2;
    graph_dfs_s.top = -1;
    graph_traversal_count = 0;
    graph_current_vertex = -1;

    for (int i = 0; i < graph_n; i++) graph_visited[i] = 0;

    sprintf(locals, "start = %d | s.top = -1", start);
    sprintf(status, "Entering non-recursive dfs(start = %d)", start);
    render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 0, status);

    for (int i = 0; i < graph_n; i++) graph_visited[i] = 0;
    sprintf(status, "Initialized visited array to 0 for all %d vertices", graph_n);
    render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 4, status);

    push(&graph_dfs_s, start);
    graph_current_vertex = start;
    sprintf(locals, "pushed start = %d | s.top = %d", start, graph_dfs_s.top);
    sprintf(status, "Pushed start vertex %d onto DFS stack", start);
    render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 5, status);

    graph_visited[start] = 1;
    sprintf(locals, "visited[%d] = 1", start);
    sprintf(status, "Marked start vertex %d as visited", start);
    render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 6, status);

    graph_traversal_list[graph_traversal_count++] = start;
    sprintf(status, "Printed start vertex %d", start);
    render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 7, status);

    while (!is_empty_stack(&graph_dfs_s)) {
        sprintf(status, "Stack not empty. Checking while (!isEmpty(&s))");
        render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 8, status);

        int curr = peek(&graph_dfs_s);
        graph_current_vertex = curr;
        int found = 0;
        sprintf(locals, "curr = %d | s.top = %d", curr, graph_dfs_s.top);
        sprintf(status, "Top of stack is curr = %d. Scanning adjacent vertices for (j = 0; j < n; j++)", curr);
        render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 10, status);

        for (int j = 0; j < graph_n; j++) {
            sprintf(locals, "curr = %d | j = %d | adj[%d][%d] = %d | visited[%d] = %d", curr, j, curr, j, graph_adj[curr][j], j, graph_visited[j]);
            sprintf(status, "Checking edge (%d, %d)", curr, j);
            render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 14, status);

            if (graph_adj[curr][j] == 1 && graph_visited[j] == 0) {
                graph_visited[j] = 1;
                sprintf(locals, "visited[%d] = 1", j);
                sprintf(status, "Found unvisited neighbor %d. Marked visited.", j);
                render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 16, status);

                graph_traversal_list[graph_traversal_count++] = j;
                sprintf(status, "Printed vertex %d. Added to path.", j);
                render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 17, status);

                push(&graph_dfs_s, j);
                sprintf(locals, "pushed %d | s.top = %d", j, graph_dfs_s.top);
                sprintf(status, "Pushed vertex %d onto DFS stack", j);
                render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 18, status);

                found = 1;
                sprintf(status, "found = 1; break from loop to explore deeper from %d", j);
                render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 20, status);
                break;
            }
        }

        if (found == 0) {
            sprintf(locals, "curr = %d | found = 0", curr);
            sprintf(status, "No unvisited neighbors for vertex %d. Backtracking!", curr);
            render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 23, status);

            int popped = pop(&graph_dfs_s);
            sprintf(locals, "popped = %d | new s.top = %d", popped, graph_dfs_s.top);
            sprintf(status, "Popped vertex %d from stack. Backtracked to previous vertex.", popped);
            render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 25, status);
        }
    }

    graph_current_vertex = -1;
    sprintf(status, "DFS complete! All reachable vertices visited.");
    render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 28, status);
    graph_algo_mode = 0;
}


void graph_menu() {
    int ch, u, v, start, n_input;
    while (1) {
        clear_screen();
        printf("\033[1;37;44m ============================================================================== \033[0m\n");
        printf("\033[1;37;44m                           GRAPH ALGORITHMS VISUALIZER                         \033[0m\n");
        printf("\033[1;37;44m ============================================================================== \033[0m\n\n");
        graph_visualize();
        printf("\n--------------------------------------------------------------------------------\n");
        printf("1. Set Number of Vertices (Current n = %d, Max = %d)\n", graph_n, MAX_GRAPH_VERTICES);
        printf("2. Add Edge (u, v)\n");
        printf("3. Breadth First Search (BFS - Non-recursive)\n");
        printf("4. Depth First Search (DFS - Non-recursive)\n");
        printf("5. Clear All Edges\n");
        printf("6. Return to Main Menu\n");
        printf("--------------------------------------------------------------------------------\n");
        printf("Enter your choice: ");
        if (scanf("%d", &ch) != 1) {
            while (getchar() != '\n');
            continue;
        }

        step_mode = 0;

        switch (ch) {
            case 1:
                printf("Enter number of vertices (2 to %d): ", MAX_GRAPH_VERTICES);
                scanf("%d", &n_input);
                if (n_input >= 2 && n_input <= MAX_GRAPH_VERTICES) {
                    graph_n = n_input;
                    for (int i = 0; i < MAX_GRAPH_VERTICES; i++) {
                        graph_visited[i] = 0;
                        for (int j = 0; j < MAX_GRAPH_VERTICES; j++) graph_adj[i][j] = 0;
                    }
                    graph_traversal_count = 0;
                    printf("Graph resized to %d vertices. All edges reset.\n", graph_n);
                    Sleep(700);
                } else {
                    printf("Invalid number of vertices!\n");
                    Sleep(700);
                }
                break;
            case 2:
                printf("Enter vertex u (0 to %d): ", graph_n - 1);
                scanf("%d", &u);
                printf("Enter vertex v (0 to %d): ", graph_n - 1);
                scanf("%d", &v);
                if (u >= 0 && u < graph_n && v >= 0 && v < graph_n) {
                    graph_add_edge(u, v);
                } else {
                    printf("Invalid vertices!\n");
                    Sleep(700);
                }
                break;
            case 3:
                printf("Enter starting vertex (0 to %d): ", graph_n - 1);
                scanf("%d", &start);
                if (start >= 0 && start < graph_n) {
                    bfs_debug(start);
                } else {
                    printf("Invalid vertex!\n");
                    Sleep(700);
                }
                break;
            case 4:
                printf("Enter starting vertex (0 to %d): ", graph_n - 1);
                scanf("%d", &start);
                if (start >= 0 && start < graph_n) {
                    dfs_debug(start);
                } else {
                    printf("Invalid vertex!\n");
                    Sleep(700);
                }
                break;
            case 5:
                for (int i = 0; i < MAX_GRAPH_VERTICES; i++) {
                    graph_visited[i] = 0;
                    for (int j = 0; j < MAX_GRAPH_VERTICES; j++) graph_adj[i][j] = 0;
                }
                graph_traversal_count = 0;
                printf("\nAll edges cleared!\n");
                Sleep(600);
                break;
            case 6:
                return;
        }
    }
}


#include "visualizer.h"

int graph_adj[MAX_VERTICES][MAX_VERTICES];
int graph_visited[MAX_VERTICES];
int graph_n = 5;
int graph_current = -1;
int graph_path[MAX_VERTICES];
int graph_path_count = 0;
int graph_algo_mode = 0;

Queue graph_queue = {{0}, -1, -1};
Stack graph_stack = {{0}, -1};

void show_graph(void)
{
    int i, j;

    printf("ADJACENCY MATRIX (n = %d)\n", graph_n);
    printf("    ");
    for (j = 0; j < graph_n; j++)
        printf("%3d", j);
    printf("\n");

    for (i = 0; i < graph_n; i++)
    {
        printf("%3d |", i);
        for (j = 0; j < graph_n; j++)
            printf("%3d", graph_adj[i][j]);
        printf("\n");
    }

    printf("\nVisited: ");
    for (i = 0; i < graph_n; i++)
    {
        if (i == graph_current)
            printf("*%d* ", i);
        else if (graph_visited[i])
            printf("%d:yes ", i);
        else
            printf("%d:no ", i);
    }
    printf("\n");

    if (graph_algo_mode == 1)
    {
        printf("BFS queue: ");
        if (is_empty_queue(&graph_queue))
        {
            printf("(empty)");
        }
        else
        {
            for (i = graph_queue.f; i <= graph_queue.r; i++)
                printf("%d ", graph_queue.a[i]);
        }
        printf("\n");
    }
    else if (graph_algo_mode == 2)
    {
        printf("DFS stack: ");
        if (is_empty_stack(&graph_stack))
        {
            printf("(empty)");
        }
        else
        {
            for (i = 0; i <= graph_stack.top; i++)
                printf("%d ", graph_stack.arr[i]);
        }
        printf("\n");
    }

    if (graph_path_count > 0)
    {
        printf("Path: ");
        for (i = 0; i < graph_path_count; i++)
        {
            printf("%d", graph_path[i]);
            if (i < graph_path_count - 1)
                printf(" -> ");
        }
        printf("\n");
    }
}

void graph_add_edge(int u, int v)
{
    if (u >= 0 && u < graph_n && v >= 0 && v < graph_n)
    {
        graph_adj[u][v] = 1;
        graph_adj[v][u] = 1;
    }
}

void graph_reset(void)
{
    int i, j;

    for (i = 0; i < MAX_VERTICES; i++)
    {
        graph_visited[i] = 0;
        for (j = 0; j < MAX_VERTICES; j++)
            graph_adj[i][j] = 0;
    }

    graph_current = -1;
    graph_path_count = 0;
    graph_algo_mode = 0;
}

/* ---------------------------------------------------------------- */
/*                            BFS                                   */
/* ---------------------------------------------------------------- */

char *bfs_code[] = {
    "void bfs(int start)",
    "{",
    "    int i, curr, j;",
    "",
    "    for (i = 0; i < graph_n; i++)",
    "        graph_visited[i] = 0;",
    "",
    "    graph_queue.f = -1;",
    "    graph_queue.r = -1;",
    "    graph_path_count = 0;",
    "",
    "    enqueue(&graph_queue, start);",
    "    graph_visited[start] = 1;",
    "",
    "    while (!is_empty_queue(&graph_queue))",
    "    {",
    "        curr = dequeue(&graph_queue);",
    "        graph_path[graph_path_count] = curr;",
    "        graph_path_count++;",
    "",
    "        for (j = 0; j < graph_n; j++)",
    "        {",
    "            if (graph_adj[curr][j] == 1 && graph_visited[j] == 0)",
    "            {",
    "                enqueue(&graph_queue, j);",
    "                graph_visited[j] = 1;",
    "            }",
    "        }",
    "    }",
    "}"
};

void bfs(int start)
{
    int curr, j;
    char locals[160];
    char status[160];
    int total = sizeof(bfs_code) / sizeof(bfs_code[0]);

    graph_algo_mode = 1;
    graph_queue.f = -1;
    graph_queue.r = -1;
    graph_path_count = 0;
    graph_current = -1;

    sprintf(locals, "start = %d | graph_n = %d", start, graph_n);
    sprintf(status, "Start BFS from vertex %d.", start);
    show_frame(MODE_GRAPH, "Graph -> BFS", "void bfs(int start)",
               "main() -> bfs()", locals, bfs_code, total, 0, status);

    for (j = 0; j < graph_n; j++)
        graph_visited[j] = 0;

    show_frame(MODE_GRAPH, "Graph -> BFS", "void bfs(int start)",
               "main() -> bfs()", locals, bfs_code, total, 4,
               "Reset the visited array.");

    enqueue(&graph_queue, start);
    sprintf(locals, "enqueued %d | front = %d | rear = %d",
            start, graph_queue.f, graph_queue.r);
    show_frame(MODE_GRAPH, "Graph -> BFS", "void bfs(int start)",
               "main() -> bfs()", locals, bfs_code, total, 11,
               "Put the start vertex in the queue.");

    graph_visited[start] = 1;
    sprintf(locals, "visited[%d] = 1", start);
    show_frame(MODE_GRAPH, "Graph -> BFS", "void bfs(int start)",
               "main() -> bfs()", locals, bfs_code, total, 12,
               "Mark the start vertex as visited.");

    while (!is_empty_queue(&graph_queue))
    {
        show_frame(MODE_GRAPH, "Graph -> BFS", "void bfs(int start)",
                   "main() -> bfs()", locals, bfs_code, total, 14,
                   "Queue is not empty, so continue.");

        curr = dequeue(&graph_queue);
        graph_current = curr;

        sprintf(locals, "curr = %d | front = %d | rear = %d",
                curr, graph_queue.f, graph_queue.r);
        show_frame(MODE_GRAPH, "Graph -> BFS", "void bfs(int start)",
                   "main() -> bfs()", locals, bfs_code, total, 16,
                   "Remove the front vertex from the queue.");

        graph_path[graph_path_count] = curr;
        graph_path_count++;

        sprintf(status, "Visit vertex %d and add it to the path.", curr);
        show_frame(MODE_GRAPH, "Graph -> BFS", "void bfs(int start)",
                   "main() -> bfs()", locals, bfs_code, total, 17, status);

        for (j = 0; j < graph_n; j++)
        {
            sprintf(locals, "curr = %d | j = %d | adj = %d | visited[%d] = %d",
                    curr, j, graph_adj[curr][j], j, graph_visited[j]);

            show_frame(MODE_GRAPH, "Graph -> BFS", "void bfs(int start)",
                       "main() -> bfs()", locals, bfs_code, total, 22,
                       "Check if this neighbour is adjacent and unvisited.");

            if (graph_adj[curr][j] == 1 && graph_visited[j] == 0)
            {
                enqueue(&graph_queue, j);
                graph_visited[j] = 1;

                sprintf(locals, "enqueued %d | front = %d | rear = %d",
                        j, graph_queue.f, graph_queue.r);
                sprintf(status, "Neighbour %d added to the queue and marked visited.", j);
                show_frame(MODE_GRAPH, "Graph -> BFS", "void bfs(int start)",
                           "main() -> bfs()", locals, bfs_code, total, 24, status);
            }
        }
    }

    graph_current = -1;
    graph_algo_mode = 0;

    sprintf(status, "BFS complete. Path: %d vertices.", graph_path_count);
    show_frame(MODE_GRAPH, "Graph -> BFS", "void bfs(int start)",
               "main() -> bfs()", locals, bfs_code, total, 29, status);
}

/* ---------------------------------------------------------------- */
/*                            DFS                                   */
/* ---------------------------------------------------------------- */

char *dfs_code[] = {
    "void dfs(int start)",
    "{",
    "    int i, curr, j, found;",
    "",
    "    for (i = 0; i < graph_n; i++)",
    "        graph_visited[i] = 0;",
    "",
    "    graph_stack.top = -1;",
    "    graph_path_count = 0;",
    "",
    "    push(&graph_stack, start);",
    "    graph_visited[start] = 1;",
    "    graph_path[graph_path_count] = start;",
    "    graph_path_count++;",
    "",
    "    while (!is_empty_stack(&graph_stack))",
    "    {",
    "        curr = peek(&graph_stack);",
    "        found = 0;",
    "",
    "        for (j = 0; j < graph_n; j++)",
    "        {",
    "            if (graph_adj[curr][j] == 1 && graph_visited[j] == 0)",
    "            {",
    "                graph_visited[j] = 1;",
    "                graph_path[graph_path_count] = j;",
    "                graph_path_count++;",
    "                push(&graph_stack, j);",
    "                found = 1;",
    "                break;",
    "            }",
    "        }",
    "",
    "        if (found == 0)",
    "            pop(&graph_stack);",
    "    }",
    "}"
};

void dfs(int start)
{
    int curr, j, found, popped;
    char locals[160];
    char status[160];
    int total = sizeof(dfs_code) / sizeof(dfs_code[0]);

    graph_algo_mode = 2;
    graph_stack.top = -1;
    graph_path_count = 0;
    graph_current = -1;

    sprintf(locals, "start = %d | graph_n = %d", start, graph_n);
    sprintf(status, "Start DFS from vertex %d.", start);
    show_frame(MODE_GRAPH, "Graph -> DFS", "void dfs(int start)",
               "main() -> dfs()", locals, dfs_code, total, 0, status);

    for (j = 0; j < graph_n; j++)
        graph_visited[j] = 0;

    show_frame(MODE_GRAPH, "Graph -> DFS", "void dfs(int start)",
               "main() -> dfs()", locals, dfs_code, total, 4,
               "Reset the visited array.");

    push(&graph_stack, start);
    graph_current = start;

    sprintf(locals, "pushed %d | top = %d", start, graph_stack.top);
    show_frame(MODE_GRAPH, "Graph -> DFS", "void dfs(int start)",
               "main() -> dfs()", locals, dfs_code, total, 10,
               "Push the start vertex on the stack.");

    graph_visited[start] = 1;
    show_frame(MODE_GRAPH, "Graph -> DFS", "void dfs(int start)",
               "main() -> dfs()", locals, dfs_code, total, 11,
               "Mark the start vertex as visited.");

    graph_path[graph_path_count] = start;
    graph_path_count++;
    show_frame(MODE_GRAPH, "Graph -> DFS", "void dfs(int start)",
               "main() -> dfs()", locals, dfs_code, total, 12,
               "Add the start vertex to the path.");

    while (!is_empty_stack(&graph_stack))
    {
        show_frame(MODE_GRAPH, "Graph -> DFS", "void dfs(int start)",
                   "main() -> dfs()", locals, dfs_code, total, 15,
                   "Stack is not empty, so continue.");

        curr = peek(&graph_stack);
        graph_current = curr;
        found = 0;

        sprintf(locals, "curr = %d | top = %d | found = 0", curr, graph_stack.top);
        show_frame(MODE_GRAPH, "Graph -> DFS", "void dfs(int start)",
                   "main() -> dfs()", locals, dfs_code, total, 17,
                   "Look at the vertex on top of the stack.");

        for (j = 0; j < graph_n; j++)
        {
            sprintf(locals, "curr = %d | j = %d | adj = %d | visited[%d] = %d",
                    curr, j, graph_adj[curr][j], j, graph_visited[j]);

            show_frame(MODE_GRAPH, "Graph -> DFS", "void dfs(int start)",
                       "main() -> dfs()", locals, dfs_code, total, 22,
                       "Check this neighbour.");

            if (graph_adj[curr][j] == 1 && graph_visited[j] == 0)
            {
                graph_visited[j] = 1;
                graph_path[graph_path_count] = j;
                graph_path_count++;

                sprintf(locals, "visited[%d] = 1 | found = 1", j);
                sprintf(status, "Found unvisited neighbour %d. Mark it visited.", j);
                show_frame(MODE_GRAPH, "Graph -> DFS", "void dfs(int start)",
                           "main() -> dfs()", locals, dfs_code, total, 24, status);

                push(&graph_stack, j);

                sprintf(locals, "pushed %d | top = %d", j, graph_stack.top);
                sprintf(status, "Push %d and go deeper from there.", j);
                show_frame(MODE_GRAPH, "Graph -> DFS", "void dfs(int start)",
                           "main() -> dfs()", locals, dfs_code, total, 27, status);

                found = 1;
                break;
            }
        }

        if (found == 0)
        {
            popped = pop(&graph_stack);

            sprintf(locals, "popped = %d | top = %d", popped, graph_stack.top);
            sprintf(status, "No unvisited neighbour for %d, so backtrack.", curr);
            show_frame(MODE_GRAPH, "Graph -> DFS", "void dfs(int start)",
                       "main() -> dfs()", locals, dfs_code, total, 34, status);
        }
    }

    graph_current = -1;
    graph_algo_mode = 0;

    sprintf(status, "DFS complete. Path: %d vertices.", graph_path_count);
    show_frame(MODE_GRAPH, "Graph -> DFS", "void dfs(int start)",
               "main() -> dfs()", locals, dfs_code, total, 36, status);
}

/* ---------------------------------------------------------------- */
/*                            menu                                  */
/* ---------------------------------------------------------------- */

void graph_menu(void)
{
    int ch, u, v, start, n;

    while (1)
    {
        clear_screen();
        printf("==============================================================\n");
        printf("                      GRAPH (BFS / DFS)\n");
        printf("==============================================================\n");
        show_graph();
        printf("\n--------------------------------------------------------------\n");
        printf("1. Set number of vertices (2 to %d)\n", MAX_VERTICES);
        printf("2. Add an edge\n");
        printf("3. BFS\n");
        printf("4. DFS\n");
        printf("5. Clear all edges\n");
        printf("6. Return to main menu\n");
        printf("--------------------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        step_mode = 0;

        switch (ch)
        {
        case 1:
            printf("Enter number of vertices: ");
            scanf("%d", &n);

            if (n >= 2 && n <= MAX_VERTICES)
            {
                graph_n = n;
                graph_reset();
                printf("\nGraph now has %d vertices and no edges.\n", graph_n);
            }
            else
            {
                printf("\nInvalid number of vertices.\n");
            }
            wait_key();
            break;

        case 2:
            printf("Enter first vertex (0 to %d): ", graph_n - 1);
            scanf("%d", &u);
            printf("Enter second vertex (0 to %d): ", graph_n - 1);
            scanf("%d", &v);

            if (u >= 0 && u < graph_n && v >= 0 && v < graph_n)
            {
                graph_add_edge(u, v);
                printf("\nEdge (%d, %d) added.\n", u, v);
            }
            else
            {
                printf("\nInvalid vertices.\n");
            }
            wait_key();
            break;

        case 3:
            printf("Enter start vertex (0 to %d): ", graph_n - 1);
            scanf("%d", &start);

            if (start >= 0 && start < graph_n)
                bfs(start);
            else
            {
                printf("\nInvalid vertex.\n");
                wait_key();
            }
            break;

        case 4:
            printf("Enter start vertex (0 to %d): ", graph_n - 1);
            scanf("%d", &start);

            if (start >= 0 && start < graph_n)
                dfs(start);
            else
            {
                printf("\nInvalid vertex.\n");
                wait_key();
            }
            break;

        case 5:
            graph_reset();
            printf("\nAll edges cleared.\n");
            wait_key();
            break;

        case 6:
            return;

        default:
            printf("\nInvalid choice\n");
            wait_key();
        }
    }
}

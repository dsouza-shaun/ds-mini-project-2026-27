#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <io.h>
#include "visualizer.h"

int step_mode = 0;

void enable_ansi() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004;
    SetConsoleMode(hOut, dwMode);
}

void clear_screen() {
    system("cls");
}

int wait_key() {
    if (!_isatty(_fileno(stdin))) {
        return getchar();
    }
    return _getch();
}

void pause_step() {
    if (!_isatty(_fileno(stdin))) {
        return;
    }
    if (step_mode == 0) {
        printf("\n\033[1;36m[ENTER/SPACE]\033[0m Step  |  \033[1;33m[A]\033[0m Auto-Play  |  \033[1;32m[C]\033[0m Continue  > ");
        while (1) {
            int ch = wait_key();
            if (ch == 13 || ch == ' ' || ch == 10) {
                break;
            } else if (ch == 'a' || ch == 'A') {
                step_mode = 1;
                break;
            } else if (ch == 'c' || ch == 'C') {
                step_mode = 2;
                break;
            }
        }
    } else if (step_mode == 1) {
        Sleep(600);
        if (_kbhit()) {
            int ch = wait_key();
            if (ch == ' ' || ch == 13) {
                step_mode = 0;
            } else if (ch == 'c' || ch == 'C') {
                step_mode = 2;
            }
        }
    } else if (step_mode == 2) {
        Sleep(40);
    }
}

void render_debugger_frame(const char *title, void (*render_visual)(void), const char *func_sig, const char *call_stack, const char *locals, const char *code[], int total_lines, int cur_line, const char *status) {
    clear_screen();
    printf("\033[1;37;44m ============================================================================== \033[0m\n");
    printf("\033[1;37;44m                    DATA STRUCTURE VISUALIZER & DEBUGGER                        \033[0m\n");
    printf("\033[1;37;44m ============================================================================== \033[0m\n");
    printf("\033[1;34m [MODE]\033[0m %s\n", title);
    printf("--------------------------------------------------------------------------------\n");
    printf("\033[1;35m[VISUALIZATION]\033[0m\n");
    if (render_visual) {
        render_visual();
    }
    printf("\n--------------------------------------------------------------------------------\n");
    printf("\033[1;33m[DEBUGGER]\033[0m %s\n", func_sig);
    printf("\033[1;36mCALL STACK:\033[0m %s\n", call_stack);
    printf("\033[1;36mVARIABLES :\033[0m %s\n", locals);
    printf("--------------------------------------------------------------------------------\n");
    printf("\033[1;32mSOURCE CODE:\033[0m\n");
    for (int i = 0; i < total_lines; i++) {
        if (i == cur_line) {
            printf("\033[1;33;44m==> %2d | %-70s\033[0m\n", i + 1, code[i]);
        } else {
            printf("    %2d | %s\n", i + 1, code[i]);
        }
    }
    printf("--------------------------------------------------------------------------------\n");
    printf("\033[1;37mSTATUS:\033[0m %s\n", status);
    pause_step();
}


int main() {
    enable_ansi();

    bst_root = NULL;
    st.top = -1;
    qu.f = -1;
    qu.r = -1;
    ll_head = NULL;

    for (int i = 0; i < MAX_GRAPH_VERTICES; i++) {
        graph_visited[i] = 0;
        for (int j = 0; j < MAX_GRAPH_VERTICES; j++) {
            graph_adj[i][j] = 0;
        }
    }

    int ch;
    while (1) {
        clear_screen();
        printf("\033[1;37;44m ============================================================================== \033[0m\n");
        printf("\033[1;37;44m                  DATA STRUCTURE VISUALIZER & CODE DEBUGGER                    \033[0m\n");
        printf("\033[1;37;44m ============================================================================== \033[0m\n\n");
        printf(" Select Data Structure to Explore & Debug:\n\n");
        printf("   \033[1;33m1.\033[0m Binary Search Tree (BST)  \033[90m[Insert, Delete, Search, Traversals]\033[0m\n");
        printf("   \033[1;33m2.\033[0m Graph Algorithms          \033[90m[Non-recursive BFS & DFS, Adjacency Matrix]\033[0m\n");
        printf("   \033[1;33m3.\033[0m Stack                     \033[90m[Push, Pop, Peek, Array state]\033[0m\n");
        printf("   \033[1;33m4.\033[0m Queues                    \033[90m[Simple Queue, Circular Queue]\033[0m\n");
        printf("   \033[1;33m5.\033[0m Linked Lists              \033[90m[Singly, Doubly]\033[0m\n");
        printf("   \033[1;31m6.\033[0m Exit\n\n");
        printf("--------------------------------------------------------------------------------\n");
        printf(" Enter your choice: ");
        if (scanf("%d", &ch) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (ch) {
            case 1:
                bst_menu();
                break;
            case 2:
                graph_menu();
                break;
            case 3:
                stack_menu();
                break;
            case 4:
                queue_menu();
                break;
            case 5:
                ll_menu();
                break;
            case 6:
                clear_screen();
                printf("\nExiting Data Structure Visualizer. Goodbye!\n");
                exit(0);
        }
    }

    return 0;
}

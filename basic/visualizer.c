/* windows.h is included first because it also has a type named SIZE. */
#include <conio.h>
#include <windows.h>

#include "visualizer.h"

int step_mode = 0;

void clear_screen(void)
{
    system("cls");
}

void wait_key(void)
{
    printf("\nPress any key to continue...");
    _getch();
}

/*
 * step_mode = 0 : wait for Enter or Space
 * step_mode = 1 : auto play, wait about 0.6 seconds
 * step_mode = 2 : continue quickly, wait about 40 ms
 */
void pause_step(void)
{
    int ch;

    if (step_mode == 0)
    {
        printf("\n[ENTER/SPACE] Step  |  [A] Auto-Play  |  [C] Continue  > ");
        while (1)
        {
            ch = _getch();
            if (ch == -1)
                return;
            if (ch == 13 || ch == 32)
                break;
            if (ch == 'a' || ch == 'A')
            {
                step_mode = 1;
                break;
            }
            if (ch == 'c' || ch == 'C')
            {
                step_mode = 2;
                break;
            }
        }
    }
    else if (step_mode == 1)
    {
        Sleep(600);
        if (_kbhit())
        {
            ch = _getch();
            if (ch == 13 || ch == 32)
                step_mode = 0;
            else if (ch == 'c' || ch == 'C')
                step_mode = 2;
        }
    }
    else
    {
        Sleep(40);
    }
}

void print_code(char *code[], int total, int current)
{
    int i;

    for (i = 0; i < total; i++)
    {
        if (i == current)
            printf("==> %2d | %s\n", i + 1, code[i]);
        else
            printf("    %2d | %s\n", i + 1, code[i]);
    }
}

/*
 * mode        : which structure to draw
 * title       : heading shown in [MODE]
 * func        : function name shown in [DEBUGGER]
 * call_stack  : small call stack text
 * locals      : variable values for this step
 * code        : array of source lines
 * total       : how many source lines there are
 * current     : which source line is running (starts at 0)
 * status      : what is happening in this step
 */
void show_frame(int mode, char *title, char *func, char *call_stack,
                char *locals, char *code[], int total, int current,
                char *status)
{
    clear_screen();
    printf("==============================================================\n");
    printf("                DATA STRUCTURE VISUALIZER\n");
    printf("==============================================================\n");
    printf("[MODE] %s\n", title);
    printf("--------------------------------------------------------------\n");
    printf("[VISUALIZATION]\n");

    if (mode == MODE_BST)
        show_bst();
    else if (mode == MODE_GRAPH)
        show_graph();
    else if (mode == MODE_STACK)
        show_stack();
    else if (mode == MODE_QUEUE)
        show_queue();
    else if (mode == MODE_CIRCULAR)
        show_circular();
    else if (mode == MODE_SLL)
        show_sll();
    else if (mode == MODE_DLL)
        show_dll();

    printf("\n--------------------------------------------------------------\n");
    printf("[DEBUGGER] %s\n", func);
    printf("CALL STACK: %s\n", call_stack);
    printf("VARIABLES : %s\n", locals);
    printf("--------------------------------------------------------------\n");
    printf("[SOURCE CODE]\n");
    print_code(code, total, current);
    printf("--------------------------------------------------------------\n");
    printf("STATUS: %s\n", status);
    pause_step();
}

int main(void)
{
    int ch;

    while (1)
    {
        clear_screen();
        printf("==============================================================\n");
        printf("            DATA STRUCTURE VISUALIZER (BASIC)\n");
        printf("==============================================================\n\n");
        printf("1. Binary Search Tree\n");
        printf("2. Graph (BFS / DFS)\n");
        printf("3. Stack\n");
        printf("4. Queues\n");
        printf("5. Linked Lists\n");
        printf("6. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        switch (ch)
        {
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
            exit(0);
        default:
            printf("\nInvalid choice\n");
            wait_key();
        }
    }

    return 0;
}

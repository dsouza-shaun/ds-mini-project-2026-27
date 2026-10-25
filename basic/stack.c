#include "visualizer.h"

Stack st = {{0}, -1};

void push(Stack *p, int ele)
{
    if (p->top == SIZE - 1)
    {
        printf("Stack Overflow\n");
    }
    else
    {
        p->top++;
        p->arr[p->top] = ele;
    }
}

int pop(Stack *p)
{
    if (p->top == -1)
    {
        printf("Stack Underflow\n");
        return -1;
    }

    return p->arr[p->top--];
}

int peek(Stack *p)
{
    if (p->top == -1)
        return -1;

    return p->arr[p->top];
}

int is_empty_stack(Stack *p)
{
    if (p->top == -1)
        return 1;

    return 0;
}

int is_full_stack(Stack *p)
{
    if (p->top == SIZE - 1)
        return 1;

    return 0;
}

void show_stack(void)
{
    int i;

    printf("STACK (top = %d, size = %d)\n", st.top, SIZE);

    if (st.top == -1)
    {
        printf("+------+\n");
        printf("|empty |\n");
        printf("+------+\n");
        return;
    }

    for (i = st.top; i >= 0; i--)
    {
        printf("+------+\n");
        if (i == st.top)
            printf("|%5d | <-- top\n", st.arr[i]);
        else
            printf("|%5d |\n", st.arr[i]);
    }
    printf("+------+\n");
}

/* ---------------------------------------------------------------- */
/*                          push                                    */
/* ---------------------------------------------------------------- */

char *push_code[] = {
    "void push(Stack *p, int ele)",
    "{",
    "    if (p->top == SIZE - 1)",
    "    {",
    "        printf(\"Stack Overflow\\n\");",
    "    }",
    "    else",
    "    {",
    "        p->top++;",
    "        p->arr[p->top] = ele;",
    "    }",
    "}"
};

void push_debug(Stack *p, int ele)
{
    char locals[160];
    char status[160];
    int total = sizeof(push_code) / sizeof(push_code[0]);

    sprintf(locals, "ele = %d | p->top = %d", ele, p->top);
    sprintf(status, "Enter push with value %d.", ele);
    show_frame(MODE_STACK, "Stack -> push()", "void push(Stack *p, int ele)",
               "main() -> push()", locals, push_code, total, 0, status);

    sprintf(status, "Check if the stack is full: top == SIZE - 1.");
    show_frame(MODE_STACK, "Stack -> push()", "void push(Stack *p, int ele)",
               "main() -> push()", locals, push_code, total, 2, status);

    if (p->top == SIZE - 1)
    {
        sprintf(status, "Stack Overflow: the stack is already full.");
        show_frame(MODE_STACK, "Stack -> push()", "void push(Stack *p, int ele)",
                   "main() -> push()", locals, push_code, total, 4, status);
    }
    else
    {
        p->top++;
        sprintf(locals, "ele = %d | p->top = %d", ele, p->top);
        sprintf(status, "Increase top to %d.", p->top);
        show_frame(MODE_STACK, "Stack -> push()", "void push(Stack *p, int ele)",
                   "main() -> push()", locals, push_code, total, 8, status);

        p->arr[p->top] = ele;
        sprintf(locals, "p->arr[%d] = %d", p->top, ele);
        sprintf(status, "Store %d at arr[%d].", ele, p->top);
        show_frame(MODE_STACK, "Stack -> push()", "void push(Stack *p, int ele)",
                   "main() -> push()", locals, push_code, total, 9, status);
    }
}

/* ---------------------------------------------------------------- */
/*                           pop                                    */
/* ---------------------------------------------------------------- */

char *pop_code[] = {
    "int pop(Stack *p)",
    "{",
    "    if (p->top == -1)",
    "    {",
    "        printf(\"Stack Underflow\\n\");",
    "        return -1;",
    "    }",
    "    else",
    "    {",
    "        return p->arr[p->top--];",
    "    }",
    "}"
};

int pop_debug(Stack *p)
{
    char locals[160];
    char status[160];
    int value;
    int total = sizeof(pop_code) / sizeof(pop_code[0]);

    sprintf(locals, "p->top = %d", p->top);
    sprintf(status, "Enter pop.");
    show_frame(MODE_STACK, "Stack -> pop()", "int pop(Stack *p)",
               "main() -> pop()", locals, pop_code, total, 0, status);

    sprintf(status, "Check if the stack is empty: top == -1.");
    show_frame(MODE_STACK, "Stack -> pop()", "int pop(Stack *p)",
               "main() -> pop()", locals, pop_code, total, 2, status);

    if (p->top == -1)
    {
        sprintf(status, "Stack Underflow: there is nothing to pop.");
        show_frame(MODE_STACK, "Stack -> pop()", "int pop(Stack *p)",
                   "main() -> pop()", locals, pop_code, total, 4, status);
        return -1;
    }

    value = p->arr[p->top];
    p->top--;

    sprintf(locals, "value = %d | new top = %d", value, p->top);
    sprintf(status, "Return %d and decrease top.", value);
    show_frame(MODE_STACK, "Stack -> pop()", "int pop(Stack *p)",
               "main() -> pop()", locals, pop_code, total, 9, status);

    return value;
}

/* ---------------------------------------------------------------- */
/*                           menu                                   */
/* ---------------------------------------------------------------- */

void stack_menu(void)
{
    int ch, ele;

    while (1)
    {
        clear_screen();
        printf("==============================================================\n");
        printf("                         STACK\n");
        printf("==============================================================\n");
        show_stack();
        printf("\n--------------------------------------------------------------\n");
        printf("1. Push\n");
        printf("2. Pop\n");
        printf("3. Peek\n");
        printf("4. Clear stack\n");
        printf("5. Return to main menu\n");
        printf("--------------------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        step_mode = 0;

        switch (ch)
        {
        case 1:
            printf("Enter data: ");
            scanf("%d", &ele);
            push_debug(&st, ele);
            break;

        case 2:
            pop_debug(&st);
            break;

        case 3:
            if (st.top == -1)
                printf("\nStack is empty.\n");
            else
                printf("\nTop element: %d\n", peek(&st));
            wait_key();
            break;

        case 4:
            st.top = -1;
            printf("\nStack cleared.\n");
            wait_key();
            break;

        case 5:
            return;

        default:
            printf("\nInvalid choice\n");
            wait_key();
        }
    }
}

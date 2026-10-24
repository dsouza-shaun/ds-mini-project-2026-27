#include "visualizer.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>

Stack st;

void push(Stack *p, int ele) {
    if (p->top == STACK_SIZE - 1) {
        printf("Stack Overflow\n");
    } else {
        p->top++;
        p->arr[p->top] = ele;
    }
}

int pop(Stack *p) {
    if (p->top == -1) {
        printf("Stack Underflow\n");
        return -1;
    } else {
        return p->arr[p->top--];
    }
}

int peek(Stack *p) {
    if (p->top == -1) return -1;
    return p->arr[p->top];
}

int is_empty_stack(Stack *p) {
    if (p->top == -1) return 1;
    return 0;
}

Queue qu;

void enqueue(Queue *p, int ele) {
    if (p->r == QUEUE_SIZE - 1) {
        printf("Queue Overflow\n");
    } else if (p->f == -1 && p->r == -1) {
        p->f = p->r = 0;
        p->a[p->r] = ele;
    } else {
        p->r++;
        p->a[p->r] = ele;
    }
}

int dequeue(Queue *p) {
    int x;
    if (p->f == -1 && p->r == -1) {
        printf("Underflow\n");
        return -1;
    } else if (p->f == p->r) {
        x = p->a[p->f];
        p->f = p->r = -1;
        return x;
    } else {
        x = p->a[p->f];
        p->f++;
        return x;
    }
}

int is_empty_queue(Queue *p) {
    if (p->f == -1 && p->r == -1) return 1;
    return 0;
}


void stack_visualize() {
    printf(" STACK (SIZE = %d):\n", STACK_SIZE);
    if (st.top == -1) {
        printf("       +---------+\n");
        printf("       | \033[90m (EMPTY) \033[0m |\n");
        printf("       +---------+\n");
        printf("   \033[90m(Stack is empty: top = -1. Use Push to add items!)\033[0m\n");
        return;
    }
    int show_from = st.top + 2;
    if (show_from >= STACK_SIZE) show_from = STACK_SIZE - 1;
    for (int i = show_from; i >= 0; i--) {
        if (i == st.top) {
            printf("   [%d] | \033[1;33;41m  %3d  \033[0m | <-- \033[1;33mTOP (%d)\033[0m\n", i, st.arr[i], st.top);
        } else if (i < st.top) {
            printf("   [%d] | \033[1;32m  %3d  \033[0m |\n", i, st.arr[i]);
        } else {
            printf("   [%d] |        |\n", i);
        }
    }
    printf("       +---------+\n");
}

const char *push_code[] = {
    "void push(Stack *p, int ele)",
    "{",
    "    if(p->top == SIZE-1)",
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

void push_debug(Stack *p, int ele) {
    char locals[128];
    char status[128];
    int total = sizeof(push_code) / sizeof(push_code[0]);

    sprintf(locals, "ele = %d | p->top = %d", ele, p->top);
    sprintf(status, "Entering push(&s, ele = %d)", ele);
    render_debugger_frame("Stack -> push()", stack_visualize, "push(Stack *p, int ele)", "main() -> push()", locals, push_code, total, 0, status);

    sprintf(status, "Checking if p->top == SIZE - 1 (%d == %d)", p->top, STACK_SIZE - 1);
    render_debugger_frame("Stack -> push()", stack_visualize, "push(Stack *p, int ele)", "main() -> push()", locals, push_code, total, 2, status);

    if (p->top == STACK_SIZE - 1) {
        sprintf(status, "Stack Overflow! Stack is completely full.");
        render_debugger_frame("Stack -> push()", stack_visualize, "push(Stack *p, int ele)", "main() -> push()", locals, push_code, total, 4, status);
    } else {
        p->top++;
        sprintf(locals, "ele = %d | p->top = %d", ele, p->top);
        sprintf(status, "Incremented p->top to %d", p->top);
        render_debugger_frame("Stack -> push()", stack_visualize, "push(Stack *p, int ele)", "main() -> push()", locals, push_code, total, 8, status);

        p->arr[p->top] = ele;
        sprintf(locals, "p->arr[%d] = %d", p->top, ele);
        sprintf(status, "Stored element %d at arr[top]", ele);
        render_debugger_frame("Stack -> push()", stack_visualize, "push(Stack *p, int ele)", "main() -> push()", locals, push_code, total, 9, status);
    }
}

const char *pop_code[] = {
    "int pop(Stack *p)",
    "{",
    "    if(p->top == -1)",
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

int pop_debug(Stack *p) {
    char locals[128];
    char status[128];
    int total = sizeof(pop_code) / sizeof(pop_code[0]);

    sprintf(locals, "p->top = %d", p->top);
    sprintf(status, "Entering pop(&s)");
    render_debugger_frame("Stack -> pop()", stack_visualize, "pop(Stack *p)", "main() -> pop()", locals, pop_code, total, 0, status);

    sprintf(status, "Checking if p->top == -1: %s", p->top == -1 ? "TRUE" : "FALSE");
    render_debugger_frame("Stack -> pop()", stack_visualize, "pop(Stack *p)", "main() -> pop()", locals, pop_code, total, 2, status);

    if (p->top == -1) {
        sprintf(status, "Stack Underflow! Stack is empty. Returning -1");
        render_debugger_frame("Stack -> pop()", stack_visualize, "pop(Stack *p)", "main() -> pop()", locals, pop_code, total, 4, status);
        return -1;
    } else {
        int val = p->arr[p->top];
        p->top--;
        sprintf(locals, "popped value = %d | new top = %d", val, p->top);
        sprintf(status, "Popped %d from stack. Decremented top.", val);
        render_debugger_frame("Stack -> pop()", stack_visualize, "pop(Stack *p)", "main() -> pop()", locals, pop_code, total, 9, status);
        return val;
    }
}

void queue_visualize() {
    printf(" QUEUE (SIZE = %d):\n\n", QUEUE_SIZE);
    int display_len = 8;
    printf(" Indices:   ");
    for (int i = 0; i < display_len; i++) {
        printf("    %d    ", i);
    }
    printf("\n          +");
    for (int i = 0; i < display_len; i++) {
        printf("-------+");
    }
    printf("\n Array:   |");
    for (int i = 0; i < display_len; i++) {
        if (qu.f != -1 && i >= qu.f && i <= qu.r) {
            printf(" \033[1;32m %3d \033[0m |", qu.a[i]);
        } else {
            printf("       |");
        }
    }
    printf("\n          +");
    for (int i = 0; i < display_len; i++) {
        printf("-------+");
    }
    printf("\n Pointers: ");
    for (int i = 0; i < display_len; i++) {
        if (qu.f != -1 && i == qu.f && i == qu.r) {
            printf(" \033[1;33m f,r   \033[0m ");
        } else if (qu.f != -1 && i == qu.f) {
            printf(" \033[1;36m  f    \033[0m ");
        } else if (qu.r != -1 && i == qu.r) {
            printf(" \033[1;35m  r    \033[0m ");
        } else {
            printf("        ");
        }
    }
    printf("\n");
    if (qu.f == -1 && qu.r == -1) {
        printf(" \033[90m(Queue is empty: f = -1, r = -1. Use Enqueue to add items!)\033[0m\n");
    }
}

const char *enqueue_code[] = {
    "void enqueue(Queue *p, int ele)",
    "{",
    "    if (p->r == SIZE - 1)",
    "    {",
    "        printf(\"Queue Overflow\\n\");",
    "    }",
    "    else if (p->f == -1 && p->r == -1)",
    "    {",
    "        p->f = p->r = 0;",
    "        p->a[p->r] = ele;",
    "    }",
    "    else",
    "    {",
    "        p->r++;",
    "        p->a[p->r] = ele;",
    "    }",
    "}"
};

void enqueue_debug(Queue *p, int ele) {
    char locals[128];
    char status[128];
    int total = sizeof(enqueue_code) / sizeof(enqueue_code[0]);

    sprintf(locals, "ele = %d | p->f = %d | p->r = %d", ele, p->f, p->r);
    sprintf(status, "Entering enqueue(&q, ele = %d)", ele);
    render_debugger_frame("Queue -> enqueue()", queue_visualize, "enqueue(Queue *p, int ele)", "main() -> enqueue()", locals, enqueue_code, total, 0, status);

    sprintf(status, "Checking if p->r == SIZE - 1 (%d == %d)", p->r, QUEUE_SIZE - 1);
    render_debugger_frame("Queue -> enqueue()", queue_visualize, "enqueue(Queue *p, int ele)", "main() -> enqueue()", locals, enqueue_code, total, 2, status);

    if (p->r == QUEUE_SIZE - 1) {
        sprintf(status, "Queue Overflow! Cannot enqueue %d.", ele);
        render_debugger_frame("Queue -> enqueue()", queue_visualize, "enqueue(Queue *p, int ele)", "main() -> enqueue()", locals, enqueue_code, total, 4, status);
    } else if (p->f == -1 && p->r == -1) {
        sprintf(status, "Queue is empty. Setting f = r = 0");
        render_debugger_frame("Queue -> enqueue()", queue_visualize, "enqueue(Queue *p, int ele)", "main() -> enqueue()", locals, enqueue_code, total, 6, status);

        p->f = p->r = 0;
        p->a[p->r] = ele;
        sprintf(locals, "p->f = 0 | p->r = 0 | p->a[0] = %d", ele);
        sprintf(status, "Stored %d at p->a[0]", ele);
        render_debugger_frame("Queue -> enqueue()", queue_visualize, "enqueue(Queue *p, int ele)", "main() -> enqueue()", locals, enqueue_code, total, 9, status);
    } else {
        p->r++;
        sprintf(locals, "p->r = %d", p->r);
        sprintf(status, "Incremented rear pointer r to %d", p->r);
        render_debugger_frame("Queue -> enqueue()", queue_visualize, "enqueue(Queue *p, int ele)", "main() -> enqueue()", locals, enqueue_code, total, 13, status);

        p->a[p->r] = ele;
        sprintf(locals, "p->a[%d] = %d", p->r, ele);
        sprintf(status, "Stored %d at p->a[p->r]", ele);
        render_debugger_frame("Queue -> enqueue()", queue_visualize, "enqueue(Queue *p, int ele)", "main() -> enqueue()", locals, enqueue_code, total, 14, status);
    }
}

const char *dequeue_code[] = {
    "int dequeue(Queue *p)",
    "{",
    "    int x;",
    "    if (p->f == -1 && p->r == -1)",
    "    {",
    "        printf(\"Underflow\\n\");",
    "        return -1;",
    "    }",
    "    else if (p->f == p->r)",
    "    {",
    "        x = p->a[p->f];",
    "        p->f = p->r = -1;",
    "        return x;",
    "    }",
    "    else",
    "    {",
    "        x = p->a[p->f];",
    "        p->f++;",
    "        return x;",
    "    }",
    "}"
};

int dequeue_debug(Queue *p) {
    char locals[128];
    char status[128];
    int total = sizeof(dequeue_code) / sizeof(dequeue_code[0]);
    int x;

    sprintf(locals, "p->f = %d | p->r = %d", p->f, p->r);
    sprintf(status, "Entering dequeue(&q)");
    render_debugger_frame("Queue -> dequeue()", queue_visualize, "dequeue(Queue *p)", "main() -> dequeue()", locals, dequeue_code, total, 0, status);

    if (p->f == -1 && p->r == -1) {
        sprintf(status, "Queue Underflow! Queue is empty. Returning -1");
        render_debugger_frame("Queue -> dequeue()", queue_visualize, "dequeue(Queue *p)", "main() -> dequeue()", locals, dequeue_code, total, 5, status);
        return -1;
    } else if (p->f == p->r) {
        x = p->a[p->f];
        p->f = p->r = -1;
        sprintf(locals, "x = %d | p->f = -1 | p->r = -1", x);
        sprintf(status, "Dequeued last element %d. Queue is now empty (f = r = -1).", x);
        render_debugger_frame("Queue -> dequeue()", queue_visualize, "dequeue(Queue *p)", "main() -> dequeue()", locals, dequeue_code, total, 10, status);
        return x;
    } else {
        x = p->a[p->f];
        p->f++;
        sprintf(locals, "x = %d | new f = %d", x, p->f);
        sprintf(status, "Dequeued element %d. Advanced front pointer f.", x);
        render_debugger_frame("Queue -> dequeue()", queue_visualize, "dequeue(Queue *p)", "main() -> dequeue()", locals, dequeue_code, total, 16, status);
        return x;
    }
}


void stack_menu() {
    int ch, ele;
    while (1) {
        clear_screen();
        printf("\033[1;37;44m ============================================================================== \033[0m\n");
        printf("\033[1;37;44m                                STACK VISUALIZER                               \033[0m\n");
        printf("\033[1;37;44m ============================================================================== \033[0m\n\n");
        stack_visualize();
        printf("\n--------------------------------------------------------------------------------\n");
        printf("1. Push Element\n");
        printf("2. Pop Element\n");
        printf("3. Peek Topmost Element\n");
        printf("4. Clear Stack\n");
        printf("5. Return to Main Menu\n");
        printf("--------------------------------------------------------------------------------\n");
        printf("Enter your choice: ");
        if (scanf("%d", &ch) != 1) {
            while (getchar() != '\n');
            continue;
        }

        step_mode = 0;

        switch (ch) {
            case 1:
                printf("Enter data to push: ");
                scanf("%d", &ele);
                push_debug(&st, ele);
                break;
            case 2:
                pop_debug(&st);
                break;
            case 3:
                if (st.top == -1) {
                    printf("\nStack is empty!\n");
                } else {
                    printf("\nTop element: %d\n", st.arr[st.top]);
                }
                printf("Press any key to continue...");
                wait_key();
                break;
            case 4:
                st.top = -1;
                printf("\nStack cleared successfully!\n");
                Sleep(600);
                break;
            case 5:
                return;
        }
    }
}

void simple_queue_menu() {
    int ch, ele;
    while (1) {
        clear_screen();
        printf("\033[1;37;44m ============================================================================== \033[0m\n");
        printf("\033[1;37;44m                                QUEUE VISUALIZER                               \033[0m\n");
        printf("\033[1;37;44m ============================================================================== \033[0m\n\n");
        queue_visualize();
        printf("\n--------------------------------------------------------------------------------\n");
        printf("1. Enqueue Element\n");
        printf("2. Dequeue Element\n");
        printf("3. Clear Queue\n");
        printf("4. Return to Main Menu\n");
        printf("--------------------------------------------------------------------------------\n");
        printf("Enter your choice: ");
        if (scanf("%d", &ch) != 1) {
            while (getchar() != '\n');
            continue;
        }

        step_mode = 0;

        switch (ch) {
            case 1:
                printf("Enter data to enqueue: ");
                scanf("%d", &ele);
                enqueue_debug(&qu, ele);
                break;
            case 2:
                dequeue_debug(&qu);
                break;
            case 3:
                qu.f = -1;
                qu.r = -1;
                printf("\nQueue cleared successfully!\n");
                Sleep(600);
                break;
            case 4:
                return;
        }
    }
}

/* Circular queue state is kept separate from the simple queue. */
static int circular_queue[QUEUE_SIZE];
static int circular_front = -1;
static int circular_rear = -1;

static int circular_queue_is_empty(void) {
    return circular_front == -1;
}

static int circular_queue_is_full(void) {
    return (circular_rear + 1) % QUEUE_SIZE == circular_front;
}

static void circular_queue_display(void) {
    int i;
    printf(" CIRCULAR QUEUE (SIZE = %d):\n", QUEUE_SIZE);
    printf(" Index:    ");
    for (i = 0; i < QUEUE_SIZE; i++) printf("%3d ", i);
    printf("\n Values:   ");
    for (i = 0; i < QUEUE_SIZE; i++) {
        if (!circular_queue_is_empty() &&
            ((circular_front <= circular_rear && i >= circular_front && i <= circular_rear) ||
             (circular_front > circular_rear && (i >= circular_front || i <= circular_rear)))) {
            printf("%3d ", circular_queue[i]);
        } else {
            printf("  . ");
        }
    }
    printf("\n Front: %d  Rear: %d\n", circular_front, circular_rear);
    if (circular_queue_is_empty()) printf(" Queue is empty.\n");
}

static void circular_queue_add(int ele) {
    if (circular_queue_is_full()) {
        printf("Circular queue is full.\n");
        return;
    }
    if (circular_queue_is_empty()) circular_front = circular_rear = 0;
    else circular_rear = (circular_rear + 1) % QUEUE_SIZE;
    circular_queue[circular_rear] = ele;
}

static void circular_queue_remove(void) {
    int ele;
    if (circular_queue_is_empty()) {
        printf("Circular queue is empty.\n");
        return;
    }
    ele = circular_queue[circular_front];
    if (circular_front == circular_rear) circular_front = circular_rear = -1;
    else circular_front = (circular_front + 1) % QUEUE_SIZE;
    printf("Removed element: %d\n", ele);
}

void circular_queue_menu(void) {
    int ch, ele;
    while (1) {
        clear_screen();
        circular_queue_display();
        printf("\n1. Enqueue\n2. Dequeue\n3. Clear Queue\n4. Return\nChoice: ");
        if (scanf("%d", &ch) != 1) {
            while (getchar() != '\n');
            continue;
        }
        if (ch == 1) {
            printf("Enter data: ");
            if (scanf("%d", &ele) == 1) circular_queue_add(ele);
            else while (getchar() != '\n');
        } else if (ch == 2) circular_queue_remove();
        else if (ch == 3) circular_front = circular_rear = -1;
        else if (ch == 4) return;
        else printf("Invalid choice.\n");
        if (ch != 4) { printf("Press any key to continue..."); wait_key(); }
    }
}

void queue_menu(void) {
    int ch;
    while (1) {
        clear_screen();
        printf("QUEUES\n\n1. Simple Queue\n2. Circular Queue\n3. Return\nChoice: ");
        if (scanf("%d", &ch) != 1) {
            while (getchar() != '\n');
            continue;
        }
        if (ch == 1) simple_queue_menu();
        else if (ch == 2) circular_queue_menu();
        else if (ch == 3) return;
        else { printf("Invalid choice.\n"); Sleep(500); }
    }
}


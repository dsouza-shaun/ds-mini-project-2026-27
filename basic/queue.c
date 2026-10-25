#include "visualizer.h"

Queue qu = {{0}, -1, -1};

void enqueue(Queue *p, int ele)
{
    if (p->r == SIZE - 1)
    {
        printf("Queue Overflow\n");
    }
    else if (p->f == -1 && p->r == -1)
    {
        p->f = 0;
        p->r = 0;
        p->a[p->r] = ele;
    }
    else
    {
        p->r++;
        p->a[p->r] = ele;
    }
}

int dequeue(Queue *p)
{
    int x;

    if (p->f == -1 && p->r == -1)
    {
        printf("Queue Underflow\n");
        return -1;
    }

    x = p->a[p->f];

    if (p->f == p->r)
    {
        p->f = -1;
        p->r = -1;
    }
    else
    {
        p->f++;
    }

    return x;
}

int is_empty_queue(Queue *p)
{
    if (p->f == -1 && p->r == -1)
        return 1;

    return 0;
}

int is_full_queue(Queue *p)
{
    if (p->r == SIZE - 1)
        return 1;

    return 0;
}

void show_queue(void)
{
    int i;

    printf("SIMPLE QUEUE (front = %d, rear = %d, size = %d)\n", qu.f, qu.r, SIZE);

    printf("Index:");
    for (i = 0; i < SIZE; i++)
        printf("%4d", i);

    printf("\nValue:");
    for (i = 0; i < SIZE; i++)
    {
        if (qu.f != -1 && i >= qu.f && i <= qu.r)
            printf("%4d", qu.a[i]);
        else
            printf("   .");
    }

    printf("\n      ");
    for (i = 0; i < SIZE; i++)
    {
        if (i == qu.f && i == qu.r)
            printf(" f,r");
        else if (i == qu.f)
            printf("   f");
        else if (i == qu.r)
            printf("   r");
        else
            printf("    ");
    }
    printf("\n");
}

/* ---------------------------------------------------------------- */
/*                        enqueue                                   */
/* ---------------------------------------------------------------- */

char *enqueue_code[] = {
    "void enqueue(Queue *p, int ele)",
    "{",
    "    if (p->r == SIZE - 1)",
    "    {",
    "        printf(\"Queue Overflow\\n\");",
    "    }",
    "    else if (p->f == -1 && p->r == -1)",
    "    {",
    "        p->f = 0;",
    "        p->r = 0;",
    "        p->a[p->r] = ele;",
    "    }",
    "    else",
    "    {",
    "        p->r++;",
    "        p->a[p->r] = ele;",
    "    }",
    "}"
};

void enqueue_debug(Queue *p, int ele)
{
    char locals[160];
    char status[160];
    int total = sizeof(enqueue_code) / sizeof(enqueue_code[0]);

    sprintf(locals, "ele = %d | p->f = %d | p->r = %d", ele, p->f, p->r);
    sprintf(status, "Enter enqueue with value %d.", ele);
    show_frame(MODE_QUEUE, "Queue -> enqueue()", "void enqueue(Queue *p, int ele)",
               "main() -> enqueue()", locals, enqueue_code, total, 0, status);

    sprintf(status, "Check if the rear is already at SIZE - 1.");
    show_frame(MODE_QUEUE, "Queue -> enqueue()", "void enqueue(Queue *p, int ele)",
               "main() -> enqueue()", locals, enqueue_code, total, 2, status);

    if (p->r == SIZE - 1)
    {
        sprintf(status, "Queue Overflow: the queue is full.");
        show_frame(MODE_QUEUE, "Queue -> enqueue()", "void enqueue(Queue *p, int ele)",
                   "main() -> enqueue()", locals, enqueue_code, total, 4, status);
    }
    else if (p->f == -1 && p->r == -1)
    {
        sprintf(status, "Queue is empty, so set front and rear to 0.");
        show_frame(MODE_QUEUE, "Queue -> enqueue()", "void enqueue(Queue *p, int ele)",
                   "main() -> enqueue()", locals, enqueue_code, total, 6, status);

        p->f = 0;
        p->r = 0;
        p->a[p->r] = ele;

        sprintf(locals, "p->f = 0 | p->r = 0 | p->a[0] = %d", ele);
        sprintf(status, "Store %d at index 0.", ele);
        show_frame(MODE_QUEUE, "Queue -> enqueue()", "void enqueue(Queue *p, int ele)",
                   "main() -> enqueue()", locals, enqueue_code, total, 10, status);
    }
    else
    {
        p->r++;

        sprintf(locals, "ele = %d | p->f = %d | p->r = %d", ele, p->f, p->r);
        sprintf(status, "Increase rear to %d.", p->r);
        show_frame(MODE_QUEUE, "Queue -> enqueue()", "void enqueue(Queue *p, int ele)",
                   "main() -> enqueue()", locals, enqueue_code, total, 14, status);

        p->a[p->r] = ele;
        sprintf(locals, "p->a[%d] = %d", p->r, ele);
        sprintf(status, "Store %d at index %d.", ele, p->r);
        show_frame(MODE_QUEUE, "Queue -> enqueue()", "void enqueue(Queue *p, int ele)",
                   "main() -> enqueue()", locals, enqueue_code, total, 15, status);
    }
}

/* ---------------------------------------------------------------- */
/*                        dequeue                                   */
/* ---------------------------------------------------------------- */

char *dequeue_code[] = {
    "int dequeue(Queue *p)",
    "{",
    "    int x;",
    "",
    "    if (p->f == -1 && p->r == -1)",
    "    {",
    "        printf(\"Queue Underflow\\n\");",
    "        return -1;",
    "    }",
    "    x = p->a[p->f];",
    "    if (p->f == p->r)",
    "    {",
    "        p->f = -1;",
    "        p->r = -1;",
    "    }",
    "    else",
    "    {",
    "        p->f++;",
    "    }",
    "    return x;",
    "}"
};

int dequeue_debug(Queue *p)
{
    char locals[160];
    char status[160];
    int x;
    int total = sizeof(dequeue_code) / sizeof(dequeue_code[0]);

    sprintf(locals, "p->f = %d | p->r = %d", p->f, p->r);
    sprintf(status, "Enter dequeue.");
    show_frame(MODE_QUEUE, "Queue -> dequeue()", "int dequeue(Queue *p)",
               "main() -> dequeue()", locals, dequeue_code, total, 0, status);

    sprintf(status, "Check if the queue is empty.");
    show_frame(MODE_QUEUE, "Queue -> dequeue()", "int dequeue(Queue *p)",
               "main() -> dequeue()", locals, dequeue_code, total, 4, status);

    if (p->f == -1 && p->r == -1)
    {
        sprintf(status, "Queue Underflow: there is nothing to dequeue.");
        show_frame(MODE_QUEUE, "Queue -> dequeue()", "int dequeue(Queue *p)",
                   "main() -> dequeue()", locals, dequeue_code, total, 6, status);
        return -1;
    }

    x = p->a[p->f];

    sprintf(locals, "x = %d | p->f = %d | p->r = %d", x, p->f, p->r);
    sprintf(status, "Read value %d from index %d.", x, p->f);
    show_frame(MODE_QUEUE, "Queue -> dequeue()", "int dequeue(Queue *p)",
               "main() -> dequeue()", locals, dequeue_code, total, 9, status);

    if (p->f == p->r)
    {
        p->f = -1;
        p->r = -1;

        sprintf(locals, "x = %d | p->f = -1 | p->r = -1", x);
        sprintf(status, "This was the only element, so the queue is now empty.");
        show_frame(MODE_QUEUE, "Queue -> dequeue()", "int dequeue(Queue *p)",
                   "main() -> dequeue()", locals, dequeue_code, total, 12, status);
    }
    else
    {
        p->f++;

        sprintf(locals, "x = %d | new front = %d", x, p->f);
        sprintf(status, "Move front to %d.", p->f);
        show_frame(MODE_QUEUE, "Queue -> dequeue()", "int dequeue(Queue *p)",
                   "main() -> dequeue()", locals, dequeue_code, total, 17, status);
    }

    return x;
}

/* ---------------------------------------------------------------- */
/*                     simple queue menu                            */
/* ---------------------------------------------------------------- */

void simple_queue_menu(void)
{
    int ch, ele;

    while (1)
    {
        clear_screen();
        printf("==============================================================\n");
        printf("                       SIMPLE QUEUE\n");
        printf("==============================================================\n");
        show_queue();
        printf("\n--------------------------------------------------------------\n");
        printf("1. Enqueue\n");
        printf("2. Dequeue\n");
        printf("3. Clear queue\n");
        printf("4. Return\n");
        printf("--------------------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        step_mode = 0;

        switch (ch)
        {
        case 1:
            printf("Enter data: ");
            scanf("%d", &ele);
            enqueue_debug(&qu, ele);
            break;

        case 2:
            dequeue_debug(&qu);
            break;

        case 3:
            qu.f = -1;
            qu.r = -1;
            printf("\nQueue cleared.\n");
            wait_key();
            break;

        case 4:
            return;

        default:
            printf("\nInvalid choice\n");
            wait_key();
        }
    }
}

/* ---------------------------------------------------------------- */
/*                     circular queue                               */
/* ---------------------------------------------------------------- */

int cq[SIZE];
int cq_front = -1;
int cq_rear = -1;

int cq_is_empty(void)
{
    if (cq_front == -1 && cq_rear == -1)
        return 1;

    return 0;
}

int cq_is_full(void)
{
    if (cq_front == (cq_rear + 1) % SIZE)
        return 1;

    return 0;
}

void cq_enqueue(int ele)
{
    if (cq_is_full())
    {
        printf("Circular Queue Overflow\n");
        return;
    }

    if (cq_front == -1 && cq_rear == -1)
    {
        cq_front = 0;
        cq_rear = 0;
    }
    else
    {
        cq_rear = (cq_rear + 1) % SIZE;
    }

    cq[cq_rear] = ele;
}

int cq_dequeue(void)
{
    int x;

    if (cq_is_empty())
    {
        printf("Circular Queue Underflow\n");
        return -1;
    }

    x = cq[cq_front];

    if (cq_front == cq_rear)
    {
        cq_front = -1;
        cq_rear = -1;
    }
    else
    {
        cq_front = (cq_front + 1) % SIZE;
    }

    return x;
}

void show_circular(void)
{
    int i, inside;

    printf("CIRCULAR QUEUE (front = %d, rear = %d, size = %d)\n",
           cq_front, cq_rear, SIZE);

    printf("Index:");
    for (i = 0; i < SIZE; i++)
        printf("%4d", i);

    printf("\nValue:");
    for (i = 0; i < SIZE; i++)
    {
        inside = 0;

        if (!cq_is_empty())
        {
            if (cq_front <= cq_rear)
            {
                if (i >= cq_front && i <= cq_rear)
                    inside = 1;
            }
            else
            {
                if (i >= cq_front || i <= cq_rear)
                    inside = 1;
            }
        }

        if (inside)
            printf("%4d", cq[i]);
        else
            printf("   .");
    }

    printf("\n      ");
    for (i = 0; i < SIZE; i++)
    {
        if (i == cq_front && i == cq_rear)
            printf(" f,r");
        else if (i == cq_front)
            printf("   f");
        else if (i == cq_rear)
            printf("   r");
        else
            printf("    ");
    }
    printf("\n");
}

void circular_queue_menu(void)
{
    int ch, ele, x;

    while (1)
    {
        clear_screen();
        printf("==============================================================\n");
        printf("                      CIRCULAR QUEUE\n");
        printf("==============================================================\n");
        show_circular();
        printf("\n--------------------------------------------------------------\n");
        printf("1. Enqueue\n");
        printf("2. Dequeue\n");
        printf("3. Clear queue\n");
        printf("4. Return\n");
        printf("--------------------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        switch (ch)
        {
        case 1:
            printf("Enter data: ");
            scanf("%d", &ele);
            cq_enqueue(ele);
            printf("\nEnqueue done.\n");
            wait_key();
            break;

        case 2:
            x = cq_dequeue();
            if (x != -1)
                printf("\nRemoved %d.\n", x);
            wait_key();
            break;

        case 3:
            cq_front = -1;
            cq_rear = -1;
            printf("\nCircular queue cleared.\n");
            wait_key();
            break;

        case 4:
            return;

        default:
            printf("\nInvalid choice\n");
            wait_key();
        }
    }
}

void queue_menu(void)
{
    int ch;

    while (1)
    {
        clear_screen();
        printf("==============================================================\n");
        printf("                         QUEUES\n");
        printf("==============================================================\n");
        printf("1. Simple Queue\n");
        printf("2. Circular Queue\n");
        printf("3. Return to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        if (ch == 1)
            simple_queue_menu();
        else if (ch == 2)
            circular_queue_menu();
        else if (ch == 3)
            return;
        else
        {
            printf("\nInvalid choice\n");
            wait_key();
        }
    }
}

// Name: Shaun Dsouza
// Roll no.: 11077
// Class: SE CE-B
// Batch: A

#include <stdio.h>
#define SIZE 5

typedef struct
{
    int a[SIZE];
    int f;
    int r;
} Queue;

void enqueue(Queue *p, int ele)
{
    if (p->f == (p->r + 1) % SIZE)
    {
        printf("Overflow\n");
        return;
    }
    else if (p->f == -1 && p->r == -1)
    {
        p->f = p->r = 0;
    }
    else
    {
        p->r = (p->r + 1) % SIZE;
    }
    p->a[p->r] = ele;
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
        p->f = p->r = -1;
    }
    else
    {
        p->f = (p->f + 1) % SIZE;
    }

    return x;
}

int isEmpty(Queue *p)
{
    if (p->f == -1 && p->r == -1)
        return 1;
    else
        return 0;
}

int isFull(Queue *p)
{
    if (p->f == (p->r + 1) % SIZE)
        return 1;
    else
        return 0;
}

void display(Queue *p)
{
    int i;
    if (isEmpty(p))
    {
        printf("Queue is empty\n");
    }
    else
    {
        for (i = p->f; i != p->r; i = (i + 1) % SIZE)
        {
            printf("%d ", p->a[i]);
        }
        printf("%d ", p->a[i]);
        printf("\n");
    }
}

int main()
{
    Queue p;
    int ch, ele, x;

    p.f = -1;
    p.r = -1;

    while (1)
    {
        printf("\nMenu");
        printf("\n1. Enqueue");
        printf("\n2. Dequeue");
        printf("\n3. Display");
        printf("\n4. Is Empty");
        printf("\n5. Is Full");
        printf("\n6. Exit");

        printf("\nEnter choice: ");
        scanf("%d", &ch);

        switch (ch)
        {
            case 1:
                printf("Enter element: ");
                scanf("%d", &ele);
                enqueue(&p, ele);
                break;
            case 2:
                x = dequeue(&p);
                if (x != -1)
                    printf("Deleted element = %d\n", x);
                break;
            case 3:
                display(&p);
                break;
            case 4:
                if (isEmpty(&p))
                    printf("Queue is Empty\n");
                else
                    printf("Queue is not Empty\n");
                break;
            case 5:
                if (isFull(&p))
                    printf("Queue is Full\n");
                else
                    printf("Queue is not Full\n");
                break;
            case 6:
                return 0;
            default:
                printf("Invalid choice\n");
        }
    }
}
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
} Deque;

int isEmpty(Deque *p)
{
    if (p->f == -1)
        return 1;
    else
        return 0;
}

int isFull(Deque *p)
{
    if ((p->f == 0 && p->r == SIZE - 1) || (p->f == p->r + 1))
        return 1;
    else
        return 0;
}

void insert_front(Deque *p, int ele)
{
    if (p->f == (p->r + 1) % SIZE)
    {
        printf("Overflow\n");
        return;
    }

    if (p->f == -1 && p->r == -1)
    {
        p->f = p->r = 0;
    }
    else
    {
        p->f = (p->f + (SIZE - 1)) % SIZE;
    }

    p->a[p->f] = ele;
}

void insert_rear(Deque *p, int ele)
{
    if (p->f == (p->r + 1) % SIZE)
    {
        printf("Overflow\n");
        return;
    }

    if (p->f == -1 && p->r == -1)
    {
        p->f = p->r = 0;
    }
    else
    {
        p->r = (p->r + 1) % SIZE;
    }

    p->a[p->r] = ele;
}

int remove_front(Deque *p)
{
    int x;

    if (p->f == -1 && p->r == -1)
    {
        printf("Underflow\n");
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

int remove_rear(Deque *p)
{
    int x;

    if (p->f == -1 && p->r == -1)
    {
        printf("Underflow\n");
        return -1;
    }

    x = p->a[p->r];

    if (p->f == p->r)
    {
        p->f = p->r = -1;
    }
    else
    {
        p->r = (p->r + (SIZE - 1)) % SIZE;
    }

    return x;
}

void display(Deque *p)
{
    int i;

    if (isEmpty(p))
    {
        printf("Deque is empty\n");
        return;
    }

    i = p->f;

    while (1)
    {
        printf("%d ", p->a[i]);

        if (i == p->r)
            break;

        i = (i + 1) % SIZE;
    }

    printf("\n");
}

int main()
{
    Deque p;
    int ch, ele, x;

    p.f = -1;
    p.r = -1;

    while (1)
    {
        printf("\nMenu");
        printf("\n1. Insert Front");
        printf("\n2. Insert Rear");
        printf("\n3. Remove Front");
        printf("\n4. Remove Rear");
        printf("\n5. Display");
        printf("\n6. Is Full");
        printf("\n7. Is Empty");
        printf("\n8. Exit");

        printf("\nEnter choice: ");
        scanf("%d", &ch);

        switch (ch)
        {
            case 1:
                printf("Enter element: ");
                scanf("%d", &ele);
                insert_front(&p, ele);
                break;
            case 2:
                printf("Enter element: ");
                scanf("%d", &ele);
                insert_rear(&p, ele);
                break;
            case 3:
                x = remove_front(&p);
                if (x != -1)
                    printf("Removed element = %d\n", x);
                break;
            case 4:
                x = remove_rear(&p);
                if (x != -1)
                    printf("Removed element = %d\n", x);
                break;
            case 5:
                display(&p);
                break;
            case 6:
                if (isFull(&p))
                    printf("Deque is Full\n");
                else
                    printf("Deque is not Full\n");
                break;
            case 7:
                if (isEmpty(&p))
                    printf("Deque is Empty\n");
                else
                    printf("Deque is not Empty\n");
                break;
            case 8:
                return 0;
            default:
                printf("Invalid choice\n");
        }
    }
}
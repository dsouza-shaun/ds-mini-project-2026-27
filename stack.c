// Name: Shaun Dsouza
// Roll no.: 11077
// Class: SE CE-B 
// Batch: A

#include <stdio.h>
#include <stdlib.h>

#define SIZE 5

typedef struct {
    int arr[SIZE];
    int top;
} Stack;

void push(Stack *p, int ele) {
    if(p->top == SIZE-1) {
        printf("Stack Overflow\n");
    } else {
        p->top++;
        p->arr[p->top] = ele;
    }
}

int pop(Stack *p) {
    if(p->top == -1) {
        printf("Stack Underflow\n");
        return -1;
    } else {
        return p->arr[p->top--];
    }
}

int peek(Stack *p) {
    return p->arr[p->top];
}

int isEmpty(Stack *p) {
    if(p->top == -1) {
        return 1;
    } else {
        return 0;
    }
}

int isFull(Stack *p) {
    if(p->top == SIZE-1) {
        return 1;
    } else {
        return 0;
    }
}

void display(Stack *p) {
    if(p->top == -1) {
        printf("Stack is empty\n");
    }
    for (int i = p->top; i >= 0; i--) {
        printf("%d\n", p->arr[i]);
    }
}

int main() {
    Stack s;
    s.top = -1;
    int ch;
    int ele;
    while(1) {
        printf("Menu\n");
        printf("1. push\n2. pop\n3. peek\n4. display\n5. isFull\n6. isEmpty\n7. exit\n");
        printf("Enter your input: ");
        scanf("%d", &ch);
        switch(ch) {
            case 1:
                printf("Enter data: ");
                scanf("%d", &ele);
                push(&s, ele);
                break;
            case 2:
                printf("Popped element: %d\n", pop(&s));
                break;
            case 3:
                printf("Topmost data is %d\n", peek(&s));
                break;
            case 4:
                display(&s);
                break;
            case 5:
                printf("isFull: %d\n", isFull(&s));
                break;
            case 6:
                printf("isEmpty: %d\n", isEmpty(&s));
                break;
            case 7:
                exit(0);
        }
    }
    return 0;
}
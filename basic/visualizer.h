#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <stdio.h>
#include <stdlib.h>

#define SIZE 10
#define MAX_VERTICES 10

#define TREE_ROWS 40
#define TREE_COLS 120

#define MODE_BST 1
#define MODE_GRAPH 2
#define MODE_STACK 3
#define MODE_QUEUE 4
#define MODE_CIRCULAR 5
#define MODE_SLL 6
#define MODE_DLL 7

typedef struct Node
{
    int data;
    struct Node *left;
    struct Node *right;
    int x;                  /* column used only while drawing the tree */
    int y;                  /* row used only while drawing the tree    */
} Node;

typedef struct
{
    int arr[SIZE];
    int top;
} Stack;

typedef struct
{
    int a[SIZE];
    int f;
    int r;
} Queue;

typedef struct LLNode
{
    int data;
    struct LLNode *next;
} LLNode;

typedef struct DNode
{
    int data;
    struct DNode *prev;
    struct DNode *next;
} DNode;

extern int step_mode;

/* shared screen and debugger helpers */
void clear_screen(void);
void wait_key(void);
void pause_step(void);
void print_code(char *code[], int total, int current);
void show_frame(int mode, char *title, char *func, char *call_stack,
                char *locals, char *code[], int total, int current,
                char *status);

/* display functions */
void show_bst(void);
void show_graph(void);
void show_stack(void);
void show_queue(void);
void show_circular(void);
void show_sll(void);
void show_dll(void);

/* menus */
void bst_menu(void);
void graph_menu(void);
void stack_menu(void);
void queue_menu(void);
void ll_menu(void);

/* stack functions (also used by graph DFS) */
void push(Stack *p, int ele);
int pop(Stack *p);
int peek(Stack *p);
int is_empty_stack(Stack *p);
int is_full_stack(Stack *p);

/* queue functions (also used by graph BFS) */
void enqueue(Queue *p, int ele);
int dequeue(Queue *p);
int is_empty_queue(Queue *p);
int is_full_queue(Queue *p);

#endif

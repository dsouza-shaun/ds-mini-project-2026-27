#ifndef VISUALIZER_H
#define VISUALIZER_H

#define STACK_SIZE 20
#define QUEUE_SIZE 20
#define MAX_GRAPH_VERTICES 10

typedef struct Node {
    struct Node *left;
    int data;
    struct Node *right;
} Node;

typedef struct {
    int arr[STACK_SIZE];
    int top;
} Stack;

typedef struct {
    int a[QUEUE_SIZE];
    int f;
    int r;
} Queue;

typedef struct LLNode {
    int data;
    struct LLNode *next;
} LLNode;

extern int step_mode;
extern Node *bst_root;
extern int bst_highlight_val;
extern int bst_node_x[1000];
extern int bst_node_y[1000];
extern int bst_inorder_idx;
extern char bst_canvas[30][120];
extern char bst_traversal_buf[256];
extern Stack st;
extern Queue qu;
extern int graph_adj[MAX_GRAPH_VERTICES][MAX_GRAPH_VERTICES];
extern int graph_visited[MAX_GRAPH_VERTICES];
extern int graph_n;
extern int graph_current_vertex;
extern int graph_traversal_list[MAX_GRAPH_VERTICES];
extern int graph_traversal_count;
extern Queue graph_bfs_q;
extern Stack graph_dfs_s;
extern int graph_algo_mode;
extern LLNode *ll_head;
extern LLNode *ll_highlight_node;

void enable_ansi(void);
void clear_screen(void);
int wait_key(void);
void pause_step(void);
void render_debugger_frame(const char *title, void (*render_visual)(void), const char *func_sig, const char *call_stack, const char *locals, const char *code[], int total_lines, int cur_line, const char *status);

void bst_menu(void);
void graph_menu(void);
void stack_menu(void);
void queue_menu(void);
void ll_menu(void);
void simple_queue_menu(void);
void singly_linked_list_menu(void);
void circular_queue_menu(void);
void doubly_linked_list_menu(void);

void push(Stack *p, int ele);
int pop(Stack *p);
int peek(Stack *p);
int is_empty_stack(Stack *p);
void enqueue(Queue *p, int ele);
int dequeue(Queue *p);
int is_empty_queue(Queue *p);

#endif

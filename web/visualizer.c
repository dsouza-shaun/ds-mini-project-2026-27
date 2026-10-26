#include <stdio.h>
#include <stdlib.h>

#define SIZE 10
#define MAX_V 10

/* ================================================================ */
/*                    trace file (for the web page)                 */
/* ================================================================ */

FILE *out = NULL;
int first_step = 1;

void begin(char *mode, char *title)
{
    out = fopen("trace.json", "w");
    if (out == NULL) { printf("Could not create trace.json\n"); return; }
    fprintf(out, "{\"mode\":\"%s\",\"title\":\"%s\",\"steps\":[", mode, title);
    first_step = 1;
}

void end_trace(void)
{
    if (out == NULL) return;
    fprintf(out, "]}\n");
    fclose(out);
    out = NULL;
}

void start_step(char *msg)
{
    if (first_step == 0) fprintf(out, ",");
    first_step = 0;
    fprintf(out, "{\"msg\":\"%s\",\"state\":\"", msg);
}

void end_step(void)
{
    fprintf(out, "\"}");
}

/* ================================================================ */
/*                      binary search tree                          */
/* ================================================================ */

typedef struct Node
{
    int data;
    struct Node *left;
    struct Node *right;
} Node;

Node *bst = NULL;
Node *cur = NULL;

void write_tree(Node *p)
{
    if (p == NULL) { fprintf(out, "-"); return; }
    fprintf(out, "(%d%s", p->data, p == cur ? "*" : "");
    write_tree(p->left);
    fprintf(out, " ");
    write_tree(p->right);
    fprintf(out, ")");
}

void save_bst(Node *root, char *msg)
{
    if (out == NULL) return;
    start_step(msg);
    write_tree(root);
    end_step();
}

Node *create_node(int key)
{
    Node *newnode;
    newnode = (Node *)malloc(sizeof(Node));
    newnode->data = key;
    newnode->left = NULL;
    newnode->right = NULL;
    return newnode;
}

Node *insert(Node *root, int key)
{
    Node *newnode = create_node(key);
    Node *p, *q;
    if (root == NULL)
    {
        root = newnode;
        cur = root;
        save_bst(root, "New node becomes the root.");
        cur = NULL;
        return root;
    }
    p = q = root;
    while (p != NULL)
    {
        q = p;
        cur = p;
        if (key < p->data) { save_bst(root, "Key is smaller, move left."); p = p->left; }
        else { save_bst(root, "Key is greater, move right."); p = p->right; }
    }
    if (key < q->data) q->left = newnode;
    else q->right = newnode;
    cur = newnode;
    save_bst(root, "New node attached.");
    cur = NULL;
    return root;
}

void inorder(Node *root)
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("%d ", root->data);
        cur = root;
        save_bst(bst, "Inorder visit.");
        cur = NULL;
        inorder(root->right);
    }
}

void preorder(Node *root)
{
    if (root != NULL)
    {
        printf("%d ", root->data);
        cur = root;
        save_bst(bst, "Preorder visit.");
        cur = NULL;
        preorder(root->left);
        preorder(root->right);
    }
}

void postorder(Node *root)
{
    if (root != NULL)
    {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->data);
        cur = root;
        save_bst(bst, "Postorder visit.");
        cur = NULL;
    }
}

int search(Node *root, int key)
{
    Node *p;
    if (root == NULL) { printf("Tree is empty\n"); return 0; }
    p = root;
    while (p != NULL)
    {
        cur = p;
        save_bst(root, "Compare the key with this node.");
        if (p->data == key) { cur = NULL; return 1; }
        if (key > p->data) p = p->right;
        else p = p->left;
    }
    cur = NULL;
    return 0;
}

Node *delete(Node *root, int key)
{
    Node *parent = root;
    Node *p = root;
    Node *succ;
    int x;
    while (p != NULL)
    {
        if (key == p->data) break;
        else if (key < p->data) { parent = p; p = p->left; }
        else { parent = p; p = p->right; }
    }
    if (p == NULL)
    {
        printf("Element %d not found in the tree.\n", key);
        save_bst(root, "Element not found.");
        return root;
    }
    cur = p;
    save_bst(root, "Found the node to delete.");
    if (p->left == NULL && p->right == NULL)
    {
        if (p == root) root = NULL;
        else if (p == parent->left) parent->left = NULL;
        else parent->right = NULL;
        free(p);
        cur = NULL;
        save_bst(root, "Leaf node removed.");
    }
    else if (p->left != NULL && p->right == NULL)
    {
        if (p == root) root = p->left;
        else if (p == parent->left) parent->left = p->left;
        else parent->right = p->left;
        free(p);
        cur = NULL;
        save_bst(root, "Node with only a left child removed.");
    }
    else if (p->right != NULL && p->left == NULL)
    {
        if (p == root) root = p->right;
        else if (p == parent->left) parent->left = p->right;
        else parent->right = p->right;
        free(p);
        cur = NULL;
        save_bst(root, "Node with only a right child removed.");
    }
    else
    {
        succ = p->right;
        while (succ->left != NULL) succ = succ->left;
        x = succ->data;
        save_bst(root, "Two children: find the inorder successor.");
        root = delete(root, x);
        p->data = x;
        cur = p;
        save_bst(root, "Successor value copied into the node.");
        cur = NULL;
    }
    return root;
}

int count_nodes(Node *root)
{
    if (root == NULL) return 0;
    return 1 + count_nodes(root->left) + count_nodes(root->right);
}

int leaf_nodes(Node *root)
{
    if (root == NULL) return 0;
    if (root->left == NULL && root->right == NULL) return 1;
    return leaf_nodes(root->left) + leaf_nodes(root->right);
}

int interior_nodes(Node *root)
{
    if (root == NULL) return 0;
    if (root->left != NULL || root->right != NULL)
        return 1 + interior_nodes(root->left) + interior_nodes(root->right);
    return 0;
}

void free_tree(Node *root)
{
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

/* ================================================================ */
/*                            stack                                 */
/* ================================================================ */

typedef struct
{
    int arr[SIZE];
    int top;
} Stack;

Stack st = {{0}, -1};

void push(Stack *p, int ele)
{
    if (p->top == SIZE - 1) printf("Stack Overflow\n");
    else { p->top++; p->arr[p->top] = ele; }
}

int pop(Stack *p)
{
    if (p->top == -1) { printf("Stack Underflow\n"); return -1; }
    return p->arr[p->top--];
}

int peek(Stack *p)
{
    return p->arr[p->top];
}

int stack_isEmpty(Stack *p)
{
    if (p->top == -1) return 1;
    return 0;
}

int stack_isFull(Stack *p)
{
    if (p->top == SIZE - 1) return 1;
    return 0;
}

void save_stack(Stack *p, char *msg)
{
    int i;
    if (out == NULL) return;
    start_step(msg);
    for (i = 0; i <= p->top; i++) fprintf(out, "%d ", p->arr[i]);
    end_step();
}

/* ================================================================ */
/*                         simple queue                             */
/* ================================================================ */

typedef struct
{
    int a[SIZE];
    int f;
    int r;
} Queue;

Queue q = {{0}, -1, -1};

void enqueue(Queue *p, int ele)
{
    if (p->r == SIZE - 1) printf("Queue Overflow\n");
    else if (p->f == -1 && p->r == -1) { p->f = p->r = 0; p->a[p->r] = ele; }
    else { p->r++; p->a[p->r] = ele; }
}

int dequeue(Queue *p)
{
    int x;
    if (p->f == -1 && p->r == -1) { printf("Queue Underflow\n"); return -1; }
    x = p->a[p->f];
    if (p->f == p->r) p->f = p->r = -1;
    else p->f++;
    return x;
}

int queue_isEmpty(Queue *p)
{
    if (p->f == -1 && p->r == -1) return 1;
    return 0;
}

int queue_isFull(Queue *p)
{
    if (p->r == SIZE - 1) return 1;
    return 0;
}

void save_queue(Queue *p, char *msg)
{
    int i;
    int inside;
    if (out == NULL) return;
    start_step(msg);
    fprintf(out, "%d %d", p->f, p->r);
    for (i = 0; i < SIZE; i++)
    {
        inside = 0;
        if (p->f != -1 && i >= p->f && i <= p->r) inside = 1;
        if (inside) fprintf(out, " %d", p->a[i]);
        else fprintf(out, " -");
    }
    end_step();
}

/* ================================================================ */
/*                        circular queue                            */
/* ================================================================ */

int cq[SIZE];
int cq_front = -1;
int cq_rear = -1;

void cq_enqueue(int ele)
{
    if (cq_front == (cq_rear + 1) % SIZE) { printf("Queue Overflow\n"); return; }
    else if (cq_front == -1 && cq_rear == -1) cq_front = cq_rear = 0;
    else cq_rear = (cq_rear + 1) % SIZE;
    cq[cq_rear] = ele;
}

int cq_dequeue(void)
{
    int x;
    if (cq_front == -1 && cq_rear == -1) { printf("Queue Underflow\n"); return -1; }
    x = cq[cq_front];
    if (cq_front == cq_rear) cq_front = cq_rear = -1;
    else cq_front = (cq_front + 1) % SIZE;
    return x;
}

int cq_isEmpty(void)
{
    if (cq_front == -1 && cq_rear == -1) return 1;
    return 0;
}

int cq_isFull(void)
{
    if (cq_front == (cq_rear + 1) % SIZE) return 1;
    return 0;
}

int cq_inside(int i)
{
    if (cq_front == -1 && cq_rear == -1) return 0;
    if (cq_front <= cq_rear)
    {
        if (i >= cq_front && i <= cq_rear) return 1;
    }
    else
    {
        if (i >= cq_front || i <= cq_rear) return 1;
    }
    return 0;
}

void save_cq(char *msg)
{
    int i;
    if (out == NULL) return;
    start_step(msg);
    fprintf(out, "%d %d", cq_front, cq_rear);
    for (i = 0; i < SIZE; i++)
    {
        if (cq_inside(i)) fprintf(out, " %d", cq[i]);
        else fprintf(out, " -");
    }
    end_step();
}

/* ================================================================ */
/*                        singly linked list                        */
/* ================================================================ */

typedef struct LNode
{
    int data;
    struct LNode *next;
} LNode;

LNode *sll = NULL;
LNode *sll_cur = NULL;

LNode *sll_create_node(void)
{
    LNode *newnode;
    newnode = (LNode *)malloc(sizeof(LNode));
    printf("Enter data: ");
    if (scanf("%d", &newnode->data) != 1) newnode->data = 0;
    newnode->next = NULL;
    return newnode;
}

void save_sll(LNode *head, char *msg)
{
    LNode *p;
    if (out == NULL) return;
    start_step(msg);
    for (p = head; p != NULL; p = p->next) fprintf(out, "%d%s ", p->data, p == sll_cur ? "*" : "");
    end_step();
}

LNode *sll_insert_front(LNode *head)
{
    LNode *newnode;
    save_sll(head, "List before insertion.");
    newnode = sll_create_node();
    if (head == NULL) head = newnode;
    else { newnode->next = head; head = newnode; }
    sll_cur = head;
    save_sll(head, "Inserted at the front.");
    sll_cur = NULL;
    return head;
}

LNode *sll_insert_end(LNode *head)
{
    LNode *newnode;
    LNode *tmp;
    save_sll(head, "List before insertion.");
    if (head == NULL) head = sll_create_node();
    else
    {
        tmp = head;
        while (tmp->next != NULL)
        {
            sll_cur = tmp;
            save_sll(head, "Moving to the next node.");
            tmp = tmp->next;
        }
        newnode = sll_create_node();
        tmp->next = newnode;
        sll_cur = newnode;
    }
    save_sll(head, "Inserted at the end.");
    sll_cur = NULL;
    return head;
}

LNode *sll_insert_pos(LNode *head, int pos)
{
    LNode *newnode;
    LNode *tmp;
    int i;
    if (pos < 1) { printf("Invalid position\n"); return head; }
    if (pos == 1) return sll_insert_front(head);
    if (head == NULL) { printf("Invalid position\n"); return head; }
    save_sll(head, "List before insertion.");
    tmp = head;
    for (i = 1; i < pos - 1 && tmp->next != NULL; i++)
    {
        sll_cur = tmp;
        save_sll(head, "Moving to the next node.");
        tmp = tmp->next;
    }
    if (i < pos - 1) { printf("Invalid position\n"); return head; }
    newnode = sll_create_node();
    newnode->next = tmp->next;
    tmp->next = newnode;
    sll_cur = newnode;
    save_sll(head, "Inserted at the position.");
    sll_cur = NULL;
    return head;
}

LNode *sll_delete_front(LNode *head)
{
    LNode *tmp;
    if (head == NULL) { printf("List is empty\n"); return head; }
    sll_cur = head;
    save_sll(head, "Node that will be deleted.");
    sll_cur = NULL;
    tmp = head;
    head = tmp->next;
    free(tmp);
    save_sll(head, "Deleted the first node.");
    return head;
}

LNode *sll_delete_last(LNode *head)
{
    LNode *temp;
    LNode *temp1;
    if (head == NULL) { printf("List is empty\n"); return head; }
    if (head->next == NULL)
    {
        sll_cur = head;
        save_sll(head, "Only one node, so remove it.");
        free(head);
        head = NULL;
        sll_cur = NULL;
        save_sll(head, "Deleted the only node.");
        return head;
    }
    temp = head;
    temp1 = head->next;
    while (temp1->next != NULL)
    {
        sll_cur = temp1;
        save_sll(head, "Moving to the next node.");
        temp = temp1;
        temp1 = temp1->next;
    }
    sll_cur = temp1;
    save_sll(head, "Last node found.");
    temp->next = NULL;
    free(temp1);
    sll_cur = NULL;
    save_sll(head, "Deleted the last node.");
    return head;
}

LNode *sll_delete_pos(int pos, LNode *head)
{
    LNode *temp;
    LNode *temp1;
    int count = 0;
    int i;
    temp = head;
    while (temp != NULL) { count++; temp = temp->next; }
    if (pos < 1 || pos > count) { printf("Invalid position\n"); return head; }
    if (pos == 1) return sll_delete_front(head);
    save_sll(head, "List before deletion.");
    temp1 = head;
    temp = head;
    for (i = 1; i < pos; i++)
    {
        sll_cur = temp;
        save_sll(head, "Moving to the next node.");
        temp1 = temp;
        temp = temp->next;
    }
    sll_cur = temp;
    save_sll(head, "Node to delete found.");
    temp1->next = temp->next;
    free(temp);
    sll_cur = NULL;
    save_sll(head, "Deleted the node at the position.");
    return head;
}

int sll_count(LNode *head)
{
    int c = 0;
    LNode *tmp = head;
    while (tmp != NULL) { c++; tmp = tmp->next; }
    return c;
}

void sll_free(LNode *head)
{
    LNode *tmp;
    while (head != NULL) { tmp = head; head = head->next; free(tmp); }
}

/* ================================================================ */
/*                        doubly linked list                        */
/* ================================================================ */

typedef struct DNode
{
    int data;
    struct DNode *prev;
    struct DNode *next;
} DNode;

DNode *dll = NULL;
DNode *dll_cur = NULL;

DNode *dll_create_node(void)
{
    DNode *newnode;
    newnode = (DNode *)malloc(sizeof(DNode));
    printf("Enter data: ");
    if (scanf("%d", &newnode->data) != 1) newnode->data = 0;
    newnode->prev = NULL;
    newnode->next = NULL;
    return newnode;
}

void save_dll(DNode *head, char *msg)
{
    DNode *p;
    if (out == NULL) return;
    start_step(msg);
    for (p = head; p != NULL; p = p->next) fprintf(out, "%d%s ", p->data, p == dll_cur ? "*" : "");
    end_step();
}

DNode *dll_insert_begin(DNode *head)
{
    DNode *newnode;
    save_dll(head, "List before insertion.");
    newnode = dll_create_node();
    if (head == NULL) head = newnode;
    else { newnode->next = head; head->prev = newnode; head = newnode; }
    dll_cur = head;
    save_dll(head, "Inserted at the beginning.");
    dll_cur = NULL;
    return head;
}

DNode *dll_insert_end(DNode *head)
{
    DNode *newnode;
    DNode *tmp;
    save_dll(head, "List before insertion.");
    if (head == NULL) head = dll_create_node();
    else
    {
        tmp = head;
        while (tmp->next != NULL)
        {
            dll_cur = tmp;
            save_dll(head, "Moving to the next node.");
            tmp = tmp->next;
        }
        newnode = dll_create_node();
        newnode->prev = tmp;
        tmp->next = newnode;
        dll_cur = newnode;
    }
    save_dll(head, "Inserted at the end.");
    dll_cur = NULL;
    return head;
}

DNode *dll_insert_pos(DNode *head, int pos)
{
    DNode *newnode;
    DNode *tmp;
    int i = 1;
    if (pos <= 0) return head;
    if (pos == 1) return dll_insert_begin(head);
    if (head == NULL) return head;
    save_dll(head, "List before insertion.");
    tmp = head;
    while (i < pos - 1 && tmp->next != NULL)
    {
        dll_cur = tmp;
        save_dll(head, "Moving to the next node.");
        tmp = tmp->next;
        i++;
    }
    if (i != pos - 1) return head;
    newnode = dll_create_node();
    newnode->prev = tmp;
    newnode->next = tmp->next;
    if (tmp->next != NULL) tmp->next->prev = newnode;
    tmp->next = newnode;
    dll_cur = newnode;
    save_dll(head, "Inserted at the position.");
    dll_cur = NULL;
    return head;
}

DNode *dll_delete_first(DNode *head)
{
    DNode *tmp;
    if (head == NULL) { printf("List is empty\n"); return head; }
    dll_cur = head;
    save_dll(head, "Node that will be deleted.");
    dll_cur = NULL;
    tmp = head;
    if (head->next == NULL) head = NULL;
    else { head = head->next; head->prev = NULL; }
    free(tmp);
    save_dll(head, "Deleted the first node.");
    return head;
}

DNode *dll_delete_last(DNode *head)
{
    DNode *tmp;
    if (head == NULL) { printf("List is empty\n"); return head; }
    tmp = head;
    while (tmp->next != NULL)
    {
        dll_cur = tmp;
        save_dll(head, "Moving to the next node.");
        tmp = tmp->next;
    }
    dll_cur = tmp;
    save_dll(head, "Last node found.");
    if (tmp->prev == NULL) head = NULL;
    else tmp->prev->next = NULL;
    free(tmp);
    dll_cur = NULL;
    save_dll(head, "Deleted the last node.");
    return head;
}

DNode *dll_delete_pos(DNode *head, int pos)
{
    DNode *tmp;
    int i = 1;
    if (head == NULL || pos <= 0) return head;
    if (pos == 1) return dll_delete_first(head);
    save_dll(head, "List before deletion.");
    tmp = head;
    while (i < pos && tmp != NULL)
    {
        dll_cur = tmp;
        save_dll(head, "Moving to the next node.");
        tmp = tmp->next;
        i++;
    }
    if (tmp == NULL) { dll_cur = NULL; return head; }
    dll_cur = tmp;
    save_dll(head, "Node to delete found.");
    if (tmp->next != NULL) tmp->next->prev = tmp->prev;
    if (tmp->prev != NULL) tmp->prev->next = tmp->next;
    free(tmp);
    dll_cur = NULL;
    save_dll(head, "Deleted the node at the position.");
    return head;
}

void dll_traverse_forward(DNode *head)
{
    DNode *tmp = head;
    if (head == NULL) { printf("List is empty\n"); save_dll(head, "List is empty."); return; }
    while (tmp != NULL)
    {
        dll_cur = tmp;
        save_dll(head, "Visit this node.");
        printf("%d -> ", tmp->data);
        tmp = tmp->next;
    }
    dll_cur = NULL;
    printf("NULL\n");
}

void dll_traverse_backward(DNode *head)
{
    DNode *tmp = head;
    if (head == NULL) { printf("List is empty\n"); save_dll(head, "List is empty."); return; }
    while (tmp->next != NULL) tmp = tmp->next;
    while (tmp != NULL)
    {
        dll_cur = tmp;
        save_dll(head, "Visit this node.");
        printf("%d -> ", tmp->data);
        tmp = tmp->prev;
    }
    dll_cur = NULL;
    printf("NULL\n");
}

void dll_free(DNode *head)
{
    DNode *tmp;
    while (head != NULL) { tmp = head; head = head->next; free(tmp); }
}

/* ================================================================ */
/*                            graph                                 */
/* ================================================================ */

int gadj[MAX_V][MAX_V];
int gvis[MAX_V];
int gn = 5;
int gcur = -1;
int gpath[MAX_V];
int gpath_n = 0;
int gaux = 0; /* 0 = none, 1 = queue, 2 = stack */

Queue gq = {{0}, -1, -1};
Stack gs = {{0}, -1};

void save_graph(char *msg)
{
    int i, j;
    if (out == NULL) return;
    start_step(msg);
    fprintf(out, "%d;", gn);
    for (i = 0; i < gn; i++)
    {
        for (j = 0; j < gn; j++) fprintf(out, "%d ", gadj[i][j]);
        fprintf(out, ";");
    }
    for (i = 0; i < gn; i++) fprintf(out, "%d ", gvis[i]);
    fprintf(out, ";%d;", gcur);
    if (gaux == 1)
    {
        fprintf(out, "Q;");
        for (i = gq.f; i <= gq.r && gq.f != -1; i++) fprintf(out, "%d ", gq.a[i]);
    }
    else if (gaux == 2)
    {
        fprintf(out, "S;");
        for (i = 0; i <= gs.top; i++) fprintf(out, "%d ", gs.arr[i]);
    }
    else fprintf(out, "-;");
    fprintf(out, ";");
    for (i = 0; i < gpath_n; i++) fprintf(out, "%d ", gpath[i]);
    end_step();
}

void add_edge(int u, int v)
{
    if (u >= 0 && u < gn && v >= 0 && v < gn) { gadj[u][v] = 1; gadj[v][u] = 1; }
}

void graph_reset(void)
{
    int i, j;
    for (i = 0; i < MAX_V; i++)
    {
        gvis[i] = 0;
        for (j = 0; j < MAX_V; j++) gadj[i][j] = 0;
    }
    gcur = -1;
    gpath_n = 0;
    gaux = 0;
}

void bfs(int start)
{
    int i, v;
    for (i = 0; i < gn; i++) gvis[i] = 0;
    gq.f = -1;
    gq.r = -1;
    gpath_n = 0;
    gaux = 1;
    gcur = -1;
    enqueue(&gq, start);
    gvis[start] = 1;
    save_graph("Start vertex added to the queue.");
    while (!queue_isEmpty(&gq))
    {
        v = dequeue(&gq);
        gcur = v;
        gpath[gpath_n] = v;
        gpath_n++;
        save_graph("Front vertex removed and visited.");
        for (i = 0; i < gn; i++)
        {
            if (gadj[v][i] == 1 && gvis[i] == 0)
            {
                gvis[i] = 1;
                enqueue(&gq, i);
                save_graph("Unvisited neighbour added to the queue.");
            }
        }
    }
    gcur = -1;
    save_graph("BFS complete.");
    gaux = 0;
}

void dfs(int start)
{
    int i, v, found;
    for (i = 0; i < gn; i++) gvis[i] = 0;
    gs.top = -1;
    gpath_n = 0;
    gaux = 2;
    gcur = -1;
    push(&gs, start);
    gvis[start] = 1;
    gpath[gpath_n] = start;
    gpath_n++;
    gcur = start;
    save_graph("Start vertex pushed on the stack.");
    while (!stack_isEmpty(&gs))
    {
        v = peek(&gs);
        gcur = v;
        found = 0;
        save_graph("Look at the vertex on top of the stack.");
        for (i = 0; i < gn; i++)
        {
            if (gadj[v][i] == 1 && gvis[i] == 0)
            {
                gvis[i] = 1;
                push(&gs, i);
                gpath[gpath_n] = i;
                gpath_n++;
                gcur = i;
                save_graph("Unvisited neighbour pushed. Go deeper.");
                found = 1;
                break;
            }
        }
        if (found == 0)
        {
            pop(&gs);
            gcur = -1;
            if (gs.top >= 0) gcur = gs.arr[gs.top];
            save_graph("No unvisited neighbour, so backtrack.");
        }
    }
    gcur = -1;
    save_graph("DFS complete.");
    gaux = 0;
}

/* ================================================================ */
/*                            menus                                 */
/* ================================================================ */

void bst_menu(void)
{
    int ch, key;
    while (1)
    {
        printf("\n--- BINARY SEARCH TREE ---\n");
        printf("1. Insert\n");
        printf("2. Search\n");
        printf("3. Delete\n");
        printf("4. Inorder\n");
        printf("5. Preorder\n");
        printf("6. Postorder\n");
        printf("7. Count nodes\n");
        printf("8. Clear\n");
        printf("9. Back\n");
        printf("Enter choice: ");
        if (scanf("%d", &ch) != 1) return;
        if (ch == 9) return;
        switch (ch)
        {
        case 1:
            printf("Enter data: ");
            scanf("%d", &key);
            begin("bst", "Insert");
            bst = insert(bst, key);
            end_trace();
            break;
        case 2:
            if (bst == NULL) { printf("Tree is empty\n"); break; }
            printf("Enter element to search: ");
            scanf("%d", &key);
            begin("bst", "Search");
            if (search(bst, key)) printf("Element found\n");
            else printf("Element not found\n");
            end_trace();
            break;
        case 3:
            if (bst == NULL) { printf("Tree is empty\n"); break; }
            printf("Enter element to delete: ");
            scanf("%d", &key);
            begin("bst", "Delete");
            bst = delete(bst, key);
            end_trace();
            break;
        case 4:
            if (bst == NULL) { printf("Tree is empty\n"); break; }
            begin("bst", "Inorder");
            inorder(bst);
            end_trace();
            printf("\n");
            break;
        case 5:
            if (bst == NULL) { printf("Tree is empty\n"); break; }
            begin("bst", "Preorder");
            preorder(bst);
            end_trace();
            printf("\n");
            break;
        case 6:
            if (bst == NULL) { printf("Tree is empty\n"); break; }
            begin("bst", "Postorder");
            postorder(bst);
            end_trace();
            printf("\n");
            break;
        case 7:
            printf("Number of nodes: %d\n", count_nodes(bst));
            printf("Number of leaf nodes: %d\n", leaf_nodes(bst));
            printf("Number of interior nodes: %d\n", interior_nodes(bst));
            break;
        case 8:
            begin("bst", "Clear");
            save_bst(bst, "Tree before clear.");
            free_tree(bst);
            bst = NULL;
            save_bst(bst, "Tree cleared.");
            end_trace();
            break;
        default:
            printf("Invalid choice\n");
        }
    }
}

void stack_menu(void)
{
    int ch, ele, x;
    while (1)
    {
        printf("\n--- STACK ---\n");
        printf("1. Push\n");
        printf("2. Pop\n");
        printf("3. Peek\n");
        printf("4. Clear\n");
        printf("5. Back\n");
        printf("Enter choice: ");
        if (scanf("%d", &ch) != 1) return;
        switch (ch)
        {
        case 1:
            printf("Enter element: ");
            scanf("%d", &ele);
            begin("stack", "Push");
            save_stack(&st, "Stack before push.");
            push(&st, ele);
            save_stack(&st, "Value pushed on top.");
            end_trace();
            break;
        case 2:
            begin("stack", "Pop");
            save_stack(&st, "Stack before pop.");
            x = pop(&st);
            save_stack(&st, "Value removed from the top.");
            end_trace();
            if (x != -1) printf("Deleted element = %d\n", x);
            break;
        case 3:
            if (stack_isEmpty(&st)) printf("Stack is empty\n");
            else printf("Top element = %d\n", peek(&st));
            break;
        case 4:
            begin("stack", "Clear");
            save_stack(&st, "Stack before clear.");
            st.top = -1;
            save_stack(&st, "Stack cleared.");
            end_trace();
            break;
        case 5:
            return;
        default:
            printf("Invalid choice\n");
        }
    }
}

void simple_queue_menu(void)
{
    int ch, ele, x;
    while (1)
    {
        printf("\n--- SIMPLE QUEUE ---\n");
        printf("1. Enqueue\n");
        printf("2. Dequeue\n");
        printf("3. Is Empty\n");
        printf("4. Is Full\n");
        printf("5. Clear\n");
        printf("6. Back\n");
        printf("Enter choice: ");
        if (scanf("%d", &ch) != 1) return;
        switch (ch)
        {
        case 1:
            printf("Enter element: ");
            scanf("%d", &ele);
            begin("queue", "Enqueue");
            save_queue(&q, "Queue before enqueue.");
            enqueue(&q, ele);
            save_queue(&q, "Value added at the rear.");
            end_trace();
            break;
        case 2:
            begin("queue", "Dequeue");
            save_queue(&q, "Queue before dequeue.");
            x = dequeue(&q);
            save_queue(&q, "Value removed from the front.");
            end_trace();
            if (x != -1) printf("Deleted element = %d\n", x);
            break;
        case 3:
            if (queue_isEmpty(&q)) printf("Queue is empty\n");
            else printf("Queue is not empty\n");
            break;
        case 4:
            if (queue_isFull(&q)) printf("Queue is full\n");
            else printf("Queue is not full\n");
            break;
        case 5:
            begin("queue", "Clear");
            save_queue(&q, "Queue before clear.");
            q.f = q.r = -1;
            save_queue(&q, "Queue cleared.");
            end_trace();
            break;
        case 6:
            return;
        default:
            printf("Invalid choice\n");
        }
    }
}

void circular_queue_menu(void)
{
    int ch, ele, x;
    while (1)
    {
        printf("\n--- CIRCULAR QUEUE ---\n");
        printf("1. Enqueue\n");
        printf("2. Dequeue\n");
        printf("3. Is Empty\n");
        printf("4. Is Full\n");
        printf("5. Clear\n");
        printf("6. Back\n");
        printf("Enter choice: ");
        if (scanf("%d", &ch) != 1) return;
        switch (ch)
        {
        case 1:
            printf("Enter element: ");
            scanf("%d", &ele);
            begin("circular", "Enqueue");
            save_cq("Queue before enqueue.");
            cq_enqueue(ele);
            save_cq("Value added at the rear.");
            end_trace();
            break;
        case 2:
            begin("circular", "Dequeue");
            save_cq("Queue before dequeue.");
            x = cq_dequeue();
            save_cq("Value removed from the front.");
            end_trace();
            if (x != -1) printf("Deleted element = %d\n", x);
            break;
        case 3:
            if (cq_isEmpty()) printf("Queue is empty\n");
            else printf("Queue is not empty\n");
            break;
        case 4:
            if (cq_isFull()) printf("Queue is full\n");
            else printf("Queue is not full\n");
            break;
        case 5:
            begin("circular", "Clear");
            save_cq("Queue before clear.");
            cq_front = cq_rear = -1;
            save_cq("Queue cleared.");
            end_trace();
            break;
        case 6:
            return;
        default:
            printf("Invalid choice\n");
        }
    }
}

void queue_menu(void)
{
    int ch;
    while (1)
    {
        printf("\n--- QUEUE ---\n");
        printf("1. Simple Queue\n");
        printf("2. Circular Queue\n");
        printf("3. Back\n");
        printf("Enter choice: ");
        if (scanf("%d", &ch) != 1) return;
        if (ch == 1) simple_queue_menu();
        else if (ch == 2) circular_queue_menu();
        else if (ch == 3) return;
        else printf("Invalid choice\n");
    }
}

void singly_menu(void)
{
    int ch, pos;
    while (1)
    {
        printf("\n--- SINGLY LINKED LIST ---\n");
        printf("1. Insert at front\n");
        printf("2. Insert at end\n");
        printf("3. Insert at position\n");
        printf("4. Delete front\n");
        printf("5. Delete last\n");
        printf("6. Delete at position\n");
        printf("7. Count\n");
        printf("8. Clear\n");
        printf("9. Back\n");
        printf("Enter choice: ");
        if (scanf("%d", &ch) != 1) return;
        switch (ch)
        {
        case 1:
            begin("sll", "Insert at front");
            sll = sll_insert_front(sll);
            end_trace();
            break;
        case 2:
            begin("sll", "Insert at end");
            sll = sll_insert_end(sll);
            end_trace();
            break;
        case 3:
            printf("Enter position: ");
            scanf("%d", &pos);
            begin("sll", "Insert at position");
            sll = sll_insert_pos(sll, pos);
            end_trace();
            break;
        case 4:
            begin("sll", "Delete front");
            sll = sll_delete_front(sll);
            end_trace();
            break;
        case 5:
            begin("sll", "Delete last");
            sll = sll_delete_last(sll);
            end_trace();
            break;
        case 6:
            printf("Enter position: ");
            scanf("%d", &pos);
            begin("sll", "Delete at position");
            sll = sll_delete_pos(pos, sll);
            end_trace();
            break;
        case 7:
            printf("Number of nodes: %d\n", sll_count(sll));
            break;
        case 8:
            begin("sll", "Clear");
            save_sll(sll, "List before clear.");
            sll_free(sll);
            sll = NULL;
            save_sll(sll, "List cleared.");
            end_trace();
            break;
        case 9:
            return;
        default:
            printf("Invalid choice\n");
        }
    }
}

void doubly_menu(void)
{
    int ch, pos;
    while (1)
    {
        printf("\n--- DOUBLY LINKED LIST ---\n");
        printf("1. Insert at beginning\n");
        printf("2. Insert at end\n");
        printf("3. Insert at position\n");
        printf("4. Delete first\n");
        printf("5. Delete last\n");
        printf("6. Delete at position\n");
        printf("7. Traverse forward\n");
        printf("8. Traverse backward\n");
        printf("9. Clear\n");
        printf("10. Back\n");
        printf("Enter choice: ");
        if (scanf("%d", &ch) != 1) return;
        switch (ch)
        {
        case 1:
            begin("dll", "Insert at beginning");
            dll = dll_insert_begin(dll);
            end_trace();
            break;
        case 2:
            begin("dll", "Insert at end");
            dll = dll_insert_end(dll);
            end_trace();
            break;
        case 3:
            printf("Enter position: ");
            scanf("%d", &pos);
            begin("dll", "Insert at position");
            dll = dll_insert_pos(dll, pos);
            end_trace();
            break;
        case 4:
            begin("dll", "Delete first");
            dll = dll_delete_first(dll);
            end_trace();
            break;
        case 5:
            begin("dll", "Delete last");
            dll = dll_delete_last(dll);
            end_trace();
            break;
        case 6:
            printf("Enter position: ");
            scanf("%d", &pos);
            begin("dll", "Delete at position");
            dll = dll_delete_pos(dll, pos);
            end_trace();
            break;
        case 7:
            printf("List: ");
            begin("dll", "Traverse forward");
            dll_traverse_forward(dll);
            end_trace();
            break;
        case 8:
            printf("List: ");
            begin("dll", "Traverse backward");
            dll_traverse_backward(dll);
            end_trace();
            break;
        case 9:
            begin("dll", "Clear");
            save_dll(dll, "List before clear.");
            dll_free(dll);
            dll = NULL;
            save_dll(dll, "List cleared.");
            end_trace();
            break;
        case 10:
            return;
        default:
            printf("Invalid choice\n");
        }
    }
}

void list_menu(void)
{
    int ch;
    while (1)
    {
        printf("\n--- LINKED LIST ---\n");
        printf("1. Singly Linked List\n");
        printf("2. Doubly Linked List\n");
        printf("3. Back\n");
        printf("Enter choice: ");
        if (scanf("%d", &ch) != 1) return;
        if (ch == 1) singly_menu();
        else if (ch == 2) doubly_menu();
        else if (ch == 3) return;
        else printf("Invalid choice\n");
    }
}

void graph_menu(void)
{
    int ch, u, v, start, n;
    while (1)
    {
        printf("\n--- GRAPH ---\n");
        printf("1. Set number of vertices\n");
        printf("2. Add edge\n");
        printf("3. BFS\n");
        printf("4. DFS\n");
        printf("5. Clear edges\n");
        printf("6. Back\n");
        printf("Enter choice: ");
        if (scanf("%d", &ch) != 1) return;
        switch (ch)
        {
        case 1:
            printf("Enter number of vertices (2 to %d): ", MAX_V);
            scanf("%d", &n);
            if (n >= 2 && n <= MAX_V)
            {
                begin("graph", "Set vertices");
                save_graph("Graph before resize.");
                gn = n;
                graph_reset();
                save_graph("Graph resized.");
                end_trace();
                printf("Graph now has %d vertices.\n", gn);
            }
            else printf("Invalid number of vertices\n");
            break;
        case 2:
            printf("Enter first vertex (0 to %d): ", gn - 1);
            scanf("%d", &u);
            printf("Enter second vertex (0 to %d): ", gn - 1);
            scanf("%d", &v);
            if (u >= 0 && u < gn && v >= 0 && v < gn)
            {
                begin("graph", "Add edge");
                save_graph("Graph before adding the edge.");
                add_edge(u, v);
                save_graph("Edge added.");
                end_trace();
                printf("Edge added\n");
            }
            else printf("Invalid vertices\n");
            break;
        case 3:
            printf("Enter start vertex (0 to %d): ", gn - 1);
            scanf("%d", &start);
            if (start >= 0 && start < gn) { begin("graph", "BFS"); bfs(start); end_trace(); }
            else printf("Invalid vertex\n");
            break;
        case 4:
            printf("Enter start vertex (0 to %d): ", gn - 1);
            scanf("%d", &start);
            if (start >= 0 && start < gn) { begin("graph", "DFS"); dfs(start); end_trace(); }
            else printf("Invalid vertex\n");
            break;
        case 5:
            begin("graph", "Clear");
            save_graph("Graph before clear.");
            graph_reset();
            save_graph("All edges cleared.");
            end_trace();
            printf("Edges cleared\n");
            break;
        case 6:
            return;
        default:
            printf("Invalid choice\n");
        }
    }
}

int main(void)
{
    int ch;
    printf("Build the structures in the C menu.\n");
    printf("Open viewer.html through serve.py to watch them.\n");
    while (1)
    {
        printf("\n=================================\n");
        printf("   DATA STRUCTURE VISUALIZER\n");
        printf("=================================\n");
        printf("1. Binary Search Tree\n");
        printf("2. Graph\n");
        printf("3. Stack\n");
        printf("4. Queue\n");
        printf("5. Linked List\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &ch) != 1) break;
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
            list_menu();
            break;
        case 6:
            return 0;
        default:
            printf("Invalid choice\n");
        }
    }
    return 0;
}

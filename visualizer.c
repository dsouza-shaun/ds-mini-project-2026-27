#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <io.h>

#define STACK_SIZE 20
#define QUEUE_SIZE 20
#define MAX_GRAPH_VERTICES 10

int step_mode = 0;

void enable_ansi() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004;
    SetConsoleMode(hOut, dwMode);
}

void clear_screen() {
    system("cls");
}

int wait_key() {
    if (!_isatty(_fileno(stdin))) {
        return getchar();
    }
    return _getch();
}

void pause_step() {
    if (!_isatty(_fileno(stdin))) {
        return;
    }
    if (step_mode == 0) {
        printf("\n\033[1;36m[ENTER/SPACE]\033[0m Step  |  \033[1;33m[A]\033[0m Auto-Play  |  \033[1;32m[C]\033[0m Continue  > ");
        while (1) {
            int ch = wait_key();
            if (ch == 13 || ch == ' ' || ch == 10) {
                break;
            } else if (ch == 'a' || ch == 'A') {
                step_mode = 1;
                break;
            } else if (ch == 'c' || ch == 'C') {
                step_mode = 2;
                break;
            }
        }
    } else if (step_mode == 1) {
        Sleep(600);
        if (_kbhit()) {
            int ch = wait_key();
            if (ch == ' ' || ch == 13) {
                step_mode = 0;
            } else if (ch == 'c' || ch == 'C') {
                step_mode = 2;
            }
        }
    } else if (step_mode == 2) {
        Sleep(40);
    }
}

void render_debugger_frame(const char *title, void (*render_visual)(void), const char *func_sig, const char *call_stack, const char *locals, const char *code[], int total_lines, int cur_line, const char *status) {
    clear_screen();
    printf("\033[1;37;44m ============================================================================== \033[0m\n");
    printf("\033[1;37;44m               DATA STRUCTURE VISUALIZER & VS CODE DEBUGGER                     \033[0m\n");
    printf("\033[1;37;44m ============================================================================== \033[0m\n");
    printf("\033[1;34m [MODE]\033[0m %s\n", title);
    printf("--------------------------------------------------------------------------------\n");
    printf("\033[1;35m[VISUALIZATION]\033[0m\n");
    if (render_visual) {
        render_visual();
    }
    printf("\n--------------------------------------------------------------------------------\n");
    printf("\033[1;33m[VS CODE DEBUGGER]\033[0m %s\n", func_sig);
    printf("\033[1;36mCALL STACK:\033[0m %s\n", call_stack);
    printf("\033[1;36mVARIABLES :\033[0m %s\n", locals);
    printf("--------------------------------------------------------------------------------\n");
    printf("\033[1;32mSOURCE CODE:\033[0m\n");
    for (int i = 0; i < total_lines; i++) {
        if (i == cur_line) {
            printf("\033[1;33;44m==> %2d | %-70s\033[0m\n", i + 1, code[i]);
        } else {
            printf("    %2d | %s\n", i + 1, code[i]);
        }
    }
    printf("--------------------------------------------------------------------------------\n");
    printf("\033[1;37mSTATUS:\033[0m %s\n", status);
    pause_step();
}

typedef struct Node {
    struct Node *left;
    int data;
    struct Node *right;
} Node;

Node *bst_root = NULL;
int bst_highlight_val = -1;
int bst_node_x[1000];
int bst_node_y[1000];
int bst_inorder_idx = 0;
char bst_canvas[30][120];
char bst_traversal_buf[256];

Node *create_node(int key) {
    Node *newnode = (Node *)malloc(sizeof(Node));
    newnode->data = key;
    newnode->left = NULL;
    newnode->right = NULL;
    return newnode;
}

void bst_compute_positions(Node *root, int depth) {
    if (root == NULL) return;
    bst_compute_positions(root->left, depth + 1);
    if (root->data >= 0 && root->data < 1000) {
        bst_node_x[root->data] = bst_inorder_idx * 7 + 4;
        bst_node_y[root->data] = depth * 3;
    }
    bst_inorder_idx++;
    bst_compute_positions(root->right, depth + 1);
}

void bst_paint_tree(Node *root) {
    if (root == NULL) return;
    if (root->data >= 0 && root->data < 1000) {
        int x = bst_node_x[root->data];
        int y = bst_node_y[root->data];
        char buf[16];
        if (root->data == bst_highlight_val) {
            sprintf(buf, "*[%2d]*", root->data);
            for (int i = 0; buf[i]; i++) {
                if (y < 28 && (x - 1 + i) >= 0 && (x - 1 + i) < 118) {
                    bst_canvas[y][x - 1 + i] = buf[i];
                }
            }
        } else {
            sprintf(buf, "[%2d]", root->data);
            for (int i = 0; buf[i]; i++) {
                if (y < 28 && (x + i) >= 0 && (x + i) < 118) {
                    bst_canvas[y][x + i] = buf[i];
                }
            }
        }

        if (root->left && root->left->data >= 0 && root->left->data < 1000) {
            int lx = bst_node_x[root->left->data];
            int ly = bst_node_y[root->left->data];
            for (int c = lx + 3; c < x; c++) {
                if (y + 1 < 28 && c < 118) bst_canvas[y + 1][c] = '-';
            }
            if (y + 1 < 28 && lx + 2 < 118) bst_canvas[y + 1][lx + 2] = '/';
            if (ly - 1 < 28 && lx + 2 < 118) bst_canvas[ly - 1][lx + 2] = '|';
            bst_paint_tree(root->left);
        }

        if (root->right && root->right->data >= 0 && root->right->data < 1000) {
            int rx = bst_node_x[root->right->data];
            int ry = bst_node_y[root->right->data];
            for (int c = x + 4; c < rx + 1; c++) {
                if (y + 1 < 28 && c < 118) bst_canvas[y + 1][c] = '-';
            }
            if (y + 1 < 28 && rx + 1 < 118) bst_canvas[y + 1][rx + 1] = '\\';
            if (ry - 1 < 28 && rx + 1 < 118) bst_canvas[ry - 1][rx + 1] = '|';
            bst_paint_tree(root->right);
        }
    }
}

void bst_visualize() {
    if (bst_root == NULL) {
        printf(" \033[90m(Tree is empty. Insert nodes to visualize!)\033[0m\n");
        return;
    }
    bst_inorder_idx = 0;
    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < 119; j++) {
            bst_canvas[i][j] = ' ';
        }
        bst_canvas[i][119] = '\0';
    }
    bst_compute_positions(bst_root, 0);
    bst_paint_tree(bst_root);

    int max_rows = 15;
    for (int i = 0; i < max_rows; i++) {
        int last_char = 118;
        while (last_char >= 0 && bst_canvas[i][last_char] == ' ') last_char--;
        if (last_char < 0) {
            bst_canvas[i][0] = '\0';
        } else {
            bst_canvas[i][last_char + 1] = '\0';
        }
        if (strlen(bst_canvas[i]) > 0) {
            for (int j = 0; bst_canvas[i][j]; j++) {
                if (bst_canvas[i][j] == '*' && bst_canvas[i][j + 1] == '[') {
                    printf("\033[1;33;41m");
                }
                putchar(bst_canvas[i][j]);
                if (bst_canvas[i][j] == '*' && j > 0 && bst_canvas[i][j - 1] == ']') {
                    printf("\033[0m");
                }
            }
            printf("\n");
        }
    }

    if (strlen(bst_traversal_buf) > 0) {
        printf("\n\033[1;32mTRAVERSAL OUTPUT:\033[0m %s\n", bst_traversal_buf);
    }
}

Node *insert(Node *root, int key) {
    Node *newnode = create_node(key);
    if (root == NULL) {
        root = newnode;
    } else {
        Node *p, *q;
        p = q = root;
        while (p != NULL) {
            if (key < p->data) {
                q = p;
                p = p->left;
            } else {
                q = p;
                p = p->right;
            }
        }
        if (key > q->data) {
            q->right = newnode;
        } else {
            q->left = newnode;
        }
    }
    return root;
}

const char *insert_code[] = {
    "Node *insert(Node *root, int key)",
    "{",
    "    Node *newnode = create_node(key);",
    "    if (root == NULL)",
    "    {",
    "        root = newnode;",
    "    }",
    "    else",
    "    {",
    "        Node *p, *q;",
    "        p = q = root;",
    "        while (p != NULL)",
    "        {",
    "            if (key < p->data)",
    "            {",
    "                q = p;",
    "                p = p->left;",
    "            }",
    "            else",
    "            {",
    "                q = p;",
    "                p = p->right;",
    "            }",
    "        }",
    "        if (key > q->data)",
    "        {",
    "            q->right = newnode;",
    "        }",
    "        else",
    "        {",
    "            q->left = newnode;",
    "        }",
    "    }",
    "    return root;",
    "}"
};

Node *insert_debug(Node *root, int key) {
    char locals[128];
    char status[128];
    int total = sizeof(insert_code) / sizeof(insert_code[0]);

    bst_highlight_val = -1;
    sprintf(locals, "key = %d | root = %s", key, root ? "Node" : "NULL");
    sprintf(status, "Entering insert(root, key = %d)", key);
    render_debugger_frame("BST -> insert()", bst_visualize, "insert(Node *root, int key)", "main() -> insert()", locals, insert_code, total, 0, status);

    Node *newnode = create_node(key);
    sprintf(locals, "key = %d | newnode->data = %d", key, newnode->data);
    sprintf(status, "Allocated newnode with data = %d", key);
    render_debugger_frame("BST -> insert()", bst_visualize, "insert(Node *root, int key)", "main() -> insert()", locals, insert_code, total, 2, status);

    sprintf(status, "Checking if root == NULL: %s", root == NULL ? "TRUE" : "FALSE");
    render_debugger_frame("BST -> insert()", bst_visualize, "insert(Node *root, int key)", "main() -> insert()", locals, insert_code, total, 3, status);

    if (root == NULL) {
        root = newnode;
        bst_root = root;
        bst_highlight_val = root->data;
        sprintf(locals, "root = [%d] | key = %d", root->data, key);
        sprintf(status, "Tree was empty. root is now set to newnode [%d]", key);
        render_debugger_frame("BST -> insert()", bst_visualize, "insert(Node *root, int key)", "main() -> insert()", locals, insert_code, total, 5, status);
    } else {
        Node *p, *q;
        p = q = root;
        bst_highlight_val = p->data;
        sprintf(locals, "p = [%d] | q = [%d] | key = %d", p->data, q->data, key);
        sprintf(status, "Initialized pointer p and q to root [%d]", p->data);
        render_debugger_frame("BST -> insert()", bst_visualize, "insert(Node *root, int key)", "main() -> insert()", locals, insert_code, total, 10, status);

        while (p != NULL) {
            bst_highlight_val = p->data;
            sprintf(locals, "p = [%d] | q = [%d] | key = %d", p->data, q ? q->data : -1, key);
            sprintf(status, "p != NULL. Comparing key (%d) with p->data (%d)", key, p->data);
            render_debugger_frame("BST -> insert()", bst_visualize, "insert(Node *root, int key)", "main() -> insert()", locals, insert_code, total, 11, status);

            if (key < p->data) {
                sprintf(status, "%d < %d is TRUE. Moving to left child", key, p->data);
                render_debugger_frame("BST -> insert()", bst_visualize, "insert(Node *root, int key)", "main() -> insert()", locals, insert_code, total, 13, status);

                q = p;
                sprintf(locals, "q = [%d] | p = [%d]", q->data, p->data);
                sprintf(status, "q = p (saving parent)");
                render_debugger_frame("BST -> insert()", bst_visualize, "insert(Node *root, int key)", "main() -> insert()", locals, insert_code, total, 15, status);

                p = p->left;
                if (p) bst_highlight_val = p->data;
                else bst_highlight_val = -1;
                sprintf(locals, "q = [%d] | p = %s", q->data, p ? "Node" : "NULL");
                sprintf(status, "p = p->left");
                render_debugger_frame("BST -> insert()", bst_visualize, "insert(Node *root, int key)", "main() -> insert()", locals, insert_code, total, 16, status);
            } else {
                sprintf(status, "%d >= %d is FALSE. Moving to right child", key, p->data);
                render_debugger_frame("BST -> insert()", bst_visualize, "insert(Node *root, int key)", "main() -> insert()", locals, insert_code, total, 18, status);

                q = p;
                sprintf(locals, "q = [%d] | p = [%d]", q->data, p->data);
                sprintf(status, "q = p (saving parent)");
                render_debugger_frame("BST -> insert()", bst_visualize, "insert(Node *root, int key)", "main() -> insert()", locals, insert_code, total, 20, status);

                p = p->right;
                if (p) bst_highlight_val = p->data;
                else bst_highlight_val = -1;
                sprintf(locals, "q = [%d] | p = %s", q->data, p ? "Node" : "NULL");
                sprintf(status, "p = p->right");
                render_debugger_frame("BST -> insert()", bst_visualize, "insert(Node *root, int key)", "main() -> insert()", locals, insert_code, total, 21, status);
            }
        }

        bst_highlight_val = q->data;
        sprintf(locals, "q = [%d] | key = %d", q->data, key);
        sprintf(status, "Found insertion spot under parent q [%d]. Checking key > q->data", q->data);
        render_debugger_frame("BST -> insert()", bst_visualize, "insert(Node *root, int key)", "main() -> insert()", locals, insert_code, total, 24, status);

        if (key > q->data) {
            q->right = newnode;
            bst_highlight_val = newnode->data;
            sprintf(status, "Attached newnode [%d] as right child of [%d]", newnode->data, q->data);
            render_debugger_frame("BST -> insert()", bst_visualize, "insert(Node *root, int key)", "main() -> insert()", locals, insert_code, total, 26, status);
        } else {
            q->left = newnode;
            bst_highlight_val = newnode->data;
            sprintf(status, "Attached newnode [%d] as left child of [%d]", newnode->data, q->data);
            render_debugger_frame("BST -> insert()", bst_visualize, "insert(Node *root, int key)", "main() -> insert()", locals, insert_code, total, 30, status);
        }
    }

    sprintf(locals, "root = [%d]", root->data);
    sprintf(status, "Insert complete! Returning root.");
    render_debugger_frame("BST -> insert()", bst_visualize, "insert(Node *root, int key)", "main() -> insert()", locals, insert_code, total, 33, status);
    bst_highlight_val = -1;
    return root;
}

const char *search_code[] = {
    "int search(Node *root, int key)",
    "{",
    "    if (root == NULL)",
    "    {",
    "        printf(\"Tree is empty\\n\");",
    "        return 0;",
    "    }",
    "    else",
    "    {",
    "        Node *p = root;",
    "        while (p != NULL)",
    "        {",
    "            if (p->data == key)",
    "            {",
    "                return 1;",
    "            }",
    "            if (key > p->data)",
    "            {",
    "                p = p->right;",
    "            }",
    "            else",
    "            {",
    "                p = p->left;",
    "            }",
    "        }",
    "        return 0;",
    "    }",
    "}"
};

int search_debug(Node *root, int key) {
    char locals[128];
    char status[128];
    int total = sizeof(search_code) / sizeof(search_code[0]);

    sprintf(locals, "key = %d | root = %s", key, root ? "Node" : "NULL");
    sprintf(status, "Entering search(root, key = %d)", key);
    render_debugger_frame("BST -> search()", bst_visualize, "int search(Node *root, int key)", "main() -> search()", locals, search_code, total, 0, status);

    if (root == NULL) {
        sprintf(status, "Tree is empty. Returning 0");
        render_debugger_frame("BST -> search()", bst_visualize, "int search(Node *root, int key)", "main() -> search()", locals, search_code, total, 4, status);
        return 0;
    }

    Node *p = root;
    bst_highlight_val = p->data;
    sprintf(locals, "p = [%d] | key = %d", p->data, key);
    sprintf(status, "p = root [%d]", p->data);
    render_debugger_frame("BST -> search()", bst_visualize, "int search(Node *root, int key)", "main() -> search()", locals, search_code, total, 9, status);

    while (p != NULL) {
        bst_highlight_val = p->data;
        sprintf(locals, "p = [%d] | key = %d", p->data, key);
        sprintf(status, "Checking if p->data (%d) == key (%d)", p->data, key);
        render_debugger_frame("BST -> search()", bst_visualize, "int search(Node *root, int key)", "main() -> search()", locals, search_code, total, 12, status);

        if (p->data == key) {
            sprintf(status, "Key %d FOUND at node [%d]! Returning 1", key, p->data);
            render_debugger_frame("BST -> search()", bst_visualize, "int search(Node *root, int key)", "main() -> search()", locals, search_code, total, 14, status);
            return 1;
        }

        if (key > p->data) {
            sprintf(status, "%d > %d is TRUE. Moving right: p = p->right", key, p->data);
            render_debugger_frame("BST -> search()", bst_visualize, "int search(Node *root, int key)", "main() -> search()", locals, search_code, total, 18, status);
            p = p->right;
        } else {
            sprintf(status, "%d < %d is TRUE. Moving left: p = p->left", key, p->data);
            render_debugger_frame("BST -> search()", bst_visualize, "int search(Node *root, int key)", "main() -> search()", locals, search_code, total, 22, status);
            p = p->left;
        }
    }

    bst_highlight_val = -1;
    sprintf(status, "p reached NULL. Key %d not found! Returning 0", key);
    render_debugger_frame("BST -> search()", bst_visualize, "int search(Node *root, int key)", "main() -> search()", locals, search_code, total, 25, status);
    return 0;
}

const char *delete_code[] = {
    "Node *delete(Node *root, int key)",
    "{",
    "    Node *parent, *p;",
    "    parent = p = root;",
    "    int flag = 0;",
    "    while (p != NULL)",
    "    {",
    "        if (key == p->data) { flag = 1; break; }",
    "        else if (key < p->data) { parent = p; p = p->left; }",
    "        else { parent = p; p = p->right; }",
    "    }",
    "    if (flag == 1)",
    "    {",
    "        if (p->left == NULL && p->right == NULL) {",
    "            if (p == parent->left) parent->left = NULL;",
    "            else parent->right = NULL;",
    "            free(p);",
    "        } else if (p->left != NULL && p->right == NULL) {",
    "            if (p == parent->left) parent->left = p->left;",
    "            else parent->right = p->left;",
    "            free(p);",
    "        } else if (p->right != NULL && p->left == NULL) {",
    "            if (p == parent->left) parent->left = p->right;",
    "            else parent->right = p->right;",
    "            free(p);",
    "        } else {",
    "            Node *tmp = p->right;",
    "            while (tmp->left != NULL) tmp = tmp->left;",
    "            int x = tmp->data;",
    "            root = delete(root, x);",
    "            p->data = x;",
    "        }",
    "    }",
    "    return root;",
    "}"
};

Node *delete_debug(Node *root, int key) {
    char locals[128];
    char status[128];
    int total = sizeof(delete_code) / sizeof(delete_code[0]);

    bst_highlight_val = -1;
    Node *parent, *p;
    parent = p = root;
    int flag = 0;

    sprintf(locals, "key = %d | root = %s", key, root ? "Node" : "NULL");
    sprintf(status, "Entering delete(root, key = %d)", key);
    render_debugger_frame("BST -> delete()", bst_visualize, "delete(Node *root, int key)", "main() -> delete()", locals, delete_code, total, 0, status);

    while (p != NULL) {
        bst_highlight_val = p->data;
        sprintf(locals, "p = [%d] | parent = [%d] | key = %d", p->data, parent->data, key);
        sprintf(status, "Scanning tree: current p = [%d]", p->data);
        render_debugger_frame("BST -> delete()", bst_visualize, "delete(Node *root, int key)", "main() -> delete()", locals, delete_code, total, 5, status);

        if (key == p->data) {
            flag = 1;
            sprintf(status, "Match found! key %d == p->data. flag = 1", key);
            render_debugger_frame("BST -> delete()", bst_visualize, "delete(Node *root, int key)", "main() -> delete()", locals, delete_code, total, 7, status);
            break;
        } else if (key < p->data) {
            parent = p;
            p = p->left;
            sprintf(status, "%d < p->data. Moving left: parent = [%d], p = %s", key, parent->data, p ? "Node" : "NULL");
            render_debugger_frame("BST -> delete()", bst_visualize, "delete(Node *root, int key)", "main() -> delete()", locals, delete_code, total, 8, status);
        } else {
            parent = p;
            p = p->right;
            sprintf(status, "%d > p->data. Moving right: parent = [%d], p = %s", key, parent->data, p ? "Node" : "NULL");
            render_debugger_frame("BST -> delete()", bst_visualize, "delete(Node *root, int key)", "main() -> delete()", locals, delete_code, total, 9, status);
        }
    }

    if (flag == 1) {
        if (p->left == NULL && p->right == NULL) {
            sprintf(status, "Case 1: Node [%d] is a leaf node (no children).", p->data);
            render_debugger_frame("BST -> delete()", bst_visualize, "delete(Node *root, int key)", "main() -> delete()", locals, delete_code, total, 13, status);
            if (p == root) {
                root = NULL;
            } else if (p == parent->left) {
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
            free(p);
            bst_root = root;
            bst_highlight_val = -1;
            sprintf(status, "Leaf node freed and unlinked from parent.");
            render_debugger_frame("BST -> delete()", bst_visualize, "delete(Node *root, int key)", "main() -> delete()", locals, delete_code, total, 16, status);
        } else if (p->left != NULL && p->right == NULL) {
            sprintf(status, "Case 2: Node [%d] has only left child [%d].", p->data, p->left->data);
            render_debugger_frame("BST -> delete()", bst_visualize, "delete(Node *root, int key)", "main() -> delete()", locals, delete_code, total, 17, status);
            if (p == root) {
                root = p->left;
            } else if (p == parent->left) {
                parent->left = p->left;
            } else {
                parent->right = p->left;
            }
            free(p);
            bst_root = root;
            bst_highlight_val = -1;
            sprintf(status, "Node freed and replaced by left child.");
            render_debugger_frame("BST -> delete()", bst_visualize, "delete(Node *root, int key)", "main() -> delete()", locals, delete_code, total, 20, status);
        } else if (p->right != NULL && p->left == NULL) {
            sprintf(status, "Case 2: Node [%d] has only right child [%d].", p->data, p->right->data);
            render_debugger_frame("BST -> delete()", bst_visualize, "delete(Node *root, int key)", "main() -> delete()", locals, delete_code, total, 21, status);
            if (p == root) {
                root = p->right;
            } else if (p == parent->left) {
                parent->left = p->right;
            } else {
                parent->right = p->right;
            }
            free(p);
            bst_root = root;
            bst_highlight_val = -1;
            sprintf(status, "Node freed and replaced by right child.");
            render_debugger_frame("BST -> delete()", bst_visualize, "delete(Node *root, int key)", "main() -> delete()", locals, delete_code, total, 24, status);
        } else {
            sprintf(status, "Case 3: Node [%d] has two children. Finding inorder successor.", p->data);
            render_debugger_frame("BST -> delete()", bst_visualize, "delete(Node *root, int key)", "main() -> delete()", locals, delete_code, total, 25, status);
            Node *tmp = p->right;
            while (tmp->left != NULL) {
                tmp = tmp->left;
            }
            int x = tmp->data;
            sprintf(locals, "p = [%d] | inorder successor x = %d", p->data, x);
            sprintf(status, "Inorder successor is [%d]. Recursively deleting it.", x);
            render_debugger_frame("BST -> delete()", bst_visualize, "delete(Node *root, int key)", "main() -> delete()", locals, delete_code, total, 29, status);
            root = delete_debug(root, x);
            p->data = x;
            bst_root = root;
            sprintf(status, "Replaced p->data with inorder successor %d.", x);
            render_debugger_frame("BST -> delete()", bst_visualize, "delete(Node *root, int key)", "main() -> delete()", locals, delete_code, total, 30, status);
        }
    } else {
        sprintf(status, "Element %d not found in the tree.", key);
        render_debugger_frame("BST -> delete()", bst_visualize, "delete(Node *root, int key)", "main() -> delete()", locals, delete_code, total, 32, status);
    }

    bst_highlight_val = -1;
    return root;
}

const char *inorder_code[] = {
    "void inorder(Node *root)",
    "{",
    "    if (root != NULL)",
    "    {",
    "        inorder(root->left);",
    "        printf(\"%d \", root->data);",
    "        inorder(root->right);",
    "    }",
    "}"
};

void inorder_debug(Node *root) {
    char locals[128];
    char status[128];
    char stack_trace[128];
    int total = sizeof(inorder_code) / sizeof(inorder_code[0]);

    sprintf(locals, "root = %s", root ? "Node" : "NULL");
    sprintf(stack_trace, "inorder(root = %s)", root ? "Node" : "NULL");
    sprintf(status, "Checking if root != NULL");
    render_debugger_frame("BST -> inorder()", bst_visualize, "inorder(Node *root)", stack_trace, locals, inorder_code, total, 2, status);

    if (root != NULL) {
        bst_highlight_val = root->data;
        sprintf(locals, "root->data = %d", root->data);
        sprintf(stack_trace, "inorder(%d) -> inorder(%s)", root->data, root->left ? "left" : "NULL");
        sprintf(status, "Calling inorder(root->left)");
        render_debugger_frame("BST -> inorder()", bst_visualize, "inorder(Node *root)", stack_trace, locals, inorder_code, total, 4, status);

        inorder_debug(root->left);

        bst_highlight_val = root->data;
        char num[16];
        sprintf(num, "%d ", root->data);
        strcat(bst_traversal_buf, num);

        sprintf(locals, "root->data = %d", root->data);
        sprintf(stack_trace, "inorder(%d)", root->data);
        sprintf(status, "Visiting node: printed %d", root->data);
        render_debugger_frame("BST -> inorder()", bst_visualize, "inorder(Node *root)", stack_trace, locals, inorder_code, total, 5, status);

        sprintf(stack_trace, "inorder(%d) -> inorder(%s)", root->data, root->right ? "right" : "NULL");
        sprintf(status, "Calling inorder(root->right)");
        render_debugger_frame("BST -> inorder()", bst_visualize, "inorder(Node *root)", stack_trace, locals, inorder_code, total, 6, status);

        inorder_debug(root->right);
    }
}

const char *preorder_code[] = {
    "void preorder(Node *root)",
    "{",
    "    if (root != NULL)",
    "    {",
    "        printf(\"%d \", root->data);",
    "        preorder(root->left);",
    "        preorder(root->right);",
    "    }",
    "}"
};

void preorder_debug(Node *root) {
    char locals[128];
    char status[128];
    char stack_trace[128];
    int total = sizeof(preorder_code) / sizeof(preorder_code[0]);

    if (root != NULL) {
        bst_highlight_val = root->data;
        char num[16];
        sprintf(num, "%d ", root->data);
        strcat(bst_traversal_buf, num);

        sprintf(locals, "root->data = %d", root->data);
        sprintf(stack_trace, "preorder(%d)", root->data);
        sprintf(status, "Visiting node: printed %d", root->data);
        render_debugger_frame("BST -> preorder()", bst_visualize, "preorder(Node *root)", stack_trace, locals, preorder_code, total, 4, status);

        sprintf(stack_trace, "preorder(%d) -> preorder(%s)", root->data, root->left ? "left" : "NULL");
        sprintf(status, "Calling preorder(root->left)");
        render_debugger_frame("BST -> preorder()", bst_visualize, "preorder(Node *root)", stack_trace, locals, preorder_code, total, 5, status);

        preorder_debug(root->left);

        bst_highlight_val = root->data;
        sprintf(stack_trace, "preorder(%d) -> preorder(%s)", root->data, root->right ? "right" : "NULL");
        sprintf(status, "Calling preorder(root->right)");
        render_debugger_frame("BST -> preorder()", bst_visualize, "preorder(Node *root)", stack_trace, locals, preorder_code, total, 6, status);

        preorder_debug(root->right);
    }
}

const char *postorder_code[] = {
    "void postorder(Node *root)",
    "{",
    "    if (root != NULL)",
    "    {",
    "        postorder(root->left);",
    "        postorder(root->right);",
    "        printf(\"%d \", root->data);",
    "    }",
    "}"
};

void postorder_debug(Node *root) {
    char locals[128];
    char status[128];
    char stack_trace[128];
    int total = sizeof(postorder_code) / sizeof(postorder_code[0]);

    if (root != NULL) {
        bst_highlight_val = root->data;
        sprintf(locals, "root->data = %d", root->data);
        sprintf(stack_trace, "postorder(%d) -> postorder(left)", root->data);
        sprintf(status, "Calling postorder(root->left)");
        render_debugger_frame("BST -> postorder()", bst_visualize, "postorder(Node *root)", stack_trace, locals, postorder_code, total, 4, status);

        postorder_debug(root->left);

        bst_highlight_val = root->data;
        sprintf(stack_trace, "postorder(%d) -> postorder(right)", root->data);
        sprintf(status, "Calling postorder(root->right)");
        render_debugger_frame("BST -> postorder()", bst_visualize, "postorder(Node *root)", stack_trace, locals, postorder_code, total, 5, status);

        postorder_debug(root->right);

        bst_highlight_val = root->data;
        char num[16];
        sprintf(num, "%d ", root->data);
        strcat(bst_traversal_buf, num);

        sprintf(locals, "root->data = %d", root->data);
        sprintf(stack_trace, "postorder(%d)", root->data);
        sprintf(status, "Visiting node: printed %d", root->data);
        render_debugger_frame("BST -> postorder()", bst_visualize, "postorder(Node *root)", stack_trace, locals, postorder_code, total, 6, status);
    }
}

int count_nodes(Node *root) {
    if (root == NULL) return 0;
    return 1 + count_nodes(root->left) + count_nodes(root->right);
}

int leaf_nodes(Node *root) {
    if (root == NULL) return 0;
    if (root->left == NULL && root->right == NULL) return 1;
    return leaf_nodes(root->left) + leaf_nodes(root->right);
}

int interior_nodes(Node *root) {
    if (root == NULL) return 0;
    if (root->left != NULL || root->right != NULL)
        return 1 + interior_nodes(root->left) + interior_nodes(root->right);
    return 0;
}

void free_tree(Node *root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

typedef struct {
    int arr[STACK_SIZE];
    int top;
} Stack;

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

typedef struct {
    int a[QUEUE_SIZE];
    int f;
    int r;
} Queue;

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

int graph_adj[MAX_GRAPH_VERTICES][MAX_GRAPH_VERTICES];
int graph_visited[MAX_GRAPH_VERTICES];
int graph_n = 5;
int graph_current_vertex = -1;
int graph_traversal_list[MAX_GRAPH_VERTICES];
int graph_traversal_count = 0;
Queue graph_bfs_q;
Stack graph_dfs_s;
int graph_algo_mode = 0;

void graph_visualize() {
    printf(" ADJACENCY MATRIX (n = %d):          VERTICES & EDGES:\n", graph_n);
    printf("    ");
    for (int j = 0; j < graph_n; j++) {
        printf(" %d ", j);
    }
    printf("        ");
    for (int i = 0; i < graph_n; i++) {
        if (i == graph_current_vertex) {
            printf("\033[1;33;41m*[%d]*\033[0m ", i);
        } else if (graph_visited[i]) {
            printf("\033[1;32m[%d]\033[0m ", i);
        } else {
            printf("[%d] ", i);
        }
    }
    printf("\n");

    for (int i = 0; i < graph_n; i++) {
        if (i == graph_current_vertex) {
            printf(" \033[1;33m%d |\033[0m", i);
        } else {
            printf(" %d |", i);
        }
        for (int j = 0; j < graph_n; j++) {
            if (graph_adj[i][j]) {
                printf(" \033[1;32m1\033[0m ");
            } else {
                printf(" 0 ");
            }
        }
        printf("        ");
        printf("Vertex %d connects to: ", i);
        int has = 0;
        for (int j = 0; j < graph_n; j++) {
            if (graph_adj[i][j]) {
                printf("%d ", j);
                has = 1;
            }
        }
        if (!has) printf("(none)");
        printf("\n");
    }

    printf("\n VISITED STATUS: [ ");
    for (int i = 0; i < graph_n; i++) {
        printf("%d: %s ", i, graph_visited[i] ? "\033[1;32mYES\033[0m" : "NO ");
    }
    printf("]\n");

    if (graph_algo_mode == 1) {
        printf(" BFS QUEUE     : [ ");
        if (!is_empty_queue(&graph_bfs_q)) {
            for (int i = graph_bfs_q.f; i <= graph_bfs_q.r; i++) {
                printf("%d ", graph_bfs_q.a[i]);
            }
        }
        printf("] (f: %d, r: %d)\n", graph_bfs_q.f, graph_bfs_q.r);
    } else if (graph_algo_mode == 2) {
        printf(" DFS STACK     : [ ");
        if (!is_empty_stack(&graph_dfs_s)) {
            for (int i = 0; i <= graph_dfs_s.top; i++) {
                printf("%d ", graph_dfs_s.arr[i]);
            }
        }
        printf("] (top: %d)\n", graph_dfs_s.top);
    }

    if (graph_traversal_count > 0) {
        printf(" \033[1;32mTRAVERSAL PATH:\033[0m ");
        for (int i = 0; i < graph_traversal_count; i++) {
            printf("%d%s", graph_traversal_list[i], (i == graph_traversal_count - 1) ? "" : " -> ");
        }
        printf("\n");
    }
}

void graph_add_edge(int u, int v) {
    if (u >= 0 && u < graph_n && v >= 0 && v < graph_n) {
        graph_adj[u][v] = 1;
        graph_adj[v][u] = 1;
    }
}

const char *bfs_code[] = {
    "void bfs(int start)",
    "{",
    "    Queue q; q.f = -1; q.r = -1;",
    "    int i, j;",
    "    for (i = 0; i < n; i++) visited[i] = 0;",
    "    enqueue(&q, start);",
    "    visited[start] = 1;",
    "    while (!isEmpty(&q))",
    "    {",
    "        int curr = dequeue(&q);",
    "        printf(\"%d \", curr);",
    "        for (j = 0; j < n; j++)",
    "        {",
    "            if (adj[curr][j] == 1 && visited[j] == 0)",
    "            {",
    "                enqueue(&q, j);",
    "                visited[j] = 1;",
    "            }",
    "        }",
    "    }",
    "}"
};

void bfs_debug(int start) {
    char locals[128];
    char status[128];
    int total = sizeof(bfs_code) / sizeof(bfs_code[0]);

    graph_algo_mode = 1;
    graph_bfs_q.f = -1;
    graph_bfs_q.r = -1;
    graph_traversal_count = 0;
    graph_current_vertex = -1;

    for (int i = 0; i < graph_n; i++) graph_visited[i] = 0;

    sprintf(locals, "start = %d | q.f = -1 | q.r = -1", start);
    sprintf(status, "Entering bfs(start = %d)", start);
    render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 0, status);

    for (int i = 0; i < graph_n; i++) graph_visited[i] = 0;
    sprintf(status, "Initialized visited array to 0 for all %d vertices", graph_n);
    render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 4, status);

    enqueue(&graph_bfs_q, start);
    sprintf(locals, "enqueued start = %d | q.f = %d | q.r = %d", start, graph_bfs_q.f, graph_bfs_q.r);
    sprintf(status, "Enqueued start vertex %d into queue", start);
    render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 5, status);

    graph_visited[start] = 1;
    sprintf(locals, "visited[%d] = 1", start);
    sprintf(status, "Marked vertex %d as visited", start);
    render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 6, status);

    while (!is_empty_queue(&graph_bfs_q)) {
        sprintf(status, "Queue not empty. Checking while (!isEmpty(&q))");
        render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 7, status);

        int curr = dequeue(&graph_bfs_q);
        graph_current_vertex = curr;
        sprintf(locals, "curr = %d", curr);
        sprintf(status, "Dequeued curr = %d", curr);
        render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 9, status);

        graph_traversal_list[graph_traversal_count++] = curr;
        sprintf(status, "Visited vertex %d. Output added to path.", curr);
        render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 10, status);

        for (int j = 0; j < graph_n; j++) {
            sprintf(locals, "curr = %d | j = %d | adj[%d][%d] = %d | visited[%d] = %d", curr, j, curr, j, graph_adj[curr][j], j, graph_visited[j]);
            sprintf(status, "Checking neighbor %d of %d", j, curr);
            render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 13, status);

            if (graph_adj[curr][j] == 1 && graph_visited[j] == 0) {
                enqueue(&graph_bfs_q, j);
                graph_visited[j] = 1;
                sprintf(locals, "enqueued %d | visited[%d] = 1", j, j);
                sprintf(status, "Neighbor %d is adjacent and unvisited. Enqueue and mark visited.", j);
                render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 15, status);
            }
        }
    }

    graph_current_vertex = -1;
    sprintf(status, "BFS complete! All reachable vertices visited.");
    render_debugger_frame("Graph -> bfs()", graph_visualize, "bfs(int start)", "main() -> bfs()", locals, bfs_code, total, 19, status);
    graph_algo_mode = 0;
}

const char *dfs_code[] = {
    "void dfs(int start)",
    "{",
    "    Stack s; s.top = -1;",
    "    int i, j;",
    "    for (i = 0; i < n; i++) visited[i] = 0;",
    "    push(&s, start);",
    "    visited[start] = 1;",
    "    printf(\"%d \", start);",
    "    while (!isEmpty(&s))",
    "    {",
    "        int curr = peek(&s);",
    "        int found = 0;",
    "        for (j = 0; j < n; j++)",
    "        {",
    "            if (adj[curr][j] == 1 && visited[j] == 0)",
    "            {",
    "                visited[j] = 1;",
    "                printf(\"%d \", j);",
    "                push(&s, j);",
    "                found = 1;",
    "                break;",
    "            }",
    "        }",
    "        if (found == 0)",
    "        {",
    "            pop(&s);",
    "        }",
    "    }",
    "}"
};

void dfs_debug(int start) {
    char locals[128];
    char status[128];
    int total = sizeof(dfs_code) / sizeof(dfs_code[0]);

    graph_algo_mode = 2;
    graph_dfs_s.top = -1;
    graph_traversal_count = 0;
    graph_current_vertex = -1;

    for (int i = 0; i < graph_n; i++) graph_visited[i] = 0;

    sprintf(locals, "start = %d | s.top = -1", start);
    sprintf(status, "Entering non-recursive dfs(start = %d)", start);
    render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 0, status);

    for (int i = 0; i < graph_n; i++) graph_visited[i] = 0;
    sprintf(status, "Initialized visited array to 0 for all %d vertices", graph_n);
    render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 4, status);

    push(&graph_dfs_s, start);
    graph_current_vertex = start;
    sprintf(locals, "pushed start = %d | s.top = %d", start, graph_dfs_s.top);
    sprintf(status, "Pushed start vertex %d onto DFS stack", start);
    render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 5, status);

    graph_visited[start] = 1;
    sprintf(locals, "visited[%d] = 1", start);
    sprintf(status, "Marked start vertex %d as visited", start);
    render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 6, status);

    graph_traversal_list[graph_traversal_count++] = start;
    sprintf(status, "Printed start vertex %d", start);
    render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 7, status);

    while (!is_empty_stack(&graph_dfs_s)) {
        sprintf(status, "Stack not empty. Checking while (!isEmpty(&s))");
        render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 8, status);

        int curr = peek(&graph_dfs_s);
        graph_current_vertex = curr;
        int found = 0;
        sprintf(locals, "curr = %d | s.top = %d", curr, graph_dfs_s.top);
        sprintf(status, "Top of stack is curr = %d. Scanning adjacent vertices for (j = 0; j < n; j++)", curr);
        render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 10, status);

        for (int j = 0; j < graph_n; j++) {
            sprintf(locals, "curr = %d | j = %d | adj[%d][%d] = %d | visited[%d] = %d", curr, j, curr, j, graph_adj[curr][j], j, graph_visited[j]);
            sprintf(status, "Checking edge (%d, %d)", curr, j);
            render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 14, status);

            if (graph_adj[curr][j] == 1 && graph_visited[j] == 0) {
                graph_visited[j] = 1;
                sprintf(locals, "visited[%d] = 1", j);
                sprintf(status, "Found unvisited neighbor %d. Marked visited.", j);
                render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 16, status);

                graph_traversal_list[graph_traversal_count++] = j;
                sprintf(status, "Printed vertex %d. Added to path.", j);
                render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 17, status);

                push(&graph_dfs_s, j);
                sprintf(locals, "pushed %d | s.top = %d", j, graph_dfs_s.top);
                sprintf(status, "Pushed vertex %d onto DFS stack", j);
                render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 18, status);

                found = 1;
                sprintf(status, "found = 1; break from loop to explore deeper from %d", j);
                render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 20, status);
                break;
            }
        }

        if (found == 0) {
            sprintf(locals, "curr = %d | found = 0", curr);
            sprintf(status, "No unvisited neighbors for vertex %d. Backtracking!", curr);
            render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 23, status);

            int popped = pop(&graph_dfs_s);
            sprintf(locals, "popped = %d | new s.top = %d", popped, graph_dfs_s.top);
            sprintf(status, "Popped vertex %d from stack. Backtracked to previous vertex.", popped);
            render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 25, status);
        }
    }

    graph_current_vertex = -1;
    sprintf(status, "DFS complete! All reachable vertices visited.");
    render_debugger_frame("Graph -> dfs()", graph_visualize, "dfs(int start)", "main() -> dfs()", locals, dfs_code, total, 28, status);
    graph_algo_mode = 0;
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

typedef struct LLNode {
    int data;
    struct LLNode *next;
} LLNode;

LLNode *ll_head = NULL;
LLNode *ll_highlight_node = NULL;

void ll_visualize() {
    printf(" SINGLY LINKED LIST:\n\n ");
    if (ll_head == NULL) {
        printf("head -> NULL \033[90m(List is empty. Use Insert to add nodes!)\033[0m\n");
        return;
    }
    printf("head -> ");
    LLNode *tmp = ll_head;
    while (tmp != NULL) {
        if (tmp == ll_highlight_node) {
            printf("\033[1;33;41m*[%2d | * ]*\033[0m -> ", tmp->data);
        } else {
            printf("\033[1;32m[%2d | * ]\033[0m -> ", tmp->data);
        }
        tmp = tmp->next;
    }
    printf("NULL\n");
}

const char *insert_front_code[] = {
    "Node* insert_front(Node* head, int data)",
    "{",
    "    Node* newnode = (Node*) malloc(sizeof(Node));",
    "    newnode->data = data;",
    "    newnode->next = NULL;",
    "    if (head == NULL) {",
    "        head = newnode;",
    "    } else {",
    "        newnode->next = head;",
    "        head = newnode;",
    "    }",
    "    return head;",
    "}"
};

LLNode *insert_front_debug(LLNode *head, int data) {
    char locals[128];
    char status[128];
    int total = sizeof(insert_front_code) / sizeof(insert_front_code[0]);

    sprintf(locals, "data = %d | head = %s", data, head ? "Node" : "NULL");
    sprintf(status, "Entering insert_front(head, %d)", data);
    render_debugger_frame("Linked List -> insert_front()", ll_visualize, "insert_front(Node* head, int data)", "main() -> insert_front()", locals, insert_front_code, total, 0, status);

    LLNode *newnode = (LLNode *)malloc(sizeof(LLNode));
    newnode->data = data;
    newnode->next = NULL;
    sprintf(locals, "newnode->data = %d | newnode->next = NULL", data);
    sprintf(status, "Allocated newnode with data = %d", data);
    render_debugger_frame("Linked List -> insert_front()", ll_visualize, "insert_front(Node* head, int data)", "main() -> insert_front()", locals, insert_front_code, total, 2, status);

    if (head == NULL) {
        head = newnode;
        ll_head = head;
        ll_highlight_node = head;
        sprintf(status, "List was empty. head is now set to newnode [%d]", data);
        render_debugger_frame("Linked List -> insert_front()", ll_visualize, "insert_front(Node* head, int data)", "main() -> insert_front()", locals, insert_front_code, total, 6, status);
    } else {
        newnode->next = head;
        sprintf(status, "Pointed newnode->next to current head [%d]", head->data);
        render_debugger_frame("Linked List -> insert_front()", ll_visualize, "insert_front(Node* head, int data)", "main() -> insert_front()", locals, insert_front_code, total, 8, status);

        head = newnode;
        ll_head = head;
        ll_highlight_node = head;
        sprintf(status, "Updated head to point to newnode [%d]", data);
        render_debugger_frame("Linked List -> insert_front()", ll_visualize, "insert_front(Node* head, int data)", "main() -> insert_front()", locals, insert_front_code, total, 9, status);
    }

    ll_highlight_node = NULL;
    return head;
}

const char *insert_end_code[] = {
    "Node* insert_end(Node* head, int data)",
    "{",
    "    Node* newnode = (Node*) malloc(sizeof(Node));",
    "    newnode->data = data;",
    "    newnode->next = NULL;",
    "    if (head == NULL) {",
    "        head = newnode;",
    "    } else {",
    "        Node* tmp = head;",
    "        while (tmp->next != NULL) {",
    "            tmp = tmp->next;",
    "        }",
    "        tmp->next = newnode;",
    "    }",
    "    return head;",
    "}"
};

LLNode *insert_end_debug(LLNode *head, int data) {
    char locals[128];
    char status[128];
    int total = sizeof(insert_end_code) / sizeof(insert_end_code[0]);

    sprintf(locals, "data = %d | head = %s", data, head ? "Node" : "NULL");
    sprintf(status, "Entering insert_end(head, %d)", data);
    render_debugger_frame("Linked List -> insert_end()", ll_visualize, "insert_end(Node* head, int data)", "main() -> insert_end()", locals, insert_end_code, total, 0, status);

    LLNode *newnode = (LLNode *)malloc(sizeof(LLNode));
    newnode->data = data;
    newnode->next = NULL;
    sprintf(locals, "newnode->data = %d", data);
    sprintf(status, "Allocated newnode with data = %d", data);
    render_debugger_frame("Linked List -> insert_end()", ll_visualize, "insert_end(Node* head, int data)", "main() -> insert_end()", locals, insert_end_code, total, 2, status);

    if (head == NULL) {
        head = newnode;
        ll_head = head;
        ll_highlight_node = head;
        sprintf(status, "List was empty. head is now set to newnode [%d]", data);
        render_debugger_frame("Linked List -> insert_end()", ll_visualize, "insert_end(Node* head, int data)", "main() -> insert_end()", locals, insert_end_code, total, 6, status);
    } else {
        LLNode *tmp = head;
        ll_highlight_node = tmp;
        sprintf(locals, "tmp = [%d]", tmp->data);
        sprintf(status, "tmp = head [%d]. Traversing to end of list", tmp->data);
        render_debugger_frame("Linked List -> insert_end()", ll_visualize, "insert_end(Node* head, int data)", "main() -> insert_end()", locals, insert_end_code, total, 8, status);

        while (tmp->next != NULL) {
            tmp = tmp->next;
            ll_highlight_node = tmp;
            sprintf(locals, "tmp = [%d]", tmp->data);
            sprintf(status, "Advanced tmp to [%d]", tmp->data);
            render_debugger_frame("Linked List -> insert_end()", ll_visualize, "insert_end(Node* head, int data)", "main() -> insert_end()", locals, insert_end_code, total, 10, status);
        }

        tmp->next = newnode;
        ll_highlight_node = newnode;
        sprintf(status, "Attached newnode [%d] to tmp->next", data);
        render_debugger_frame("Linked List -> insert_end()", ll_visualize, "insert_end(Node* head, int data)", "main() -> insert_end()", locals, insert_end_code, total, 12, status);
    }

    ll_highlight_node = NULL;
    return head;
}

const char *delete_front_code[] = {
    "Node* delete_front(Node* head)",
    "{",
    "    Node* tmp;",
    "    if (head == NULL) {",
    "        printf(\"List is empty\\n\");",
    "    } else {",
    "        tmp = head;",
    "        head = tmp->next;",
    "        tmp->next = NULL;",
    "        free(tmp);",
    "    }",
    "    return head;",
    "}"
};

LLNode *delete_front_debug(LLNode *head) {
    char locals[128];
    char status[128];
    int total = sizeof(delete_front_code) / sizeof(delete_front_code[0]);

    sprintf(locals, "head = %s", head ? "Node" : "NULL");
    sprintf(status, "Entering delete_front(head)");
    render_debugger_frame("Linked List -> delete_front()", ll_visualize, "delete_front(Node* head)", "main() -> delete_front()", locals, delete_front_code, total, 0, status);

    if (head == NULL) {
        sprintf(status, "List is empty. Nothing to delete.");
        render_debugger_frame("Linked List -> delete_front()", ll_visualize, "delete_front(Node* head)", "main() -> delete_front()", locals, delete_front_code, total, 4, status);
    } else {
        LLNode *tmp = head;
        ll_highlight_node = tmp;
        sprintf(locals, "tmp = [%d]", tmp->data);
        sprintf(status, "tmp = head (saving front node [%d])", tmp->data);
        render_debugger_frame("Linked List -> delete_front()", ll_visualize, "delete_front(Node* head)", "main() -> delete_front()", locals, delete_front_code, total, 6, status);

        head = tmp->next;
        ll_head = head;
        sprintf(status, "head advanced to tmp->next");
        render_debugger_frame("Linked List -> delete_front()", ll_visualize, "delete_front(Node* head)", "main() -> delete_front()", locals, delete_front_code, total, 7, status);

        free(tmp);
        ll_highlight_node = NULL;
        sprintf(status, "Freed front node.");
        render_debugger_frame("Linked List -> delete_front()", ll_visualize, "delete_front(Node* head)", "main() -> delete_front()", locals, delete_front_code, total, 9, status);
    }

    return head;
}

void free_ll(LLNode *head) {
    while (head != NULL) {
        LLNode *tmp = head;
        head = head->next;
        free(tmp);
    }
}

void bst_menu() {
    int ch, key;
    while (1) {
        clear_screen();
        printf("\033[1;37;44m ============================================================================== \033[0m\n");
        printf("\033[1;37;44m                      BINARY SEARCH TREE (BST) VISUALIZER                      \033[0m\n");
        printf("\033[1;37;44m ============================================================================== \033[0m\n\n");
        bst_visualize();
        printf("\n--------------------------------------------------------------------------------\n");
        printf("1. Insert Node\n");
        printf("2. Search Key\n");
        printf("3. Delete Node\n");
        printf("4. Inorder Traversal\n");
        printf("5. Preorder Traversal\n");
        printf("6. Postorder Traversal\n");
        printf("7. Tree Statistics (Count, Leaves, Interior)\n");
        printf("8. Clear Tree (Empty all)\n");
        printf("9. Return to Main Menu\n");
        printf("--------------------------------------------------------------------------------\n");
        printf("Enter your choice: ");
        if (scanf("%d", &ch) != 1) {
            while (getchar() != '\n');
            continue;
        }

        step_mode = 0;
        bst_traversal_buf[0] = '\0';

        switch (ch) {
            case 1:
                printf("Enter data to insert: ");
                scanf("%d", &key);
                bst_root = insert_debug(bst_root, key);
                break;
            case 2:
                if (bst_root == NULL) {
                    printf("\nTree is empty! Please insert nodes first.\nPress any key to continue...");
                    wait_key();
                    break;
                }
                printf("Enter key to search: ");
                scanf("%d", &key);
                search_debug(bst_root, key);
                break;
            case 3:
                if (bst_root == NULL) {
                    printf("\nTree is empty! Please insert nodes first.\nPress any key to continue...");
                    wait_key();
                    break;
                }
                printf("Enter key to delete: ");
                scanf("%d", &key);
                bst_root = delete_debug(bst_root, key);
                break;
            case 4:
                if (bst_root == NULL) {
                    printf("\nTree is empty! Please insert nodes first.\nPress any key to continue...");
                    wait_key();
                    break;
                }
                bst_traversal_buf[0] = '\0';
                inorder_debug(bst_root);
                bst_highlight_val = -1;
                break;
            case 5:
                if (bst_root == NULL) {
                    printf("\nTree is empty! Please insert nodes first.\nPress any key to continue...");
                    wait_key();
                    break;
                }
                bst_traversal_buf[0] = '\0';
                preorder_debug(bst_root);
                bst_highlight_val = -1;
                break;
            case 6:
                if (bst_root == NULL) {
                    printf("\nTree is empty! Please insert nodes first.\nPress any key to continue...");
                    wait_key();
                    break;
                }
                bst_traversal_buf[0] = '\0';
                postorder_debug(bst_root);
                bst_highlight_val = -1;
                break;
            case 7:
                printf("\nTotal Nodes   : %d\n", count_nodes(bst_root));
                printf("Leaf Nodes    : %d\n", leaf_nodes(bst_root));
                printf("Interior Nodes: %d\n", interior_nodes(bst_root));
                printf("\nPress any key to continue...");
                wait_key();
                break;
            case 8:
                free_tree(bst_root);
                bst_root = NULL;
                bst_highlight_val = -1;
                printf("\nTree cleared successfully!\n");
                Sleep(600);
                break;
            case 9:
                return;
        }
    }
}

void graph_menu() {
    int ch, u, v, start, n_input;
    while (1) {
        clear_screen();
        printf("\033[1;37;44m ============================================================================== \033[0m\n");
        printf("\033[1;37;44m                           GRAPH ALGORITHMS VISUALIZER                         \033[0m\n");
        printf("\033[1;37;44m ============================================================================== \033[0m\n\n");
        graph_visualize();
        printf("\n--------------------------------------------------------------------------------\n");
        printf("1. Set Number of Vertices (Current n = %d, Max = %d)\n", graph_n, MAX_GRAPH_VERTICES);
        printf("2. Add Edge (u, v)\n");
        printf("3. Breadth First Search (BFS - Non-recursive)\n");
        printf("4. Depth First Search (DFS - Non-recursive)\n");
        printf("5. Clear All Edges\n");
        printf("6. Return to Main Menu\n");
        printf("--------------------------------------------------------------------------------\n");
        printf("Enter your choice: ");
        if (scanf("%d", &ch) != 1) {
            while (getchar() != '\n');
            continue;
        }

        step_mode = 0;

        switch (ch) {
            case 1:
                printf("Enter number of vertices (2 to %d): ", MAX_GRAPH_VERTICES);
                scanf("%d", &n_input);
                if (n_input >= 2 && n_input <= MAX_GRAPH_VERTICES) {
                    graph_n = n_input;
                    for (int i = 0; i < MAX_GRAPH_VERTICES; i++) {
                        graph_visited[i] = 0;
                        for (int j = 0; j < MAX_GRAPH_VERTICES; j++) graph_adj[i][j] = 0;
                    }
                    graph_traversal_count = 0;
                    printf("Graph resized to %d vertices. All edges reset.\n", graph_n);
                    Sleep(700);
                } else {
                    printf("Invalid number of vertices!\n");
                    Sleep(700);
                }
                break;
            case 2:
                printf("Enter vertex u (0 to %d): ", graph_n - 1);
                scanf("%d", &u);
                printf("Enter vertex v (0 to %d): ", graph_n - 1);
                scanf("%d", &v);
                if (u >= 0 && u < graph_n && v >= 0 && v < graph_n) {
                    graph_add_edge(u, v);
                } else {
                    printf("Invalid vertices!\n");
                    Sleep(700);
                }
                break;
            case 3:
                printf("Enter starting vertex (0 to %d): ", graph_n - 1);
                scanf("%d", &start);
                if (start >= 0 && start < graph_n) {
                    bfs_debug(start);
                } else {
                    printf("Invalid vertex!\n");
                    Sleep(700);
                }
                break;
            case 4:
                printf("Enter starting vertex (0 to %d): ", graph_n - 1);
                scanf("%d", &start);
                if (start >= 0 && start < graph_n) {
                    dfs_debug(start);
                } else {
                    printf("Invalid vertex!\n");
                    Sleep(700);
                }
                break;
            case 5:
                for (int i = 0; i < MAX_GRAPH_VERTICES; i++) {
                    graph_visited[i] = 0;
                    for (int j = 0; j < MAX_GRAPH_VERTICES; j++) graph_adj[i][j] = 0;
                }
                graph_traversal_count = 0;
                printf("\nAll edges cleared!\n");
                Sleep(600);
                break;
            case 6:
                return;
        }
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

void queue_menu() {
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

void ll_menu() {
    int ch, ele;
    while (1) {
        clear_screen();
        printf("\033[1;37;44m ============================================================================== \033[0m\n");
        printf("\033[1;37;44m                         SINGLY LINKED LIST VISUALIZER                         \033[0m\n");
        printf("\033[1;37;44m ============================================================================== \033[0m\n\n");
        ll_visualize();
        printf("\n--------------------------------------------------------------------------------\n");
        printf("1. Insert Front\n");
        printf("2. Insert End\n");
        printf("3. Delete Front\n");
        printf("4. Clear List\n");
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
                printf("Enter data: ");
                scanf("%d", &ele);
                ll_head = insert_front_debug(ll_head, ele);
                break;
            case 2:
                printf("Enter data: ");
                scanf("%d", &ele);
                ll_head = insert_end_debug(ll_head, ele);
                break;
            case 3:
                ll_head = delete_front_debug(ll_head);
                break;
            case 4:
                free_ll(ll_head);
                ll_head = NULL;
                printf("\nList cleared successfully!\n");
                Sleep(600);
                break;
            case 5:
                return;
        }
    }
}

int main() {
    enable_ansi();

    bst_root = NULL;
    st.top = -1;
    qu.f = -1;
    qu.r = -1;
    ll_head = NULL;

    for (int i = 0; i < MAX_GRAPH_VERTICES; i++) {
        graph_visited[i] = 0;
        for (int j = 0; j < MAX_GRAPH_VERTICES; j++) {
            graph_adj[i][j] = 0;
        }
    }

    int ch;
    while (1) {
        clear_screen();
        printf("\033[1;37;44m ============================================================================== \033[0m\n");
        printf("\033[1;37;44m                  DATA STRUCTURE VISUALIZER & CODE DEBUGGER                    \033[0m\n");
        printf("\033[1;37;44m ============================================================================== \033[0m\n\n");
        printf(" Select Data Structure to Explore & Debug:\n\n");
        printf("   \033[1;33m1.\033[0m Binary Search Tree (BST)  \033[90m[Insert, Delete, Search, Traversals]\033[0m\n");
        printf("   \033[1;33m2.\033[0m Graph Algorithms          \033[90m[Non-recursive BFS & DFS, Adjacency Matrix]\033[0m\n");
        printf("   \033[1;33m3.\033[0m Stack                     \033[90m[Push, Pop, Peek, Array state]\033[0m\n");
        printf("   \033[1;33m4.\033[0m Queue                     \033[90m[Enqueue, Dequeue, Front/Rear pointers]\033[0m\n");
        printf("   \033[1;33m5.\033[0m Singly Linked List        \033[90m[Insert Front, Insert End, Delete Front]\033[0m\n");
        printf("   \033[1;31m6.\033[0m Exit\n\n");
        printf("--------------------------------------------------------------------------------\n");
        printf(" Enter your choice: ");
        if (scanf("%d", &ch) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (ch) {
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
                clear_screen();
                printf("\nExiting Data Structure Visualizer. Goodbye!\n");
                exit(0);
        }
    }

    return 0;
}

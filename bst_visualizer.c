#include "visualizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

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


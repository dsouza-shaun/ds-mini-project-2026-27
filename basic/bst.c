#include "visualizer.h"

Node *bst_root = NULL;
int bst_highlight = -1;
int bst_traversal[100];
int bst_traversal_count = 0;
int bst_delete_found = 0;

char bst_canvas[TREE_ROWS][TREE_COLS];
int bst_next_x = 0;

/* ---------------------------------------------------------------- */
/*                     tree drawing functions                       */
/* ---------------------------------------------------------------- */

/*
 * Every node gets a column (x) and a row (y).
 * Inorder traversal gives the columns, so left children always
 * get smaller columns than their parent and nodes never overlap.
 */
void bst_set_positions(Node *root, int depth)
{
    if (root == NULL)
        return;

    bst_set_positions(root->left, depth + 1);

    root->x = 4 + bst_next_x * 8;
    root->y = depth * 3;
    bst_next_x++;

    bst_set_positions(root->right, depth + 1);
}

void bst_put_char(int row, int col, char ch)
{
    if (row >= 0 && row < TREE_ROWS && col >= 0 && col < TREE_COLS)
        bst_canvas[row][col] = ch;
}

void bst_draw_node(Node *node)
{
    char buf[20];
    int len;
    int i;

    len = sprintf(buf, "[%3d]", node->data);
    for (i = 0; i < len; i++)
        bst_put_char(node->y, node->x + i, buf[i]);

    if (node->data == bst_highlight)
        bst_put_char(node->y, node->x + len, '*');
}

/* parent and child are linked with:  ----/  and then  |
 *                                        |            |
 *                                     [parent]     [child]
 * The | under the parent shows that both branches start there.
 */
void bst_draw_left_branch(Node *parent, Node *child)
{
    int px = parent->x + 2;
    int cx = child->x + 2;
    int c;

    for (c = cx + 1; c < px; c++)
        bst_put_char(parent->y + 1, c, '-');

    bst_put_char(parent->y + 1, cx, '/');
    bst_put_char(parent->y + 1, px, '|');
    bst_put_char(child->y - 1, cx, '|');
}

void bst_draw_right_branch(Node *parent, Node *child)
{
    int px = parent->x + 2;
    int cx = child->x + 2;
    int c;

    for (c = px + 1; c < cx; c++)
        bst_put_char(parent->y + 1, c, '-');

    bst_put_char(parent->y + 1, cx, '\\');
    bst_put_char(parent->y + 1, px, '|');
    bst_put_char(child->y - 1, cx, '|');
}

void bst_draw_tree(Node *root)
{
    if (root == NULL)
        return;

    bst_draw_node(root);

    if (root->left != NULL)
    {
        bst_draw_left_branch(root, root->left);
        bst_draw_tree(root->left);
    }

    if (root->right != NULL)
    {
        bst_draw_right_branch(root, root->right);
        bst_draw_tree(root->right);
    }
}

void show_bst(void)
{
    int row, col, last;

    if (bst_root == NULL)
    {
        printf("(Tree is empty)\n");
        return;
    }

    for (row = 0; row < TREE_ROWS; row++)
        for (col = 0; col < TREE_COLS; col++)
            bst_canvas[row][col] = ' ';

    bst_next_x = 0;
    bst_set_positions(bst_root, 0);
    bst_draw_tree(bst_root);

    /* print only rows that contain something */
    for (row = 0; row < TREE_ROWS; row++)
    {
        last = -1;
        for (col = 0; col < TREE_COLS; col++)
            if (bst_canvas[row][col] != ' ')
                last = col;

        for (col = 0; col <= last; col++)
            printf("%c", bst_canvas[row][col]);

        if (last >= 0)
            printf("\n");
    }

    if (bst_traversal_count > 0)
    {
        printf("\nTraversal output: ");
        for (row = 0; row < bst_traversal_count; row++)
            printf("%d ", bst_traversal[row]);
        printf("\n");
    }
}

/* ---------------------------------------------------------------- */
/*                         insert                                   */
/* ---------------------------------------------------------------- */

Node *bst_new_node(int key)
{
    Node *newnode;

    newnode = (Node *)malloc(sizeof(Node));
    newnode->data = key;
    newnode->left = NULL;
    newnode->right = NULL;
    newnode->x = 0;
    newnode->y = 0;

    return newnode;
}

char *insert_code[] = {
    "void bst_insert(int key)",
    "{",
    "    Node *newnode = bst_new_node(key);",
    "    Node *p, *q;",
    "",
    "    if (bst_root == NULL)",
    "    {",
    "        bst_root = newnode;",
    "        return;",
    "    }",
    "",
    "    p = bst_root;",
    "    q = NULL;",
    "",
    "    while (p != NULL)",
    "    {",
    "        q = p;",
    "        if (key < p->data)",
    "            p = p->left;",
    "        else",
    "            p = p->right;",
    "    }",
    "",
    "    if (key < q->data)",
    "        q->left = newnode;",
    "    else",
    "        q->right = newnode;",
    "}"
};

void bst_insert(int key)
{
    Node *newnode, *p, *q;
    char locals[160];
    char status[160];
    int total = sizeof(insert_code) / sizeof(insert_code[0]);

    newnode = bst_new_node(key);

    sprintf(locals, "key = %d | newnode = [%3d]", key, key);
    sprintf(status, "Create a new node for %d.", key);
    show_frame(MODE_BST, "BST -> insert()", "void bst_insert(int key)",
               "main() -> bst_insert()", locals, insert_code, total, 2, status);

    if (bst_root == NULL)
    {
        bst_root = newnode;
        bst_highlight = newnode->data;

        sprintf(locals, "bst_root = [%3d]", bst_root->data);
        sprintf(status, "Tree was empty, so the new node becomes the root.");
        show_frame(MODE_BST, "BST -> insert()", "void bst_insert(int key)",
                   "main() -> bst_insert()", locals, insert_code, total, 7, status);

        bst_highlight = -1;
        return;
    }

    p = bst_root;
    q = NULL;

    sprintf(locals, "p = [%3d] | q = NULL", p->data);
    sprintf(status, "Start at the root and look for the insertion point.");
    show_frame(MODE_BST, "BST -> insert()", "void bst_insert(int key)",
               "main() -> bst_insert()", locals, insert_code, total, 11, status);

    while (p != NULL)
    {
        q = p;
        bst_highlight = p->data;

        sprintf(locals, "p = [%3d] | q = [%3d] | key = %d", p->data, q->data, key);
        sprintf(status, "Compare key %d with node %d.", key, p->data);
        show_frame(MODE_BST, "BST -> insert()", "void bst_insert(int key)",
                   "main() -> bst_insert()", locals, insert_code, total, 17, status);

        if (key < p->data)
        {
            p = p->left;
            sprintf(status, "%d is smaller than %d, so move left.", key, q->data);
            show_frame(MODE_BST, "BST -> insert()", "void bst_insert(int key)",
                       "main() -> bst_insert()", locals, insert_code, total, 18, status);
        }
        else
        {
            p = p->right;
            sprintf(status, "%d is not smaller than %d, so move right.", key, q->data);
            show_frame(MODE_BST, "BST -> insert()", "void bst_insert(int key)",
                       "main() -> bst_insert()", locals, insert_code, total, 20, status);
        }
    }

    if (key < q->data)
    {
        q->left = newnode;
        bst_highlight = newnode->data;
        sprintf(locals, "q = [%3d] | newnode = [%3d]", q->data, key);
        sprintf(status, "Attach %d as the left child of %d.", key, q->data);
        show_frame(MODE_BST, "BST -> insert()", "void bst_insert(int key)",
                   "main() -> bst_insert()", locals, insert_code, total, 24, status);
    }
    else
    {
        q->right = newnode;
        bst_highlight = newnode->data;
        sprintf(locals, "q = [%3d] | newnode = [%3d]", q->data, key);
        sprintf(status, "Attach %d as the right child of %d.", key, q->data);
        show_frame(MODE_BST, "BST -> insert()", "void bst_insert(int key)",
                   "main() -> bst_insert()", locals, insert_code, total, 26, status);
    }

    bst_highlight = -1;
}

/* ---------------------------------------------------------------- */
/*                         search                                   */
/* ---------------------------------------------------------------- */

char *search_code[] = {
    "int bst_search(int key)",
    "{",
    "    Node *p = bst_root;",
    "",
    "    while (p != NULL)",
    "    {",
    "        if (p->data == key)",
    "            return 1;",
    "        if (key < p->data)",
    "            p = p->left;",
    "        else",
    "            p = p->right;",
    "    }",
    "    return 0;",
    "}"
};

int bst_search(int key)
{
    Node *p;
    char locals[160];
    char status[160];
    int total = sizeof(search_code) / sizeof(search_code[0]);

    p = bst_root;

    if (p == NULL)
        sprintf(locals, "p = NULL");
    else
        sprintf(locals, "p = [%3d] | key = %d", p->data, key);

    sprintf(status, "Start searching for %d from the root.", key);
    show_frame(MODE_BST, "BST -> search()", "int bst_search(int key)",
               "main() -> bst_search()", locals, search_code, total, 2, status);

    while (p != NULL)
    {
        bst_highlight = p->data;

        sprintf(locals, "p = [%3d] | key = %d", p->data, key);
        sprintf(status, "Compare key %d with node %d.", key, p->data);
        show_frame(MODE_BST, "BST -> search()", "int bst_search(int key)",
                   "main() -> bst_search()", locals, search_code, total, 6, status);

        if (p->data == key)
        {
            sprintf(status, "Key %d found at node %d.", key, p->data);
            show_frame(MODE_BST, "BST -> search()", "int bst_search(int key)",
                       "main() -> bst_search()", locals, search_code, total, 7, status);
            bst_highlight = -1;
            return 1;
        }

        if (key < p->data)
        {
            sprintf(status, "%d is smaller than %d, so move left.", key, p->data);
            p = p->left;

            if (p == NULL)
            {
                bst_highlight = -1;
                sprintf(locals, "p = NULL | key = %d", key);
            }
            else
            {
                bst_highlight = p->data;
                sprintf(locals, "p = [%3d] | key = %d", p->data, key);
            }

            show_frame(MODE_BST, "BST -> search()", "int bst_search(int key)",
                       "main() -> bst_search()", locals, search_code, total, 9, status);
        }
        else
        {
            sprintf(status, "%d is greater than %d, so move right.", key, p->data);
            p = p->right;

            if (p == NULL)
            {
                bst_highlight = -1;
                sprintf(locals, "p = NULL | key = %d", key);
            }
            else
            {
                bst_highlight = p->data;
                sprintf(locals, "p = [%3d] | key = %d", p->data, key);
            }

            show_frame(MODE_BST, "BST -> search()", "int bst_search(int key)",
                       "main() -> bst_search()", locals, search_code, total, 11, status);
        }
    }

    bst_highlight = -1;
    sprintf(locals, "p = NULL | key = %d", key);
    sprintf(status, "Reached NULL, so %d is not in the tree.", key);
    show_frame(MODE_BST, "BST -> search()", "int bst_search(int key)",
               "main() -> bst_search()", locals, search_code, total, 13, status);

    return 0;
}

/* ---------------------------------------------------------------- */
/*                         delete                                   */
/* ---------------------------------------------------------------- */

Node *bst_min(Node *root)
{
    while (root->left != NULL)
        root = root->left;

    return root;
}

char *delete_code[] = {
    "Node *bst_delete_node(Node *root, int key)",
    "{",
    "    Node *temp;",
    "",
    "    if (root == NULL)",
    "        return NULL;",
    "",
    "    if (key < root->data)",
    "        root->left = bst_delete_node(root->left, key);",
    "    else if (key > root->data)",
    "        root->right = bst_delete_node(root->right, key);",
    "    else",
    "    {",
    "        if (root->left == NULL)",
    "        {",
    "            temp = root->right;",
    "            free(root);",
    "            return temp;",
    "        }",
    "        if (root->right == NULL)",
    "        {",
    "            temp = root->left;",
    "            free(root);",
    "            return temp;",
    "        }",
    "        temp = bst_min(root->right);",
    "        root->data = temp->data;",
    "        root->right = bst_delete_node(root->right, temp->data);",
    "    }",
    "    return root;",
    "}"
};

Node *bst_delete_node(Node *root, int key)
{
    Node *temp;
    char locals[160];
    char status[160];
    int total = sizeof(delete_code) / sizeof(delete_code[0]);

    if (root == NULL)
    {
        sprintf(locals, "root = NULL | key = %d", key);
        sprintf(status, "Key %d is not in this part of the tree.", key);
        show_frame(MODE_BST, "BST -> delete()", "Node *bst_delete_node(Node *root, int key)",
                   "main() -> bst_delete() -> bst_delete_node()", locals,
                   delete_code, total, 5, status);
        return NULL;
    }

    sprintf(locals, "root = [%3d] | key = %d", root->data, key);
    sprintf(status, "At node %d, compare it with key %d.", root->data, key);
    show_frame(MODE_BST, "BST -> delete()", "Node *bst_delete_node(Node *root, int key)",
               "main() -> bst_delete() -> bst_delete_node()", locals,
               delete_code, total, 7, status);

    if (key < root->data)
    {
        sprintf(status, "%d is smaller than %d, so go left.", key, root->data);
        show_frame(MODE_BST, "BST -> delete()", "Node *bst_delete_node(Node *root, int key)",
                   "main() -> bst_delete() -> bst_delete_node()", locals,
                   delete_code, total, 8, status);

        root->left = bst_delete_node(root->left, key);
        return root;
    }

    if (key > root->data)
    {
        sprintf(status, "%d is greater than %d, so go right.", key, root->data);
        show_frame(MODE_BST, "BST -> delete()", "Node *bst_delete_node(Node *root, int key)",
                   "main() -> bst_delete() -> bst_delete_node()", locals,
                   delete_code, total, 10, status);

        root->right = bst_delete_node(root->right, key);
        return root;
    }

    /* this is the node to delete */
    bst_delete_found = 1;
    bst_highlight = root->data;

    sprintf(status, "Found %d. Now check its children.", key);
    show_frame(MODE_BST, "BST -> delete()", "Node *bst_delete_node(Node *root, int key)",
               "main() -> bst_delete() -> bst_delete_node()", locals,
               delete_code, total, 12, status);

    if (root->left == NULL)
    {
        temp = root->right;

        if (temp == NULL)
            sprintf(status, "No children. Free %d and return NULL.", key);
        else
            sprintf(status, "Only a right child. Free %d and return %d.", key, temp->data);

        show_frame(MODE_BST, "BST -> delete()", "Node *bst_delete_node(Node *root, int key)",
                   "main() -> bst_delete() -> bst_delete_node()", locals,
                   delete_code, total, 15, status);

        free(root);
        return temp;
    }

    if (root->right == NULL)
    {
        temp = root->left;

        sprintf(status, "Only a left child. Free %d and return %d.", key, temp->data);
        show_frame(MODE_BST, "BST -> delete()", "Node *bst_delete_node(Node *root, int key)",
                   "main() -> bst_delete() -> bst_delete_node()", locals,
                   delete_code, total, 21, status);

        free(root);
        return temp;
    }

    temp = bst_min(root->right);

    sprintf(status, "Two children. The inorder successor is %d.", temp->data);
    show_frame(MODE_BST, "BST -> delete()", "Node *bst_delete_node(Node *root, int key)",
               "main() -> bst_delete() -> bst_delete_node()", locals,
               delete_code, total, 25, status);

    root->data = temp->data;

    sprintf(locals, "root = [%3d] | key = %d", root->data, key);
    sprintf(status, "Copy the successor value %d into this node.", temp->data);
    show_frame(MODE_BST, "BST -> delete()", "Node *bst_delete_node(Node *root, int key)",
               "main() -> bst_delete() -> bst_delete_node()", locals,
               delete_code, total, 26, status);

    sprintf(status, "Delete the old successor node with value %d.", temp->data);
    show_frame(MODE_BST, "BST -> delete()", "Node *bst_delete_node(Node *root, int key)",
               "main() -> bst_delete() -> bst_delete_node()", locals,
               delete_code, total, 27, status);

    root->right = bst_delete_node(root->right, temp->data);
    return root;
}

char *delete_start_code[] = {
    "void bst_delete(int key)",
    "{",
    "    bst_root = bst_delete_node(bst_root, key);",
    "}"
};

void bst_delete(int key)
{
    char locals[160];
    char status[160];
    int total = sizeof(delete_start_code) / sizeof(delete_start_code[0]);

    bst_delete_found = 0;
    bst_highlight = -1;

    sprintf(locals, "key = %d | bst_root = %s", key, bst_root == NULL ? "NULL" : "node");
    sprintf(status, "Start deleting %d by calling bst_delete_node().", key);
    show_frame(MODE_BST, "BST -> delete()", "void bst_delete(int key)",
               "main() -> bst_delete()", locals, delete_start_code, total, 0, status);

    bst_root = bst_delete_node(bst_root, key);
    bst_highlight = -1;

    if (bst_delete_found)
        sprintf(status, "Key %d was deleted. The tree is updated.", key);
    else
        sprintf(status, "Key %d was not found, so nothing changed.", key);

    show_frame(MODE_BST, "BST -> delete()", "void bst_delete(int key)",
               "main() -> bst_delete()", locals, delete_start_code, total, 2, status);
}

/* ---------------------------------------------------------------- */
/*                        traversals                                */
/* ---------------------------------------------------------------- */

void bst_add_traversal(int value)
{
    if (bst_traversal_count < 100)
    {
        bst_traversal[bst_traversal_count] = value;
        bst_traversal_count++;
    }
}

/*
 * The traversal functions store every visited value in bst_traversal
 * instead of printing it, so the value can be shown above the tree.
 * The code panel shows the normal printf version of each traversal.
 */
char *inorder_code[] = {
    "void bst_inorder(Node *root)",
    "{",
    "    if (root != NULL)",
    "    {",
    "        bst_inorder(root->left);",
    "        printf(\"%d \", root->data);",
    "        bst_inorder(root->right);",
    "    }",
    "}"
};

void bst_inorder(Node *root)
{
    char locals[160];
    char status[160];
    int total = sizeof(inorder_code) / sizeof(inorder_code[0]);

    if (root != NULL)
    {
        bst_highlight = root->data;
        sprintf(locals, "root = [%3d]", root->data);
        sprintf(status, "Inorder: go to the left child of %d first.", root->data);
        show_frame(MODE_BST, "BST -> inorder()", "void bst_inorder(Node *root)",
                   "main() -> bst_inorder()", locals, inorder_code, total, 4, status);

        bst_inorder(root->left);

        bst_highlight = root->data;
        bst_add_traversal(root->data);
        sprintf(status, "Visit %d and add it to the output.", root->data);
        show_frame(MODE_BST, "BST -> inorder()", "void bst_inorder(Node *root)",
                   "main() -> bst_inorder()", locals, inorder_code, total, 5, status);

        sprintf(status, "Now go to the right child of %d.", root->data);
        show_frame(MODE_BST, "BST -> inorder()", "void bst_inorder(Node *root)",
                   "main() -> bst_inorder()", locals, inorder_code, total, 6, status);

        bst_inorder(root->right);
    }
}

char *preorder_code[] = {
    "void bst_preorder(Node *root)",
    "{",
    "    if (root != NULL)",
    "    {",
    "        printf(\"%d \", root->data);",
    "        bst_preorder(root->left);",
    "        bst_preorder(root->right);",
    "    }",
    "}"
};

void bst_preorder(Node *root)
{
    char locals[160];
    char status[160];
    int total = sizeof(preorder_code) / sizeof(preorder_code[0]);

    if (root != NULL)
    {
        bst_highlight = root->data;
        bst_add_traversal(root->data);
        sprintf(locals, "root = [%3d]", root->data);
        sprintf(status, "Preorder: visit %d first.", root->data);
        show_frame(MODE_BST, "BST -> preorder()", "void bst_preorder(Node *root)",
                   "main() -> bst_preorder()", locals, preorder_code, total, 4, status);

        sprintf(status, "Now go to the left child of %d.", root->data);
        show_frame(MODE_BST, "BST -> preorder()", "void bst_preorder(Node *root)",
                   "main() -> bst_preorder()", locals, preorder_code, total, 5, status);

        bst_preorder(root->left);

        bst_highlight = root->data;
        sprintf(status, "Now go to the right child of %d.", root->data);
        show_frame(MODE_BST, "BST -> preorder()", "void bst_preorder(Node *root)",
                   "main() -> bst_preorder()", locals, preorder_code, total, 6, status);

        bst_preorder(root->right);
    }
}

char *postorder_code[] = {
    "void bst_postorder(Node *root)",
    "{",
    "    if (root != NULL)",
    "    {",
    "        bst_postorder(root->left);",
    "        bst_postorder(root->right);",
    "        printf(\"%d \", root->data);",
    "    }",
    "}"
};

void bst_postorder(Node *root)
{
    char locals[160];
    char status[160];
    int total = sizeof(postorder_code) / sizeof(postorder_code[0]);

    if (root != NULL)
    {
        bst_highlight = root->data;
        sprintf(locals, "root = [%3d]", root->data);
        sprintf(status, "Postorder: go to the left child of %d first.", root->data);
        show_frame(MODE_BST, "BST -> postorder()", "void bst_postorder(Node *root)",
                   "main() -> bst_postorder()", locals, postorder_code, total, 4, status);

        bst_postorder(root->left);

        bst_highlight = root->data;
        sprintf(status, "Now go to the right child of %d.", root->data);
        show_frame(MODE_BST, "BST -> postorder()", "void bst_postorder(Node *root)",
                   "main() -> bst_postorder()", locals, postorder_code, total, 5, status);

        bst_postorder(root->right);

        bst_highlight = root->data;
        bst_add_traversal(root->data);
        sprintf(status, "Visit %d last and add it to the output.", root->data);
        show_frame(MODE_BST, "BST -> postorder()", "void bst_postorder(Node *root)",
                   "main() -> bst_postorder()", locals, postorder_code, total, 6, status);
    }
}

/* ---------------------------------------------------------------- */
/*                     counting and clearing                        */
/* ---------------------------------------------------------------- */

int bst_count(Node *root)
{
    if (root == NULL)
        return 0;

    return 1 + bst_count(root->left) + bst_count(root->right);
}

int bst_leaf_count(Node *root)
{
    if (root == NULL)
        return 0;

    if (root->left == NULL && root->right == NULL)
        return 1;

    return bst_leaf_count(root->left) + bst_leaf_count(root->right);
}

int bst_interior_count(Node *root)
{
    if (root == NULL)
        return 0;

    if (root->left != NULL || root->right != NULL)
        return 1 + bst_interior_count(root->left) + bst_interior_count(root->right);

    return 0;
}

void bst_free(Node *root)
{
    if (root == NULL)
        return;

    bst_free(root->left);
    bst_free(root->right);
    free(root);
}

/* ---------------------------------------------------------------- */
/*                           menu                                   */
/* ---------------------------------------------------------------- */

void bst_menu(void)
{
    int ch, key;

    while (1)
    {
        clear_screen();
        printf("==============================================================\n");
        printf("                    BINARY SEARCH TREE\n");
        printf("==============================================================\n");
        show_bst();
        printf("\n--------------------------------------------------------------\n");
        printf("1. Insert\n");
        printf("2. Search\n");
        printf("3. Delete\n");
        printf("4. Inorder traversal\n");
        printf("5. Preorder traversal\n");
        printf("6. Postorder traversal\n");
        printf("7. Tree statistics\n");
        printf("8. Clear tree\n");
        printf("9. Return to main menu\n");
        printf("--------------------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        bst_traversal_count = 0;
        bst_highlight = -1;
        step_mode = 0;

        switch (ch)
        {
        case 1:
            printf("Enter data: ");
            scanf("%d", &key);
            bst_insert(key);
            break;

        case 2:
            printf("Enter key to search: ");
            scanf("%d", &key);
            bst_search(key);
            break;

        case 3:
            printf("Enter key to delete: ");
            scanf("%d", &key);
            bst_delete(key);
            break;

        case 4:
            if (bst_root == NULL)
            {
                printf("\nTree is empty.\n");
                wait_key();
                break;
            }
            bst_inorder(bst_root);
            break;

        case 5:
            if (bst_root == NULL)
            {
                printf("\nTree is empty.\n");
                wait_key();
                break;
            }
            bst_preorder(bst_root);
            break;

        case 6:
            if (bst_root == NULL)
            {
                printf("\nTree is empty.\n");
                wait_key();
                break;
            }
            bst_postorder(bst_root);
            break;

        case 7:
            printf("\nTotal nodes   : %d\n", bst_count(bst_root));
            printf("Leaf nodes    : %d\n", bst_leaf_count(bst_root));
            printf("Interior nodes: %d\n", bst_interior_count(bst_root));
            wait_key();
            break;

        case 8:
            bst_free(bst_root);
            bst_root = NULL;
            printf("\nTree cleared.\n");
            wait_key();
            break;

        case 9:
            return;

        default:
            printf("\nInvalid choice\n");
            wait_key();
        }
    }
}

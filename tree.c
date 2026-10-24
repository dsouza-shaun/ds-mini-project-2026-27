// Name: Shaun Dsouza
// Roll no.: 11077
// Class: SE CE-B
// Batch: A

#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    struct Node *left;
    int data;
    struct Node *right;
} Node;

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
    if (root == NULL)
    {
        root = newnode;
    }
    else
    {
        Node *p, *q;
        p = q = root;
        while (p != NULL)
        {
            if (key < p->data)
            {
                q = p;
                p = p->left;
            }
            else
            {
                q = p;
                p = p->right;
            }
        }
        if (key > q->data)
        {
            q->right = newnode;
        }
        else
        {
            q->left = newnode;
        }
    }
    return root;
}

void inorder(Node *root)
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

void preorder(Node *root)
{
    if (root != NULL)
    {
        printf("%d ", root->data);
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
    }
}

int search(Node *root, int key)
{
    if (root == NULL)
    {
        printf("Tree is empty\n");
        return 0;
    }
    else
    {
        Node *p = root;
        while (p != NULL)
        {
            if (p->data == key)
            {
                return 1;
            }
            if (key > p->data)
            {
                p = p->right;
            }
            else
            {
                p = p->left;
            }
        }
        return 0;
    }
}

Node *delete(Node *root, int key)
{
    Node *parent, *p;
    parent = p = root;
    int flag = 0;

    while (p != NULL)
    {
        if (key == p->data)
        {
            flag = 1;
            break;
        }
        else if (key < p->data)
        {
            parent = p;
            p = p->left;
        }
        else
        {
            parent = p;
            p = p->right;
        }
    }

    if (flag == 1)
    {
        if (p->left == NULL && p->right == NULL)
        {
            if (p == parent->left)
                parent->left = NULL;
            else
                parent->right = NULL;
            free(p);
        }
        else if (p->left != NULL && p->right == NULL)
        {
            if (p == parent->left)
                parent->left = p->left;
            else
                parent->right = p->left;
            free(p);
        }
        else if (p->right != NULL && p->left == NULL)
        {
            if (p == parent->left)
                parent->left = p->right;
            else
                parent->right = p->right;
            free(p);
        }
        else
        {
            Node *tmp = p->right;
            while (tmp->left != NULL)
            {
                tmp = tmp->left;
            }
            int x = tmp->data;
            root = delete(root, x);
            p->data = x;
        }
    }
    else
    {
        printf("Element %d not found in the tree.\n", key);
    }

    return root;
}

int count_nodes(Node *root)
{
    if (root == NULL)
    {
        return 0;
    }
    return 1 + count_nodes(root->left) + count_nodes(root->right);
}

int leaf_nodes(Node *root)
{
    if (root == NULL)
    {
        return 0;
    }
    if (root->left == NULL && root->right == NULL)
    {
        return 1;
    }
    return leaf_nodes(root->left) + leaf_nodes(root->right);
}

int interior_nodes(Node *root)
{
    if (root == NULL)
    {
        return 0;
    }
    if (root->left != NULL || root->right != NULL)
    {
        return 1 + interior_nodes(root->left) + interior_nodes(root->right);
    }
    return 0;
}

int main()
{
    Node *root = NULL;
    int ch, key;
    do
    {
        printf("\nMenu");
        printf("\n1. Insert");
        printf("\n2. Delete");
        printf("\n3. Traverse");
        printf("\n4. Search");
        printf("\n5. Count nodes");
        printf("\n6. Leaf nodes");
        printf("\n7. Interior nodes");
        printf("\n8. Exit");

        printf("\nEnter your choice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            printf("\nEnter data: ");
            scanf("%d", &key);
            root = insert(root, key);
            break;
        case 2:
            printf("\nEnter element to delete: ");
            scanf("%d", &key);
            root = delete(root, key);
            printf("\n");
            inorder(root);
            break;
        case 3:
            printf("\nInorder traversal:\n");
            inorder(root);
            printf("\n");

            printf("\nPreorder traversal:\n");
            preorder(root);
            printf("\n");

            printf("\nPostorder traversal:\n");
            postorder(root);
            printf("\n");
            break;
        case 4:
            printf("\nEnter element to search: ");
            scanf("%d", &key);
            if (search(root, key))
            {
                printf("\nElement found\n");
            }
            else
            {
                printf("\nElement not found\n");
            }
            break;
        case 5:
            printf("Number of nodes: %d\n", count_nodes(root));
            break;
        case 6:
            printf("Number of leaf nodes: %d\n", leaf_nodes(root));
            break;
        case 7:
            printf("Number of interior nodes: %d\n", interior_nodes(root));
            break;
        case 8:
            exit(0);
            break;
        default:
            printf("\nInvalid choice\n");
        }
    } while (ch != 8);

    return 0;
}
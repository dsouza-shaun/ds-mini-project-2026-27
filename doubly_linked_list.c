// Name: Shaun Dsouza
// Roll no.: 11077
// Class: SE CE-B
// Batch: A

#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *prev;
    struct Node *next;
} Node;

Node* create_node()
{
    Node *newnode = (Node*) malloc(sizeof(Node));
    printf("Enter data: ");
    scanf("%d", &newnode->data);
    newnode->prev = NULL;
    newnode->next = NULL;
    return newnode;
}

Node* insert_end(Node *head)
{
    Node* newnode;
    if(head == NULL) {
        newnode = create_node();
        head = newnode;
    } else {
        Node* tmp = head;
        while(tmp->next != NULL) {
            tmp = tmp->next;
        }
        newnode = create_node();
        newnode->prev = tmp;
        tmp->next = newnode;
    }

    return head;
}

Node* insert_begin(Node *head)
{
    Node* newnode;
    if(head == NULL) {
        newnode = create_node();
        head = newnode;
    } else {
        newnode = create_node();
        newnode->next = head;
        head->prev = newnode;
        head = newnode;
    }

    return head;
}

Node* insert_pos(Node *head, int pos)
{
    Node* newnode;
    if(pos <= 0) {
        return head;
    }
    if(pos == 1) {
        return insert_begin(head);
    }
    if(head == NULL) {
        return head;
    }
    Node* tmp = head;
    int i = 1;
    while(i < pos - 1 && tmp->next != NULL) {
        tmp = tmp->next;
        i++;
    }
    if(i != pos - 1) {
        return head;
    }
    newnode = create_node();
    newnode->prev = tmp;
    newnode->next = tmp->next;
    if(tmp->next != NULL) {
        tmp->next->prev = newnode;
    }
    tmp->next = newnode;

    return head;
}

Node* delete_first(Node *head)
{
    if (head != NULL)
    {
        Node* tmp = head;
        if (head->next == NULL)
        {
            head = NULL;
        }
        else
        {
            head = head->next;
            head->prev = NULL;
        }

        free(tmp);
    }

    return head;
}

Node* delete_last(Node *head)
{
    if (head != NULL)
    {
        Node* tmp = head;
        while(tmp->next != NULL) {
            tmp = tmp->next;
        }
        if(tmp->prev == NULL) {
            head = NULL;
        } else {
            tmp->prev->next = NULL;
        }
        free(tmp);
    }

    return head;
}

Node* delete_pos(Node *head, int pos)
{
    if (head != NULL && pos > 0)
    {
        if(pos == 1) {
            return delete_first(head);
        }
        Node* tmp = head;
        int i = 1;
        while(i < pos && tmp != NULL) {
            tmp = tmp->next;
            i++;
        }
        if(tmp == NULL) {
            return head;
        }
        if(tmp->next != NULL) {
            tmp->next->prev = tmp->prev;
        }
        if(tmp->prev != NULL) {
            tmp->prev->next = tmp->next;
        }
        free(tmp);
    }

    return head;
}

void traverse_forward(Node *head)
{
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }
    Node* tmp = head;
    while (tmp != NULL)
    {
        printf("%d -> ", tmp->data);
        tmp = tmp->next;
    }
    printf("NULL\n");
}

void traverse_backward(Node *head)
{
    if (head == NULL) {
        printf("List is empty\n");
        return;
    }
    Node* tmp = head;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    while (tmp != NULL)
    {
        printf("%d -> ", tmp->data);
        tmp = tmp->prev;
    }
    printf("NULL\n");
}

int main()
{
    Node *head = NULL;
    int ch, pos;

    while(1)
    {
        printf("\nDOUBLY LINKED LIST\n");
        printf("1. Insert Beginning\n");
        printf("2. Insert End\n");
        printf("3. Insert at Position\n");
        printf("4. Delete Beginning\n");
        printf("5. Delete End\n");
        printf("6. Delete at Position\n");
        printf("7. Traverse Forward\n");
        printf("8. Traverse Backward\n");
        printf("9. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &ch);

        switch(ch)
        {
            case 1:
                head = insert_begin(head);
                break;
            case 2:
                head = insert_end(head);
                break;
            case 3:
                printf("Enter position: ");
                scanf("%d", &pos);
                head = insert_pos(head, pos);
                break;
            case 4:
                head = delete_first(head);
                break;
            case 5:
                head = delete_last(head);
                break;
            case 6:
                printf("Enter position: ");
                scanf("%d", &pos);
                head = delete_pos(head, pos);
                break;
            case 7:
                traverse_forward(head);
                break;
            case 8:
                traverse_backward(head);
                break;
            case 9:
                return 0;
            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}

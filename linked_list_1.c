// Name: Shaun Dsouza
// Roll no.: 11077
// Class: SE CE-B
// Batch: A

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* create_node() {
    Node* newnode;
    newnode = (Node*) malloc(sizeof(Node));
    printf("Enter data: ");
    scanf("%d", &newnode->data);
    newnode->next = NULL;
    return newnode;
}

void display(Node* head) {
    Node* tmp = head;
    if (head == NULL) {
        printf("List is empty\n");
    }
    while (tmp != NULL) {
        printf("%d -> ", tmp->data);
        tmp = tmp->next;
    }
    printf("NULL\n");
}

int count(Node* head) {
    int c = 0;
    Node* tmp = head;
    while (tmp != NULL) {
        c++;
        tmp = tmp->next;
    }
    return c;
}

Node* insert_front(Node* head) {
    Node* newnode;
    if (head == NULL) {
        newnode = create_node();
        head = newnode;
    } else {
        newnode = create_node();
        newnode->next = head;
        head = newnode;
    }
    return head;
}

Node* insert_end(Node* head) {
    Node* tmp;
    if (head == NULL) {
        head = create_node();
    } else {
        tmp = head;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = create_node();
    }
    return head;
}

Node* insert_pos(Node* head, int pos) {
    Node *newnode, *tmp;
    int i = 1;
    if (head != NULL) {
        newnode = create_node();
        tmp = head;
        while (i < pos) {
            tmp = tmp->next;
            i++;
        }
        newnode->next = tmp->next;
        tmp->next = newnode;
    } else {
        head = create_node();
    }
    return head;
}

Node* delete_front(Node* head) {
    Node* tmp;
    if (head == NULL) {
        printf("List is empty\n");
    } else {
        tmp = head;
        head = tmp->next;
        tmp->next = NULL;
        free(tmp);
    }
    return head;
}

Node* delete_last(Node* head) {
    Node *temp, *temp1;
    temp = head;
    temp1 = temp;
    if (head == NULL) {
        printf("List is empty\n");
    } else if (head->next == NULL) {
        free(head);
        head = NULL;
    } else {
        while (temp1->next != NULL) {
            temp = temp1;
            temp1 = temp1->next;
        }
        temp->next = NULL;
        free(temp1);
    }
    return head;
}

Node* delete_pos(int pos, Node* head) {
    int i = 0;
    Node *temp, *temp1;
    temp = head;
    temp1 = temp;
    if (head != NULL) {
        if (pos == 0) {
            head = head->next;
            free(temp);
        } else {
            while (i != pos) {
                temp1 = temp;
                temp = temp->next;
                i++;
            }
            temp1->next = temp->next;
            free(temp);
        }
    }
    return head;
}

int main() {
    Node* head = NULL;
    int ch, pos;
    while (1) {
        printf("\nMenu\n");
        printf("1. Insert at begin\n2. Insert at end\n3. Insert at specific position\n4. Delete end\n5. Delete begin\n6. Delete at specific position\n7. Traverse\n8. Count\n9. Exit\n");
        printf("Enter your input: ");
        scanf("%d", &ch);
        switch (ch) {
            case 1:
                head = insert_front(head);
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
                head = delete_last(head);
                break;
            case 5:
                head = delete_front(head);
                break;
            case 6:
                printf("Enter position: ");
                scanf("%d", &pos);
                head = delete_pos(pos, head);
                break;
            case 7:
                display(head);
                break;
            case 8:
                printf("Number of nodes: %d\n", count(head));
                break;
            case 9:
                exit(0);
        }
    }
    return 0;
}

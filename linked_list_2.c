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
        return;
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

Node* create_ll() {
    int n;
    printf("Enter number of nodes: ");
    scanf("%d", &n);
    Node* head;
    head = create_node();
    int i = 1;
    Node* tmp = head;
    while (i < n) {
        tmp->next = create_node();
        tmp = tmp->next;
        i++;
    }
    return head;
}

void copy(Node* head) {
    if (head != NULL) {
        Node* head1 = (Node*) malloc(sizeof(Node));
        Node* tmp = head;
        Node* tmp1 = head1;
        while (tmp->next != NULL) {
            tmp1->data = tmp->data;
            tmp1->next = (Node*) malloc(sizeof(Node));
            tmp = tmp->next;
            tmp1 = tmp1->next;
        }
        tmp1->data = tmp->data;
        tmp1->next = NULL;
        printf("Original Linked List: \n");
        display(head);
        printf("Replicated Linked List: \n");
        display(head1);
    }
}

Node* concatenate(Node* head1, Node* head2) {
    if (head1 == NULL) return head2;
    if (head2 == NULL) return head1;
    Node* p = head1;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = head2;
    return head1;
}

void split(Node* head) {
    Node* slow, *fast;
    slow = fast = head;
    while (fast->next != NULL && fast->next->next != NULL) {
        fast = fast->next->next;
        slow = slow->next;
    }
    Node* head1 = slow->next;
    slow->next = NULL;
    printf("First part: ");
    display(head);
    printf("Second part: ");
    display(head1);
}

Node* reverse(Node* head) {
    Node* prev, *curr, *next_n;
    if (head != NULL) {
        prev = NULL;
        curr = head;
        next_n = curr->next;
        while (curr != NULL) {
            curr->next = prev;
            prev = curr;
            curr = next_n;
            if (next_n != NULL)
                next_n = next_n->next;
        }
    }
    return prev;
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

int main() {
    Node* head = NULL;
    Node* head2 = NULL;
    int ch;
    while (1) {
        printf("\nMenu\n");
        printf("1. Create linked list\n2. Display\n3. Count nodes\n4. Copy\n5. Concatenate\n6. Split\n7. Reverse\n8. Delete last\n9. Exit\n");
        printf("Enter your input: ");
        scanf("%d", &ch);
        switch (ch) {
            case 1:
                head = create_ll();
                break;
            case 2:
                display(head);
                break;
            case 3:
                printf("Number of nodes: %d\n", count(head));
                break;
            case 4:
                copy(head);
                break;
            case 5:
                printf("Create second list:\n");
                head2 = create_ll();
                head = concatenate(head, head2);
                display(head);
                break;
            case 6:
                split(head);
                break;
            case 7:
                head = reverse(head);
                display(head);
                break;
            case 8:
                head = delete_last(head);
                break;
            case 9:
                exit(0);
        }
    }
    return 0;
}

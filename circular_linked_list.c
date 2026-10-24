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
    do {
        printf("%d -> ", tmp->data);
        tmp = tmp->next;
    } while (tmp != head);
    printf("HEAD\n");
}

int count(Node* head) {
    int c = 0;
    Node* tmp = head;
    if (head == NULL) {
        return 0;
    }
    do {
        c++;
        tmp = tmp->next;
    } while (tmp != head);
    return c;
}

Node* insert_front(Node* head) {
    Node *newnode, *tmp;
    newnode = create_node();
    if (head == NULL) {
        head = newnode;
        newnode->next = head;
    } else {
        tmp = head;
        while (tmp->next != head) {
            tmp = tmp->next;
        }
        newnode->next = head;
        tmp->next = newnode;
        head = newnode;
    }
    return head;
}

Node* insert_end(Node* head) {
    Node *newnode, *tmp;
    newnode = create_node();
    if (head == NULL) {
        head = newnode;
        newnode->next = head;
    } else {
        tmp = head;
        while (tmp->next != head) {
            tmp = tmp->next;
        }
        tmp->next = newnode;
        newnode->next = head;
    }
    return head;
}

Node* insert_pos(Node* head, int pos) {
    Node *newnode, *tmp;
    int i = 0;
    if (head == NULL) {
        if (pos == 0) {
            head = create_node();
            head->next = head;
        }
    } else if (pos == 0) {
        head = insert_front(head);
    } else {
        tmp = head;
        newnode = create_node();
        while (i < pos - 1 && tmp->next != head) {
            tmp = tmp->next;
            i++;
        }
        if (i == pos - 1) {
            newnode->next = tmp->next;
            tmp->next = newnode;
        } else {
            free(newnode);
        }
    }
    return head;
}

Node* delete_front(Node* head) {
    Node *tmp, *last;
    if (head == NULL) {
        printf("List is empty\n");
    } else if (head->next == head) {
        free(head);
        head = NULL;
    } else {
        tmp = head;
        last = head;
        while (last->next != head) {
            last = last->next;
        }
        head = head->next;
        last->next = head;
        free(tmp);
    }
    return head;
}

Node* delete_last(Node* head) {
    Node *temp, *temp1;
    if (head == NULL) {
        printf("List is empty\n");
    } else if (head->next == head) {
        free(head);
        head = NULL;
    } else {
        temp = head;
        while (temp->next->next != head) {
            temp = temp->next;
        }
        temp1 = temp->next;
        temp->next = head;
        free(temp1);
    }
    return head;
}

Node* delete_pos(int pos, Node* head) {
    int i = 0;
    Node *temp, *temp1;

    if (head == NULL) {
        printf("List is empty\n");
    } else if (pos == 0) {
        head = delete_front(head);
    } else {
        temp = head;
        while (i < pos - 1 && temp->next != head) {
            temp = temp->next;
            i++;
        }
        if (i == pos - 1 && temp->next != head) {
            temp1 = temp->next;
            temp->next = temp1->next;
            free(temp1);
        }
    }
    return head;
}

Node* remove_occurrences(Node* head, int x) {
    Node* temp= head;
    Node* temp1= head;
    while(temp->next != head) {
        if(x==temp->data){
            temp1->next = temp-> next;
            temp->next = NULL;
            free(temp);
            temp=temp->next;
        }
        temp1=temp;
        temp=temp->next;
    }
}

int main() {
    Node* head = NULL;
    int ch, pos, n;

    while (1) {
        printf("\nMenu\n");
        printf("1. Insert at begin\n2. Insert at end\n3. Insert at specific position\n4. Delete end\n5. Delete begin\n6. Delete at specific position\n7. Traverse\n8. Remove occurences\n9. Count\n10. Exit\n");
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
                printf("Enter x: \n");
                scanf("%d", &n);
                head = remove_occurrences(head, n);
                break;
            case 9:
                printf("Number of nodes: %d\n", count(head));
                break;
            case 10:
                exit(0);
        }
    }
    return 0;
}

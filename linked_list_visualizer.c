#include "visualizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

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


void singly_linked_list_menu() {
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

typedef struct DNode {
    int data;
    struct DNode *prev;
    struct DNode *next;
} DNode;

static DNode *doubly_head = NULL;

static void doubly_display(void) {
    DNode *tmp = doubly_head;
    if (tmp == NULL) {
        printf(" head -> NULL  (List is empty. Use Insert to add nodes!)\n");
        return;
    }
    printf(" head -> ");
    while (tmp != NULL) {
        printf("[%d]", tmp->data);
        if (tmp->next != NULL) printf(" <-> ");
        if (tmp->next == NULL) break;
        tmp = tmp->next;
    }
    printf(" -> NULL\n");
}

static DNode *doubly_new_node(int data) {
    DNode *node = (DNode *)malloc(sizeof(DNode));
    if (node == NULL) {
        printf("Could not allocate memory for a new node.\n");
        return NULL;
    }
    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

static void doubly_insert_begin(int data) {
    DNode *node = doubly_new_node(data);
    if (node == NULL) return;
    node->next = doubly_head;
    if (doubly_head != NULL) doubly_head->prev = node;
    doubly_head = node;
}

static void doubly_insert_end(int data) {
    DNode *node = doubly_new_node(data);
    DNode *tmp = doubly_head;
    if (node == NULL) return;
    if (doubly_head == NULL) {
        doubly_head = node;
        return;
    }
    while (tmp->next != NULL) tmp = tmp->next;
    tmp->next = node;
    node->prev = tmp;
}

static void doubly_insert_position(int data, int position) {
    int i;
    DNode *tmp = doubly_head;
    DNode *node;
    if (position < 1) { printf("Position must start at 1.\n"); return; }
    if (position == 1) { doubly_insert_begin(data); return; }
    for (i = 1; tmp != NULL && i < position - 1; i++) tmp = tmp->next;
    if (tmp == NULL) { printf("Position is outside the list.\n"); return; }
    node = doubly_new_node(data);
    if (node == NULL) return;
    node->next = tmp->next;
    node->prev = tmp;
    if (tmp->next != NULL) tmp->next->prev = node;
    tmp->next = node;
}

static void doubly_delete_position(int position) {
    int i;
    DNode *tmp = doubly_head;
    if (position < 1) { printf("Position must start at 1.\n"); return; }
    for (i = 1; tmp != NULL && i < position; i++) tmp = tmp->next;
    if (tmp == NULL) { printf("Position is outside the list.\n"); return; }
    if (tmp->prev != NULL) tmp->prev->next = tmp->next;
    else doubly_head = tmp->next;
    if (tmp->next != NULL) tmp->next->prev = tmp->prev;
    free(tmp);
}

static void doubly_display_backward(void) {
    DNode *tmp = doubly_head;
    if (tmp == NULL) { printf("Doubly linked list is empty.\n"); return; }
    while (tmp->next != NULL) tmp = tmp->next;
    printf(" tail -> ");
    while (tmp != NULL) {
        printf("[%d]", tmp->data);
        if (tmp->prev != NULL) printf(" <-> ");
        if (tmp->prev == NULL) break;
        tmp = tmp->prev;
    }
    printf(" -> NULL\n");
}

static void doubly_clear(void) {
    while (doubly_head != NULL) {
        DNode *tmp = doubly_head;
        doubly_head = doubly_head->next;
        free(tmp);
    }
}

void doubly_linked_list_menu(void) {
    int ch, data, position;
    while (1) {
        clear_screen();
        printf("\033[1;37;44m ============================================================================== \033[0m\n");
        printf("\033[1;37;44m                         DOUBLY LINKED LIST VISUALIZER                         \033[0m\n");
        printf("\033[1;37;44m ============================================================================== \033[0m\n\n");
        doubly_display();
        printf("\n--------------------------------------------------------------------------------\n");
        printf("1. Insert Beginning\n2. Insert End\n3. Insert at Position\n");
        printf("4. Delete Beginning\n5. Delete End\n6. Delete at Position\n");
        printf("7. Traverse Forward\n8. Traverse Backward\n9. Clear List\n10. Return\n");
        printf("--------------------------------------------------------------------------------\nChoice: ");
        if (scanf("%d", &ch) != 1) {
            while (getchar() != '\n');
            continue;
        }
        if (ch >= 1 && ch <= 3) {
            printf("Enter data: ");
            if (scanf("%d", &data) != 1) { while (getchar() != '\n'); continue; }
            if (ch == 1) doubly_insert_begin(data);
            else if (ch == 2) doubly_insert_end(data);
            else {
                printf("Enter position (starting at 1): ");
                if (scanf("%d", &position) != 1) { while (getchar() != '\n'); continue; }
                doubly_insert_position(data, position);
            }
        } else if (ch >= 4 && ch <= 6) {
            if (ch == 4) doubly_delete_position(1);
            else if (ch == 5) {
                DNode *tmp = doubly_head;
                if (tmp != NULL) {
                    int count = 1;
                    while (tmp->next != NULL) { tmp = tmp->next; count++; }
                    doubly_delete_position(count);
                } else printf("Doubly linked list is empty.\n");
            } else {
                printf("Enter position (starting at 1): ");
                if (scanf("%d", &position) != 1) { while (getchar() != '\n'); continue; }
                doubly_delete_position(position);
            }
        } else if (ch == 7) doubly_display();
        else if (ch == 8) doubly_display_backward();
        else if (ch == 9) doubly_clear();
        else if (ch == 10) return;
        else printf("Invalid choice.\n");
        if (ch != 10) { printf("Press any key to continue..."); wait_key(); }
    }
}

void ll_menu(void) {
    int ch;
    while (1) {
        clear_screen();
        printf("LINKED LISTS\n\n1. Singly Linked List\n2. Doubly Linked List\n3. Return\nChoice: ");
        if (scanf("%d", &ch) != 1) {
            while (getchar() != '\n');
            continue;
        }
        if (ch == 1) singly_linked_list_menu();
        else if (ch == 2) doubly_linked_list_menu();
        else if (ch == 3) return;
        else { printf("Invalid choice.\n"); Sleep(500); }
    }
}


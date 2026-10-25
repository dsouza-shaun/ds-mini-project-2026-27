#include "visualizer.h"

LLNode *sll_head = NULL;
LLNode *sll_highlight = NULL;

LLNode *sll_new_node(int data)
{
    LLNode *newnode;

    newnode = (LLNode *)malloc(sizeof(LLNode));
    newnode->data = data;
    newnode->next = NULL;

    return newnode;
}

void show_sll(void)
{
    LLNode *tmp;

    printf("SINGLY LINKED LIST:\n");
    printf("head -> ");

    tmp = sll_head;

    if (tmp == NULL)
    {
        printf("NULL\n");
        return;
    }

    while (tmp != NULL)
    {
        if (tmp == sll_highlight)
            printf("*[%d]* -> ", tmp->data);
        else
            printf("[%d] -> ", tmp->data);

        tmp = tmp->next;
    }

    printf("NULL\n");
}

/* ---------------------------------------------------------------- */
/*                      insert at front                             */
/* ---------------------------------------------------------------- */

char *sll_front_code[] = {
    "LLNode *sll_insert_front(int data)",
    "{",
    "    LLNode *newnode = sll_new_node(data);",
    "",
    "    if (sll_head == NULL)",
    "    {",
    "        sll_head = newnode;",
    "    }",
    "    else",
    "    {",
    "        newnode->next = sll_head;",
    "        sll_head = newnode;",
    "    }",
    "",
    "    return sll_head;",
    "}"
};

LLNode *sll_insert_front(int data)
{
    LLNode *newnode;
    char locals[160];
    char status[160];
    int total = sizeof(sll_front_code) / sizeof(sll_front_code[0]);

    newnode = sll_new_node(data);

    sprintf(locals, "data = %d | newnode->next = NULL", data);
    sprintf(status, "Create a new node with data %d.", data);
    show_frame(MODE_SLL, "Linked List -> insert_front()", "LLNode *sll_insert_front(int data)",
               "main() -> sll_insert_front()", locals, sll_front_code, total, 2, status);

    if (sll_head == NULL)
    {
        sll_head = newnode;
        sll_highlight = newnode;

        sprintf(status, "List was empty. The new node becomes the head.");
        show_frame(MODE_SLL, "Linked List -> insert_front()", "LLNode *sll_insert_front(int data)",
                   "main() -> sll_insert_front()", locals, sll_front_code, total, 6, status);
    }
    else
    {
        newnode->next = sll_head;
        sprintf(status, "Point newnode->next to the old head [%d].", sll_head->data);
        show_frame(MODE_SLL, "Linked List -> insert_front()", "LLNode *sll_insert_front(int data)",
                   "main() -> sll_insert_front()", locals, sll_front_code, total, 10, status);

        sll_head = newnode;
        sll_highlight = newnode;
        sprintf(status, "Move head to the new node [%d].", data);
        show_frame(MODE_SLL, "Linked List -> insert_front()", "LLNode *sll_insert_front(int data)",
                   "main() -> sll_insert_front()", locals, sll_front_code, total, 11, status);
    }

    sll_highlight = NULL;
    return sll_head;
}

/* ---------------------------------------------------------------- */
/*                       insert at end                              */
/* ---------------------------------------------------------------- */

char *sll_end_code[] = {
    "LLNode *sll_insert_end(int data)",
    "{",
    "    LLNode *newnode = sll_new_node(data);",
    "    LLNode *tmp;",
    "",
    "    if (sll_head == NULL)",
    "    {",
    "        sll_head = newnode;",
    "    }",
    "    else",
    "    {",
    "        tmp = sll_head;",
    "        while (tmp->next != NULL)",
    "        {",
    "            tmp = tmp->next;",
    "        }",
    "        tmp->next = newnode;",
    "    }",
    "",
    "    return sll_head;",
    "}"
};

LLNode *sll_insert_end(int data)
{
    LLNode *newnode, *tmp;
    char locals[160];
    char status[160];
    int total = sizeof(sll_end_code) / sizeof(sll_end_code[0]);

    newnode = sll_new_node(data);

    sprintf(locals, "data = %d | newnode->next = NULL", data);
    sprintf(status, "Create a new node with data %d.", data);
    show_frame(MODE_SLL, "Linked List -> insert_end()", "LLNode *sll_insert_end(int data)",
               "main() -> sll_insert_end()", locals, sll_end_code, total, 2, status);

    if (sll_head == NULL)
    {
        sll_head = newnode;
        sll_highlight = newnode;

        sprintf(status, "List was empty. The new node becomes the head.");
        show_frame(MODE_SLL, "Linked List -> insert_end()", "LLNode *sll_insert_end(int data)",
                   "main() -> sll_insert_end()", locals, sll_end_code, total, 7, status);
    }
    else
    {
        tmp = sll_head;
        sll_highlight = tmp;

        sprintf(locals, "tmp = [%d]", tmp->data);
        sprintf(status, "Start from the head and walk to the last node.");
        show_frame(MODE_SLL, "Linked List -> insert_end()", "LLNode *sll_insert_end(int data)",
                   "main() -> sll_insert_end()", locals, sll_end_code, total, 11, status);

        while (tmp->next != NULL)
        {
            tmp = tmp->next;
            sll_highlight = tmp;

            sprintf(locals, "tmp = [%d]", tmp->data);
            sprintf(status, "Move tmp to the next node [%d].", tmp->data);
            show_frame(MODE_SLL, "Linked List -> insert_end()", "LLNode *sll_insert_end(int data)",
                       "main() -> sll_insert_end()", locals, sll_end_code, total, 14, status);
        }

        tmp->next = newnode;
        sll_highlight = newnode;

        sprintf(locals, "tmp = [%d] | newnode = [%d]", tmp->data, data);
        sprintf(status, "Attach the new node after %d.", tmp->data);
        show_frame(MODE_SLL, "Linked List -> insert_end()", "LLNode *sll_insert_end(int data)",
                   "main() -> sll_insert_end()", locals, sll_end_code, total, 16, status);
    }

    sll_highlight = NULL;
    return sll_head;
}

/* ---------------------------------------------------------------- */
/*                     delete from front                            */
/* ---------------------------------------------------------------- */

char *sll_delete_front_code[] = {
    "LLNode *sll_delete_front(void)",
    "{",
    "    LLNode *tmp;",
    "",
    "    if (sll_head == NULL)",
    "    {",
    "        printf(\"List is empty\\n\");",
    "        return NULL;",
    "    }",
    "",
    "    tmp = sll_head;",
    "    sll_head = tmp->next;",
    "    free(tmp);",
    "",
    "    return sll_head;",
    "}"
};

LLNode *sll_delete_front(void)
{
    LLNode *tmp;
    char locals[160];
    char status[160];
    int total = sizeof(sll_delete_front_code) / sizeof(sll_delete_front_code[0]);

    sprintf(locals, "sll_head = %s", sll_head == NULL ? "NULL" : "node");
    sprintf(status, "Enter delete_front.");
    show_frame(MODE_SLL, "Linked List -> delete_front()", "LLNode *sll_delete_front(void)",
               "main() -> sll_delete_front()", locals, sll_delete_front_code, total, 4, status);

    if (sll_head == NULL)
    {
        sprintf(status, "List is empty. Nothing to delete.");
        show_frame(MODE_SLL, "Linked List -> delete_front()", "LLNode *sll_delete_front(void)",
                   "main() -> sll_delete_front()", locals, sll_delete_front_code, total, 6, status);
        return NULL;
    }

    tmp = sll_head;
    sll_highlight = tmp;

    sprintf(locals, "tmp = [%d]", tmp->data);
    sprintf(status, "Save the head node [%d] in tmp.", tmp->data);
    show_frame(MODE_SLL, "Linked List -> delete_front()", "LLNode *sll_delete_front(void)",
               "main() -> sll_delete_front()", locals, sll_delete_front_code, total, 11, status);

    sll_head = tmp->next;
    sll_highlight = NULL;

    if (sll_head == NULL)
        sprintf(status, "head becomes NULL. The list is now empty.");
    else
        sprintf(status, "Move head to the next node [%d].", sll_head->data);

    show_frame(MODE_SLL, "Linked List -> delete_front()", "LLNode *sll_delete_front(void)",
               "main() -> sll_delete_front()", locals, sll_delete_front_code, total, 12, status);

    free(tmp);
    return sll_head;
}

/* ---------------------------------------------------------------- */
/*                  other singly list operations                    */
/* ---------------------------------------------------------------- */

int sll_count(void)
{
    int count = 0;
    LLNode *tmp = sll_head;

    while (tmp != NULL)
    {
        count++;
        tmp = tmp->next;
    }

    return count;
}

void sll_insert_pos(int data, int pos)
{
    LLNode *newnode, *tmp;
    int i;

    if (pos < 1 || pos > sll_count() + 1)
    {
        printf("Invalid position\n");
        return;
    }

    newnode = sll_new_node(data);

    if (pos == 1)
    {
        newnode->next = sll_head;
        sll_head = newnode;
        return;
    }

    tmp = sll_head;
    i = 1;

    while (i < pos - 1 && tmp->next != NULL)
    {
        tmp = tmp->next;
        i++;
    }

    newnode->next = tmp->next;
    tmp->next = newnode;
}

void sll_delete_end(void)
{
    LLNode *tmp, *prev;

    if (sll_head == NULL)
    {
        printf("List is empty\n");
        return;
    }

    if (sll_head->next == NULL)
    {
        free(sll_head);
        sll_head = NULL;
        return;
    }

    prev = sll_head;
    tmp = sll_head->next;

    while (tmp->next != NULL)
    {
        prev = tmp;
        tmp = tmp->next;
    }

    prev->next = NULL;
    free(tmp);
}

void sll_delete_pos(int pos)
{
    LLNode *tmp, *prev;
    int i;

    if (sll_head == NULL)
    {
        printf("List is empty\n");
        return;
    }

    if (pos < 1 || pos > sll_count())
    {
        printf("Invalid position\n");
        return;
    }

    if (pos == 1)
    {
        tmp = sll_head;
        sll_head = tmp->next;
        free(tmp);
        return;
    }

    prev = sll_head;

    for (i = 1; i < pos - 1; i++)
        prev = prev->next;

    tmp = prev->next;
    prev->next = tmp->next;
    free(tmp);
}

void sll_free(void)
{
    LLNode *tmp;

    while (sll_head != NULL)
    {
        tmp = sll_head;
        sll_head = sll_head->next;
        free(tmp);
    }
}

/* ---------------------------------------------------------------- */
/*                     singly list menu                             */
/* ---------------------------------------------------------------- */

void singly_menu(void)
{
    int ch, data, pos;

    while (1)
    {
        clear_screen();
        printf("==============================================================\n");
        printf("                    SINGLY LINKED LIST\n");
        printf("==============================================================\n");
        show_sll();
        printf("\n--------------------------------------------------------------\n");
        printf("1.  Insert at front\n");
        printf("2.  Insert at end\n");
        printf("3.  Insert at position\n");
        printf("4.  Delete from front\n");
        printf("5.  Delete from end\n");
        printf("6.  Delete at position\n");
        printf("7.  Display list\n");
        printf("8.  Count nodes\n");
        printf("9.  Clear list\n");
        printf("10. Return\n");
        printf("--------------------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        step_mode = 0;

        switch (ch)
        {
        case 1:
            printf("Enter data: ");
            scanf("%d", &data);
            sll_insert_front(data);
            break;

        case 2:
            printf("Enter data: ");
            scanf("%d", &data);
            sll_insert_end(data);
            break;

        case 3:
            printf("Enter data: ");
            scanf("%d", &data);
            printf("Enter position: ");
            scanf("%d", &pos);
            sll_insert_pos(data, pos);
            printf("\nInsert done.\n");
            wait_key();
            break;

        case 4:
            sll_delete_front();
            break;

        case 5:
            sll_delete_end();
            printf("\nDelete done.\n");
            wait_key();
            break;

        case 6:
            printf("Enter position: ");
            scanf("%d", &pos);
            sll_delete_pos(pos);
            printf("\nDelete done.\n");
            wait_key();
            break;

        case 7:
            show_sll();
            wait_key();
            break;

        case 8:
            printf("\nNumber of nodes: %d\n", sll_count());
            wait_key();
            break;

        case 9:
            sll_free();
            printf("\nList cleared.\n");
            wait_key();
            break;

        case 10:
            return;

        default:
            printf("\nInvalid choice\n");
            wait_key();
        }
    }
}

/* ---------------------------------------------------------------- */
/*                     doubly linked list                           */
/* ---------------------------------------------------------------- */

DNode *dll_head = NULL;

DNode *dll_new_node(int data)
{
    DNode *newnode;

    newnode = (DNode *)malloc(sizeof(DNode));
    newnode->data = data;
    newnode->prev = NULL;
    newnode->next = NULL;

    return newnode;
}

void show_dll(void)
{
    DNode *tmp;

    printf("DOUBLY LINKED LIST (forward):\n");
    printf("head -> ");

    tmp = dll_head;

    if (tmp == NULL)
    {
        printf("NULL\n");
        return;
    }

    while (tmp != NULL)
    {
        printf("[%d]", tmp->data);

        if (tmp->next != NULL)
            printf(" <-> ");

        tmp = tmp->next;
    }

    printf(" -> NULL\n");
}

void show_dll_backward(void)
{
    DNode *tmp;

    printf("DOUBLY LINKED LIST (backward):\n");
    printf("tail -> ");

    if (dll_head == NULL)
    {
        printf("NULL\n");
        return;
    }

    tmp = dll_head;
    while (tmp->next != NULL)
        tmp = tmp->next;

    while (tmp != NULL)
    {
        printf("[%d]", tmp->data);

        if (tmp->prev != NULL)
            printf(" <-> ");

        tmp = tmp->prev;
    }

    printf(" -> NULL\n");
}

void dll_insert_begin(int data)
{
    DNode *newnode;

    newnode = dll_new_node(data);
    newnode->next = dll_head;

    if (dll_head != NULL)
        dll_head->prev = newnode;

    dll_head = newnode;
}

void dll_insert_end(int data)
{
    DNode *newnode, *tmp;

    newnode = dll_new_node(data);

    if (dll_head == NULL)
    {
        dll_head = newnode;
        return;
    }

    tmp = dll_head;
    while (tmp->next != NULL)
        tmp = tmp->next;

    tmp->next = newnode;
    newnode->prev = tmp;
}

void dll_insert_pos(int data, int pos)
{
    DNode *newnode, *tmp;
    int i;

    if (pos < 1)
    {
        printf("Position must start at 1.\n");
        return;
    }

    if (pos == 1)
    {
        dll_insert_begin(data);
        return;
    }

    if (dll_head == NULL)
    {
        printf("List is empty. Insert at position 1 instead.\n");
        return;
    }

    tmp = dll_head;
    for (i = 1; i < pos - 1 && tmp != NULL; i++)
        tmp = tmp->next;

    if (tmp == NULL)
    {
        printf("Position is outside the list.\n");
        return;
    }

    newnode = dll_new_node(data);
    newnode->next = tmp->next;
    newnode->prev = tmp;

    if (tmp->next != NULL)
        tmp->next->prev = newnode;

    tmp->next = newnode;
}

void dll_delete_begin(void)
{
    DNode *tmp;

    if (dll_head == NULL)
    {
        printf("List is empty.\n");
        return;
    }

    tmp = dll_head;
    dll_head = dll_head->next;

    if (dll_head != NULL)
        dll_head->prev = NULL;

    free(tmp);
}

void dll_delete_end(void)
{
    DNode *tmp;

    if (dll_head == NULL)
    {
        printf("List is empty.\n");
        return;
    }

    tmp = dll_head;
    while (tmp->next != NULL)
        tmp = tmp->next;

    if (tmp->prev == NULL)
        dll_head = NULL;
    else
        tmp->prev->next = NULL;

    free(tmp);
}

void dll_delete_pos(int pos)
{
    DNode *tmp;
    int i;

    if (dll_head == NULL)
    {
        printf("List is empty.\n");
        return;
    }

    if (pos < 1)
    {
        printf("Position must start at 1.\n");
        return;
    }

    tmp = dll_head;
    for (i = 1; i < pos && tmp != NULL; i++)
        tmp = tmp->next;

    if (tmp == NULL)
    {
        printf("Position is outside the list.\n");
        return;
    }

    if (tmp->prev != NULL)
        tmp->prev->next = tmp->next;
    else
        dll_head = tmp->next;

    if (tmp->next != NULL)
        tmp->next->prev = tmp->prev;

    free(tmp);
}

void dll_free(void)
{
    DNode *tmp;

    while (dll_head != NULL)
    {
        tmp = dll_head;
        dll_head = dll_head->next;
        free(tmp);
    }
}

/* ---------------------------------------------------------------- */
/*                     doubly list menu                             */
/* ---------------------------------------------------------------- */

void doubly_menu(void)
{
    int ch, data, pos;

    while (1)
    {
        clear_screen();
        printf("==============================================================\n");
        printf("                    DOUBLY LINKED LIST\n");
        printf("==============================================================\n");
        show_dll();
        printf("\n--------------------------------------------------------------\n");
        printf("1.  Insert at beginning\n");
        printf("2.  Insert at end\n");
        printf("3.  Insert at position\n");
        printf("4.  Delete from beginning\n");
        printf("5.  Delete from end\n");
        printf("6.  Delete at position\n");
        printf("7.  Show forward\n");
        printf("8.  Show backward\n");
        printf("9.  Clear list\n");
        printf("10. Return\n");
        printf("--------------------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        switch (ch)
        {
        case 1:
            printf("Enter data: ");
            scanf("%d", &data);
            dll_insert_begin(data);
            printf("\nInsert done.\n");
            wait_key();
            break;

        case 2:
            printf("Enter data: ");
            scanf("%d", &data);
            dll_insert_end(data);
            printf("\nInsert done.\n");
            wait_key();
            break;

        case 3:
            printf("Enter data: ");
            scanf("%d", &data);
            printf("Enter position: ");
            scanf("%d", &pos);
            dll_insert_pos(data, pos);
            printf("\nInsert done.\n");
            wait_key();
            break;

        case 4:
            dll_delete_begin();
            printf("\nDelete done.\n");
            wait_key();
            break;

        case 5:
            dll_delete_end();
            printf("\nDelete done.\n");
            wait_key();
            break;

        case 6:
            printf("Enter position: ");
            scanf("%d", &pos);
            dll_delete_pos(pos);
            printf("\nDelete done.\n");
            wait_key();
            break;

        case 7:
            show_dll();
            wait_key();
            break;

        case 8:
            show_dll_backward();
            wait_key();
            break;

        case 9:
            dll_free();
            printf("\nList cleared.\n");
            wait_key();
            break;

        case 10:
            return;

        default:
            printf("\nInvalid choice\n");
            wait_key();
        }
    }
}

void ll_menu(void)
{
    int ch;

    while (1)
    {
        clear_screen();
        printf("==============================================================\n");
        printf("                      LINKED LISTS\n");
        printf("==============================================================\n");
        printf("1. Singly Linked List\n");
        printf("2. Doubly Linked List\n");
        printf("3. Return to main menu\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        if (ch == 1)
            singly_menu();
        else if (ch == 2)
            doubly_menu();
        else if (ch == 3)
            return;
        else
        {
            printf("\nInvalid choice\n");
            wait_key();
        }
    }
}

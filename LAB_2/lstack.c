#include "stack.h"

void new_lstack(LStack** out) {
    LStack* stack = (LStack*)malloc(sizeof(LStack));
    if (stack == NULL) {
        *out = NULL;
        return ;
    }
    stack -> size = 0;
    stack -> list = NULL;
    *out = stack;
}

static void delete_list(List* head) {
    List* tmp;
    while (head != NULL) {
        tmp = head;
        head = head -> next;
        free(tmp);
    }
}

static List* add_new_node(List* head, char data) {
    List* new = (List*)malloc(sizeof(List));
    if (new == NULL) {
        delete_list(head);
        return NULL;
    }
    new -> data = data;
    new -> next = head;
    return new;
}

int push_back_lstack(LStack* stack, char data) {
    stack -> list = add_new_node(stack -> list, data);
    if (stack -> list == NULL) {
        stack -> size = 0;
        return -1;
    }
    ++(stack -> size);
    return 0;
}

static List* remove_head(List* head) {
    List* next = head -> next;
    free(head);
    return next;
}

char pop_back_lstack(LStack* stack) {
    if (stack -> size == 0)
        return 0;
    char data = (stack -> list) -> data;
    stack -> list = remove_head(stack -> list);
    --(stack -> size);
    return data;
}

char check_back_lstack(LStack* stack) {
    if (stack -> size == 0)
        return 0;
    return ((stack -> list) -> data);
}

void delete_lstack(LStack** stack) {
    if (stack == NULL || *stack == NULL)
        return ;
    List* head = (*stack) -> list;
    List* tmp;
    while (head != NULL) {
        tmp = head;
        head = head -> next;
        free(tmp);
    }
    free(*stack);
    *stack = NULL;
}


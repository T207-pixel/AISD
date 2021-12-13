#ifndef STACK_H
#define STACK_H
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

// stack based on linked list
typedef struct _List {
    char data;
    struct _List* next;
} List;

typedef struct _LStack {
    size_t size;
    List* list;
} LStack;
// stack based on fixed size vector
typedef struct _VStack {
    size_t size;
    size_t max_size;
    char* vector;
} VStack;

void new_vstack(VStack** stack, size_t size);
int push_back_vstack(VStack* stack, char data);
char pop_back_vstack(VStack* stack);
char check_back_vstack(VStack* stack);
void delete_vstack(VStack** stack);

void new_lstack(LStack** stack);
int push_back_lstack(LStack* stack, char data);
char pop_back_lstack(LStack* stack);
char check_back_lstack(LStack* stack);
void delete_lstack(LStack** stack);

int get_priority(char sym);
int decision_vstack(size_t size);
int decision_lstack();
#endif

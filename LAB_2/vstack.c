#include "stack.h"
#include <stdio.h>

void new_vstack(VStack** out, size_t size) {
    VStack* stack = (VStack*)malloc(sizeof(VStack));
    if (stack == NULL) {
        *out = NULL;
        return;
    }
    stack -> size = 0;
    stack -> vector = (char*)malloc(sizeof(char) * size);
    if (stack -> vector == NULL)
        stack -> max_size = 0;
    else
        stack -> max_size = size;
    for (size_t i = 0; i < stack -> max_size; ++i)
        stack -> vector[i] = 0;
    *out = stack;
}

int push_back_vstack(VStack* stack, char data) {
    if (stack == NULL)
        return -1;
    if (stack -> size >= stack -> max_size)
        return -1;
    stack -> vector[stack -> size] = data;
    ++(stack -> size);
    return 0;
}

char pop_back_vstack(VStack* stack) {
    if (stack == NULL)
        return 0;
    if (stack -> size == 0)
        return 0;
    --(stack -> size);
    char data = stack -> vector[stack -> size];
    stack -> vector[stack -> size] = 0;
    return data;
}

char check_back_vstack(VStack* stack) {
    if (stack == NULL)
        return 0;
    if (stack -> size == 0)
        return 0;
    return stack -> vector[stack -> size - 1];
}

void delete_vstack(VStack** stack) {
    if (stack == NULL || *stack == NULL)
        return ;
    free((*stack) -> vector);
    free(*stack);
    *stack = NULL;
}


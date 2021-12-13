#include "stack.h"

// function to get priority of math op
int get_priority(char sym) {
    if (sym == '*' || sym == '/')
        return 2;
    if (sym == '+' || sym == '-')
        return 1;
    return 0;
}

int main(void) {
    char option = '1';
    int size = 0;
    printf("Please specify what struct to use:\nVector - 1\nList - 2\n");
    option = getc(stdin);
    if (option == '1') {
        printf("Please print vector size\n");
        scanf("%d", &size);
        decision_vstack(size);
    }
    else if (option == '2') {
        decision_lstack();
    }
    else
        printf("incorrect option");
    return 0;
}

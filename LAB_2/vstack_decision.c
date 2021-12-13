#include "stack.h"

static int add_sym(VStack* op, VStack* out, char sym) {
    char tmp;
    int priority;
    // if sym is alphabetic then we put to out stack
    if (isalpha(sym))
        return (push_back_vstack(out, sym));
    // if we see ')' then we put it op stack until we find it's pair
    else if (sym == ')')
        return (push_back_vstack(op, sym));
    // if we find pair '(' we put all ops between '()' to out stack
    else if (sym == '(') {
        while ((tmp = pop_back_vstack(op)) != ')') {
            if (push_back_vstack(out, tmp) == -1)
                return -1;
        }
    }
    // if we find math op we put it to op but firstly move all ops with >= priority from op stack to out
    else {
        priority = get_priority(sym);
        while (get_priority(check_back_vstack(op)) >= priority) {
            if (push_back_vstack(out, pop_back_vstack(op)) == -1)
                return -1;
        }
        return (push_back_vstack(op, sym));
    }
    return 0;
}

// function to print stack from head to tail
static void print_vstack(VStack* stack) {
    char tmp;
    while ((tmp = pop_back_vstack(stack)) != 0)
        printf("%c", tmp);
}

static int get_prefix(VStack* in, VStack* op, VStack* out) {
    char sym;
    // get postfix notation of our revert input
    while ((sym = pop_back_vstack(in)) != 0) {
        if (add_sym(op, out, sym) == -1)
            return -1;
    }
    while((sym = pop_back_vstack(op)) != 0)
        if (push_back_vstack(out, sym) == -1)
            return -1;
    // print our postfix notation in revert order
    print_vstack(out);
    printf("\n");
}

static void clean_memory(VStack** in, VStack** op, VStack** out) {
    delete_vstack(in);
    delete_vstack(op);
    delete_vstack(out);
}

int decision_vstack(size_t size) {
    VStack* in;
    VStack* op;
    VStack* out;
    char sym;
    const char* alloc_error = "Allocation error or not enough size in vector\n";

    new_vstack(&in, size);
    new_vstack(&op, size);
    new_vstack(&out, size);
    if (!in || !op || !out){
        printf("%s", alloc_error);
        return -1;
    }
    printf("Please input expression:");
    while ((sym = getc(stdin)) != EOF) {
        if (sym == ' ')
            continue ;
        else if (sym == '\n') {
            // line ends and we get prefix notation
            if (get_prefix(in, op, out) == -1) {
                printf("%s", alloc_error);
                clean_memory(&in, &op, &out);
                return -1;
            }
        }
        else {
            // put input to stack "in" to make it revert
            if (push_back_vstack(in, sym) == -1) {
                printf("%s", alloc_error);
                clean_memory(&in, &op, &out);
                return -1;
            }
        }
    }
    clean_memory(&in, &op, &out);
}

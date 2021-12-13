#include "stack.h"

static int add_sym(LStack* op, LStack* out, char sym) {
    char tmp;
    int priority;
    // if sym is alphabetic then we put to out stack
    if (isalpha(sym))
        return (push_back_lstack(out, sym));
    // if we see ')' then we put it op stack until we find it's pair
    else if (sym == ')')
        return (push_back_lstack(op, sym));
    // if we find pair '(' we put all ops between '()' to out stack
    else if (sym == '(') {
        while ((tmp = pop_back_lstack(op)) != ')') {
            if (push_back_lstack(out, tmp) == -1)
                return -1;
        }
    }
    // if we find math op we put it to op but firstly move all ops with >= priority from op stack to out
    else {
        priority = get_priority(sym);
        while (get_priority(check_back_lstack(op)) >= priority) {
            if (push_back_lstack(out, pop_back_lstack(op)) == -1)
                return -1;
        }
        return (push_back_lstack(op, sym));
    }
    return 0;
}

// function to print stack from head to tail
static void print_lstack(LStack* stack) {
    char tmp;
    while ((tmp = pop_back_lstack(stack)) != 0)
        printf("%c", tmp);
}

static int get_prefix(LStack* in, LStack* op, LStack* out) {
    char sym;
    // get postfix notation of our revert input
    while ((sym = pop_back_lstack(in)) != 0) {
        if (add_sym(op, out, sym) == -1)
            return -1;
    }
    while((sym = pop_back_lstack(op)) != 0)
        if (push_back_lstack(out, sym) == -1)
            return -1;
    // print our postfix notation in revert order
    print_lstack(out);
    printf("\n");
}

static void clean_memory(LStack** in, LStack** op, LStack** out) {
    delete_lstack(in);
    delete_lstack(op);
    delete_lstack(out);
}

int decision_lstack() {
    LStack* in;
    LStack* op;
    LStack* out;
    char sym;
    const char* alloc_error = "Allocation error or not enough size in vector\n";
    new_lstack(&in);
    new_lstack(&op);
    new_lstack(&out);
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
            if (push_back_lstack(in, sym) == -1) {
                printf("%s", alloc_error);
                clean_memory(&in, &op, &out);
                return -1;
            }
        }
    }
    clean_memory(&in, &op, &out);
}

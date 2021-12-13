#ifndef RED-BLACK_TREE_EXPERIMENTAL_H_INCLUDED
#define RED-BLACK_TREE_EXPERIMENTAL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <conio.h>

#define BLACK 0
#define RED 1

#define CMP_EQ(a, b) ((a) == (b))
#define CMP_LT(a, b) ((a) < (b))
#define CMP_GT(a, b) ((a) > (b))

typedef struct rb_node{
    int data;
    struct rb_node *right;
    struct rb_node *left;
    struct rb_node *parent;
    int color;
} Node_rb;

Node_rb* create(Node_rb* root, int key);
Node_rb *grandparent(Node_rb *node);
Node_rb *func_parent(Node_rb *node);
Node_rb *search_root(Node_rb *current_node);
Node_rb *uncle(Node_rb *node);
Node_rb* getFreeNode(int value, Node_rb *parent);
Node_rb *insert(Node_rb *head, int value);


Node_rb *rotate_left(Node_rb *root);
Node_rb *rotate_right(Node_rb *root);

Node_rb *insert_case1(Node_rb *new_node);
Node_rb *insert_case_vine_right(Node_rb *new_node);
Node_rb *insert_case2(Node_rb *new_node);
Node_rb *insert_case3(Node_rb *new_node);
void insert_case4(Node_rb *new_node);
void insert_case5(Node_rb *new_node);
Node_rb *add_rb_node(Node_rb *root, int data, Node_rb *parent);
void printTree(Node_rb *root, Node_rb *color, const char *dir, int level);
void printTree_simple(Node_rb *root, const char *dir, int level);//will not be used
void print_menu();

#endif

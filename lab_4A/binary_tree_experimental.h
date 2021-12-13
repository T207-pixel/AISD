#ifndef BINARY_TREE_EXPERIMENTAL_H_INCLUDED
#define BINARY_TREE_EXPERIMENTAL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


typedef int T;

#define CMP_EQ(a, b) ((a) == (b))
#define CMP_LT(a, b) ((a) < (b))
#define CMP_GT(a, b) ((a) > (b))

typedef struct Node {
    T data;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
} Node;

Node* create(Node* root, int key);
static Node* getFreeNode(T value, Node *parent);
void insert(Node **head, int value);
Node* getMinNode(Node *root);
Node* getMaxNode(Node *root);
Node* getNodeByValue(Node *root, T value);
static void removeNodeByPtr(Node *target, Node **root);
void deleteValue(Node **root, T value);
void printTree(Node *root, const char *dir, int level);
static int max(int a, int b);
int height(Node *root);
void mirror(Node *node);
#endif

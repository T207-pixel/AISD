#ifndef BINARY_TREE_H
    #define BINARY_TREE_H
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #define MAX_LINE_SIZE 10000
    #define RED 1
    #define BLACK 0

    //structs
    typedef struct s_info {
        int num1;
        int num2;
        char* str;
       struct s_info* next;
       struct s_info* parent;
    }               t_info;

    typedef struct s_node {
        unsigned int key;
        struct s_node* left;
        struct s_node* right;
        char           color;
        struct s_info* info;
    }               t_node;

    typedef struct s_tree {
        int n_keys;
        struct s_node *head;
    }               t_tree;

    //functions
    t_tree* new_tree(void);
    int add_node(t_tree* tree, unsigned int key, t_info info);
    void print_tree(t_tree* tree);
    void print_table(t_tree* tree, unsigned int limit, int islimit);
    t_node* find_key(t_tree* tree, unsigned int key);
    void del_node(t_tree* tree, unsigned int key, unsigned int number);
    t_node* find_closest_not_equal_key(t_tree* tree, unsigned int key);
    void delete_tree(t_tree** tree);
    int load_tree_from_file(t_tree* tree, char* filename);
    void print_info_list(t_node* cur);
#endif

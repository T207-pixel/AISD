#include "red-black_tree_experimental.h"

// add_rb_node works horribly


void main() {

    Node_rb *root;
    Node_rb *parent = NULL; //WRONG
    int option = 0;
    int data;

    printf("Enter first element in binary tree\n");
        scanf("%d", &data);
        root = create(root, data);
        Node_rb* node;


    while(1){
        print_menu();
        scanf("%d", &option);
        if (option == 1)
        {
            printf("Enter key\n");
            scanf("%d", &data);
            root = add_rb_node(&*root, data, parent);// have to change root here
        }
        else if (option == 2)
        {

        }
        else if (option == 3)
        {
            printTree(root, root->color, "root_BLACK", 1);
        }
        else if (option == 4)
        {
            break;
        }
    }


/*
    printf("BENCHMARK\n");
    insert(root, 2);
    insert(root, 9);
    insert(root, 7);
    printTree(root, root->color, "root_BLACK", 1);
    //root = rotate_left(root);
    printf("\n");
    Node_rb *test = insert(root, 100);
    root = search_root(test);
    printTree(root, root->color, "root_BLACK", 1);
*/
}






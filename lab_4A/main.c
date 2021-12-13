#include "binary_tree_experimental.h"

void main() {

    Node* root = NULL;//ADDED NULL FOR BUTY
    int option = 0;
    T value;
    T data;
    int key;
    printf("Enter first element in binary tree\n");
    scanf("%d", &key);
    root = create(root, key);
    Node* node;

    while (1)
    {
        printf("Choose option:\n1) Insert new node\n2) Get min node\n3) Get max node\n4) Delete value");
        printf("\n5) Get parent of node\n6) Print tree\n7) Count height\n8) Mirror tree\n9) Exit\n");

        scanf("%d", &option);
        if (option <= 0)
            {
                printf("incorrect option\n");
                break ;
            }

            else if (option == 1)
            {
                printf("Please specify the key to add\n");
                scanf("%d", &data);
                insert(&root, data);

            }

                 else if (option == 2)
                        printf("min = %d\n", getMinNode(root)->data);

                     else if (option == 3)
                            printf("max = %d\n", getMaxNode(root)->data);


                         else if (option == 4)
                        {
                            printf("Enter which node you would like to delete\n");
                            scanf("%d", &data);
                            deleteValue(&root, data);
                        }

                             else if (option == 5)
                            {

                                printf("Enter node in which parent you are interested\n");

                                // NEW ADDITION
                                scanf("%d", &data);
                                if (data == key)
                                    printf("This parent does not have a parent, because it is a nood");
                                else
                                {
                                printf("parent of %d is %d\n", data, getNodeByValue(root, data)->parent->data);

                                }
                            }

                                 else if (option == 6)
                                {
                                   printTree(root, "root", 1);
                                }

                                    else if (option == 7)
                                    {
                                        printf("Start counting from one ");
                                        printf("Height = %d\n", height(root));
                                    }

                                         else if (option == 8)
                                        {
                                            mirror(root);
                                            printTree(root, "root", 1);
                                        }

                                             else if (option == 9)
                                            {
                                                printf("Program is completed");
                                                break;
                                            }

    }
}




#include "binary_tree.h"

int main(int argc, char** argv) {
    t_tree* tree = new_tree();
    int option = 0;
    unsigned int key;
    unsigned int rec_number;
    int num1, num2;
    char buffer[MAX_LINE_SIZE];
    t_info info;
    t_node* node;
    if (argc == 2 && !strcmp(argv[1], "profiling")) {
        for (int i = 0; i < 10000; ++i) {
            info = (t_info){.num1 = i, .num2 = i, .str = "test"};
            add_node(tree, (unsigned int)rand(), info);
        }
        for (int i = 0; i < 10000; ++i) {
            find_closest_not_equal_key(tree, (unsigned int)rand());
        }
        for (int i = 0; i < 10000; ++i) {
            del_node(tree, (unsigned int)rand(), 1);
        }
    }
    else {
        while (1) {
            printf("Choose option:\n1. Add new record\n2. Delete record by key\n");
            printf("3. Print table\n4. Find element by key\n");
            printf("5. Find elements with the key closest to specified but not equal\n6. Load table from file\n");
            printf("7. Print tree\n");
            printf("8. Exit\n");
            if (scanf("%d", &option) <= 0) {
                printf("incorrect option\n");
                break ;
            }
            if (option == 8)
                break ;
            if (option == 1) {
                printf("please specify the key\n");
                if (scanf("%u", &key) <= 0) {
                    printf("incorrect input\n");
                    break ;
                }
                printf("Please input record that consists of two integers and string separated with spaces\n");
                if (scanf("%d", &num1) <= 0 || \
                        scanf("%d", &num2) <= 0 || \
                        scanf("%s", buffer) <= 0) {
                    printf("incorrect input\n");
                    break ;
                }
                info = (t_info){.num1 = num1, .num2 = num2, .str = buffer};
                if (add_node(tree, key, info) == -1) {
                    printf("malloc error\n");
                    break ;
                }
            }
            else if (option == 2) {
                printf("please specify the key and record number\n");
                if (scanf("%u", &key) <= 0 || scanf("%u", &rec_number) <= 0) {
                    printf("incorrect input\n");
                    break ;
                }
                del_node(tree, key, rec_number);
            }
            else if (option == 3) {
                printf("Please print 1 to specify the key until that to print table content or 0 to print all the table\n");
                if (scanf("%d", &num1) <= 0) {
                    printf("incorrect input\n");
                    break ;
                }
                if (num1 == 0)
                    print_table(tree, 0, 0);
                else if (num1 == 1) {
                    printf("Please specify the key\n");
                    if (scanf("%u", &key) <= 0) {
                        printf("incorrect input\n");
                        break ;
                    }
                    print_table(tree, key, 1);
                }
            }
            else if (option == 4) {
                printf("please specify the key\n");
                if (scanf("%u", &key) <= 0) {
                    printf("incorrect input\n");
                    break ;
                }
                node = find_key(tree, key);
                if (!node)
                    printf("node with key %u not found in table\n", key);
                else
                    print_info_list(node);
            }
            else if (option == 5) {
                printf("please specify the key\n");
                if (scanf("%u", &key) <= 0) {
                    printf("incorrect input\n");
                    break ;
                }
                node = find_closest_not_equal_key(tree, key);
                if (!node)
                    printf("node with key closest to %u not found in table\n", key);
                else
                    print_info_list(node);
            }
            else if (option == 6) {
                printf("Please input the file name\n");
                if (scanf("%s", buffer) <= 0) {
                    printf("incorrect input\n");
                    break ;
                }
                if (load_tree_from_file(tree, buffer) == -1) {
                    printf("error occured\n");
                    break ;
                }
            }
            else if (option == 7)
                print_tree(tree);
            else {
                printf("incorrect option!\n");
                break ;
            }
        }
    }
    if (tree)
        delete_tree(&tree);
    return 0;
}

#include "table.h"

void print_menu(void) {
    printf("Please choose option:\n");
    printf("0. Create empty table\n");
    printf("1. Add info into table\n");
    printf("2. Delete info from table by key1 and release\n");
    printf("3. Delete info from table by key1 (all releases)\n");
    printf("4. Find info in table by key1 and release\n");
    printf("5. Find info in table by key1 (all releases)\n");
    printf("6. Find info in table by key2 and release\n");
    printf("7. Find info in table by key2 (all releases)\n");
    printf("8. Print table\n");
    printf("9. Exit\n");
}

int main(void) {
    Table* table = NULL;
    int option, max_size, key1, key2, rel;
    char str[100] = {0, };
    while(1) {
        print_menu();
        if (scanf("%d", &option) <= 0) {
            printf("Invalid input\n");
            break ;
        }
        if (option == 9)
            break ;
        else if (option == 0) {
            printf("Please input max size of table:\n");
            if (table != NULL || scanf("%d", &max_size) <= 0 || max_size <= 0) {
                printf("Invalid input\n");
                break ;
            }
            if (!(table = create_table(max_size, max_size))) {
                printf("Allocation error\n");
                break ;
            }
        }
        else if (option == 1) {
            printf("Please input key1, key2 and info string:\n");
            if (table == NULL || scanf("%d %d %s", &key1, &key2, str) <= 2 || key2 < 0) {
                printf("Invalid input\n");
                break ;
            }
            add_to_table(table, key1, key2, str);
        }
        else if (option == 2) {
            printf("Please input key1 and release1:\n");
            if (table == NULL || scanf("%d %d", &key1, &rel) <= 1 || key1 < 0 || rel < 0) {
                printf("Invalid input\n");
                break ;
            }
            del_from_keyspace1_by_release(table, key1, rel);
        }
        else if (option == 3) {
            printf("Please input key1:\n");
            if (table == NULL || scanf("%d", &key1) <= 0 || key1 < 0) {
                printf("Invalid input\n");
                break ;
            }
            del_node_from_keyspace1(table, key1);
        }
        else if (option == 4) {
            printf("Please input key1 and release1:\n");
            if (table == NULL || scanf("%d %d", &key1, &rel) <= 1 || key1 < 0 || rel < 0) {
                printf("Invalid input\n");
                break ;
            }
            Table* tmp = find_release_in_ks1_as_table(table, key1, rel);
            if (tmp)
                print_table(tmp);
            else
                printf("not found\n");
            delete_table(&tmp);
        }
        else if (option == 5) {
            printf("Please input key1:\n");
            if (table == NULL || scanf("%d", &key1) <= 0 || key1 < 0) {
                printf("Invalid input\n");
                break ;
            }
            Table* tmp = find_all_releases_in_ks1_as_table(table, key1);
            if (tmp)
                print_table(tmp);
            else
                printf("not found\n");
            delete_table(&tmp);
        }
        else if (option == 6) {
            printf("Please input key2 and release2:\n");
            if (table == NULL || scanf("%d %d", &key2, &rel) <= 1 || key2 < 0 || rel < 0) {
                printf("Invalid input\n");
                break ;
            }
            Table* tmp = find_release_in_ks2_as_table(table, key2, rel);
            if (tmp)
                print_table(tmp);
            else
                printf("not found\n");
            delete_table(&tmp);
        }
        else if (option == 7) {
            printf("Please input key2:\n");
            if (table == NULL || scanf("%d", &key2) <= 0 || key2 < 0) {
                printf("Invalid input\n");
                break ;
            }
            Table* tmp = find_all_releases_in_ks2_as_table(table, key2);
            if (tmp)
                print_table(tmp);
            else
                printf("not found\n");
            delete_table(&tmp);
        }
        else if (option == 8) {
            print_table(table);
        }
    }
    delete_table(&table);
    return 0;
}

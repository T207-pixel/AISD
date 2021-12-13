#include "table.h"

void print_menu(void) {
    printf("Please choose option:\n");
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
    int load = 0;
    char desc[30];
    char info[30];
    printf("Would you like to table from file? 1 - Yes, 0 - No\n");
    if (scanf("%d", &load) <= 0 || load < 0 || load > 1) {
        printf("Invalid option\n");
        return -1;
    }
    if (load == 0) {
        printf("Please specify table max size, filename for description and filename for info\n");
        if (scanf("%d %s %s", &max_size, desc, info) < 3 || max_size <= 0) {
            printf("Invalid value\n");
            return -1;
        }
        table = create_table(max_size, max_size, info);
    }
    else {
        printf("Please specify table filename for description and filename for info\n");
        if (scanf("%s %s", desc, info) < 2) {
            printf("Invalid value\n");
            return -1;
        }
        table = create_table(0, 0, info);
        load_table_description_from_file(table, desc);
    }
    while(1) {
        print_menu();
        if (scanf("%d", &option) <= 0) {
            printf("Invalid input\n");
            break ;
        }
        if (option == 9) {
            dump_table_description_to_file(table, desc);
            break ;
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

#ifndef TABLE_H_INCLUDED
#define TABLE_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define HASH_CONSTANT 37

typedef long       InfoType;
typedef unsigned int   RelType;
typedef unsigned int   RelType2;
typedef unsigned int   KeyType1;
typedef unsigned int   IndexType1;
typedef unsigned int   IndexType2;
typedef char        BusyType2;
typedef unsigned int   KeyType2;
typedef long        OffsetType;
typedef unsigned int  NumberType;

struct s_key_space_1;

typedef struct s_item {
    InfoType info;
    size_t info_len;
    RelType release;
    struct s_item* next;
    IndexType2 index2;
    KeyType1    key1;
    KeyType2    key2;
}                   Item;

typedef struct s_key_space_1 {
    KeyType1 key;
    NumberType   number;
    Item* info;
    struct s_key_space_1* next;
}                   KeySpace1;

typedef struct s_key_space_2 {
    BusyType2 busy;
    KeyType2 key;
    RelType2 release;
    Item* info;
}                   KeySpace2;

typedef struct s_table {
    char* filename;
    KeySpace1* ks1;
    KeySpace2* ks2;
    IndexType2 msize2;
    IndexType2 csize2;
    IndexType1 size1;
}                   Table;

int add_to_table(Table* table, KeyType1 key1, KeyType2 key2, const char* str);
void print_table(Table* table);
Table* create_table(IndexType1 msize1, IndexType2 msize2, const char* filename);
int del_from_keyspace1_by_release(Table* table, KeyType1 key1, RelType release);
int del_node_from_keyspace1(Table* table, KeyType1 key);
Table* find_release_in_ks1_as_table(Table* table, KeyType1 key1, RelType release);
Table* find_all_releases_in_ks1_as_table(Table* table, KeyType1 key1);
void delete_table(Table** table);
Table* find_all_releases_in_ks2_as_table(Table* table, KeyType2 key2);
Table* find_release_in_ks2_as_table(Table* table, KeyType2 key2, RelType2 release);
int dump_table_description_to_file(Table* table, const char* description_filename);
int load_table_description_from_file(Table* table, const char* description_filename);
int cp_to_table(Table* table, KeyType1 key1, KeyType2 key2, InfoType info, InfoType len);

#endif // TABLE_H_INCLUDED

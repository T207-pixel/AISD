#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define HASH_CONSTANT 37

typedef char*       InfoType;// указатель на информацию
typedef unsigned int   RelType;// версия элемента
typedef unsigned int   RelType2;// версия элемента
typedef unsigned int   KeyType1;
typedef unsigned int   IndexType1;
typedef unsigned int   IndexType2;
typedef char        BusyType2;
typedef unsigned int   KeyType2;

struct s_key_space_1;

typedef struct s_item {
    InfoType info; //указатель на информацию
    RelType release; //версия элемента
    struct s_item* next; // выпадающтй односвязный списоок в ks1
    struct s_key_space_1*  node1; // указатель на узел в первом прострвнстве ключей
    IndexType2 index2; //индекс (после хеш фу-ии) в хештабл
    KeyType1    key1;
    KeyType2    key2;
}                   Item;

//Первое пространство ключей организовано как просматриваемая таблица, организованная одновяз списком
typedef struct s_key_space_1 {
    KeyType1 key; // ключ элемента
    Item* info; // указатель на информацию
    struct s_key_space_1* next; // указатель на следующий элемент
}                   KeySpace1;

//Второе пространство ключей организовано как перемешанная таблица, использующая перемешивание сложением
typedef struct s_key_space_2 {
    BusyType2 busy; // признак занятости элемента
    KeyType2 key; // ключ элемента
    RelType2 release; //номер версии элемента
    Item* info; //указатель на информацию
}                   KeySpace2;

typedef struct s_table {
    KeySpace1* ks1; //указатель на первое пространство ключей
    KeySpace2* ks2; //указатель на второе пространство ключей
    IndexType2 msize2; // размер массива
    IndexType2 csize2; // кол-во эл в массиве
}                   Table;

int add_to_table(Table* table, KeyType1 key1, KeyType2 key2, const char* str);
void print_table(Table* table);
Table* create_table(IndexType1 msize1, IndexType2 msize2);
int del_from_keyspace1_by_release(Table* table, KeyType1 key1, RelType release);
int del_node_from_keyspace1(Table* table, KeyType1 key);
Table* find_release_in_ks1_as_table(Table* table, KeyType1 key1, RelType release);
Table* find_all_releases_in_ks1_as_table(Table* table, KeyType1 key1);
void delete_table(Table** table);
Table* find_all_releases_in_ks2_as_table(Table* table, KeyType2 key2);
Table* find_release_in_ks2_as_table(Table* table, KeyType2 key2, RelType2 release);

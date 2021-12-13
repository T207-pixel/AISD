#include "table.h"

Table* create_table(IndexType1 msize1, IndexType2 msize2, const char* filename) {
    Table* table = (Table*)malloc(sizeof(Table));
    if (!table) return NULL;
    table -> msize2 = msize2;
    table -> csize2 = 0;
    table -> ks1 = NULL;
    table -> ks2 = (KeySpace2*)malloc(sizeof(KeySpace2) * msize2);
    table -> filename = strdup(filename);
    KeySpace2* ks2 = table -> ks2;
    for (IndexType1 i = 0; i < msize2; ++i)
        ks2[i].busy = 0;
    return table;
}

int dump_table_description_to_file(Table* table, const char* description_filename) {//описатель таблицы
    FILE* description = fopen(description_filename, "wb");
    if (!description) return -1;
    Item* item;
    fwrite(&(table -> csize2), sizeof(IndexType2), 1, description);
    fwrite(&(table -> msize2), sizeof(IndexType2), 1, description);
    fwrite(&(table -> size1), sizeof(IndexType1), 1, description);
    for (int i = 0; i < table -> msize2; ++i) {
        fwrite(&(table -> ks2[i].busy), sizeof(BusyType2), 1, description);
        fwrite(&(table -> ks2[i].key), sizeof(KeyType2), 1, description);
        fwrite(&(table -> ks2[i].release), sizeof(RelType2), 1, description);
    }
    KeySpace1* ks1 = table -> ks1;
    while (ks1) {
        fwrite(&(ks1 -> key), sizeof(KeyType1), 1, description);
        fwrite(&(ks1 -> number), sizeof(NumberType), 1, description);
        ks1 = ks1 -> next;
    }
    ks1 = table -> ks1;
    while (ks1) {
        item = ks1 -> info;
        for(int i = 0; i < ks1 -> number; ++i) {
            fwrite(&(item -> info), sizeof(InfoType), 1, description);
            fwrite(&(item -> info_len), sizeof(InfoType), 1, description);
            fwrite(&(item -> release), sizeof(RelType), 1, description);
            fwrite(&(item -> index2), sizeof(IndexType2), 1, description);
            fwrite(&(item -> key1), sizeof(KeyType1), 1, description);
            fwrite(&(item -> key2), sizeof(KeyType2), 1, description);
            item = item -> next;
        }
        ks1 = ks1 -> next;
    }
    fclose(description);
    return 0;
}

int load_table_description_from_file(Table* table, const char* description_filename) {//загрузка из файла в режиме чтения
    FILE* description = fopen(description_filename, "rb");
    if (!description) return -1;
    Item* item;
    fread(&(table -> csize2), sizeof(IndexType2), 1, description);
    fread(&(table -> msize2), sizeof(IndexType2), 1, description);
    KeySpace2* tmp = table -> ks2;
    free(tmp);
    table -> ks2 = (KeySpace2*)malloc(sizeof(KeySpace2) *(size_t)(table -> msize2));
    for (int i = 0; i < table -> msize2; ++i) {
        table -> ks2[i].info = NULL;
        table -> ks2[i].busy = 0;
    }
    fread(&(table -> size1), sizeof(IndexType1), 1, description);
    for (int i = 0; i < table -> msize2; ++i) {
        fread(&(table -> ks2[i].busy), sizeof(BusyType2), 1, description);
        fread(&(table -> ks2[i].key), sizeof(KeyType2), 1, description);
        fread(&(table -> ks2[i].release), sizeof(RelType2), 1, description);
    }
    if (table -> size1 == 0)
        return 0;
    table -> ks1 = (KeySpace1*)malloc(sizeof(KeySpace1));
    KeySpace1* ks1 = table -> ks1;
    for (int i = 0; i < table -> size1; ++i) {
        fread(&(ks1 -> key), sizeof(KeyType1), 1, description);
        fread(&(ks1 -> number), sizeof(NumberType), 1, description);
        ks1 -> info = (Item*)malloc(sizeof(Item));
        Item* tmp_item = ks1 -> info;
        tmp_item -> next = NULL;
        for (int j = 1; j < ks1 -> number; ++j) {
            tmp_item -> next = (Item*)malloc(sizeof(Item));
            tmp_item -> next -> next = NULL;
            tmp_item = tmp_item -> next;
        }
        ks1 -> next = NULL;
        if (i < table -> size1 - 1){
            ks1 -> next = (KeySpace1*)malloc(sizeof(KeySpace1));
            ks1 -> next -> next = NULL;
        }
        ks1 = ks1 -> next;
    }
    ks1 = table -> ks1;
    while (ks1 != NULL) {
        item = ks1 -> info;
        for(int i = 0; i < ks1 -> number; ++i) {
            fread(&(item -> info), sizeof(InfoType), 1, description);
            fread(&(item -> info_len), sizeof(InfoType), 1, description);
            fread(&(item -> release), sizeof(RelType), 1, description);
            fread(&(item -> index2), sizeof(IndexType2), 1, description);
            fread(&(item -> key1), sizeof(KeyType1), 1, description);
            fread(&(item -> key2), sizeof(KeyType2), 1, description);
            table -> ks2[item -> index2].info = item;
            item = item -> next;
        }
        ks1 = ks1 -> next;
    }
    fclose(description);
    return 0;
}

KeySpace1* search_in_keyspace1(KeySpace1* ks1, KeyType1 key1) {
    while (ks1) {
        if (ks1 -> key == key1)
            break;
        ks1 = ks1 -> next;
    }
    return ks1;
}

Item* find_release_in_keyspace1(KeySpace1* ks1, KeyType1 key1, RelType release) {
    ks1 = search_in_keyspace1(ks1, key1);
    if (!ks1) return NULL;
    Item* tmp = ks1 -> info;
    while (tmp) {
        if (tmp -> release == release)
            break;
        tmp = tmp -> next;
    }
    return tmp;
}

KeySpace1* create_keyspace1_node(KeyType1 key, Item* info) {
    KeySpace1* new_node = (KeySpace1*)malloc(sizeof(KeySpace1));
    if (!new_node) return NULL;
    new_node -> key = key;
    new_node -> info = info;
    return new_node;
}

Table* find_release_in_ks1_as_table(Table* table, KeyType1 key1, RelType release) {
    Item* item = find_release_in_keyspace1(table -> ks1, key1, release);
    if (!item) return NULL;
    Table* new_table = create_table(1, 1, table -> filename);
    if (!new_table) return NULL;
    cp_to_table(new_table, key1, item -> key2, item -> info, item -> info_len);
    return new_table;
}

Table* find_all_releases_in_ks1_as_table(Table* table, KeyType1 key1) {
    KeySpace1* ks1 = search_in_keyspace1(table -> ks1, key1);
    if (!ks1) return NULL;
    Item* item = ks1 -> info;
    if (!item) return NULL;
    Table* new_table = create_table(table -> msize2, table -> msize2, table -> filename);
    if (!new_table) return NULL;
    while (item) {
        cp_to_table(new_table, key1, item -> key2, item -> info, item -> info_len);
        item = item -> next;
    }
    return new_table;
}

IndexType2 hash(KeyType2 key, IndexType2 max_size) {
    IndexType2 result = 0;
    unsigned char byte;
    for (int i = 0; i < sizeof(KeyType2); ++i) {
        byte = (key >> 8 * i) & 0xFF;
        result += (HASH_CONSTANT * result + byte) % max_size;
        result %= max_size;
    }
    return result;
}

Item* create_item(InfoType info, KeyType1 key1, KeyType2 key2) {
    Item* item = (Item*)malloc(sizeof(Item));
    if (!item) return NULL;
    item -> info = info;
    item -> release = 0;
    item -> next = NULL;
    item -> index2 = 0;
    item -> key1 = key1;
    item -> key2 = key2;
    return item;
}

static int get_item_release(Item* item) {
    if (item != NULL)
        return item -> release + 1;
    return 0;
}

int cp_to_table(Table* table, KeyType1 key1, KeyType2 key2, InfoType info, InfoType len) {
    if (!table || table -> csize2 == table -> msize2)
        return -1;
    table -> csize2 += 1;
    Item* item = create_item(info, key1, key2);
    item -> info_len = len;
    KeySpace1* ks1 = table -> ks1;
    KeySpace1* tmp;
    if ((tmp = search_in_keyspace1(ks1, key1))) {
        RelType last = tmp -> info -> release;
        item -> release = get_item_release(tmp -> info);
        item -> next = tmp -> info;
        tmp -> info = item;
        tmp -> number += 1;
    }
    else {
        tmp = create_keyspace1_node(key1, item);
        if (!tmp) return -1;
        tmp -> next = ks1;
        tmp -> number = 1;
        table -> ks1 = tmp;
        table -> size1 += 1;
    }
    int i = 0;
    while (1) {
        IndexType2 h = hash(key2 + i, table -> msize2);
//        printf("h = %u\n", h);
        if (table -> ks2[h].busy == 1)
            ++i;
        else {
//            printf("h = %u\n", h);
            table -> ks2[h].info = item;
            table -> ks2[h].release = i;
            table -> ks2[h].busy = 1;
            item -> index2 = h;
            break ;
        }
    }
    return 0;
}

int add_to_table(Table* table, KeyType1 key1, KeyType2 key2, const char* str) {
    if (!table || table -> csize2 == table -> msize2)
        return -1;
    table -> csize2 += 1;
    FILE* info_file = fopen(table -> filename, "ab");
    if (!info_file) return -1;
    fseek(info_file, 0, SEEK_END);
    InfoType offset = ftell(info_file);
    fwrite(str, strlen(str), 1, info_file);
    fclose(info_file);
    Item* item = create_item(offset, key1, key2);
    item -> info_len = strlen(str);
    KeySpace1* ks1 = table -> ks1;
    KeySpace1* tmp;
    if ((tmp = search_in_keyspace1(ks1, key1))) {
        RelType last = tmp -> info -> release;
        item -> release = get_item_release(tmp -> info);
        item -> next = tmp -> info;
        tmp -> info = item;
        tmp -> number += 1;
    }
    else {
        tmp = create_keyspace1_node(key1, item);
        if (!tmp) return -1;
        tmp -> next = ks1;
        tmp -> number = 1;
        table -> ks1 = tmp;
        table -> size1 += 1;
    }
    int i = 0;
    while (1) {
        IndexType2 h = hash(key2 + i, table -> msize2);
        printf("key = %u, h = %u\n", key2, h);

        if (table -> ks2[h].busy == 1)
            ++i;
        else {
//            printf("h = %u\n", h);
            table -> ks2[h].info = item;
            table -> ks2[h].release = i;
            table -> ks2[h].busy = 1;
            item -> index2 = h;
            break ;
        }
    }
    return 0;
}

void print_table(Table* table) {
    KeySpace1* ks1 = table -> ks1;
    KeySpace2* ks2 = table -> ks2;
    char buff[1000];
    FILE* file = fopen(table -> filename, "rb");
    if (!file) return ;
    printf("Keyspace1:\n%10s %10s %15s %10s %10s\n", "key1", "key2", "string", "release_ks1", "release_ks2");
    while (ks1 != NULL) {
        Item* item = ks1 -> info;
        fseek(file, item -> info, SEEK_SET);
        fread(buff, item -> info_len, 1, file);
        buff[item -> info_len] = 0;
        rewind(file);
        while (item != NULL) {
            printf("%10u %10u %15s %10d %10d\n", item -> key1, item -> key2, \
                buff, item -> release, table -> ks2[item -> index2].release);
            item = item -> next;
        }
        ks1 = ks1 -> next;
    }
    printf("Keyspace2:\n%10s %10s %15s %10s %10s\n", "key1", "key2", "string", "release_ks1", "release_ks2");
    for (int i = 0; i < table -> msize2; ++i) {
        if (ks2[i].busy == 1 && ks2[i].info != NULL) {
            fseek(file, ks2[i].info -> info, SEEK_SET);
            fread(buff, ks2[i].info -> info_len, 1, file);
            rewind(file);
            printf("%10u %10u %15s %10d %10d\n", ks2[i].info -> key1, \
                ks2[i].info -> key2, buff, \
                ks2[i].info -> release, ks2[i].release);
        }
    }
    fclose(file);
}

int del_from_keyspace1_by_release(Table* table, KeyType1 key1, RelType release) {
    Item* item = find_release_in_keyspace1(table -> ks1, key1, release);
    if (!item) return -1;
    KeySpace1* node = search_in_keyspace1(table -> ks1, key1);
    Item* tmp = node -> info;
    int to_del_node = 1;
    table -> csize2 -= 1;
    if (tmp == item) {
        if (item -> next) {
            node -> info = item -> next;
            to_del_node = 0;
        }
        else if (table -> ks1 == node)
            table -> ks1 = node -> next;
        else {
            KeySpace1* tmp_ks1 = table -> ks1;
            while (tmp_ks1 -> next != node)
                tmp_ks1 = tmp_ks1 -> next;
            tmp_ks1 -> next = node -> next;
        }
        table -> ks2[item -> index2].info = NULL;
        free(item);
        if (to_del_node)
            free(node);
        return 0;
    }
    while (tmp -> next) {
        if (tmp -> next == item)
            tmp -> next = item -> next;
        tmp = tmp -> next;
    }
    table -> ks2[item -> index2].info = NULL;
    free(item);
    return 0;
}

int del_node_from_keyspace1(Table* table, KeyType1 key) {
    KeySpace1* node = search_in_keyspace1(table -> ks1, key);
    if (!node) return -1;
    if (table -> ks1 == node)
        table -> ks1 = node -> next;
    else {
        KeySpace1* tmp_ks1 = table -> ks1;
        while (tmp_ks1 -> next != node)
            tmp_ks1 = tmp_ks1 -> next;
        tmp_ks1 -> next = node -> next;
    }
    Item* tmp = node -> info;
    while (tmp) {
        node -> info = tmp -> next;
        table -> ks2[tmp -> index2]. info = NULL;
        free(tmp);
        tmp = node -> info;
    }
    free(node);
    return 0;
}

Item* find_release_in_keyspace2(Table* table, KeyType2 key, RelType2 release) {
    KeySpace2* ks2 = table -> ks2;
    IndexType2 index = hash(key + release, table -> msize2);
    if (ks2[index].busy)
        return ks2[index].info;
    return NULL;
}

Table* find_release_in_ks2_as_table(Table* table, KeyType2 key2, RelType2 release) {
    Item* item = find_release_in_keyspace2(table, key2, release);
    if (!item) return NULL;
    Table* new_table = create_table(1, 1, table -> filename);
    if (!new_table) return NULL;
    cp_to_table(new_table, item -> key1, key2, item -> info, item -> info_len);
    return new_table;
}

Table* find_all_releases_in_ks2_as_table(Table* table, KeyType2 key2) {
    int n = 0;
    KeySpace2* ks2 = table -> ks2;
    for (int i = 0; i < table -> msize2; ++i) {
        IndexType2 h = hash(key2 + i, table -> msize2);
        if (ks2[h].busy == 0)
            break ;
        if (ks2[h].info != NULL)
            ++n;
    }
    if (n == 0)
        return NULL;
    Table* new_table = create_table(n, n, table -> filename);
    if (!new_table) return NULL;
    for (int i = 0; i < table -> msize2; ++i) {
        IndexType2 h = hash(key2 + i, table -> msize2);
        if (ks2[h].busy == 0)
            break ;
        if (ks2[h].info != NULL)
            cp_to_table(new_table, ks2[h].info -> key1, key2, ks2[h].info -> info, ks2[h].info -> info_len);
    }
    return new_table;
}

void delete_table(Table** table) {
    if (!table || !(*table))
        return ;
    free((*table) -> filename);
    free((*table) -> ks2);
    KeySpace1* ks1 = (*table) -> ks1;
    KeySpace1* tmp_ks1;
    while(ks1) {
        tmp_ks1 = ks1;
        Item* item = ks1 -> info;
        Item* tmp = item;
        while (item) {
            item = item -> next;
            free(tmp);
            tmp = item;
        }
        ks1 = ks1 -> next;
        free(tmp_ks1);
    }
    free(*table);
    *table = NULL;
}


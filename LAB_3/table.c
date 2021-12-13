#include "table.h"

Table* create_table(IndexType1 msize1, IndexType2 msize2) {
    Table* table = (Table*)malloc(sizeof(Table));
    if (!table) return NULL;
    table -> msize2 = msize2;
    table -> csize2 = 0;
    table -> ks1 = NULL;
    table -> ks2 = (KeySpace2*)malloc(sizeof(KeySpace2) * msize2);
    KeySpace2* ks2 = table -> ks2;
    for (IndexType1 i = 0; i < msize2; ++i)
        ks2[i].busy = 0;
    return table;
}

InfoType create_info(const char* str) {
    return strdup(str);
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
    Table* new_table = create_table(1, 1);
    if (!new_table) return NULL;
    add_to_table(new_table, key1, item -> key2, item -> info);
    return new_table;
}

Table* find_all_releases_in_ks1_as_table(Table* table, KeyType1 key1) {
    KeySpace1* ks1 = search_in_keyspace1(table -> ks1, key1);
    if (!ks1) return NULL;
    Item* item = ks1 -> info;
    if (!item) return NULL;
    Table* new_table = create_table(table -> msize2, table -> msize2);
    if (!new_table) return NULL;
    while (item) {
        add_to_table(new_table, key1, item -> key2, item -> info);
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
    item -> node1 = NULL;
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

int add_to_table(Table* table, KeyType1 key1, KeyType2 key2, const char* str) {
    if (!table || table -> csize2 == table -> msize2)
        return -1;
    table -> csize2 += 1;
    InfoType info = create_info(str);
    Item* item = create_item(info, key1, key2);
    KeySpace1* ks1 = table -> ks1;
    KeySpace1* tmp;
    if ((tmp = search_in_keyspace1(ks1, key1))) {
        RelType last = tmp -> info -> release;
        item -> release = get_item_release(tmp -> info);
        item -> next = tmp -> info;
        tmp -> info = item;
        item -> node1 = tmp;
    }
    else {
        tmp = create_keyspace1_node(key1, item);
        if (!tmp) return -1;
        tmp -> next = ks1;
        table -> ks1 = tmp;
        item -> node1 = tmp;
    }
    int i = 0;
    while (1) {
        IndexType2 h = hash(key2 + i, table -> msize2);
//        printf("h = %u\n", h);
        if (table -> ks2[h].busy == 1)
            ++i;
        else {
 //           printf("h = %u\n", h);
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
    printf("Keyspace1:\n%10s %10s %15s %10s %10s\n", "key1", "key2", "string", "release_ks1", "release_ks2");
    while (ks1 != NULL) {
        Item* item = ks1 -> info;
        while (item != NULL) {
            printf("%10u %10u %15s %10d %10d\n", item -> key1, item -> key2, \
                item -> info, item -> release, table -> ks2[item -> index2].release);
            item = item -> next;
        }
        ks1 = ks1 -> next;
    }
    printf("Keyspace2:\n%10s %10s %15s %10s %10s\n", "key1", "key2", "string", "release_ks1", "release_ks2");
    for (int i = 0; i < table -> msize2; ++i) {
        if (ks2[i].busy == 1 && ks2[i].info != NULL)
            printf("%10u %10u %15s %10d %10d\n", ks2[i].info -> key1, \
                ks2[i].info -> key2, ks2[i].info -> info, \
                ks2[i].info -> release, ks2[i].release);
    }
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
        free(item -> info);
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
    free(item -> info);
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
        free(tmp -> info);
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
    Table* new_table = create_table(1, 1);
    if (!new_table) return NULL;
    add_to_table(new_table, item -> key1, key2, item -> info);
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
    Table* new_table = create_table(n, n);
    if (!new_table) return NULL;
    for (int i = 0; i < table -> msize2; ++i) {
        IndexType2 h = hash(key2 + i, table -> msize2);
        if (ks2[h].busy == 0)
            break ;
        if (ks2[h].info != NULL)
            add_to_table(new_table, ks2[h].info -> key1, key2, ks2[h].info -> info);
    }
    return new_table;
}

void delete_table(Table** table) {
    if (!table || !(*table))
        return ;
    free((*table) -> ks2);
    KeySpace1* ks1 = (*table) -> ks1;
    KeySpace1* tmp_ks1;
    while(ks1) {
        tmp_ks1 = ks1;
        Item* item = ks1 -> info;
        Item* tmp = item;
        while (item) {
            item = item -> next;
            free(tmp -> info);
            free(tmp);
            tmp = item;
        }
        ks1 = ks1 -> next;
        free(tmp_ks1);
    }
    free(*table);
    *table = NULL;
}


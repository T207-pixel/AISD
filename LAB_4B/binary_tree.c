#include "binary_tree.h"


t_tree* new_tree(void) {
    t_tree* tree = (t_tree*)malloc(sizeof(t_tree));
    tree -> head = NULL;
    return tree;
}

static t_info* new_info(t_info src) {
    t_info* info = (t_info*)malloc(sizeof(t_info));
    if (!info) return NULL;
    info -> num1 = src.num1;
    info -> num2 = src.num2;
    info -> str = strdup(src.str);
    info -> next = NULL;
    info -> parent = NULL;
    if (!(info -> str)) return NULL;
    return info;
}

static t_node* new_node(unsigned int key, t_info info) {
    t_node* node = (t_node*)malloc(sizeof(t_node));
    if (!node) return NULL;
    node -> key = key;
    node -> info = new_info(info);
    node -> left = NULL;
    node -> right = NULL;
    node -> color = RED;
    if (!(node -> info)) return NULL;
    return node;
}

static int add_info_to_end_of_list(t_info* head, t_info src) {
    t_info* tmp = head;
    while (tmp -> next)
        tmp = tmp -> next;
    tmp -> next = new_info(src);
    if (tmp -> next) {
        tmp -> next -> parent = tmp;
        return (1);
    }
    return 0;
}

static int is_red(t_node *node)
{
  if (node)
    return node -> color == RED;
  return 0;
}

static void invert_color(t_node *node)
{
  node -> color = !(node->color);
  node -> left->color = !(node -> left->color);
  node -> right->color = !(node -> right->color);
}

static t_node* rotate_left(t_node *left)
{
  t_node		*right;
  if (!left)
    return NULL;
  right = left->right;
  left->right = right->left;
  right->left = left;
  right->color = left->color;
  left->color = RED;
  return right;
}

static t_node* rotate_right(t_node *right)
{
  t_node		*left;
  if(!right)
    return NULL;
  left = right->left;
  right->left = left->right;
  left->right = right;
  left->color = right->color;
  right->color = RED;
  return left;
}

static t_node* balance(t_node *node)
{
  if (is_red(node -> right))
    node = rotate_left(node);
  if (is_red(node -> left) && is_red(node -> left -> left))
    node = rotate_right(node);
  if (is_red(node -> left) && is_red(node -> right))
    invert_color(node);
  return (node);
}

static t_node* connect_new_node(t_node* cur, unsigned int key, t_info info, int* err) {
    if (!cur) {
        cur = new_node(key, info);
        if (!cur) {
            *err = -1;
            return NULL;
        }
    }
    else if (key < cur -> key)
        cur -> left = connect_new_node(cur -> left, key, info, err);
    else if (key == cur -> key) {
        if (!add_info_to_end_of_list(cur -> info, info)) {
            *err = -1;
            return NULL;
        }
    }
    else
        cur -> right = connect_new_node(cur -> right, key, info, err);
    if (is_red(cur -> right) && !is_red(cur -> left))
        cur = rotate_left(cur);
    if (is_red(cur -> left) && is_red(cur -> left->left))
        cur = rotate_right(cur);
    if (is_red(cur -> left) && is_red(cur -> right))
        invert_color(cur);
    return cur;
}

int add_node(t_tree* tree, unsigned int key, t_info info) {
    int err = 0;
    tree -> head = connect_new_node(tree -> head, key, info, &err);
    if (tree -> head)
        tree -> head -> color = BLACK;
    return err;
}

static t_node* search_key(t_node* cur, unsigned int key) {
    if (!cur)
        return NULL;
    if (cur -> key == key)
        return cur;
    if (cur -> key > key)
        return search_key(cur -> left, key);
    return search_key(cur -> right, key);
}

t_node* find_key(t_tree* tree, unsigned int key) {
    return search_key(tree -> head, key);
}

// t_node* find_max_key_under_limit(t_tree* tree, unsigned int limit) {
//     t_node* cur = tree -> head;
//     t_node* max_found = NULL;
//     while (cur) {
//         if (cur -> key == limit)
//             return cur;
//         if (cur -> key > limit)
//             cur = cur -> left;
//         else {
//             max_found = cur;
//             cur = cur -> right;
//         }
//     }
//     return max_found;
// }

int abs(int a) {
    return a >= 0 ? a : -a;
}

static void recursive_find_closest(t_node* cur, unsigned int key, int* min_diff, t_node** closest) {
    int cur_diff = cur -> key - key;
    // printf("cur key = %u\n", cur -> key);
    if (cur_diff != 0 && (abs(cur_diff) < abs(*min_diff) || *min_diff == 0)) {
        *closest = cur;
        *min_diff = cur_diff;
    }
    if (cur -> key < key) {
        if (cur -> right)
            recursive_find_closest(cur -> right, key, min_diff, closest);
    }
    else if (cur -> key > key) {
        if (cur -> left)
            recursive_find_closest(cur -> left, key, min_diff, closest);
    }
    else {
        if (cur -> left)
            recursive_find_closest(cur -> left, key, min_diff, closest);
        if (cur -> right)
            recursive_find_closest(cur -> right, key, min_diff, closest);
    }
}

t_node* find_closest_not_equal_key(t_tree* tree, unsigned int key) {
    t_node* cur = tree -> head;
    t_node* closest = NULL;
    int diff = 0;
    if (!cur)
        return NULL;
    recursive_find_closest(cur, key, &diff, &closest);
    return closest;
}

static t_node* move_left(t_node *cur)
{
  invert_color(cur);
  if (cur && cur -> right && is_red(cur -> right -> left))
  {
    cur -> right = rotate_right(cur -> right);
    cur = rotate_left(cur);
    invert_color(cur);
  }
  return cur;
}

static t_node* move_right(t_node *cur)
{
  invert_color(cur);
  if (cur && cur -> left && is_red(cur -> left -> left))
  {
    cur = rotate_right(cur);
    invert_color(cur);
  }
  return cur;
}

static t_node* del_min(t_node* cur)
{
  if (!cur)
    return NULL;
  if (!cur -> left)
  {
    free(cur -> info -> str);
    free(cur -> info);
    free(cur);
    return NULL;
  }
  if (!is_red(cur -> left) && !is_red(cur -> left -> left))
    cur = move_left(cur);
  cur -> left = del_min(cur -> left);
  return balance(cur);
}

static t_node* get_min(t_node* cur)
{
  if (!cur)
    return NULL;
  while (cur -> left)
    cur = cur -> left;
  return cur;
}

static t_node* recursive_del_node(t_node* cur, unsigned int key) {
    t_node* tmp;
    t_info* tmp_info;
    if (!cur)
      return NULL;
    if (key < cur -> key)
    {
      if (cur -> left)
      {
        if (!is_red(cur -> left) && !is_red(cur -> left -> left))
    	    cur = move_left(cur);
        cur -> left = recursive_del_node(cur -> left, key);
      }
    }
    else
    {
      if (is_red(cur -> left))
        cur = rotate_right(cur);
      if (key == cur->key && !cur->right)
      {
        free(cur -> info -> str);
        free(cur -> info);
        free(cur);
        return NULL;
      }
      if (cur->right)
      {
        if (!is_red(cur -> right) && !is_red(cur -> right -> left))
    	    cur = move_right(cur);
        if (key == cur -> key)
        {
    	    tmp = get_min(cur -> right);
    	    cur -> key = tmp -> key;
            tmp_info = tmp -> info;
            tmp -> info = cur -> info;
    	    cur -> info = tmp_info;
    	    cur -> right = del_min(cur -> right);
        }
        else
    	    cur -> right = recursive_del_node(cur -> right, key);
      }
    }
    return balance(cur);
}

void del_node(t_tree* tree, unsigned int key, unsigned int number)
{
    t_node* node_to_del = search_key(tree -> head, key);
    if (!node_to_del)
        return;
    // if we have multiple records under the key we are going to delete just one record
    // specified by given number
    if (node_to_del -> info -> next) {
        t_info* tmp_info = node_to_del -> info;
        int i = 0;
        while (++i < number && tmp_info)
            tmp_info = tmp_info -> next;
        if (tmp_info && number == i) {
            if (tmp_info -> parent)
                tmp_info -> parent -> next = tmp_info -> next;
            if (tmp_info -> next)
                tmp_info -> next -> parent = tmp_info -> parent;
            free(tmp_info -> str);
            free(tmp_info);
        }
        return ;
    }
    // if we have only one record under the key we are going to finally delete the node
    tree -> head = recursive_del_node(tree -> head, key);
    if (tree -> head)
        tree -> head -> color = BLACK;
}


static void recursive_print_tree(t_node* cur, int i) {
    if (!cur)
        return;
    recursive_print_tree(cur -> right, i + 2);
    for (int k = 0; k < i; k++)
        printf(" ");
    printf("%-10u ", cur -> key);
    printf("\n");
    recursive_print_tree(cur -> left, i + 2);
}

void print_tree(t_tree* tree) {
    recursive_print_tree(tree -> head, 0);
}

static void recursive_print_table(t_node* cur, unsigned int limit, int islimit) {
    if (!cur)
        return;
    recursive_print_table(cur -> left, limit, islimit);
    t_info* tmp = cur -> info;
    if (!islimit || cur -> key <= limit) {
        while (tmp) {
            printf("%-10u\t%-10d\t%-10d\t%s\n", cur -> key, tmp -> num1, tmp -> num2, tmp -> str);
            tmp = tmp -> next;
        }
    }
    if (!islimit || cur -> key < limit)
        recursive_print_table(cur -> right, limit, islimit);
}

void print_table(t_tree* tree, unsigned int limit, int islimit) {
    printf("%-10s\t%-10s\t%-10s\t%-10s\n", "key", "num1", "num2", "str");
    recursive_print_table(tree -> head, limit, islimit);
}

static void recursive_delete_tree(t_node* cur) {
    if (!cur)
        return;
    recursive_delete_tree(cur -> left);
    recursive_delete_tree(cur -> right);
    t_info* tmp;
    t_info* next;
    tmp = cur -> info;
    while (tmp) {
        next = tmp -> next;
        free(tmp -> str);
        free(tmp);
        tmp = next;
    }
    free(cur);
}

void delete_tree(t_tree** tree) {
    recursive_delete_tree((*tree) -> head);
    free(*tree);
    *tree = NULL;
}

void print_info_list(t_node* cur) {
    t_info* head = cur -> info;
    while (head) {
        printf("key: %u, record: %10d\t%10d\t%s\n", cur -> key, \
                head -> num1, head -> num2, head -> str);
        head = head -> next;
    }
}

int load_tree_from_file(t_tree* tree, char* filename) {
    FILE* file;
    char buffer[MAX_LINE_SIZE];
    unsigned int key;
    int num1, num2;
    t_info info;
    file = fopen(filename, "r");
    if (!file) {
        printf("file \"%s\" doesn't exist or is inaccessible\n", filename);
        return -1;
    }
    while (fscanf(file, "%u", &key) == 1) {
        if (fscanf(file, "%d %d %s", &num1, &num2, buffer) == 3) {
            info.num1 = num1;
            info.num2 = num2;
            info.str = buffer;
            if (add_node(tree, key, info) == -1) {
                printf("malloc error\n");
                fclose(file);
                return -1;
            }
        }
        else {
            printf("incorrect file\n");
            fclose(file);
            return -1;
        }
    }
    fclose(file);
    return 0;
}

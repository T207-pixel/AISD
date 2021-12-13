#include "binary_tree_experimental.h"

    Node* create(Node* root, int key)
        {
            Node *tmp =(Node*)malloc(sizeof(Node));
            tmp -> parent = NULL;
            tmp -> data = key;
            tmp -> left = tmp -> right = NULL;
            root = tmp;
            return root;
        }

static Node* getFreeNode(T value, Node *parent) {
            Node* tmp = (Node*) malloc(sizeof(Node));
            tmp->left = tmp->right = NULL;
            tmp->data = value;
            tmp->parent = parent;
            return tmp;
    }

    void insert(Node **head, int value) {
    Node *tmp = NULL;
    if (*head == NULL) {
        *head = getFreeNode(value, NULL);
        return;
    }

    tmp = *head;
    while (tmp) {
        if (CMP_GT(value, tmp->data)) {
            if (tmp->right) {
                tmp = tmp->right;
                continue;
            } else {
                tmp->right = getFreeNode(value, tmp);
                return;
            }
        } else if (CMP_LT(value, tmp->data)) {
            if (tmp->left) {
                tmp = tmp->left;
                continue;
            } else {
                tmp->left = getFreeNode(value, tmp);
                return;
            }
        } else {
            exit(2);
        }
    }
}

    Node* getMinNode(Node *root) {
        while (root->left) {
            root = root->left;
        }
        return root;
    }

    Node* getMaxNode(Node *root) {
        while (root->right) {
            root = root->right;
        }
        return root;
    }

Node *getNodeByValue(Node *root, T value) {
    while (root) {
        if (CMP_GT(root->data, value)) {
            root = root->left;
            continue;
        } else if (CMP_LT(root->data, value)) {
            root = root->right;
            continue;
        } else {
            return root;
        }
    }
    return NULL;
}

static void removeNodeByPtr(Node *target, Node **root) {
            //NEW EDITION
            if (target->left && target->right) {
                Node *localMax = getMaxNode(target->left);
                target->data = localMax->data;
                removeNodeByPtr(localMax, *root);
                return;
            }
            else if (target->left) {
                if (target->parent ==  NULL)
                    {
                        *root = target->left;
                        (*root)->parent = NULL;
                        return;
                    }
                else if (target == target->parent->left) {
                    target->parent->left = target->left;
                }
                    else {
                        target->parent->right = target->left;
                    }
                }
                        else if (target->right) {
                                if (target->parent ==  NULL)
                                {
                                    *root = target->right;
                                    (*root)->parent = NULL;
                                    return;
                                }
                            else if (target == target->parent->right) {
                                target->parent->right = target->right;
                            }
                            else {
                                target->parent->left = target->right;
                            }
                        }
                                else {
                                 if (target == target->parent->left) {
                                     target->parent->left = NULL;
                                  } else {
                                     target->parent->right = NULL;
                                 }
                                }
    free(target);
}

    void deleteValue(Node **root, T value) {
        Node *target = getNodeByValue(*root, value);
        removeNodeByPtr(target, root);
    }

    void printTree(Node *root, const char *dir, int level) {
        if (root) {
            printf("lvl %d %s = %d\n", level, dir, root->data);
            printTree(root->left, "left", level+1);
            printTree(root->right, "right", level+1);
        }
    }

static int max(int a, int b){
            if (a > b)
                {
                    return a;
                }
                else
                {
                    return b;
                }
            }

int height(Node *root) {
            int counter_1 = 1;
            int counter_2 = 1;
            int res = 0;

            while (root->left) {
                root = root->left;
                counter_1++;
            }

            while (root->right) {
                root = root->right;
                counter_2++;
            }
            res = max(counter_1, counter_2);
            return res;
    }

void mirror(Node *node)
{
  if (node!=NULL)
  {
    Node* tmp;

    // do the subtrees
    mirror(node->left);
    mirror(node->right);

    // swap the pointers in this node
    tmp        = node->left;
    node->left  = node->right;
    node->right = tmp;
  }
}




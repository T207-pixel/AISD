#include "red-black_tree_experimental.h"

    Node_rb* create(Node_rb* root, int key)
        {
            Node_rb *tmp =(Node_rb*)malloc(sizeof(Node_rb));
            tmp -> parent = NULL;
            tmp -> data = key;
            tmp -> left = tmp -> right = NULL;
            root = tmp;
            tmp -> color = BLACK;
            return root;
        }

    Node_rb *grandparent(Node_rb *node)
        {
            if ((node != NULL) && (node->parent != NULL))
                return node->parent->parent;
            else
                return NULL;
        }

//***fOR DEBUG***/
    Node_rb *func_parent(Node_rb *node)
    {
        if (node != NULL && node->parent != NULL)
        {
            Node_rb *parent = node->parent;
            return parent->data;
        }
    }

    Node_rb *search_root(Node_rb *current_node)
    {
        while(current_node->parent != NULL)
        {
            current_node = current_node->parent;
        }
        return current_node;// means root of rb-tree
    }

    Node_rb *uncle(Node_rb *node)
        {
            Node_rb *g = grandparent(node);
            if (g == NULL)
                return NULL; // No grandparent means no uncle
            if (node->parent == g->left)
                return g->right;
            else
                return g->left;
        }

////////***for firstly inserting node without balancing////////
        Node_rb* getFreeNode(int value, Node_rb *parent)
        {
                Node_rb* tmp = (Node_rb*) malloc(sizeof(Node_rb));
                tmp->left = tmp->right = NULL;
                tmp->data = value;
                tmp->parent = parent;
                tmp->color = RED; //initially is red
                return tmp;
        }

Node_rb *insert(Node_rb *head, int value) {
            Node_rb *tmp = NULL;
            if (head == NULL) {
                head = getFreeNode(value, NULL);
                return head;
            }

            tmp = head;
            while (tmp) {
                if (CMP_GT(value, tmp->data)) {
                    if (tmp->right) {
                        tmp = tmp->right;
//                        tmp->color = RED; //maybe useless
                        continue;
                    } else {
                        tmp->right = getFreeNode(value, tmp);
                        tmp->right->color = RED; //maybe useless
                        return tmp->right;
                    }
                } else if (CMP_LT(value, tmp->data)) {
                    if (tmp->left) {
                        tmp = tmp->left;
//                        tmp->color = RED; //maybe useless
                        continue;
                    } else {
                        tmp->left = getFreeNode(value, tmp);
                        tmp->left->color = RED; //maybe useless
                        return tmp->left;
                    }
                } else {
                    exit(2);
                }
            }
        }
////////***END***////////

    Node_rb *rotate_left(Node_rb *root)
        {
            Node_rb *new_root = root->right;
            if (root->parent != NULL)
            {
                Node_rb *tmp2 = root->parent;
                if (tmp2->left == root)  //checking if (root) subtree is left or right for outweighing parent to new root
                    tmp2->left = new_root;
                else
                {
                    tmp2->right = new_root;
                }
                new_root->parent = tmp2;
            }
            if (new_root->left != NULL)
                {
                    root->right = new_root->left;
                    Node_rb *tmp1 = root->right;
                    tmp1->parent = root;
                }
                else
                {
                    root->right = NULL;
                }
                new_root->left = root;
                root->parent = new_root;
        return new_root;
        }

    Node_rb *rotate_right(Node_rb *root)
        {
            Node_rb *new_root = root->left;
            if (root->parent != NULL)
            {
                printf("DEBUG1");
                Node_rb *tmp3 = root->parent;
                if (tmp3->left == root)  //checking if (root) subtree is left or right for outweighing parent to new root
                    tmp3->left = new_root;
                else
                {
                    tmp3->right = new_root;
                }
                new_root->parent = tmp3;
            }
                if (new_root->right != NULL)
                {
                    root->left = new_root->right;
                    Node_rb *tmp4 = root->left;
                    tmp4->parent = root;
                }
                else
                {
                    root->left = NULL;
                }
                 new_root->right = root;
                 root->parent = new_root;
            return new_root;
        }

//////***rb_insert_fixup***//////
    Node_rb *insert_case1(Node_rb *new_node)
        {
            if (new_node->parent == NULL)
                {
                    new_node->color = BLACK;
                    Node_rb *current = search_root(new_node);
                    return current;
                }
            else
                {
                    printf("DEBUG2_ENTERED\n");
                    insert_case2(new_node);
                }
           }

    Node_rb *insert_case2(Node_rb *new_node)
        {
            if (new_node->parent->color == BLACK)
                {
                    Node_rb *current = search_root(new_node);
                    return current;
                }
            else
               {
                printf("DEBUG_VINE_ENTERED\n");
                insert_case_vine_right(new_node);
                }
        }

    Node_rb *insert_case_vine_right(Node_rb *new_node)
        {
            if (new_node->parent->parent->left == NULL)
            {
                printf("UNCLE_CHECK_LEFT");

                Node_rb *root = grandparent(new_node);
                Node_rb *new_root = rotate_left(root);
                new_root->color = BLACK;
                new_root->left->color = RED;
                new_root->right->color = RED;
                //if (new_root->parent != NULL)  //WORKS
                    return new_root;  //somehow we have to print tree with new root
                    //Node_rb *current = search_root(new_node);
                    //return current;
 /*               else                                        //new
                {
                    while(new_root->parent != NULL)
                    {
                        new_root = new_root->parent;
                    }
                    return new_root;
                }                                           //till here new addition
*/
            }
            else if(new_node->parent->parent->right == NULL)
            {
                printf("UNCLE_CHECK_RIGHT");
                Node_rb *root = grandparent(new_node);
                Node_rb *new_root = rotate_right(root);
                new_root->color = BLACK;
                new_root->left->color = RED;
                new_root->right->color = RED;
               // if (new_root->parent == NULL)
                    return new_root;
              }
/*                else
                {
                    while(new_root->parent != NULL)
                    {
                        new_root = new_root->parent;
                    }
                    return new_root;
                }
*/
            else
            {
                printf("DEBUG_INSERT_CASE_3_ENTERED\n");
                insert_case3(new_node);
            }

        }

    Node_rb *insert_case3(Node_rb *new_node)   //working whith this func
        {
            Node_rb *u = uncle(new_node), *g, *root;

            if ((u != NULL) && (u->color == RED)) {
            // && (n->parent->color == RED) Второе условие проверяется в insert_case2, то есть родитель уже является красным.
                new_node->parent->color = BLACK;
                u->color = BLACK;
                g = grandparent(new_node);
                g->color = RED;
                insert_case1(g);// FAIL IN RECURSION
                //root = search_root(new_node);
                //return root;
                //somehow we do not go through case1 correctly
            } else
            {
                printf("DEBUG_INSERT_CASE_4_ENTERED\n");
                insert_case4(new_node);
            }
        }

    void insert_case4(Node_rb *new_node)
        {
            Node_rb *g = grandparent(new_node);

            if ((new_node == new_node->parent->right) && (new_node->parent == g->left))
                {
                    rotate_left(new_node->parent);
                    new_node = new_node->left;
                }
            else if ((new_node == new_node->parent->left) && (new_node->parent == g->right))
                {
                    rotate_right(new_node->parent);
                    new_node = new_node->right;
                }
            insert_case5(new_node);
            printf("DEBUG_INSERT_CASE_5_ENTERED\n");
        }

    void insert_case5(Node_rb *new_node)
        {
            //printf("DEBUG5");
            Node_rb *g = grandparent(new_node);

            new_node->parent->color = BLACK;
            g->color = RED;
            if ((new_node == new_node->parent->left) && (new_node->parent == g->left))
                {
                    rotate_right(g);
                }
            else
                { /* (n == n->parent->right) && (n->parent == g->right) */
                rotate_left(g);
                }
        }
////////***END***////////


//////***final insert with balancing***//////
    Node_rb *add_rb_node(Node_rb *root, int data, Node_rb *parent)
        {
              Node_rb *ins_balance_node = insert(root, data);
              Node_rb *try1 = insert_case1(ins_balance_node);
              return try1;
        }
////////***END***////////














    Node_rb* getMinNode(Node_rb *root) {
        while (root->left) {
            root = root->left;
        }
        return root;
    }

    Node_rb* getMaxNode(Node_rb *root) {
        while (root->right) {
            root = root->right;
        }
        return root;
    }

/*   //MAYBE WILL BE NEEDED FOR DELETEING
Node_rb *getNodeByValue(Node_rb *root, int value) {
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
*/

    void printTree(Node_rb *root, Node_rb *color, const char *dir, int level) {
            if (root /*&& root->left != NULL && root->right != NULL*/ ) {
                printf("lvl %d %s = %d\n", level, dir, root->data);
                if (root->left != NULL && root->left->color == RED)
                {
                    printTree(root->left, root->color, "left_RED", level+1);
                    printTree(root->right, root->color, "right_RED", level+1);
                }
                else if (root->right != NULL && root->right->color == RED)
                {
                    printTree(root->right, root->color, "right_RED", level+1);
                    printTree(root->left, root->color, "left_RED", level+1);
                }
                else if (root->left != NULL && root->left->color == BLACK)
                {
                    printTree(root->left, root->color, "left_BLACK", level+1);
                    printTree(root->right, root->color, "right_BLACK", level+1);
                }
                else if (root->right != NULL && root->right->color == BLACK)
                {
                    printTree(root->right, root->color, "right_BLACK", level+1);
                    printTree(root->left, root->color, "left_BLACK", level+1);
                }
            }
        }

    void print_menu()
        {
            printf("***Choose option***\n");
            printf("1) Insert new node\n");
            printf("2) Test\n");
            printf("3) Print tree\n");
            printf("4) Exit\n");
        }
/*
    void printTree_simple(Node_rb *root, const char *dir, int level) { //NOT USED
        if (root) {
            printf("lvl %d %s = %d\n", level, dir, root->data);
            printTree_simple(root->left, "left", level+1);
            printTree_simple(root->right, "right", level+1);
        }
    }
*/










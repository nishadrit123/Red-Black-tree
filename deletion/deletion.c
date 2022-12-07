# include <stdio.h>
# include <stdlib.h>
# include <limits.h>

typedef struct node{
    int data, colour;  // 0 for black, 1 for red, -1 for double black
    struct node* left;
    struct node* right;
    struct node* parent;
}node;

typedef node* RB;

void init(RB* t);
void insert(RB* t, int x);
void preorder(RB t);
node* find_uncle(node* a);
void left_left(RB* t, RB n);
void right_right(RB* t, RB n);
void left_right(RB* t, RB n);
void right_left(RB* t, RB n);
void delete(RB* t, int x, node* fn);
void remove_node(RB *t, int x);
node* find_node(RB t, int x);
node* find_sibling(RB t, node* a);
node* inorderpres(node* root);
node* inordersuc(node* root);
void remove_DB(RB* t, node* db);
void swap(int* a, int* b);
int red_away_DB(RB t, node* db);

int main(){
    RB t;
    init(&t);
    insert(&t, 50);
    insert(&t, 30);
    insert(&t, 65);
    insert(&t, 15);
    insert(&t, 35);
    insert(&t, 55);
    insert(&t, 70);
    insert(&t, 68);
    insert(&t, 80);
    insert(&t, 90);
    remove_node(&t, 90);
    remove_node(&t, 70);
    remove_node(&t, 80);
    remove_node(&t, 68);
    // remove_node(&t, 50);
    // remove_node(&t, 55);
    // remove_node(&t, 15);
    // remove_node(&t, 30);
    // remove_node(&t, 35);
    // remove_node(&t, 65);
    preorder(t);
    return 0;
}

void init(RB* t){
    *t = NULL; return;
}

void insert(RB* t, int x){
    node* nn = (node*)malloc(sizeof(node));
    nn->data = x;
    nn->colour = 1;
    nn->left = NULL; nn->right = NULL; nn->parent = NULL;
    if (*t == NULL){
        *t = nn; 
        nn->colour = 0; return;
    }
    node* p = *t;
    node* q = NULL;
    while(p != NULL){
        q = p;
        if (x < p->data) p = p->left;
        else if (x > p->data) p = p->right;
    }
    if (x < q->data) q->left = nn;
    else if (x > q->data) q->right = nn;
    nn->parent = q;

    while (nn->parent != NULL){
        if (nn->colour == 0 && (q->colour == 0 || q->colour == 1)) return;
        else if (nn->colour == 1 && q->colour == 0) return;
        else{
            node* u = find_uncle(nn);
            // Case 1 : if uncle is red
            if (u != NULL && u->colour == 1){
                if (q->colour == 0) return;
                u->colour = 0; nn->parent->colour = 0;
                if (u->parent != *t){
                    u->parent->colour = 1;
                }
                nn = u->parent;
                q = nn->parent;
            }
            // Case 2 : if uncle is black or NULL
            else if (u == NULL || u->colour == 0){
                node* grf = nn->parent->parent;
                if (nn->data < grf->data && nn->data > nn->parent->data){
                    left_right(t, grf);
                    if (grf->colour == 0) grf->colour = 1;
                    else grf->colour = 0;
                    if (nn->colour == 0) nn->colour = 1;
                    else nn->colour = 0;
                    q = nn->parent;
                }
                else if (nn->data > grf->data && nn->data < nn->parent->data){
                    right_left(t, grf);
                    if (grf->colour == 0) grf->colour = 1;
                    else grf->colour = 0;
                    if (nn->colour == 0) nn->colour = 1;
                    else nn->colour = 0;
                    q = nn->parent;
                }
                else if (nn->data < grf->data && nn->data < nn->parent->data){
                    left_left(t, grf);
                    if (grf->colour == 0) grf->colour = 1;
                    else grf->colour = 0;
                    if (nn->parent->colour == 0) nn->parent->colour = 1;
                    else nn->parent->colour = 0;
                    nn = nn->parent; 
                    q = nn->parent;
                }
                else if (nn->data > grf->data && nn->data > nn->parent->data){
                    right_right(t, grf);
                    if (grf->colour == 0) grf->colour = 1;
                    else grf->colour = 0;
                    if (nn->parent->colour == 0) nn->parent->colour = 1;
                    else nn->parent->colour = 0;
                    nn = nn->parent; 
                    q = nn->parent;
                }
            }
        }
    }
}

void preorder(RB t){
    if (t==NULL){
        return;
    }
    if (t->colour == 0) printf("%d -> black\n",t->data);
    else if (t->colour == 1) printf("%d -> red\n",t->data);
    else printf("%d -> DB\n",t->data);
    preorder(t->left);
    preorder(t->right);  
}

node* find_uncle(node* a){
    node* grf = a->parent->parent;
    if (grf != NULL){
        if (a->data < grf->data) return grf->right;
        else return grf->left;
    }
}

void left_left(RB* t, RB n){
    RB A = n;
    RB B = n->left;
    A->left = B->right;
    if (B->right != NULL) B->right->parent = A;
    B->right = A;
    if (A == *t) *t = B;
    else{
        if (A->parent->left == A) A->parent->left = B;
        else A->parent->right = B;
    }
    B->parent = A->parent;
    A->parent = B;
}

void right_right(RB* t, RB n){
    RB A = n;
    RB B = n->right;
    A->right = B->left;
    if (B->left != NULL) B->left->parent = A;
    B->left = A;
    if (A == *t) *t = B;
    else{
        if (A->parent->left == A) A->parent->left = B;
        else A->parent->right = B;
    }
    B->parent = A->parent;
    A->parent = B;
}

void left_right(RB* t, RB n){
    right_right(t, n->left);
    left_left(t, n);
}

void right_left(RB* t, RB n){
    left_left(t, n->right);
    right_right(t, n);
}

void delete(RB* t, int x, node* fn){
    if ((fn->right == NULL && fn->left == NULL) && fn->colour == 1){
        if (fn->parent->left == fn) fn->parent->left = NULL;
        else fn->parent->right = NULL;
        free(fn);
        return;
    }
    else if ((fn->right == NULL && fn->left == NULL) && fn->colour == 0 && fn == *t){
        free(fn);
        *t = NULL;
        return;
    }
    else if ((fn->right == NULL && fn->left == NULL) && fn->colour == 0){
        fn->colour = -1;
        remove_DB(t, fn);
        return;
    }
    else{
        if (fn->right != NULL){
            node* ios = inordersuc(fn);
            fn->data = ios->data;
            delete(t, ios->data, ios);
        }
        else if (fn->left != NULL){
            node* iop = inorderpres(fn);
            fn->data = iop->data;
            delete(t, iop->data, iop);
        }
    }
    return;
}

node* find_node(RB t, int x){
    if (x == t->data) return t;
    else if (x < t->data) return find_node(t->left, x);
    else return find_node(t->right, x);
}

node* find_sibling(RB t, node* a){
    node* par = a->parent;
    if (par != NULL){
        if (par->left == a) return par->right;
        else return par->left;
    }
}

node* inorderpres(node* root){
    root = root->left;
    while(root->right!=NULL){
        root = root->right;
    }
    return root;
}

node* inordersuc(node* root){
    root = root->right;
    while(root->left!=NULL){
        root = root->left;
    }
    return root;
}

void remove_DB(RB* t, node* db){
    // Case 1 : If root is DB
    if (db == *t){
        db->colour = 0;
        return;
    }
    else{
        node* sib = find_sibling(*t, db);
        // Case 2 : If sibling of DB is black and both its children are black
        if ((sib == NULL || sib->colour == 0) && ((sib->left == NULL || 
        sib->left->colour == 0) && (sib->right == NULL || sib->right->colour == 0))){
            if (db->left == NULL && db->right == NULL){
                if (db->parent->colour == 0) db->parent->colour = -1;
                else db->parent->colour = 0;
                sib->colour = 1;
                if (db->parent->right == db) db->parent->right = NULL;
                else db->parent->left = NULL;
                free(db);
                if (sib->parent->colour == -1) remove_DB(t, sib->parent);
                return;
            }
            else{
                if (db->parent->colour == 0) db->parent->colour = -1;
                else db->parent->colour = 0;
                sib->colour = 1;
                if (db->parent->colour == -1) remove_DB(t, db->parent);
                db->colour = 0;
                return;
            }
        }
        //  Case 3 : If sibling of DB is red
        else if (sib->colour == 1){
            swap(&(sib->colour), &(sib->parent->colour));
            if (db->parent->left == db) right_right(t, db->parent);
            else left_left(t, db->parent);
            remove_DB(t, db);
        }
        // Case 4 : If sibling of DB is black and its child far from DB is red
        else if (sib->colour == 0 && red_away_DB(*t, db)){
            swap(&(db->parent->colour), &(sib->colour));
            if (db->parent->left == db){
                db->parent->left = NULL; free(db);
                right_right(t, sib->parent);
                sib->right->colour = 0;
            }
            else if (db->parent->right == db){
                db->parent->right = NULL; free(db);
                left_left(t, sib->parent);
                sib->left->colour = 0;
            }
            return;
        }
        // Case 5 : If sibling of DB is black and its child near to DB is red
        else if (sib->colour == 0 && !(red_away_DB(*t, db))){
            if (db->parent->left == db){
                swap(&(sib->colour), &(sib->left->colour));;
                left_left(t, sib);
                remove_DB(t, db);
            }
            else if (db->parent->right == db){
                swap(&(sib->colour), &(sib->right->colour));
                right_right(t, sib);
                remove_DB(t, db);
            }
            return;
        }
    }
    return;
}

void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
    return;
}

int red_away_DB(RB t, node* db){
    node* sib = find_sibling(t, db);
    if (sib->parent->left == sib){
        if (sib->left != NULL)
            if (sib->left->colour == 1) return 1;
    }
    else{
        if (sib->right != NULL)
            if (sib->right->colour == 1) return 1;
    }
    return 0;
}

void remove_node(RB* t, int x){
    node* fn = find_node(*t, x);
    delete(t, x, fn);
    return;
}
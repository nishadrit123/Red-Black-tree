# include <stdio.h>
# include <stdlib.h>

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

int main(){
    RB t;
    init(&t);
    insert(&t, 10);
    insert(&t, 18);
    insert(&t, 7);
    insert(&t, 15);
    insert(&t, 16);
    insert(&t, 30);
    insert(&t, 25);
    insert(&t, 40);
    insert(&t, 60);
    insert(&t, 2);
    insert(&t, 1);
    insert(&t, 70);
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
    else printf("%d -> red\n",t->data);
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
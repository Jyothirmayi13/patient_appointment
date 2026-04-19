#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
AVLNODE create(Appointment appt) {
    AVLNODE node = (AVLNODE)malloc(sizeof(AVLNODE));
    if (node == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    node->data   = appt;
    node->ht = 1;
    node->left = node->right = NULL; 
    return node;
}
int height(AVLNODE root) {
    int lh, rh;
    if(root == NULL){
        return 0;
    }
    if(root->left == NULL)
        lh = 0;
    else
        lh = 1 + root->left->ht;
    if(root->right == NULL)
        rh = 0;
    else
        rh = 1 + root->right->ht;
    if(lh > rh)
        return lh;
    else
        return rh;
}
int balancefactor(AVLNODE root) {
    int lh, rh;
    if(root == NULL)
        return 0;
    if(root->left == NULL)
        lh = 0;
    else
        lh = 1 + root->left->ht;
    if(root->right == NULL)
        rh = 0;
    else
        rh = 1 + root->right->ht;
    return (lh - rh);
}


AVLNODE rotateRight(AVLNODE x) {
    AVLNODE y;
    y = x->left;
    x->left = y->right;
    y->right = x;
    x->ht = height(x);
    y->ht = height(y);
    return y;
}

AVLNODE rotateLeft(AVLNODE x) {
    AVLNODE y;
    y = x->right;
    x->right = y->left;
    y->left = x;
    x->ht = height(x);
    y->ht = height(y);
    return y;
}

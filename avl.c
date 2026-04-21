#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
AVLNODE create(Appointment appt) {
    AVLNODE node = (AVLNODE)malloc(sizeof(struct node));
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
AVLNODE insertNodeInAVL(AVLNODE root, Appointment appt) {
    if(appt==root->data
    if(root == NULL) {
        root = newNode(appt);
        printf("Appointment booked successfully.\n");
    }
    else if(appt.id > root->data.id) {
        root->right = insertNodeInAVL(root->right, appt);
        if(balancefactor(root) == -2) {
            if(appt.id > root->right->data.id)
                root = rotateLeft(root);      
            else
                root = RL(root); 
        }
    }
    else if(appt.id < root->data.id) {
        root->left = insertNodeInAVL(root->left, appt);
        if(balancefactor(root) == 2) {
            if(appt.id < root->left->data.id)
                root = rotateRight(root);      
            else
                root = LR(root);  
        }
    }
    else {
        printf("Appointment ID %d already exists.\n", appt.id);
    }
    root->ht = height(root);
    return root;
}

AVLNODE deleteNodeInAVL(AVLNODE root, int id) {
    AVLNODE temp;

    if(root == NULL) {
        printf("Appointment ID %d not found.\n", id);
        return root;
    }

    if(id < root->data.id)
        root->left = deleteNodeInAVL(root->left, id);
    else if(id > root->data.id)
        root->right = deleteNodeInAVL(root->right, id);
    else {
        
        if(root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        
        if(root->left == NULL) {
            temp = root->right;
            free(root);
            return temp;
        }
        
        if(root->right == NULL) {
            temp = root->left;
            free(root);
            return temp;
        }
        
        temp = root->right;
        while(temp->left != NULL)
            temp = temp->left;

        
        root->data = temp->data;

        
        root->right = deleteNodeInAVL(root->right, temp->data.id);
    }

    root->ht = height(root);
    int bf = balancefactor(root);

    
    if(bf == 2) {
        if(balancefactor(root->left) >= 0)
            root = rotateRight(root);          
        else
            root = LR(root);      
    }
    else if(bf == -2) {
        if(balancefactor(root->right) <= 0)
            root = rotateLeft(root);           
        else
            root = RL(root);      
    }

    return root;
}
AVLNODE LR(AVLNODE root) {
    root->left = rotateLeft(root->left);
    return rotateRight(root);
}

AVLNODE RL(AVLNODE root) {
    root->right = rotateRight(root->right);
    return rotateLeft(root);
}
AVLNODE minValueNode(AVLNODE root) {
    AVLNODE current = root;
    while(current->left != NULL)
        current = current->left;
    return current;
}
AVLNODE search(AVLNODE root, int id) {
    if(root == NULL)
        return NULL;
    if(id == root->data.id)
        return root;
    else if(id < root->data.id)
        return search(root->left, id);
    else
        return search(root->right, id);
}

    

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

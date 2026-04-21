#include <stdio.h>
#include <patient_history.c>
#include "avl.h"
AVLNODE searchID(AVLNODE root, int id)
{
    if (root == NULL)
    { printf("No data found\n");
        return NULL;}
   if (id == root->data.id)
   {  printf("Data found\n");
        return root;  }
    else if (id < root->data.id)
        return search(root->left, id);
  
    else
        return search(root->right, id);
    
}

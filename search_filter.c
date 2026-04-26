#include <stdio.h>
#include <string.h>
#include "avl.h"

void searchByName(AVLNODE root, const char* name) {
    if(root == NULL) return;
    searchByName(root->left, name);
    if(strcmp(root->data.patientName, name) == 0)
        displayAppointment(root->data);
    searchByName(root->right, name);
}

void searchByDate(AVLNODE root, const char* date) {
    if(root == NULL) return;
    searchByDate(root->left, date);
    if(strcmp(root->data.date, date) == 0)
        displayAppointment(root->data);
    searchByDate(root->right, date);
}

void searchByDoctor(AVLNODE root, const char* docName) {
    if(root == NULL) return;
    searchByDoctor(root->left, docName);
    if(strcmp(root->data.docName, docName) == 0)
        displayAppointment(root->data);
    searchByDoctor(root->right, docName);
}

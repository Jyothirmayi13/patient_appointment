
#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

void saveToFile(AVLNODE root, const char* filename) {
    if(root == NULL) return;
    saveToFile(root->left, filename);
    FILE* fp = fopen(filename, "a");
    if(fp == NULL) { printf("Error opening file.\n"); return; }
    fprintf(fp, "%d,%s,%s,%s,%s,%d\n",
        root->data.id,
        root->data.patientName,
        root->data.docName,
        root->data.date,
        root->data.timeSlot,
        root->data.priority);
    fclose(fp);
    saveToFile(root->right, filename);
}

AVLNODE loadFromFile(AVLNODE root, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("No existing data found. Starting fresh.\n");
        return root;
    }
    Appointment appt;
    while(fscanf(fp, "%d,%49[^,],%49[^,],%14[^,],%9[^,],%d\n",
        &appt.id, appt.patientName, appt.docName,
        appt.date, appt.timeSlot, &appt.priority) == 6) {
        initQueue(&appt.history);
        root = insertNodeInAVL(root, appt);
    }
    fclose(fp);
    return root;
}

#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

void initQueue(Queue* q) {
    q->front = q->rear = NULL;
    
}

void enqueue(Queue* q, int appointId) {
    QNode* newNode = (QNode*)malloc(sizeof(QNode));
    if(newNode == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    newNode->appointId = appointId;
    newNode->next = NULL;

    if(q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

void printHistory(Queue* q) {
    if(q->front == NULL) {
        printf("No history found.\n");
        return;
    }
    QNode* temp = q->front;
    printf("Appointment History (IDs): ");
    while(temp != NULL) {
        printf("%d ", temp->appointID);
        temp = temp->next;
    }
    printf("\n");
}

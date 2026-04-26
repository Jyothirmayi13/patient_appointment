#ifndef AVL_H
#define AVL_H

typedef struct QNode {
    int appointID;
    struct QNode* next;
} QNode;
typedef struct Queue {
    QNode* front;
    QNode* rear;
} Queue;

typedef struct Appointment {
    int id;                 
    char patientName[50];
    char docName[50];
    char date[15];           
    char timeSlot[10];       
    int priority;
 Queue history;
} Appointment;
struct node {
    Appointment data;
    int ht;
    struct node* left;
    struct node* right;

};

typedef struct node* AVLNODE;
AVLNODE create(Appointment appt);
int height(AVLNODE root);
int balancefactor(AVLNODE root);
AVLNODE rotateLeft(AVLNODE x);
AVLNODE LR(AVLNODE root);
AVLNODE RL(AVLNODE root);
AVLNODE rotateRight(AVLNODE x);
AVLNODE insertNodeInAVL(AVLNODE root, Appointment appt);
AVLNODE deleteNodeInAVL(AVLNODE root, int id);
AVLNODE search(AVLNODE root, int id);
void initQueue(Queue* q);
AVLNODE minValueNode(AVLNODE root);
void addToHistory(AVLNODE node, Appointment appt);
void saveToFile(AVLNODE root, const char* filename);
AVLNODE loadFromFile(AVLNODE root, const char* filename);
void searchByName(AVLNODE root, const char* name);
void searchByDate(AVLNODE root, const char* date);
void searchByDoctor(AVLNODE root, const char* docName);
void displayAppointment(Appointment appt);
void displayAll(AVLNODE root);
void enqueue(Queue* q, int appointId);
void printHistory(Queue* q);
#endif

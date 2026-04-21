typedef struct Appointment {
    int id;                 
    char patientName[50];
    char docName[50];
    char date[15];           
    char timeSlot[10];       
    int priority;
struct Queue history;
} Appointment;
struct node {
    Appointment data;
    int ht;
    struct node* left;
    struct node* right;
struct Queue history; 
};
typedef struct QNode {
    Appointment data;
    struct QNode* next;
} QNode;
typedef struct Queue {
    QNode* front;
    QNode* rear;
} Queue;

typedef struct node* AVLNODE;
AVLNODE create(Appointment appt);
int height(AVLNODE root);
int balancefactor(AVLNODE root);
AVLNODE rotateLeft(AVLNODE x);
AVLNODE rotateRight(AVLNODE x);
AVLNODE insertNodeInAVL(AVLNODE root, Appointment appt);
AVLNODE deleteNodeInAVL(AVLNODE root, int id);
AVLNODE search(AVLNODE root, int id);
void addToHistory(AVLNODE node, Appointment appt);

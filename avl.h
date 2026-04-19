typedef struct Appointment {
    int id;                 
    char patientName[50];
    char docName[50];
    char date[15];           
    char timeSlot[10];       
    int priority;           
} Appointment;
struct node {
    Appointment data;
    int ht;
    struct node* left;
    struct node* right;
};
typedef struct node* AVLNODE;

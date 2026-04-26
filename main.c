#include <stdio.h>
#include <string.h>
#include "avl.h"

#define FILENAME "appointments.txt"

int main() {
    AVLNODE root = NULL;
    int choice, id;

    root = loadFromFile(root, FILENAME);

    while(1) {
        printf("\n===== Appointment Management System =====\n");
        printf("1. Insert Appointment\n");
        printf("2. Search by ID\n");
        printf("3. Delete Appointment\n");
        printf("4. Display All\n");
        printf("5. Search by Name\n");
        printf("6. Search by Date\n");
        printf("7. Search by Doctor\n");
        printf("8. View History\n");
        printf("9. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if(choice == 1) {
            Appointment appt;
            printf("Enter Appointment ID: ");
            scanf("%d", &appt.id);
            printf("Enter Patient Name: ");
            scanf(" %[^\n]", appt.patientName);
            printf("Enter Doctor Name: ");
            scanf(" %[^\n]", appt.docName);
            printf("Enter Date (DD-MM-YYYY): ");
            scanf("%s", appt.date);
            printf("Enter Time Slot: ");
            scanf("%s", appt.timeSlot);
            printf("Enter Priority: ");
            scanf("%d", &appt.priority);
            initQueue(&appt.history);
            root = insertNodeInAVL(root, appt);
            printf("Appointment inserted successfully.\n");
            remove(FILENAME);
            saveToFile(root, FILENAME);
        }
        else if(choice == 2) {
            printf("Enter Appointment ID to search: ");
            scanf("%d", &id);
            AVLNODE found = search(root, id);
            if(found != NULL)
                displayAppointment(found->data);
            else
                printf("Appointment not found.\n");
        }
        else if(choice == 3) {
            printf("Enter Appointment ID to delete: ");
            scanf("%d", &id);
            root = deleteNodeInAVL(root, id);
            remove(FILENAME);
            saveToFile(root, FILENAME);
        }
        else if(choice == 4) {
            if(root == NULL)
                printf("No appointments found.\n");
            else
                displayAll(root);
        }
        else if(choice == 5) {
            char name[50];
            printf("Enter Patient Name: ");
            scanf(" %[^\n]", name);
            searchByName(root, name);
        }
        else if(choice == 6) {
            char date[15];
            printf("Enter Date (DD-MM-YYYY): ");
            scanf("%s", date);
            searchByDate(root, date);
        }
        else if(choice == 7) {
            char docName[50];
            printf("Enter Doctor Name: ");
            scanf(" %[^\n]", docName);
            searchByDoctor(root, docName);
        }
        else if(choice == 8) {
            printf("Enter Appointment ID to view history: ");
            scanf("%d", &id);
            AVLNODE node = search(root, id);
            if(node != NULL)
                printHistory(&node->data.history);
            else
                printf("Appointment not found.\n");
        }
        else if(choice == 9) {
            printf("Exiting...\n");
            break;
        }
        else {
            printf("Invalid choice.\n");
        }
    }
    return 0;
}

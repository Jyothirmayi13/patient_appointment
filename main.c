#include <stdio.h>
#include <string.h>
#include "avl.h"

int main() {
    AVLNODE root = NULL;
    int choice, id;

    while(1) {
        printf("\n===== Appointment Management System =====\n");
        printf("1. Insert Appointment\n");
        printf("2. Search Appointment\n");
        printf("3. Delete Appointment\n");
        printf("4. Add History\n");
        printf("5. Print History\n");
        printf("6. Exit\n");
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

            // Initialize history queue
            initQueue(&appt.history);

            root = insertNodeInAVL(root, appt);
            printf("Appointment inserted successfully.\n");
        }

        else if(choice == 2) {
            printf("Enter Appointment ID to search: ");
            scanf("%d", &id);

            AVLNODE found = search(root, id);
            if(found != NULL) {
                printf("Found!\n");
                printf("Patient: %s\n", found->data.patientName);
                printf("Doctor: %s\n", found->data.docName);
                printf("Date: %s\n", found->data.date);
                printf("Time: %s\n", found->data.timeSlot);
            } else {
                printf("Appointment not found.\n");
            }
        }

        else if(choice == 3) {
            printf("Enter Appointment ID to delete: ");
            scanf("%d", &id);

            root = deleteNodeInAVL(root, id);
        }

        else if(choice == 4) {
            printf("Enter Appointment ID to add history: ");
            scanf("%d", &id);

            AVLNODE node = search(root, id);
            if(node != NULL) {
                int histId;
                printf("Enter history appointment ID: ");
                scanf("%d", &histId);

                enqueue(&node->data.history, histId);
                printf("History added.\n");
            } else {
                printf("Appointment not found.\n");
            }
        }

        else if(choice == 5) {
            printf("Enter Appointment ID to view history: ");
            scanf("%d", &id);

            AVLNODE node = search(root, id);
            if(node != NULL) {
                printHistory(&node->data.history);
            } else {
                printf("Appointment not found.\n");
            }
        }

        else if(choice == 6) {
            printf("Exiting...\n");
            break;
        }

        else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}

#include <stdio.h>
#include "avl.h"

void displayAppointment(Appointment appt) {
    printf("\n--- Appointment Details ---\n");
    printf("ID       : %d\n", appt.id);
    printf("Patient  : %s\n", appt.patientName);
    printf("Doctor   : %s\n", appt.docName);
    printf("Date     : %s\n", appt.date);
    printf("Slot     : %s\n", appt.timeSlot);
    printf("Priority : %d\n", appt.priority);
}

void displayAll(AVLNODE root) {
    if(root == NULL) return;
    displayAll(root->left);
    displayAppointment(root->data);
    displayAll(root->right);
}

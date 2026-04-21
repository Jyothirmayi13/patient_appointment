
void addToHistory(AVLNODE node, Appointment appt)
{
    if (node == NULL)
        return;

    QNode* temp = (QNode*)malloc(sizeof(QNode));

    if (temp == NULL){
        printf("Memory allocation failed\n");
        return;}

    temp->data = appt;
    temp->next = NULL;
    if (node->history.rear == NULL)
     node->history.front = node->history.rear = temp;
    else
    {
        node->history.rear->next = temp;
        node->history.rear = temp;
    }

    printf("Appointment added to history\n");
}
void patientdetails(Queue *q,Appointment appt)
//traverse queue!

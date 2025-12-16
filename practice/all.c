// #include <stdio.h>

// int main() {
//     int arr[5] = {75, 30, 23, 14, 3};
//     int *ptr = arr[3];
//     int n = 2;
//     int result = *(ptr - n);
//     printf("%d\n", result);
//     int result1 = *(ptr + n);
//     printf("%d\n", result1);

//     // int *ptr = arr;
//     // int *ptr1 = arr[3];

//     // int result = *(ptr - ptr1);
//     // printf("%d\n", result);
//     // for (int i = 0; i < 5; i++) {
//     //     printf("%d ", *ptr);
//     //     ptr++;
//     // }
//     // return 0;
// }

// #include <stdio.h>

// struct Node {
//     int data;
//     struct Node *next;
// };

// int main() {
//     // create node a
//     struct Node a;
//     a.data = 10;
//     a.next = NULL;

//     // create node b
//     struct Node b;
//     b.data = 20;
//     b.next = NULL;

//     // link a -> b
//     a.next = &b;
//     // print both values
//     printf("%d %d\n", a.data, a.next->data);

//     return 0;
// }

// struct Node {
//     int data;
//     struct Node *next;
// };

// void insertAtBeginning(struct Node **head, int value) {
//     // allocate node
//     struct Node *newNode = malloc(sizeof(struct Node));
//     // assign data
//     newNode->data = value;
//     // set next to old head
//     newNode->next = *head;
//     // move head to new node
//     *head = newNode;
// }

// printTheList (struct Node *head){
//     struct Node *curr = head;
//     while (curr!=NULL) {
//         printf("%d ",curr->data);
//         curr = curr->next;
//     }
// }

// void insertAtEnd (struct Node **head, int value) {
//     struct Node * newNode = malloc(sizeof(struct Node));
//     newNode->data = value;
//     newNode->next = NULL;
//     if (*head == NULL){
//         *head = newNode;
//         return;
//     }
//     struct Node *temp = *head;
//     while (temp->next != NULL) {
//         temp = temp->next;
//     }
//     temp->next = newNode;
// }

// void insertAtPosition(struct Node **head, int value, int pos) {
//     // allocate new node
//     struct Node *newNode = malloc(sizeof(struct Node));
//     newNode->data = value;
//     newNode->next = NULL;
//     // handle position = 1 case
//     if (pos == 1) {
//         *head = newNode;
//         return;
//     }
//     // traverse to pos-1
//     struct Node *temp = *head;
//     for (int i = 1; i < pos-1 && temp->next != NULL; i++) {
//         temp = temp->next;
//     }
//     // link new 
//     newNode->next = temp->next;
//     temp->next = newNode;
// }

// void deleteAtBegin(struct Node **head) {
//     if (*head ==NULL) {
//         return;
//     }
//     struct Node *temp = *head;
//     *head = temp->next;
//     free(temp);

// }

// void deleteAtEnd(struct Node **head) {
//     if (*head == NULL) {
//         return;
//     }
//     if ((*head)->next == NULL) {
//         free(*head);
//         *head = NULL;
//         return;
//     }
//     struct Node *curr = *head;
//     while (curr->next->next != NULL) {
//         curr = curr->next;
//     }
//     struct Node *last = curr->next;
//     curr->next = NULL;
//     free(last);
// }

// void deleteAtPosition (struct Node **head, int pos) {
//     if (*head == NULL) {
//         return;
//     }
//     if (pos == 1) {
//         deleteAtBegin(head);
//         return;
//     }
//     struct Node *curr = *head;
//     for (int i = 1; i < pos-1 && curr->next != NULL; i++){
//         curr = curr->next;
//     }
//     if (curr->next == NULL) {
//         return;
//     }
//     struct Node *temp = curr->next;
//     curr->next = temp->next;
//     free(temp);
// }

// void push(int arr[], int value, int *top, int size) {
//     if (*top + 1 == size) {
//         printf("Stack is full");
//     }
//     else {
//         *top = *top + 1;
//         arr[*top] = value;
//     }
// }

// void pop (int arr[], int *top, int size) {
//     if (*top == -1){
//         printf("stack is empty");
//     }
//     else {
//         printf("pop ele: %d", arr[*top]);
//         *top = *top - 1;
//     }
// }

// void peek (int arr[], int *top) {
//     if (*top == -1) {
//         printf("stack is empty");
//     }
//     else{
//         printf("%d ",arr[*top]);
//     }
// }

// void display (int arr[], int *top){
//     if (*top == -1) {
//         printf("stack is empty");
//     }
//     for (int i = *top; i >= 0; i--) {
//         printf("%d ", arr[i]);
//     }
// }

#include <stdio.h>
#include <stdlib.h>

void enqueue(int arr[], int *front, int *rear, int size, int value) {
    if ((*rear + 1) % size == *front) {
        printf("queue is full\n");
        return;
    }

    if (*front == -1 && *rear == -1) {
        (*front) = 0;
        (*rear) = 0;
        arr[*rear] = value;
    } 
    else {
        (*rear) = (*rear + 1) % size;
        arr[*rear] = value;
    }
}

void dequeue(int arr[], int *front, int *rear, int size) {
    if (*front == -1 && *rear == -1) {
        printf("queue is empty\n");
        return -1;
    }
    if (*front == *rear) {
        *front = -1;
        *rear = -1;
    }
    else{
        *front = (*front + 1) % size;
    }
    return arr[*front];
}

void display(int arr[], int *front, int *rear) {
    if (*front == -1 && *rear == -1) {
        printf("queue is empty\n");
        return;
    }
    else {
        for (int i = *front; i <= *rear; i++) {
            printf("%d ",arr[i]);
        }
    }
}

void peek(int arr[], int *front, int *rear) {
    if (*front == -1 && *rear == -1){
        printf("the queue is empty\n");
        return;
    }
    printf("%d\n", arr[*front]);
}

int* reverseQueue(int *queue, int *front, int *rear,int size) {
    int *stack = (int *)malloc(size * sizeof(int));
    int top = -1;

    if (*front == -1) {
        free(stack);
        return queue;
    }
    while (*front <= *rear) {
        stack[++top] = peek(queue, front, rear);
        dequeue(queue, front, rear, size);
    }
    *front = -1;
    *rear = -1;

    while (top >= 0) {
        enqueue(queue, front, rear, size, stack[top--]);
    }
    free(stack);
    return queue;
}

int main() {
    int size;
    scanf("%d",&size);
    int *queue = (int *)malloc(size * sizeof(int));

    int front = -1;
    int rear = -1;

    enqueue(queue, &front, &rear, size, 10);
    enqueue(queue, &front, &rear, size, 20);
    enqueue(queue, &front, &rear, size, 30);

    display(queue, &front, &rear);

    peek(queue, &front, &rear);

    dequeue(queue, &front, &rear, size);
    dequeue(queue, &front, &rear, size);

    display(queue, &front, &rear);

    free(queue);
    return 0;
}
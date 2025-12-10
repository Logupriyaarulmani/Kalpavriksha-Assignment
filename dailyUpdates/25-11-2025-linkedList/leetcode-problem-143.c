/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
void reorderList(struct ListNode* head) {
    struct ListNode *slowPointer = head;
    struct ListNode *fastPointer = head;

    while (fastPointer != NULL && fastPointer->next != NULL) {
        slowPointer = slowPointer->next;
        fastPointer = fastPointer->next->next;
    }

    struct ListNode *secondPart = slowPointer->next;
    struct ListNode *previous = slowPointer->next = NULL;

    while (secondPart) {
        struct ListNode *temp = secondPart->next;
        secondPart->next = previous;
        previous = secondPart;
        secondPart = temp;
    }

    struct ListNode* firstPart = head;
    secondPart = previous;

    while (secondPart) {
        struct ListNode *temp1 = firstPart->next;
        struct ListNode *temp2 = secondPart->next;
        firstPart->next = secondPart;
        secondPart-> next = temp1;
        firstPart = temp1;
        secondPart = temp2;
    }
}
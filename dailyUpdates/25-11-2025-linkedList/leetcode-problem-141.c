/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
bool hasCycle(struct ListNode *head) {
    struct ListNode* slowPointer = head;
    struct ListNode* fastPointer = head;

    while (fastPointer != NULL && fastPointer->next!=NULL) {
            slowPointer = slowPointer->next;
            fastPointer = fastPointer->next->next;
            
            if (slowPointer == fastPointer) {
                return true;
            }
        }
    return false;
}
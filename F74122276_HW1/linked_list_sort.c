#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

// Split the linked list into two parts
void splitList(Node *head, Node **firstHalf, Node **secondHalf)
{
    /*
    if (head == NULL) return;

    Node *slow = head;
    Node *fast = head->next;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    
    *firstHalf = head;
    *secondHalf = slow->next; 
    slow->next = NULL;
    */
    asm volatile(
        /*
        Block A (splitList), which splits the linked list into two halves
        */
        "beq %[head], x0, return \n\t"        // if(head == NULL) return 
        "mv t0, %[head] \n\t"                 // t0 = slow = head
        "ld t1, 8(%[head]) \n\t"              // t1 = fast = head->next
        "beq t1, x0, return \n\t"             // if(head->next == NULL) return

        "Loop: \n\t"
            "beq t1, x0, break \n\t"          // if(fast == NULL) break
            "ld t2, 8(t1) \n\t"               // t2 = fast->next
            "beq t2, x0, break \n\t"          // if(fast->next == NULL) break 

            "ld t0, 8(t0) \n\t"               // slow = slow->next
            "ld t1, 16(t1) \n\t"              // fast = fast->next->next
            "j Loop \n\t"

        "break: \n\t"
            "sd %[head], 0(%[firstHalf]) \n\t" // *firstHalf = head
            "ld t3, 8(t0) \n\t"                // t3 = slow->next
            "sd t3, 0(%[secondHalf]) \n\t"     // *secondHalf = slow->next
            "sd x0, 8(t0) \n\t"                // slow->next = NULL

        "j end\n\t"

        "return: \n\t"
            "sd %[head], 0(%[firstHalf]) \n\t" // *firstHalf = head
            "sd x0, 0(%[secondHalf]) \n\t"     // *secondHalf = NULL

        "end: \n\t"

        : 
        : [head] "r"(head), [firstHalf] "r"(firstHalf), [secondHalf] "r"(secondHalf)
        : "t0", "t1", "t2", "t3"
    );
}

// Merge two sorted linked lists
Node *mergeSortedLists(Node *a, Node *b)
{
    Node *result = NULL;
    Node *tail = NULL;
    /*
    if (a == NULL) return b;
    if (b == NULL) return a;
    while (a != NULL && b != NULL) {
        Node *temp = NULL;
        if (a->data <= b->data) {
            temp = a;
            a = a->next;
        } else {
            temp = b;
            b = b->next;
        }

        if(!result) {
            result = temp;
            tail = temp;
        } else {
            tail->next = temp;
            tail = temp;
        }
    }
    if (a) tail->next = a; 
    if (b) tail->next = b; 
    */
    asm volatile(
        /*
        Block B (mergeSortedList), which merges two sorted lists into one
        */
        "beq %[a], x0, return_b \n\t"        // a == NULL
        "beq %[b], x0, return_a \n\t"        // b == NULL

        "loop: \n\t"
            "beq %[a], x0, tail_b \n\t"
            "beq %[b], x0, tail_a \n\t"

            "ld t0, 0(%[a]) \n\t"            // t0 = a->data
            "ld t1, 0(%[b]) \n\t"            // t1 = b->data
            "ble t0, t1, smaller_a \n\t"

            "mv t2, %[b] \n\t"               // t2 = temp = b
            "ld %[b], 8(%[b]) \n\t"          // b = b->next
            "j merge \n\t"

        "smaller_a: \n\t"
            "mv t2, %[a] \n\t"               // t2 = temp = a
            "ld %[a], 8(%[a]) \n\t"          // a = a->next

        "merge: \n\t"
            "beq %[result], x0, first \n\t"
            "sd t2, 8(%[tail]) \n\t"         // tail->next = temp
            "mv %[tail], t2 \n\t"            // tail = temp
            "j loop \n\t"

        "first: \n\t"
            "mv %[result], t2 \n\t"          // result = temp
            "mv %[tail], t2 \n\t"            // tail = temp
            "j loop \n\t"
        
        "tail_a: \n\t"
            "sd %[a], 8(%[tail]) \n\t"       // tail->next = a
            "j finish \n\t"

        "tail_b: \n\t"
            "beq %[b], x0, finish \n\t"
            "sd %[b], 8(%[tail]) \n\t"       // tail->next = b
            "j finish \n\t"

        "return_a: \n\t"
            "mv %[result], %[a] \n\t"
            "j finish \n\t"

        "return_b: \n\t"
            "mv %[result], %[b] \n\t"

        "finish: \n\t"

        : [result] "+r"(result), [tail] "+r"(tail), [a] "+r"(a), [b] "+r"(b)
        :
        : "t0", "t1", "t2"
    );

    return result;
}

// Merge Sort function for linked list
Node *mergeSort(Node *head)
{
    if (!head || !head->next)
        return head; // Return directly if there is only one node

    Node *firstHalf, *secondHalf;
    splitList(head, &firstHalf,
              &secondHalf); // Split the list into two sublists

    firstHalf = mergeSort(firstHalf);   // Recursively sort the left half
    secondHalf = mergeSort(secondHalf); // Recursively sort the right half

    return mergeSortedLists(firstHalf, secondHalf); // Merge the sorted sublists
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return 1;
    }
    int list_size;
    fscanf(input, "%d", &list_size);
    Node *head = (list_size > 0) ? (Node *)malloc(sizeof(Node)) : NULL;
    Node *cur = head;
    for (int i = 0; i < list_size; i++) {
        fscanf(input, "%d", &(cur->data));
        if (i + 1 < list_size)
            cur->next = (Node *)malloc(sizeof(Node));
        cur = cur->next;
    }
    fclose(input);

    // Linked list sort
    head = mergeSort(head);

    cur = head;
    while (cur) {
        printf("%d ", cur->data);
        /*
        cur = cur->next; 
        */
        asm volatile(
            /*
            Block C (Move to the next node), which updates the pointer to
            traverse the linked list
            */
            "ld %[cur], 8(%[cur]) \n\t" // t0 = cur->next

            :[cur] "+r"(cur)
            :
            :
        );
    }
    printf("\n");
    return 0;
}
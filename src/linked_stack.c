#include <stdio.h>
#include <stdlib.h>

#include "../Includes/linked_stack.h"


void peek(struct node_t *head) {
    struct node_t *temp = head;
    printf("\n");
    while (temp != NULL) {
        if (temp->token.type == IS_OP) {
            printf("%c :: ", temp->token.val);
            temp = temp->next;
        } else {
            printf("%d :: ", temp->token.val);
            temp = temp->next;
        }
    }
    printf("\n\n");
}


struct node_t *create_node(struct token_t token) {
    struct node_t *temp = malloc(sizeof(struct node_t));
    temp->token = token;
    temp->next = NULL;
    return temp;
}


struct node_t *push(struct token_t token, struct node_t **head) {
    struct node_t *new = create_node(token);
    new->next = *head;
    *head = new;
    return new;
}


void reverse(struct node_t **head) {
    struct node_t *prev = NULL;
    struct node_t *current = *head;
    struct node_t *next = NULL;
    while (current != NULL) {
    // Store next
        next = current->next;
 
        // Reverse current node's pointer
        current->next = prev;
 
        // Move pointers one position ahead.
        prev = current;
        current = next;
    }
    *head = prev;
}


struct token_t pop(struct node_t **head) {
    struct token_t popped = (*head)->token;
    *head = (*head)->next;
    return popped;
}

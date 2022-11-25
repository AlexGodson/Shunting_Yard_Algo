#ifndef _Linked_Stack
#define _Linked_Stack

#define MAX_DIGITS 8

#define _e 2.7182818285
#define _PI 3.141597653

#define NO_TYPE 0
#define IS_OP 1
#define IS_NUM 2

struct token_t {
    int val;
    int type;
    int prec;
    int assoc;
    struct token_t (*eval)(float a, float b);
};

struct node_t {
    struct token_t token;
    struct node_t *next;
};

void peek(struct node_t *head);
struct node_t *create_node(struct token_t token);
struct node_t *push(struct token_t token, struct node_t **head);
void reverse(struct node_t **head);
struct token_t pop(struct node_t **head);

#endif
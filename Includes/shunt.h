#ifndef _shunt_
#define _shunt_

#include "linked_stack.h"

#define ASSOC_N 0
#define ASSOC_L 1
#define ASSOC_R 2

#define MAX_EQ_SZ 256

#define TRUE  1
#define FALSE 0

struct operator_t {
    int op;
    int prec;
    int assoc;
    struct token_t (*eval)(float a, float b);
};

struct token_t _sum(float a, float b);
struct token_t _minus(float a, float b);
struct token_t _mult(float a, float b);
struct token_t _divide(float a, float b);
struct token_t _mod(float a, float b);
struct token_t _exp(float a, float b);
/* TODO 
float _sqrt(float a);
float _sin(float a);
float _cos(float a);
float _tan(float a);
*/

// abstraction of opening a file
FILE *_fopen(char *filename);

// Outputs the contents of a linked list to a text file
void push_to_file(struct node_t *out_stack);

// references the output text file in "file_name" against the solution
int check_answer(char *file_name);

// returns the next token (as an int), and the type of token (operator or digit)
struct token_t get_token(FILE *fptr);

// Evaluates the Expression and returns the answer
int shunt_eval(FILE *fptr, struct node_t **op_stack, struct node_t **outstack);

// turns the in-fix expression and returns
// the post-fix expression as a linked list
int shunt_exp(FILE *fptr, struct node_t **op_stack, struct node_t **out_stack);

#endif
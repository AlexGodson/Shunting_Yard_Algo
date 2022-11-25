#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "../Includes/shunt.h"

struct operator_t op[] = {
    {'^', 9,  ASSOC_R, _exp},
    {'*', 8,  ASSOC_L, _mult},
    {'/', 8,  ASSOC_L, _divide},
    {'%', 8,  ASSOC_L, _mod},
    {'+', 5,  ASSOC_L, _sum},
    {'-', 5,  ASSOC_L, _minus},
    {'(', 0,  ASSOC_N, NULL},
    {')', 0,  ASSOC_N, NULL}
};

struct token_t _sum(float a, float b) {
    struct token_t res = {
        b + a,
        IS_NUM,
        0,
    };
    return res;
}
struct token_t _minus(float a, float b) {
    struct token_t res = {
        b - a,
        IS_NUM,
        0,
    };
    return res;
}
struct token_t _mult(float a, float b) {
    struct token_t res = {
        b * a,
        IS_NUM,
        0,
    };
    return res;
}
struct token_t _exp(float a, float b) {
    struct token_t res = {
        powf(b, a),
        IS_NUM,
        0,
    };
    return res;
}
struct token_t _divide(float a, float b) {
    if (!a) { printf("Dividing by Zero\n"); exit(-1); }
    else {
        struct token_t res = {
            b / a,
            IS_NUM,
            0,
        };
    return res;
    }
}
struct token_t _mod(float a, float b) {
    if (!a) { printf("Dividing by Zero\n"); exit(-1);}
    else {
        struct token_t res = {
        fmodf(b, a),
        IS_NUM,
        0,
        };
    return res;
    }
}


// abstraction of opening a file
FILE *_fopen(char *filename) {
    char filepath[15] = "INPUTS/";
    strcat(filepath, filename);
    FILE *fptr = fopen(filepath, "r");
    if (fptr == NULL) {
        printf("ERROR: could not open file");
        printf("Location: shunt.c, _fopen()\n");
        exit(-1);
    }
    return fptr;
}


// Outputs the data from the linked list to a text file
void push_to_file(struct node_t *out_stack) {
    FILE *fptr = fopen("output.txt", "w");
    if (fptr == NULL) {
        printf("ERROR: Could not open file\n");
        printf("location: shunt.c, push_to_file()\n");
        exit(-1);
    }
    struct token_t token;
    while (out_stack != NULL) {
        token = pop(&out_stack);
        if (token.type == IS_OP) {
            fprintf(fptr, "%c", token.val);
        } else if (token.type == IS_NUM) {
            fprintf(fptr, "%d", token.val);
        }
    }
    fclose(fptr);
}


// references the output text file in "file_name" against the solution
// returns TRUE if they are equal, FALSE otherwise
int check_answer(char *input) {
    char ANS[17] = "ANSWERS/";
    strcat(ANS, input);

    FILE *answer_fptr, *output_fptr;
    output_fptr = fopen("output.txt", "r");
    answer_fptr = fopen(ANS, "r");

    if (output_fptr == NULL || answer_fptr == NULL) {
        printf("ERROR: Could not open the file\n");
        printf("Location: shunt.c, check_answer()\n");
        return -1;
    }

    char answer_str[MAX_EQ_SZ], output_str[MAX_EQ_SZ];
    fgets(answer_str, MAX_EQ_SZ, answer_fptr);
    fclose(answer_fptr);
    fgets(output_str, MAX_EQ_SZ, output_fptr);
    fclose(output_fptr);
    
    return strcmp(answer_str, output_str) ? 0 : 1;
}


// returns the next token (as an int), and the type of token (operator or digit)
struct token_t get_token(FILE *fptr) {
    struct token_t token = {fgetc(fptr), 0};
    // If the next token is a digit, check the next token for a digit too.
    // adding to an array digit by digit (max length MAX_DIGITS)
    // to be returned as an int
    if (0 <= (token.val-'0') && (token.val-'0') <= 9) {
        char digits[MAX_DIGITS] = {token.val, 0};
        int digits_place = 1;
        struct token_t next_token = {fgetc(fptr), NO_TYPE};
        if (0 <= (next_token.val-'0') && (next_token.val-'0') <= 9) {
            while (0 <= (next_token.val-'0') && (next_token.val-'0') <= 9) {
                token = next_token;
                next_token.val = fgetc(fptr);
                digits[digits_place] = token.val;
                digits_place++;
                // if the digits array overflows we have a digit that could cause an
                // integer overflow, throwing an error in that case
                if (digits_place >= MAX_DIGITS) {
                    printf("ERROR: Digit Overflow at Digit: %d", atoi(digits));
                    exit(-1);
                }
            }
        }
        // returning the "next_token" to the file stream to not lose a token
        ungetc(next_token.val, fptr);
        token.val = atoi(digits);
        token.type = IS_NUM;
        return token;;
    }
    // Looping over the operator stack to check token against operator type
    // returning the token if found
    for (int i = 0; i < sizeof(op)/sizeof(op[0]); ++i) {
        if (op[i].op == token.val) {
            token.type = IS_OP;
            token.prec = op[i].prec;
            token.assoc = op[i].assoc;
            token.eval = (op[i]).eval;
            return token;
        }
    }
    // If token is not a valid operator or a number:
    // A token of NULL type is returned, and it is ignored
    return token;
}


/*

// returns the integer result of the equation in the file
// (assuming no errors)
int shunt_eval(FILE *fptr, struct node_t **op_stack, struct node_t **out_stack) {
    struct token_t token;
    while (token.val != EOF) {
        token = get_token(fptr);
        
        if (token.type == IS_NUM) {
            push(token, out_stack);
        }
        if (token.type == IS_OP) {
            if (token.val == '(') {
                push(token, op_stack);
            }
            else if (token.val == ')') {
                while ((*op_stack)->token.val != '(') {
                    struct token_t popped_token = pop(op_stack);
                    struct token_t tok_1 = pop(out_stack), tok_2 = pop(out_stack);
                    struct token_t res = popped_token.eval(tok_1.val, tok_2.val);
                    printf("result: %d", res.val);
                    push(popped_token, out_stack);
                }
                pop(op_stack);
            }
            else if ((*op_stack) == NULL || token.prec > (*op_stack)->token.prec) {
                push(token, op_stack);
            }
            else if (token.prec <= (*op_stack)->token.prec) {
                if (token.val == ')') {
                    printf("ERROR: Invalid Expression, mismatched parenthesis\n");
                    exit(-1);
                }
                while ((*op_stack) != NULL && (token.prec <= (*op_stack)->token.prec)) {
                    struct token_t popped_token = pop(op_stack);
                    if (popped_token.val == '(') {
                        printf("ERROR: Invalid Expression, mismatched parenthesis\n");
                        exit(-1);
                    }
                    struct token_t tok_1 = pop(out_stack), tok_2 = pop(out_stack);
                    struct token_t res = popped_token.eval(tok_1.val, tok_2.val);
                    printf("result: %d", res.val);
                    push(popped_token, out_stack);
                }
                push(token, op_stack);
            }
        }
    }

    while ((*op_stack) != NULL) {
        struct token_t popped_token = pop(op_stack);
        if (popped_token.val == '(') {
            printf("ERROR: Invalid Expression, mismatched parenthesis\n");
            exit(-1);
        }
        struct token_t tok_1 = pop(out_stack), tok_2 = pop(out_stack);
        struct token_t res = popped_token.eval(tok_1.val, tok_2.val);
        printf("result: %d", res.val);
        push(popped_token, out_stack);
    }
    reverse(out_stack);
    return (*out_stack)->token.val;
}

*/

// pushes the result of the conversion to a linked list (result is backwards)
int shunt_exp(FILE *fptr, struct node_t **op_stack, struct node_t **out_stack) {
    struct token_t token = {0, NO_TYPE};
    while (token.val != EOF) {
        token = get_token(fptr);
        
        if (token.type == IS_NUM) {
            push(token, out_stack);
        }
        if (token.type == IS_OP) {
            if (token.val == '(') {
                push(token, op_stack);
            }
            else if (token.val == ')') {
                while ((*op_stack)->token.val != '(') {
                    struct token_t popped_token = pop(op_stack);
                    push(popped_token, out_stack);
                }
                pop(op_stack);
            }
            else if ((*op_stack) == NULL || token.prec > (*op_stack)->token.prec) {
                push(token, op_stack);
            }
            else if (token.prec <= (*op_stack)->token.prec) {
                if (token.val == ')') {
                    printf("ERROR: Invalid Expression, mismatched parenthesis\n");
                    exit(-1);
                }
                while ((*op_stack) != NULL && (token.prec <= (*op_stack)->token.prec)) {
                    struct token_t popped_token = pop(op_stack);
                    if (popped_token.val == '(') {
                        printf("ERROR: Invalid Expression, mismatched parenthesis\n");
                        exit(-1);
                    }
                    push(popped_token, out_stack);
                }
                push(token, op_stack);
            }
        }
    }

    while ((*op_stack) != NULL) {
        struct token_t popped_token = pop(op_stack);
        if (popped_token.val == '(') {
            printf("ERROR: Invalid Expression, mismatched parenthesis\n");
            exit(-1);
        }
        push(popped_token, out_stack);
    }
    reverse(out_stack);
    return TRUE;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Includes/linked_stack.h"
#include "../Includes/shunt.h"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Incorrect format\n");
        printf("calculator <filepath>\n");
        exit(-1);
    }

    FILE *fptr;
    char *input = argv[1];
    printf("file: %s", argv[1]);
    fptr = _fopen(input);

    // Number and Operator stacks for Shunting Yard Algorithm
    struct node_t *out_stack = NULL;
    struct node_t *op_stack = NULL;

    int is_expression = shunt_exp(fptr, &op_stack, &out_stack);

    //Printing the stacks for Debugging
    printf("Output Stack:\n");
    peek(out_stack);

    // Pushing the output stack to an output file
    push_to_file(out_stack);

    // Comparing the output to the desred answer
    if (check_answer(input) == -1) {
        printf("Answers file could not be opened, nothing to compare to\n");
    } else if (is_expression && check_answer(input)) {
        printf("The Answer is correct :)\n\n");
    } else if (is_expression && !check_answer(input)){
        printf("The Answer was incorrect :(\n");
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include "lib_matrix.h"

int main() {
    int line_count;
    printf("Hello, World!\n");
    printf("Enter the number of line\n");
    while (!getInt(&line_count)){
        printf("Try again\n");
    }
    printf("Line count -> %d\n", line_count);
    Matrix *matrix = create_matrix(line_count);
    printf("The matrix you entered:\n");
    write_matrix(matrix);
    printf("Vector b:\n");
    int *b = create_vector(matrix);
    write_vector(b, matrix->line_count);
    clear_memory(matrix);
    free(b);
    return 0;
}

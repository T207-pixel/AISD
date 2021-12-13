#include <stdio.h>
#include <stdlib.h>
#include "lib_matrix.h"

int getInt(int *a){
    int n;
    do{
        n = scanf("%d", a);
        if (n < 0) return 0;
        if (n == 0){
            printf("Error!\n");
            scanf("%*c");
        }
    } while (n == 0);
    return 1;
}


Matrix* create_matrix(int line_count){
    Matrix* matrix = (Matrix*) calloc(1, sizeof(Matrix));
    matrix->line_count = line_count;
    matrix->line = (Line*)calloc(line_count, sizeof(Line));
    for (int i=0; i < line_count; i++){
        printf("Enter the number of element in this line\n");
        int element_count;
        while (!getInt(&element_count)){
            printf("Try again\n");
        }
        matrix->line[i].element_count = element_count;
        matrix->line[i].element = (int*)calloc(element_count, sizeof(int));
        for (int j=0; j < element_count; j++){
            int number;
            printf("Enter the number\n");
            while (!getInt(&number)){
                printf("Try again\n");
            }
            matrix->line[i].element[j] = number;
        }
    }
    return matrix;
}


int write_matrix(Matrix *matrix){
    printf("Matrix:\n");
    for (int i=0; i < matrix->line_count; i++){
        for(int j=0; j < matrix->line[i].element_count; j++){
            printf("%d ", matrix->line[i].element[j]);
        }
        printf("\n");
    }
    return 0;
}


int clear_memory(Matrix *matrix){
    printf("Clearing memory...\n");
    for (int i=0; i< matrix->line_count; i++){
        free(matrix->line[i].element);
    }
    free(matrix->line);
    free(matrix);
    return 0;
}


int sum_of_element(int a){
    int sum = 0;
    while (a > 0){
        sum += a % 10;
        a /= 10;
    }
    return sum;
};


int *create_vector(Matrix *matrix){
    int *b = (int*)calloc(matrix->line_count, sizeof(int));
    for (int i=0; i < matrix->line_count; i++){
        int first_sum = -1;
        for (int j=0; j < matrix->line[i].element_count; j++){
            if (j == 0) {
                first_sum = sum_of_element(matrix->line[i].element[j]);
                b[i] += matrix->line[i].element[j];
            } else if (sum_of_element(matrix->line[i].element[j]) == first_sum){
                b[i] += matrix->line[i].element[j];
            }
        }
    }
    return b;
}


int write_vector(int *b, int count){
    for (int i=0 ; i < count; i++) printf("%d ", b[i]);
    printf("\n");
    return 0;
}

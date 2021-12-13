#ifndef LAB_1_LIB_MATRIX_H
#define LAB_1_LIB_MATRIX_H
typedef struct _Line{
    int element_count;
    int *element;
} Line;
typedef struct _Matrix{
    int line_count;
    Line *line;
} Matrix;
int getInt(int *a);
Matrix* create_matrix(int line_count);
int write_matrix(Matrix* matrix);
int clear_memory(Matrix* matrix);
int sum_of_element(int a);
int *create_vector(Matrix *matrix);
int write_vector(int *b, int count);
#endif //LAB_1_LIB_MATRIX_H

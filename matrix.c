#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h> /* for ENOSYS */

int matrix_allocate(matrix_t *m, int rows, int columns) {
    /* implement the function here ... */
    // use dynamic memory allocation to allocate a matrix
    // the 'm' in parameter is the pointer of matrix_t
    // m->content:
    m->rows = rows;
    m->columns = columns;
    m->content = malloc(m->rows * sizeof (int *));
    if (m->content == NULL) return -1;
    for(int i=0; i<m->rows; i++) {
        m->content[i] = malloc(m->columns * sizeof(int));
        if (m->content[i] == NULL) return -1;
    }
    return 0;

//    return -ENOSYS; /* ENOSYS is a  standard error code that means 'function
//                       not implemented'. We use it to force the failure of tests
//                       while the functions are not written. Make sure to remove
//                       it when you start to work on a function */
}

void matrix_free(matrix_t *m) {

    /* implement the function here ... */
    for(int i=0; i<m -> rows; i++) {
        free(m->content[i]);
    }
    free(m->content);
}

void matrix_init_zeros(matrix_t *m) {
    /* implement the function here ... */
    for (int i = 0; i < m -> rows; i++){
        for (int j = 0; j < m -> columns; j++) {
            m->content[i][j] = 0;
        }
    };
}

int matrix_init_identity(matrix_t *m){
    /* implement the function here ... */
    for (int i = 0; i < m -> rows; i++){
        for (int j = 0; j < m -> columns; j++) {
            if (i==j){
                m->content[i][j] = 1;
            } else{
                m->content[i][j] = 0;
            }
        }
        if (m->content[i] == NULL) return -1;
    };

//    return -ENOSYS;
    return 0;
}

void matrix_init_rand(matrix_t *m) {
    /* implement the function here ... */
    for (int i = 0; i < m -> rows; i++){
        for (int j = 0; j < m->columns; j++) {
            int r = (rand()%201)-100; //generate numbers from -100 to 100
            m->content[i][j] = r;
        }
    };
}

int matrix_equal(matrix_t *m1, matrix_t *m2) {
    /* implement the function here ... */
    /* computers operate on 0’s(False) and 1’s(True) */
    int equal = 1;
    if(m1->rows == m2->rows && m1->columns == m2->columns){
        for (int i = 0; i < m1 -> rows; i++){
            for (int j = 0; j < m1 -> columns; j++) {
                if (m1->content[i][j] != m2->content[i][j]){
                    equal = 0;
                    break;
                }
            }
        }
    }
    else {
        equal = 0;
    }
//    return -ENOSYS;
    return equal;
}

int matrix_sum(matrix_t *m1, matrix_t *m2, matrix_t *result) {
    /* implement the function here ... */
    int success = 0;
    matrix_allocate(result,m1 -> rows,m1 -> columns);
    if(m1->rows == m2->rows && m1->columns == m2->columns){
        for (int i = 0; i < m1 -> rows; i++){
            for (int j = 0; j < m1 -> columns; j++) {
                    result->content[i][j] = m1->content[i][j] + m2->content[i][j];
                }
            if (result->content[i] == NULL) {
                success = -1;
                break;
            }
            }
        }
    else {
        success = -1;
    }
//    return -ENOSYS;
    return success;
}

int matrix_scalar_product(matrix_t *m, int scalar, matrix_t *result) {
    /* implement the function here ... */
    int success = 0;
    matrix_allocate(result,m -> rows,m -> columns);
    for (int i = 0; i < m -> rows; i++){
        for (int j = 0; j < m -> columns; j++) {
            result->content[i][j] = m->content[i][j]*scalar;
        }
        if (result->content[i] == NULL) {
            success = -1;
            break;
        }
    }
//    return -ENOSYS;
    return success;
}

int matrix_transposition(matrix_t *m, matrix_t *result) {
    /* implement the function here ... */
    int success = 0;
    matrix_allocate(result,m -> columns,m -> rows);
    for (int i = 0; i < m -> rows; i++){
        for (int j = 0; j < m -> columns; j++) {
            result->content[j][i] = m->content[i][j];
        }
    }
    for (int i = 0; i < result -> rows; i++) {
        if (result->content[i] == NULL) {
            success = -1;
            break;
        }
    }

//    return -ENOSYS;
    return success;
}

int matrix_product(matrix_t *m1, matrix_t *m2, matrix_t *result) {
    /* implement the function here ... */
    int success = 0;
    matrix_allocate(result,m1 -> rows,m2 -> columns);
    if(m1->columns == m2->rows){
        for(int i = 0; i < m1 -> rows; i++) {
            for (int j = 0; j < m2->columns; j++) {
                int sum = 0;
                for (int k = 0; k < m1->columns; k++) {
                    sum += m1->content[i][k] * m2->content[k][j];
                }
                result->content[i][j] = sum;
            }
            if(result -> content[i]==NULL){
                success = -1;
            }
        }
    }
    else {
        success = -1;
    }
//    return -ENOSYS;
    return success;
}

int matrix_dump_file(matrix_t *m, char *output_file) {
    /* implement the function here ... */
    FILE *fptr = fopen(output_file,"w");
    int success = 0;
    if(fptr){
        for (int i = 0; i < m -> rows; i++){
            for (int j = 0; j < m -> columns; j++) {
                fprintf(fptr, "%d ",m->content[i][j]);
            }
            if (m->content[i]==NULL){
                success = -1;
                break;
            }
            else{
                fprintf(fptr, "\n");
            }
        }
        fclose(fptr);
    }
    else{
        success = -1;
    }
//    return -ENOSYS;
    return success;
}

int matrix_allocate_and_init_file(matrix_t *m, char *input_file) {
    /* implement the function here ... */
    int success = 0;
    FILE * fptr = fopen(input_file,"r");
    int row = 0;
    int col = 0;
    char ch ;
    char last = '\n';
    if(fptr==NULL){
        success = -1;
        return success;
    }

    /*Count the number of rows in each file, empty rows are ignored*/
    while (!feof(fptr)){
        ch = fgetc(fptr);
        if ((ch == '\n'|| feof(fptr)) && last != '\n'){
            row += 1;
        } // Increment count if this character is newline
        last = ch;
    }
    fclose(fptr);
//    printf("row: %d\n",row);
    /*Count the number of columns in each file, empty space between two numbers are ignored*/
    int n;
    FILE * fptr_2 = fopen(input_file,"r");
    while (fscanf(fptr_2,"%d ", &n)==1){
        col +=1;
    }
    fclose(fptr_2);
    col = col / row;
//    printf("col: %d\n",col);



    /*Read from file and construct matrix object*/
    matrix_allocate(m, row, col);
    int row_matrix = 0;
    int col_matrix = 0;
    FILE * fptr_3 = fopen(input_file,"r");
    while (fscanf(fptr_3,"%d ", &n)==1){
//        printf("%d\n",n);
//        printf("%d\n",count);
//        printf("count_col: %d\n",count%col);
        if (row_matrix < row && (col_matrix % col!=0||col_matrix==0)){
            m->content[row_matrix][col_matrix] = n;
//            printf("row_matrix: %d\n",row_matrix);
//            printf("col_matrix: %d\n",col_matrix);
//            printf("content: %d\n",m->content[row_matrix][col_matrix]);
            col_matrix += 1;
            continue;
        }
        else if(row_matrix < (row-1) && col_matrix % col==0){
//            printf("n: %d\n",n);
            row_matrix += 1;
            col_matrix = 0;
            m->content[row_matrix][col_matrix] = n;
            col_matrix+=1;
//            printf("row_matrix: %d\n",row_matrix);
//            printf("col_matrix: %d\n",col_matrix);
//            printf("content: %d\n",m->content[row_matrix][col_matrix]);
            continue;
        }

    }
    fclose(fptr_3);

    /*Check if constructing matrix object is successful*/
    for(int i = 0; i < row; i++){
        if (m -> content[i]==NULL){
            success = -1;
        }
    }
//    return -ENOSYS;
    return success;
}

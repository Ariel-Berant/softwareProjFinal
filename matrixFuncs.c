/*
 Created by ariel on 10/9/2024.
*/

#include "symnmf.h"

/*
 * Initialize a zero matrix.
 * @param rows int number of rows of the matrix
 * @param cols int number of columns of the matrix
 * @return a pointer to a zero matrix with the desired number of rows and columns
 * */
mat *initMatrix(int rows, int cols){
    int i = 0;
    double *p = calloc(rows*cols, sizeof(double));
    double **matrix = calloc(rows, sizeof(double*));
    mat *m = malloc(sizeof(mat));

    if(p == NULL || matrix == NULL || m == NULL){
        if (p != NULL) {
            free(p);
        }
        if (matrix != NULL) {
            free(matrix);
        }
        if (m != NULL) {
            free(m);
        }
        return NULL;
    }

    for(; i < rows; i++){
        matrix[i] = p + i*cols;
    }

    m->whole = p;
    m->data = matrix;
    m->rows = rows;
    m->cols = cols;

    return m;
}

/*
 * Free matrix.
 * @param m pointer to a matrix
 * @return void
 * */
void freeMatrix(mat *m){
    free(m->whole);
    free(m->data);
    free(m);
    m = NULL;
}

/*
 * Calculate Euclidean distance of two vectors(rows) in a matrix.
 * @param vectMat pointer to a matrix of vectors
 * @param i int the row num of the 1st vector
 * @param j in the row num of the 2nd vector
 * @return the Euclidean distance between the ith and jth row
 * */
double euclideanDist(mat *vectMat, int i, int j) {
    double sum = 0;
    int k;

    for(k = 0; k < vectMat->cols; k++){
        sum += pow((vectMat->data[i][k] - vectMat->data[j][k]), 2);/*sum of all cells squared*/
    }

    return sqrt(sum);
}

/*
 * Multiply a matrix by a diagonal matrix from the left.
 * @param reg a matrix pointer to a regular matrix
 * @param diag a matrix pointer to a diagonal matrix
 * @return void
 * */
void multDDGL(mat *reg, mat *diag){
    int i, j;

    for (i = 0; i < reg->rows; i++) {
        for (j = 0; j < reg->cols; j++) {
            reg->data[i][j] *= 1/(sqrt(diag->data[i][i]));/*cell calc, every cell in A is positive*/
        }
    }
}

/*
 * Multiply a matrix by a diagonal matrix from the right.
 * @param reg a matrix pointer to a regular matrix
 * @param diag a matrix pointer to a diagonal matrix
 * @return void
 * */
void multDDGR(mat *reg, mat *diag){
    int i, j;

    for (i = 0; i < reg->cols; i++) {
        for (j = 0; j < reg->rows; j++) {
            reg->data[j][i] *= 1/(sqrt(diag->data[i][i]));/*cell calc, every cell in A is positive*/
        }
    }
}

/*
 * Copy a matrix.
 * @param m a matrix pointer to a matrix
 * @return res a matrix pointer to the copy of m
 * */
mat *copyMat(mat *m){
    int i, j;
    mat *res = initMatrix(m->rows, m->cols);
    if(res == NULL){
        return NULL;
    }

    for(i = 0; i < m->rows; i++){
        for(j = 0; j < m->cols; j++){
            res->data[i][j] = m->data[i][j];/*copy cell*/
        }
    }

    return res;
}

/*
 * Print a matrix.
 * @param m a matrix pointer to a matrix
 * @return void
 * */
void printMatrix(mat *m){
    int i, j;
    for(i = 0; i < m->rows; i++){/*print every column of every row*/
        for(j = 0; j < m->cols - 1; j++){
            printf("%.4f,", m->data[i][j]);
        }
        printf("%.4f\n", m->data[i][j]);
    }
}

/*
 * Multiply two matrices.
 * @param mat1 a matrix pointer to the first matrix
 * @param mat2 a matrix pointer to the second matrix
 * @return res a matrix pointer to the result of the multiplication
 * */
mat *multMat(mat *mat1, mat *mat2){
    int i, j;
    mat *res = initMatrix(mat1->rows, mat2->cols);/*init result matrix*/
    if(res == NULL){
        return NULL;
    }

    for(i = 0; i < mat1->rows; i++){
        for(j = 0; j < mat2->cols; j++){
            res->data[i][j] = multRowByCol(mat1, mat2, i, j);/*C_i,j = A_i*B_j*/
        }
    }

    return res;
}

/*
 * Calculate the transpose of a matrix.
 * @param m pointer to a matrix
 * @return res pointer to the transposed matrix
 * */
mat *calcTranspose(mat *m){
    int i, j;
    mat *res = initMatrix(m->cols, m->rows);
    if(res == NULL){
        return NULL;
    }

    for(i = 0; i < m->rows; i++){
        for(j = 0; j < m->cols; j++){
            res->data[j][i] = m->data[i][j];/*transpose cell, (A^T)_i,j = A_j,i*/
        }
    }

    return res;
}

/*
 * Multiply a row in mat1 by a column in mat2
 * @param mat1 a matrix pointer to the first matrix
 * @param mat2 a matrix pointer to the second matrix
 * @param row an int row number
 * @param col an int column number
 * @return res an int the result of the calculation
 * */
double multRowByCol(mat *mat1, mat *mat2, int row, int col){
    double res = 0;
    int i;

    for(i = 0; i < mat1->cols; i++){
        res += mat1->data[row][i] * mat2->data[i][col];/*calculate sum of a_i,j*b_j,i for A, B matrices*/
    }

    return res;
}

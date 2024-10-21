/*
 Created by ariel on 10/9/2024.
*/

#include "symnmf.h"

mat *initMatrix(int rows, int cols){/*create a 0 matrix*/
    int i = 0;
    double *p = calloc(rows*cols, sizeof(double));
    double **matrix = calloc(rows, sizeof(double*));
    mat *m = malloc(sizeof(mat));

    if(p == NULL || matrix == NULL || m == NULL){
        free(p);
        free(matrix);
        free(m);
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

void freeMatrix(mat *m){/*free matrix*/
    free(m->whole);
    free(m->data);
    free(m);
    m = NULL;
}

void multDDGL(mat *reg, mat *diag){/*multiply matrix by diagonal matrix from left*/
    int i, j;

    for (i = 0; i < reg->rows; i++) {
        for (j = 0; j < reg->cols; j++) {
            reg->data[i][j] *= 1/(sqrt(diag->data[i][i]));/*cell calc, every cell in A is positive*/
        }
    }
}

void multDDGR(mat *reg, mat *diag){/*multiply matrix by diagonal matrix from right*/
    int i, j;

    for (i = 0; i < reg->cols; i++) {
        for (j = 0; j < reg->rows; j++) {
            reg->data[j][i] *= 1/(sqrt(diag->data[i][i]));/*cell calc, every cell in A is positive*/
        }
    }
}

mat *copyMat(mat *m){/*copy matrix*/
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

void printMatrix(mat *m){/*print matrix*/
    int i, j;
    for(i = 0; i < m->rows; i++){/*print every column of every row*/
        for(j = 0; j < m->cols - 1; j++){
            printf("%.4f,", m->data[i][j]);
        }
        printf("%.4f\n", m->data[i][j]);
    }
}

mat *multMat(mat *mat1, mat *mat2){/*multiply matrices: mat1*mat2*/
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

mat *calcTranspose(mat *m){/*calculate transpose of matrix*/
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

double multRowByCol(mat *mat1, mat *mat2, int row, int col){/*multiplies fow in mat1 by column in mat2*/
    double res = 0;
    int i;

    for(i = 0; i < mat1->cols; i++){
        res += mat1->data[row][i] * mat2->data[i][col];/*calculate sum of a_i,j*b_j,i for A, B matrices*/
    }

    return res;
}

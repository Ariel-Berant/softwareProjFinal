/*
 Created by ariel on 10/7/2024.
*/

#ifndef SOFTWAREPROJFINAL_SYMNMF_H
#define SOFTWAREPROJFINAL_SYMNMF_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define EPSILON 1e-4
#define MAX_ITER 300
#define BETA 0.5
#endif /*SOFTWAREPROJFINAL_SYMNMF_H*/

struct matrix
{
    double *whole;
    double **data;
    int rows;
    int cols;
};
typedef struct matrix mat;

/*vector/cords functions in vectorFuncs*/

/*check if file exists and/or opens*/
int exists(char *file_name);
/*calculate euclidean distance between two vectorsof same dimension*/
double euclideanDist(mat *vectMat, int i, int j);
/*get number of rows from file*/
int getRows(char *file_name);
/*get number of cols from file*/
int getCols(char *file_name);
/*turn file to matrix*/
mat *fileToMatrix(char *file_name);

/*matrix functions in matrixFuncs file*/

/*initialize empty matrix m of dimensions rows x cols*/
mat *initMatrix(int rows, int cols);
/*free function for struct matrix*/
void freeMatrix(mat *m);
/*multiply matrix by diagonal matrix from left*/
void multDDGL(mat *reg, mat *diag);
/*multiply matrix by diagonal matrix from right*/
void multDDGR(mat *reg, mat *diag);
/*copy matrix m to new matrix*/
mat *copyMat(mat *m);
/*calculate transpose of matrix*/
mat *calcTranspose(mat *m);
/*multiply the rowth row of mat1 by colth column of mat2*/
double multRowByCol(mat *mat1, mat *mat2, int row, int col);
/*multiply matrices: mat1*mat2*/
mat *multMat(mat *mat1, mat *mat2);
/*print matrix*/
void printMatrix(mat *m);

/*symNMF algorithm related functions(directly, or function related)*/

/*calculate Frobenius norm squared of two matrices*/
double calcFrobNormSq(mat *h, mat *nextH);
/*calculate a cell in the similarity matrix*/
double symCellCalc(mat *vectMat, int i, int j);
/*calculate and return similarity matrix from vectors*/
mat *symCalc(mat *vectMat);
/*calculate and return D matrix from A matrix*/
mat *ddgCalc(mat *vectMat);
/*calculate and return W matrix from A matrix*/
mat *normCalc(mat *vectMat);
/*calculate denominator(abomination of a matrix) for nextH calculation*/
mat *calcAbomination(mat *m);
/*calculate matrix for the next iteration*/
mat *calcNextIter(mat *numerator, mat *denominator, mat *h);
/*check if H has converged or iteration limit has been reached*/
int checkFinish(mat *h, mat *nextH, int currIter);
/*free all matrices in symnmf calculation*/
void freeAll(mat *numer, mat *denom, mat *currH, mat *nextH);
/*calculate symnmf matrix*/
mat *symnmfCalc(mat *h, mat *w);

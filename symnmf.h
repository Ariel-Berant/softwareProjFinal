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
#endif //SOFTWAREPROJFINAL_SYMNMF_H

struct matrix
{
    double *whole;
    double **data;
    int rows;
    int cols;
};

struct cord
{
    double value;
    struct cord *next;
};

struct vector
{
    struct vector *next;
    struct cord *cords;
};

typedef struct vector vector;
typedef struct cord cord;
typedef struct matrix mat;

void freeCords(cord *crd);
void freeData(vector *vect);
double euclideanDist(vector *vect1, vector *vect2);
mat *initMatrix(int rows, int cols);
void freeMatrix(mat *m);
int getRows(char *file_name);
int getCols(char *file_name);
cord *initCords(int cols);
vector *initVectors(int cols, int rows);
vector *createVectors(char *file_name);
void multDDGL(mat *reg, mat *diag);
void multDDGR(mat *reg, mat *diag);
void printMatrix(mat *m);
double symCellCalc(vector *v1, vector *v2);
mat *symCalc(char *file_name);
mat *ddgCalc(char *file_name);
mat *normCalc(char *file_name);
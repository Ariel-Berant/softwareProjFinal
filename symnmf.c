/*
 Created by ariel on 10/7/2024.
*/

#include "symnmf.h"

/*
 * Calculate cell in matrix using given formula.
 * @param vecMat a pointer to a matrix of vectors
 * @param i int the row num of the 1st vector
 * @param j in the row num of the 2nd vector
 * */
double symCellCalc(mat *vectMat, int i, int j){
    double dist = euclideanDist(vectMat, i, j), val;
    val = exp(-(pow(dist, 2)/2));
    return val;
}

/*
 * Calculate the Frobenius norm of a matrix.
 * @param h a pointer to the matrix of this iteration
 * @param nextH a pointer to the matrix of this iteration
 * @return sum the Forbenius norm squared
 * */
double calcFrobNormSq(mat *h, mat *nextH){
    int i, j;
    double sum = 0;

    for(i = 0; i < h->rows; i++){
        for(j = 0; j < h->cols; j++){
            sum += pow((h->data[i][j] - nextH->data[i][j]), 2);/*sum of all cells squared*/
        }
    }

    return sum;
}

/*
 * Calculate abomination of matrix (i.e (A*A^T)*A).
 * @param m a pointer to a matrix
 * @return res a pointer to the resulting matrix
 * */
mat *calcAbomination(mat *m){
    mat *first = multMat(m, calcTranspose(m)), *res;/*A*A^T*/
    if(first == NULL){
        return NULL;
    }

    res = multMat(first, m);/*(A*A^T)*A*/
    if(res == NULL){
        freeMatrix(first);
        return NULL;
    }

    return res;
}

/*
 * Calculate next iteration of H according to the SymNMF algorithm.
 * @param numerator a matrix pointer to the numerator calculated according to the SymNMF algorithm
 * @param denominator a matrix pointer to the numerator calculated according to the SymNMF algorithm
 * @param h a matrix pointer to the current matrix H
 * @return nextH a pointer to the matrix of the next iteration
 * */
mat *calcNextIter(mat *numerator, mat *denominator, mat *h) {
    int i, j;
    double res;

    mat *nextH;
    nextH = initMatrix(h->rows, h->cols);
    if(nextH == NULL){
        return NULL;
    }

    for (i = 0; i < nextH->rows; ++i) {
        for (j = 0; j < nextH->cols; ++j) {
            res = numerator->data[i][j] / denominator->data[i][j];/*calculate numerator/denominator*/
            res = (1 - BETA) + (BETA * res);/*calculate BETA*res + (1-BETA)*/
            nextH->data[i][j] = h->data[i][j] * res;/*calculate next H_i,j*/
        }
    }

    return nextH;
}

/*
 * Check if H has converged.
 * @param h a matrix pointer to the matrix of the current iteration
 * @param nextH a matrix pointer to the matrix of the next iteration
 * @param currIter an int - the iteration number
 * @return 0 if converged, 1 if not converged
 * */
int checkFinish(mat *h, mat *nextH, int currIter){

    if(calcFrobNormSq(h, nextH) < EPSILON){/*if Frobenius norm is less than EPSILON, converged*/
        return 1;
    }
    if(currIter >= MAX_ITER){/*if reached MAX_ITER, not converged*/
        return 1;
    }

    return 0;
}

/*
 * Free all matrices in SymNMF calculation.
 * @param numerator a matrix pointer to the numerator calculated according to the SymNMF algorithm
 * @param denominator a matrix pointer to the numerator calculated according to the SymNMF algorithm
 * @param h a matrix pointer to the matrix of the current iteration
 * @param nextH a matrix pointer to the matrix of the next iteration
 * @return void
 * */
void freeAll(mat *numer, mat *denom, mat *currH, mat *nextH){
    if(numer != NULL){
        freeMatrix(numer);
    }
    if(denom != NULL){
        freeMatrix(denom);
    }
    if(currH != NULL){
        freeMatrix(currH);
    }
    if(nextH != NULL){
        freeMatrix(nextH);
    }
}

/*
 * Calculate the similarity matrix of a given set of vectors (in matrix form).
 * @param vectMat a matrix of vectors (size: N*d)
 * @return m the similarity matrix A  of the input vectors (size:N*N)
 * */
mat *symCalc(mat *vectMat){
    int i = 0, j, rows = vectMat->rows;
    mat *m;

    m = initMatrix(rows, rows);/*creates a n*n matrix*/
    if(m == NULL){
        return NULL;
    }

    for(; i < rows; i++){
        for(j = i + 1; j < rows; j++){/*calculates top half and copies to bottom half*/
            m->data[i][j] = symCellCalc(vectMat, i, j);
            m->data[j][i] = m->data[i][j];
        }
    }

    return m;
}

/*
 * A wrapper function for symCalc.
 * @param file_name a character pointer to the name of the file that contains the input vectors
 * @return void
 * */
void sym(char *file_name){
    mat *m = fileToMatrix(file_name), *s;/*create A matrix from vectors*/
    if(m == NULL){
        printf("An Error Has Occurred\n");
        exit(1);
    }
    s = symCalc(m);/*create sym matrix from A matrix*/
    if (s == NULL)
    {
        printf("An Error Has Occurred\n");
        freeMatrix(m);
        exit(1);
    }
    
    printMatrix(s);
    freeMatrix(m);
    freeMatrix(s);
}

/* Calculate the diagonal degree matrix of a given set of vectors (in matrix form).
 * @param vectMat a pointer to the matrix of input vectors (size: N*d)
 * @return d the diagonal degree matrix D of the input vectors (size:N*N)
 * */
mat *ddgCalc(mat *vectMat){
    int i, j;
    double sum;

    /*create A matrix from vectors*/
    mat *m = symCalc(vectMat), *d;
    if(m == NULL){
        return NULL;
    }

    d = initMatrix(m->rows, m->cols);/*creates a n*n matrix*/
    if(d == NULL){
        return NULL;
    }

    for(i = 0; i < m->rows; i++){
        sum = 0;
        for(j = 0; j < m->cols; j++){/*sums all cells in row*/
            sum += m->data[i][j];
        }
        d->data[i][i] = sum;/*sets diagonal cell to sum*/
    }
    freeMatrix(m);
    return d;
}

/*
 * A wrapper function for ddgCalc.
 * @param file_name a character pointer to the name of the file that contains the input vectors
 * @return void
 * */
void ddg(char *file_name){
    /*create D matrix from A matrix*/
    mat *m = fileToMatrix(file_name), *d;
    if(m == NULL){
        printf("An Error Has Occurred\n");
        exit(1);
    }
    d = ddgCalc(m);
    if (d == NULL)
    {
        printf("An Error Has Occurred\n");
        freeMatrix(m);
        exit(1);
    }
    
    printMatrix(d);
    freeMatrix(d);
    freeMatrix(m);
}

/* Calculate the normalized similarity matrix of a given set of vectors (in matrix form).
 * @param vectMat a pointer to the matrix of input vectors (size: N*d)
 * @return m the normalized similarity matrix W of the input vectors (size:N*N)
 * */
mat *normCalc(mat *vectMat){
    /*create A matrix from vectors*/
    mat *m = symCalc(vectMat), *d;
    if(m == NULL){
        return NULL;
    }
    /*create D matrix from A*/
    d = ddgCalc(vectMat);
    if(d == NULL){
        freeMatrix(m);
        return NULL;
    }

    multDDGL(m, d);/*multiply A by D^(-1/2) from left*/
    multDDGR(m, d);/*multiply A by D^(-1/2) from right*/

    freeMatrix(d);
    return m;
}

/*
 * A wrapper function for normCalc.
 * @param file_name a character pointer to the name of the file that contains the input vectors
 * @return void
 * */
void norm(char *file_name){
    /*create W matrix from A matrix*/
    mat *m = fileToMatrix(file_name), *n;
    if(m == NULL){
        printf("An Error Has Occurred\n");
        exit(1);
    }
    n = normCalc(m);
    if (n == NULL)
    {
        printf("An Error Has Occurred\n");
        freeMatrix(m);
        exit(1);
    }
    printMatrix(n);
    freeMatrix(m);
    freeMatrix(n);
}

/*
 * Perform the calculations of the symmetric Non-negative Matrix Factorization algorithm.
 * @param h a matrix pointer to the matrix of the current iteration (size: N*k)
 * @param w a matrix pointer to the normalized similarity matrix W (size:N*N)
 * @return nextH a matrix pointer to the matrix of the next iteration (size: N*k)
 * */
mat *symnmfCalc(mat *h, mat *w){
    int iter = 0;
    mat *numer, *denom, *currH, *nextH;
    nextH = copyMat(h);/*copy H matrix*/
    if(nextH == NULL){
        return NULL;
    }

    do {
        if(iter != 0){
            freeMatrix(currH);/*free previous H*/
        }
        currH = nextH;
        numer = multMat(w, currH);/*calculate numerator*/
        denom = calcAbomination(currH);/*calculate abomination(aka denominator)*/

        if(numer == NULL || denom == NULL){
            freeAll(numer, denom, currH, nextH);
            return NULL;
        }

        nextH = calcNextIter(numer, denom, currH);/*calculate nextH*/
        if(nextH == NULL){
            freeAll(numer, denom, currH, nextH);
            return NULL;
        }
        iter++;
    } while(!checkFinish(currH, nextH, iter));/*check if H has converged*/

    freeMatrix(denom);
    freeMatrix(numer);
    freeMatrix(currH);
    return nextH;
}

/*
 * Classify calculation type.
 * @param str a character pointer to the calculation type
 * @return 1 if str = "sym", 2 if "ddg" = 2, 3 if str = "norm", 0 otherwise
 * */
int classifyCalc(char* str){
    if(strcmp(str, "sym") == 0){
        return 1;
    }
    if(strcmp(str, "ddg") == 0){
        return 2;
    }
    if(strcmp(str, "norm") == 0){
        return 3;
    }
    return 0;
}

/*
 * Prints matrix according to the desired goal.
 * */
int main(int argc, char *argv[]) {
    if(argc != 3){
        printf("An Error Has Occurred\n");
        return 1;
    }
    if (exists(argv[2]) == 0)
    {
        printf("An Error Has Occurred\n");
        return 1;
    }
    
    switch (classifyCalc(argv[1])){
        case 1:
            sym(argv[2]);
            break;
        case 2:
            ddg(argv[2]);
            break;
        case 3:
            norm(argv[2]);
            break;
        default:
            printf("An Error Has Occurred\n");
            break;

    }
    return 0;
}

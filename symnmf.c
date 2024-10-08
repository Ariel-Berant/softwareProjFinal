/*
 Created by ariel on 10/7/2024.
*/

#include "symnmf.h"

void freeCords(cord *crd) {/*frees cords list*/
    if (crd->next != NULL) {
        freeCords(crd->next);
    }
    free(crd);
    crd = NULL;
}

void freeData(vector *vect) {/*frees vectors list*/
    if (vect->next != NULL) {
        freeData(vect->next);
    }
    freeCords(vect->cords);
    free(vect);
    vect = NULL;
}

double euclideanDist(vector *vect1, vector *vect2) {/* calculates euclidean distance */
    double sum = 0;
    cord *v1c = vect1->cords, *v2c = vect2->cords;

    while ((v1c != NULL) && (v2c != NULL)) {
        sum += pow((v1c->value - v2c->value), 2);
        v1c = v1c->next;
        v2c = v2c->next;
    }

    return sqrt(sum);
}

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

int getRows(char *file_name){/*get number of rows in file*/
    FILE *file = fopen(file_name, "r");
    int rows = 0;
    char c;

    if(file == NULL){
        return -1;
    }

    while((c = fgetc(file)) != EOF){
        if(c == '\n'){
            rows++;
        }
    }

    fclose(file);
    return rows;
}

int getCols(char *file_name){/*get number of columns in file*/
    FILE *file = fopen(file_name, "r");
    int cols = 1;
    char c;

    if(file == NULL){
        return -1;
    }

    while((c = fgetc(file)) != '\n'){
        if(c == ','){
            cols++;
        }
    }

    fclose(file);
    return cols;
}

cord *initCords(int cols){/*create new cord list*/
    int i = 0;
    cord *head = malloc(sizeof(cord)), *curr = head;

    if(head == NULL){
        free(head);
        return NULL;
    }

    for(; i < cols - 1; i++){/*create cords for rest of columns*/
        curr->next = malloc(sizeof(cord));
        if(curr->next == NULL){
            freeCords(head);
            return NULL;
        }
        curr = curr->next;
        curr->next = NULL;/*sets ending cord's next to NULL*/
    }

    return head;
}

vector *initVectors(int cols, int rows){/*create new vector list*/
    int i = 0;
    vector *head = malloc(sizeof(vector)), *curr = head;
    cord *cordCurr;

    if(head == NULL){
        free(head);
        return NULL;
    }

    for (; i < rows; ++i) {
        cordCurr = initCords(cols);/*init and check current cords*/
        if(cordCurr == NULL){
            freeData(head);
            return NULL;
        }
        curr->cords = cordCurr;

        if(i < rows - 1){/*if not in last rows, create next row*/
            curr->next = malloc(sizeof(vector));
            if(curr->next == NULL){
                freeData(head);
                return NULL;
            }
            curr = curr->next;
        }
        curr->next = NULL;/*sets ending vector's next to NULL*/
    }

    return head;
}

vector *createVectors(char *file_name){/*create vectors from file*/
    int cols = getCols(file_name), rows = getRows(file_name);/*get rows and columns of vector matrix*/
    FILE *file = fopen(file_name, "r");
    vector *headVec = initVectors(cols, rows), *currVec = headVec;/*init empty vectors*/
    cord *currCord = currVec->cords;
    char c;
    double n;

    if(file == NULL || headVec == NULL){/*checks proper init*/
        if(headVec == NULL){
            freeData(headVec);
        }
        return NULL;
    }

    while(fscanf(file,"%lf%c", &n, &c) == 2){/*reads cord and next char(line end or ,)*/
        currCord->value = n;
        if(c == ','){/*add cord if not at end of line*/
            currCord->value = n;
            currCord = currCord->next;
        } else if(c == '\n'){/*add cord if at end of line*/
            currCord->value = n;
            currVec = currVec->next;
            currCord = currVec->cords;
        }
    }

    fclose(file);
    return headVec;
}

void multDDGL(mat *reg, mat *diag){/*multiply matrix by diagonal matrix from left*/
    int i, j;
    double sum;

    for (i = 0; i < reg->rows; i++) {
        for (j = 0; j < reg->cols; j++) {
            reg->data[i][j] *= 1/(sqrt(diag->data[i][i]));/*cell calc, every cell in A is positive*/
        }
    }
}

void multDDGR(mat *reg, mat *diag){/*multiply matrix by diagonal matrix from right*/
    int i, j;
    double sum;

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

double multRowByCol(mat *mat1, mat *mat2, int row, int col){
    double res = 0;
    int i;

    for(i = 0; i < mat1->cols; i++){
        res += mat1->data[row][i] * mat2->data[i][col];/*calculate sum of a_i,j*b_j,i for A, B matrices*/
    }

    return res;
}

double calcFrobNormSq(mat *h, mat *nextH){/*calculate Frobenius norm of matrix*/
    int i, j;
    double sum = 0;

    for(i = 0; i < h->rows; i++){
        for(j = 0; j < h->cols; j++){
            sum += pow((h->data[i][j] - nextH->data[i][j]), 2);/*sum of all cells squared*/
        }
    }

    return sum;
}

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

mat *calcAbomination(mat *m){/*calculate abomination of matrix(i.e (A*A^T)*A)*/
    mat *first = multMat(m, calcTranspose(m));/*A*A^T*/
    if(first == NULL){
        return NULL;
    }

    mat *res = multMat(first, m);/*(A*A^T)*A*/
    if(res == NULL){
        freeMatrix(first);
        return NULL;
    }

    return res;
}

mat *calcNextIter(mat *numerator, mat *denominator, mat *h) {/*calculate next iteration of H*/
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

int checkFinish(mat *h, mat *nextH, int currIter){/*check if H has converged*/
    int i, j;
    double diff;

    if(calcFrobNormSq(h, nextH) < EPSILON){/*if Frobenius norm is less than EPSILON, converged*/
        return 1;
    }
    if(currIter >= MAX_ITER){/*if reached MAX_ITER, not converged*/
        return 1;
    }

    return 0;
}

void freeAll(mat *numer, mat *denom, mat *currH, mat *nextH){/*free all matrices*/
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

mat *calcSymnmf(mat *h, mat *w){/*calculate symnmf matrix*/
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
        if(numer == NULL){
            freeAll(numer, denom, currH, nextH);
            return NULL;
        }

        denom = calcAbomination(currH);/*calculate abomination(aka denominator)*/
        if(denom == NULL){
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

void printMatrix(mat *m){/*print matrix*/
    int i, j;
    for(i = 0; i < m->rows; i++){/*print every column of every row*/
        for(j = 0; j < m->cols; j++){
            printf("%.4f,", m->data[i][j]);
            if(j == m->cols - 1){
                printf("%.4f\n", m->data[i][j]);/*print end of row*/
            }
        }
    }
}

double symCellCalc(vector *v1, vector *v2){/*calculate cell in matrix using given formula*/
    double dist = euclideanDist(v1, v2), val;
    val = exp(-(pow(dist, 2)/2));
    return val;
}

mat *symCalc(char *file_name){/*turn vector mat(n*m) to A(n*n)*/
    vector *v = createVectors(file_name);/*create vectors from file*/
    if(v == NULL){
        return NULL;
    }

    int i = 0, j, rows = getRows(file_name);
    vector *v1 = v, *v2 = v->next;
    cord *c = NULL;

    mat *m = initMatrix(rows, rows);/*creates a n*n matrix*/
    if(m == NULL){
        freeData(v);
        return NULL;
    }

    for(; i < rows; i++){
        for(j = i + 1; j < rows; j++){/*calculates top half and copies to bottom half*/
            if(i == j){
                m->data[i][j] = 0;
            } else {
                m->data[i][j] = symCellCalc(v1, v2);
                m->data[j][i] = m->data[i][j];
            }
            v2 = v2->next;
        }
        v1 = v1->next;
        v2 = v1->next;
    }

    freeData(v);
    return m;
}

void sym(char *file_name){/*sym wrap*/
    mat *m = symCalc(file_name);/*create A matrix from vectors*/
    if(m == NULL){
        printf("An Error Has Occurred");
    }
    printMatrix(m);
    freeMatrix(m);
}

mat *ddgCalc(char *file_name){/*turns A(n*n) to D(n*n)*/
    int i, j;
    double sum;

    /*create A matrix from vectors*/
    mat *m = symCalc(file_name);
    if(m == NULL){
        return NULL;
    }

    mat *d = initMatrix(m->rows, m->cols);/*creates a n*n matrix*/
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

    return d;
}

void ddg(char *file_name){/*ddg wrap*/
    /*create D matrix from A matrix*/
    mat *d = ddgCalc(file_name);
    if(d == NULL){
        printf("An Error Has Occurred");
    }
    printMatrix(d);
    freeMatrix(d);
}

mat *normCalc(char *file_name){/*turns A(n*n) to W(n*n)*/
    /*create A matrix from vectors*/
    mat *m = symCalc(file_name);
    if(m == NULL){
        return NULL;
    }
    /*create D matrix from A*/
    mat *d = ddgCalc(file_name);
    if(d == NULL){
        freeMatrix(m);
        return NULL;
    }

    multDDGL(m, d);/*multiply A by D^(-1/2) from left*/
    multDDGR(m, d);/*multiply A by D^(-1/2) from right*/

    freeMatrix(d);
    return m;
}

void norm(char *file_name){/*norm wrap*/
    /*create W matrix from A matrix*/
    mat *m = normCalc(file_name);
    if(m == NULL){
        printf("An Error Has Occurred");
    }
    printMatrix(m);
    freeMatrix(m);
}



mat *symnmfCalc(mat *m, int k){/*calculate symnmf matrix*/
    return NULL;
}

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

int main(int argc, char *argv[]) {
    if(argc != 3){
        printf("An Error Has Occurred");
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
            printf("An Error Has Occurred");
            break;

    }
    return 0;
}

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

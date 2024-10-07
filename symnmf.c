/*
 Created by ariel on 10/7/2024.
*/

#include "symnmf.h"

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
    cord *head = malloc(sizeof(cord)), *curr = head;

    if(head == NULL){
        free(head);
        return NULL;
    }

    for(int i = 0; i < cols - 1; i++){/*create cords for rest of columns*/
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

mat *symCalc(vector *v, int rows, int cols){/*turn vector mat(n*m) to A(n*n)*/
    int i = 0, j;
    vector *v1 = v, *v2 = v->next;
    cord *c = NULL;
    mat *m = initMatrix(rows, rows);/*creates a n*n matrix*/

    if(m == NULL){
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
    }

    return m;
}

void sym(char *file_name){/*sym wrap*/
    vector *v = createVectors(file_name);/*create vectors from file*/
    if(v == NULL){
        printf("An Error Has Occurred");
    }
    mat *m = symCalc(v, getRows(file_name), getCols(file_name));/*create A matrix from vectors*/
    if(m == NULL){
        freeData(v);
        printf("An Error Has Occurred");
    }
    printMatrix(m);
    freeData(v);
    freeMatrix(m);
}

int main(int argc, char *argv[]) {
    if(argc != 3){
        printf("An Error Has Occurred");
        return 1;
    }
    if(strcmp(argv[1], "sym") == 0){
        sym(argv[2]);
    } else {
        printf("An Error Has Occurred");
    }
    return 0;
}

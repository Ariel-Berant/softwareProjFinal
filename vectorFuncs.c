/*
 Created by ariel on 10/9/2024.
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
    cord *currCord = currVec->cords;/*init empty cords*/
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
            if (currVec != NULL) {
                currCord = currVec->cords;
            }
        }
    }

    fclose(file);
    return headVec;
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

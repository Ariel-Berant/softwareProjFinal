/*
 Created by ariel on 10/9/2024.
*/

#include "symnmf.h"

int exists(char *file_name){/*check if file exists and/or opens*/
    FILE *file = fopen(file_name, "r");/*try to open file*/
    if(file == NULL){
        return 0;
    }
    fclose(file);
    return 1;
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

mat *fileToMatrix(char *file_name){/*turn file to matrix*/
    /*get rows and columns of vector matrix, and init variables*/
    int rows = getRows(file_name), cols = getCols(file_name), i, j;
    double n;
    FILE *file = fopen(file_name, "r");
    mat *m;
    char c;
    if (rows == -1 || cols == -1 || file == NULL) {
        if (file != NULL) {
            fclose(file);
        }
        return NULL;
    }
    
    m = initMatrix(rows, cols);/*initiate matrix*/
    if(m == NULL){
        return NULL;
    }

    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            fscanf(file,"%lf%c", &n, &c);/*read number and next char*/
            m->data[i][j] = n;
        }
    }
    
    fclose(file);
    return m;
}

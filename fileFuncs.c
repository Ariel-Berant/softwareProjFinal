/*
 Created by ariel on 10/9/2024.
*/

#include "symnmf.h"

/*
 * Check if file exists and/or opens.
 * @param file_name a character pointer to the name of the file
 * @return 1 if file exists and can be opened, 0 otherwise
 * */
int exists(char *file_name){
    FILE *file = fopen(file_name, "r");/*try to open file*/
    if(file == NULL){
        return 0;
    }
    fclose(file);
    return 1;
}

/*
 * Get number of rows in file of input vectors.
 * @param file_name a character pointer to the name of the file that contains the input vectors
 * @return rows number of rows N (i.e. the number of input vectors)
 * */
int getRows(char *file_name){
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

/*
 * Get number of columns in file of input vectors.
 * @param file_name a character pointer to the name of the file that contains the input vectors
 * @return rows number of columns d (i.e. the dimension of each vector)
 * */
int getCols(char *file_name){
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

/*
 * Turn file of input vectors into a matrix.
 * @param file_name a character pointer to the name of the file that contains the input vectors
 * @return m pointer to a matrix with entries from the data, NULL if unsuccessful
 * */
mat *fileToMatrix(char *file_name){
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

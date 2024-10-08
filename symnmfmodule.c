/*
 Created by ariel on 10/7/2024.
*/

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "symnmf.h"

static PyObject* rowToPy(double *row, int cols){
    int i = 0;
    PyObject *pyList = PyList_New(0);
    for(; i < cols; i++){
        PyList_Append(pyList, PyFloat_FromDouble(row[i]));
    }
    return pyList;
}

static PyObject* matrixToPy(mat *m){
    int i = 0;
    PyObject *pyList = PyList_New(0);
    for(; i < m->rows; i++){
        PyList_Append(pyList, rowToPy(m->data[i], m->cols));
    }
    return pyList;
}

static PyObject* symCalcPy(PyObject *self, PyObject *args){
    char *file_name;
    if(!PyArg_ParseTuple(args, "s", &file_name)){/*parse desired file name*/
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }
    mat *m = symCalc(file_name);/*calculate the matrix using function*/
    if(m == NULL){
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }
    PyObject *pyMatrix = matrixToPy(m);/*turn matrix into python matrix*/
    freeMatrix(m);
    return pyMatrix;
}

static PyObject* ddgCalcPy(PyObject *self, PyObject *args){
    char *file_name;
    if(!PyArg_ParseTuple(args, "s", &file_name)){/*parse desired file name*/
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }
    mat *m = ddgCalc(file_name);/*calculate the matrix using function*/
    if(m == NULL){
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }
    PyObject *pyMatrix = matrixToPy(m);/*turn matrix into python matrix*/
    freeMatrix(m);
    return pyMatrix;
}

static PyObject* normCalcPy(PyObject *self, PyObject *args){
    char *file_name;
    if(!PyArg_ParseTuple(args, "s", &file_name)){/*parse desired file name*/
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }
    mat *m = normCalc(file_name);/*calculate the matrix using function*/
    if(m == NULL){
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }
    PyObject *pyMatrix = matrixToPy(m);/*turn matrix into python matrix*/
    freeMatrix(m);
    return pyMatrix;
}

static PyMethodDef symnmfMethods[] = {
    {"sym",
     (PyCFunction)symCalcPy,
     METH_VARARGS,
     "Calculate the sym matrix"
     },
    {"ddg",
     (PyCFunction)ddgCalcPy,
     METH_VARARGS,
     "Calculate the ddg matrix"
     },
    {"norm",
     (PyCFunction)normCalcPy,
     METH_VARARGS,
     "Calculate the norm matrix"
     },
    {"symnmf",
     (PyCFunction)symnmfCalcPy,
     METH_VARARGS,
     "Calculate the norm matrix"
    },
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef symnmfmodule = {
    PyModuleDef_HEAD_INIT,
    "symnmf",
    "A module that calculates the sym, ddg ,norm and symnmf matrices",
    -1,
    symnmfMethods
};

PyMODINIT_FUNC PyInit_symnmf(void){
    PyObject *m;
    m = PyModule_Create(&symnmfmodule);
    if(!m){
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }
    return PyModule_Create(&symnmfmodule);
}
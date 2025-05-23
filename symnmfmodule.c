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

static mat* pyToMatrix(PyObject *pyMatrix){/*turns python list of list to matrix*/
    int rows = PyList_Size(pyMatrix);
    int cols = PyList_Size(PyList_GetItem(pyMatrix, 0));/*gets row and column legths*/
    int i = 0, j;
    PyObject *row;

    mat *m = initMatrix(rows, cols);
    if(m == NULL){
        return NULL;
    }

    for(; i < rows; i++){
        row = PyList_GetItem(pyMatrix, i);/*get current row*/
        j = 0;/*restart to beginning of row*/
        for(; j < cols; j++){
            /*copy list of lists cell to matrix cell*/
            m->data[i][j] = PyFloat_AsDouble(PyList_GetItem(row, j));
        }
    }

    return m;
}

static PyObject* symCalcPy(PyObject *self, PyObject *args){/*python wrapper for sym*/
    PyObject *pyMatrix;
    mat *m, *s;
    if(!PyArg_ParseTuple(args, "O", &pyMatrix)){/*parse desired matrix*/
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }
    m = pyToMatrix(pyMatrix);/*try to initialize the matrix*/
    if(m == NULL){/*in case of failure*/
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }
    s = symCalc(m);/*calculate the matrix using function*/
    if(s == NULL){
        freeMatrix(m);
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }

    pyMatrix = matrixToPy(s);/*turn matrix into python matrix*/
    freeMatrix(m);
    freeMatrix(s);
    return pyMatrix;
}

static PyObject* ddgCalcPy(PyObject *self, PyObject *args){/*python wrapper for ddg*/
    PyObject *pyMatrix;
    mat *m, *d;
    if(!PyArg_ParseTuple(args, "O", &pyMatrix)){/*parse desired matrix*/
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }
    m = pyToMatrix(pyMatrix);/*try to initialize the matrix*/
    if(m == NULL){/*in case of failure*/
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }
    d = ddgCalc(m);/*calculate the matrix using function*/
    if(d == NULL){
        freeMatrix(m);
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }
    pyMatrix = matrixToPy(d);/*turn matrix into python matrix*/
    freeMatrix(m);
    freeMatrix(d);
    return pyMatrix;
}

static PyObject* normCalcPy(PyObject *self, PyObject *args){/*python wrapper for norm*/
    PyObject *pyMatrix;
    mat *m, *n;
    if(!PyArg_ParseTuple(args, "O", &pyMatrix)){/*parse desired matrix*/
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }
    m = pyToMatrix(pyMatrix);/*try to initialize the matrix*/
    if(m == NULL){/*in case of failure*/
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }
    n = normCalc(m);/*calculate the matrix using function*/
    if(n == NULL){
        freeMatrix(m);
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }
    pyMatrix = matrixToPy(n);/*turn matrix into python matrix*/
    freeMatrix(m);
    freeMatrix(n);
    return pyMatrix;
}

static PyObject* symnmfCalcPy(PyObject *self, PyObject *args){/*python wrapper for symnmf*/
    PyObject *pyH, *pyW;
    if(!PyArg_ParseTuple(args, "OO", &pyH, &pyW)){/*parse given matrices*/
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }

    mat *h = pyToMatrix(pyH);/*try to initialize the matrices*/
    mat *w = pyToMatrix(pyW);

    if(h == NULL || w == NULL){
        if(h != NULL){/*free either matrix if only one init failed*/
            freeMatrix(h);
        }
        if(w != NULL){
            freeMatrix(w);
        }
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }

    mat *m = symnmfCalc(h, w);/*calculate the matrix using function*/
    if(m == NULL){/*in case of failure*/
        freeMatrix(h);
        freeMatrix(w);
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }
    PyObject *pyMatrix = matrixToPy(m);/*turn matrix into python matrix*/
    freeMatrix(m);
    freeMatrix(h);
    freeMatrix(w);
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
    "symnmf_c",
    "A module that calculates the sym, ddg ,norm and symnmf matrices",
    -1,
    symnmfMethods
};

PyMODINIT_FUNC PyInit_symnmf_c(void){
    PyObject *m;
    m = PyModule_Create(&symnmfmodule);
    if(!m){
        PyErr_SetString(PyExc_ValueError, "An Error Has Occurred");
        return NULL;
    }
    return PyModule_Create(&symnmfmodule);
}
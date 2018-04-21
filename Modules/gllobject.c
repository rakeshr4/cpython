#include "Python.h"
// #include "internal/pystate.h"
#include "accu.h"

// #ifdef STDC_HEADERS
// #include <stddef.h>
// #else
// #include <sys/types.h>          /* For size_t */
// #endif

#include "gllobject.h"
#ifndef PyList_MAXFREELIST
#define PyList_MAXFREELIST 80
#endif
static PyGllObject *free_list[PyList_MAXFREELIST];
static int numfree = 0;


static PyTypeObject PyGll_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "gillessList.Gll",
    .tp_doc = "Gll objects",
    .tp_basicsize = sizeof(PyGllObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
};

static PyObject *
list_create(PyObject *self, PyObject *args) {
    int size;
    PyArg_ParseTuple(args, "i", &size); 
    PyGllObject *op;
// #ifdef SHOW_ALLOC_COUNT
//     static int initialized = 0;
//     if (!initialized) {
//         Py_AtExit(show_alloc);
//         initialized = 1;
//     }
// #endif

    if (size < 0) {
        PyErr_BadInternalCall();
        return NULL;
    }
    if (numfree) {
        // ;
        numfree--;
        op = free_list[numfree];
        _Py_NewReference((PyObject *)op);
        // #ifdef SHOW_ALLOC_COUNT
        //     count_reuse++;
        // #endif
    } else {
        op = PyObject_GC_New(PyGllObject, &PyGll_Type);
        if (op == NULL)
            return NULL;
// #ifdef SHOW_ALLOC_COUNT
//         count_alloc++;
// #endif
    }
    if (size <= 0)
        op->ob_item = NULL;
    else {
        op->ob_item = (PyObject **) PyMem_Calloc(size, sizeof(PyObject *));
        if (op->ob_item == NULL) {
            Py_DECREF(op);
            return PyErr_NoMemory();
        }
    }
    Py_SIZE(op) = size;
    op->allocated = size;
    return (PyObject *) op;
    // return Py_BuildValue("s", "list create!");
}

static PyObject *
list_append(PyObject *self, PyObject *args) {
    return Py_BuildValue("s", "list append!");
}

static PyObject *
list_extend(PyObject *self, PyObject *args) {
    return Py_BuildValue("s", "list extend!");
}

static PyObject *
set_item(PyObject *self, PyObject *args)
{
    Py_ssize_t i;
    PyObject *gll, *newitem;
    PyArg_ParseTuple(args, "OnO", &gll, &i, &newitem); 
    PyObject **p;
    if (!PyGll_Check(gll)) {
        Py_XDECREF(newitem);
        PyErr_BadInternalCall();
        return NULL;
    }
    if (i < 0 || i >= Py_SIZE(gll)) {
        Py_XDECREF(newitem);
        PyErr_SetString(PyExc_IndexError,
                        "list assignment index out of range");
        return NULL;
    }
    p = ((PyListObject *)gll) -> ob_item + i;
    Py_XSETREF(*p, newitem);
    return Py_BuildValue("i", 0);

}

static PyObject *
get_item(PyObject *self, PyObject *args) {

    Py_ssize_t i;
    PyObject *op;
    PyArg_ParseTuple(args, "On", &op, &i);
    if (!PyGll_Check(op)) {
        PyErr_BadInternalCall();
        return NULL;
    }
    if (i < 0 || i >= Py_SIZE(op)) {
        PyErr_SetString(PyExc_IndexError, "list index out of range");
        return NULL;
    }
    PyObject *item = ((PyGllObject *)op) -> ob_item[i];
    if(item == NULL) {
        Py_RETURN_NONE;
    }
    else {
        return item;
    }
}

// Module functions table.

static PyMethodDef
module_functions[] = {
    { "list_create", list_create, METH_VARARGS, "Create list." },
    { "list_append", list_append, METH_VARARGS, "list append" },
    { "list_extend", list_extend, METH_VARARGS, "list extend" },
    { "get_item", get_item, METH_VARARGS, "Get item in list" },
    { "set_item", set_item, METH_VARARGS, "Set item in list" },
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "gillessList",     /* m_name */
    "This is a module",  /* m_doc */
    -1,                  /* m_size */
    module_functions,    /* m_methods */
    NULL,                /* m_reload */
    NULL,                /* m_traverse */
    NULL,                /* m_clear */
    NULL,                /* m_free */
};


// This function is called to initialize the module.

PyMODINIT_FUNC 
PyInit_gillessList(void)
{
    PyObject *m = PyModule_Create(&moduledef);
    if (m == NULL)
        return NULL;
    Py_INCREF(&PyGll_Type);
    PyModule_AddObject(m, "gll", (PyObject *) &PyGll_Type);
    return m;
}
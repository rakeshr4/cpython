#include "Python.h"
// #include "internal/pystate.h"
#include "accu.h"

// #ifdef STDC_HEADERS
// #include <stddef.h>
// #else
// #include <sys/types.h>          /* For size_t */
// #endif

#include "gflobject.h"
#ifndef PyList_MAXFREELIST
#define PyList_MAXFREELIST 80
#endif
static PyGflObject *free_list[PyList_MAXFREELIST];
static int numfree = 0;


static PyTypeObject PyGfl_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    .tp_name = "gilfullList.Gfl",
    .tp_doc = "Gfl objects",
    .tp_basicsize = sizeof(PyGflObject),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_free = PyObject_GC_Del,
    .tp_alloc = PyType_GenericAlloc
};


void
PyGflObject_dealloc(PyGflObject *op)
{
    Py_ssize_t i;
    Py_TRASHCAN_SAFE_BEGIN(op)
    if (op->ob_item != NULL) {
        i = Py_SIZE(op);
        while (--i >= 0) {
            Py_XDECREF(op->ob_item[i]);
        }
        PyMem_FREE(op->ob_item);
    }
    if (numfree < PyList_MAXFREELIST && PyGfl_CheckExact(op))
        free_list[numfree++] = op;
    else
        Py_TYPE(op)->tp_free((PyObject *)op);
    Py_TRASHCAN_SAFE_END(op)
}

static PyObject *
PyGflObject_repr(PyGflObject *v)
{
    Py_ssize_t i;
    PyObject *s;
    _PyUnicodeWriter writer;

    if (Py_SIZE(v) == 0) {
        return PyUnicode_FromString("[]");
    }

    i = Py_ReprEnter((PyObject*)v);
    if (i != 0) {
        return i > 0 ? PyUnicode_FromString("[...]") : NULL;
    }

    _PyUnicodeWriter_Init(&writer);
    writer.overallocate = 1;
    /* "[" + "1" + ", 2" * (len - 1) + "]" */
    writer.min_length = 1 + 1 + (2 + 1) * (Py_SIZE(v) - 1) + 1;

    if (_PyUnicodeWriter_WriteChar(&writer, '[') < 0)
        goto error;

     // Do repr() on each element.  Note that this may mutate the list,
     //   so must refetch the list size on each iteration. 
    for (i = 0; i < Py_SIZE(v); ++i) {
        if (i > 0) {
            if (_PyUnicodeWriter_WriteASCIIString(&writer, ", ", 2) < 0)
                goto error;
        }

        s = PyObject_Repr(v->ob_item[i]);
        if (s == NULL)
            goto error;

        if (_PyUnicodeWriter_WriteStr(&writer, s) < 0) {
            Py_DECREF(s);
            goto error;
        }
        Py_DECREF(s);
    }

    writer.overallocate = 0;
    if (_PyUnicodeWriter_WriteChar(&writer, ']') < 0)
        goto error;

    Py_ReprLeave((PyObject *)v);
    return _PyUnicodeWriter_Finish(&writer);

error:
    _PyUnicodeWriter_Dealloc(&writer);
    Py_ReprLeave((PyObject *)v);
    return NULL;
}

static PyObject *
list_create(PyObject *self, PyObject *args) {
    Py_ssize_t size;
    PyArg_ParseTuple(args, "n", &size);
    PyGflObject *op = NULL;

    if (size < 0) {
        PyErr_BadInternalCall();
        return NULL;
    }
    if (numfree) {
        // ;
        numfree--;
        op = free_list[numfree];
        _Py_NewReference((PyObject *)op);
    } else {
        op = PyObject_GC_New(PyGflObject, &PyGfl_Type);
        if (op == NULL)
            return NULL;
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
}

static int
list_resize(PyGflObject *self, Py_ssize_t newsize)
{
    PyObject **items;
    size_t new_allocated, num_allocated_bytes;
    Py_ssize_t allocated = self->allocated;

     if (allocated >= newsize && newsize >= (allocated >> 1)) {
        assert(self->ob_item != NULL || newsize == 0);
        Py_SIZE(self) = newsize;
        return 0;
    }

    new_allocated = (size_t)newsize + (newsize >> 3) + (newsize < 9 ? 3 : 6);
    if (new_allocated > (size_t)PY_SSIZE_T_MAX / sizeof(PyObject *)) {
        PyErr_NoMemory();
        return -1;
    }

    if (newsize == 0)
        new_allocated = 0;
    num_allocated_bytes = new_allocated * sizeof(PyObject *);
    items = (PyObject **)PyMem_Realloc(self->ob_item, num_allocated_bytes);
    if (items == NULL) {
        PyErr_NoMemory();
        return -1;
    }
    self->ob_item = items;
    Py_SIZE(self) = newsize;
    self->allocated = new_allocated;
    return 0;
}

static int
app1_gfl(PyGflObject *self, PyObject *s)
{
    Py_ssize_t n = PyGfl_GET_SIZE(self);

    assert (s != NULL);
    if (n == PY_SSIZE_T_MAX) {
        PyErr_SetString(PyExc_OverflowError,
            "cannot add more objects to list");
        return -1;
    }

    if (list_resize(self, n+1) < 0)
        return -1;

    Py_INCREF(s);
    PyGfl_SET_ITEM(self, n, s);
    return 0;
}

static PyObject *
list_append(PyObject *self, PyObject *args) {
	PyGflObject *ob;
    PyObject *s;
    int x;
    PyArg_ParseTuple(args, "Oi", &ob, &x); 
    s = Py_BuildValue("i", x);
    if (s!= NULL) {
		return Py_BuildValue("i", app1_gfl(ob, s));
    }
    else {
        return Py_BuildValue("i", -1);
    }
}

static PyObject *
list_extend(PyObject *self, PyObject *args) {
    return Py_BuildValue("s", "list extend!");
}

static PyObject *
set_item(PyObject *self, PyObject *args)
{
    Py_ssize_t i;
    PyGflObject *gfl;
    PyObject *newitem;
    int x;

    PyArg_ParseTuple(args, "Oni", &gfl, &i, &x); 
    newitem = Py_BuildValue("i", x);
    PyObject **p;

    if (!PyGfl_Check(gfl)) {
        Py_XDECREF(newitem);
        PyErr_BadInternalCall();
        return NULL;
    }
    if (i < 0 || i >= Py_SIZE(gfl)) {
        Py_XDECREF(newitem);
        PyErr_SetString(PyExc_IndexError,
                        "list assignment index out of range");
        return NULL;
    }
    p = (gfl) -> ob_item + i;
    Py_XSETREF(*p, newitem);
    return Py_BuildValue("i", 0);
}
static PyObject *indexerr = NULL;

static PyObject *
get_item(PyObject *self, PyObject *args) {

    Py_ssize_t i;
    PyGflObject *op;
    PyArg_ParseTuple(args, "On", &op, &i);

    if (!PyGfl_Check(op)) {
        PyErr_BadInternalCall();
        return NULL;
    }

    if (i < 0 || i >= Py_SIZE(op)) {
        PyErr_SetString(PyExc_IndexError, "list index out of range");
        return NULL;
    }

    PyObject *item = (op) -> ob_item[i];
    if(item == NULL) {
        Py_RETURN_NONE;
    }
    else {
        Py_INCREF(item);
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
    "gilfullList",     /* m_name */
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
PyInit_gilfullList(void)
{
    PyObject *m = PyModule_Create(&moduledef);
    if (m == NULL)
        return NULL;
    PyGfl_Type.tp_dealloc= (destructor)PyGflObject_dealloc;
    PyGfl_Type.tp_repr = (reprfunc)PyGflObject_repr;
    Py_INCREF(&PyGfl_Type);
    PyModule_AddObject(m, "gfl", (PyObject *) &PyGfl_Type);
    return m;
}

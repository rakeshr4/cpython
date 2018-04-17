#include "Python.h"

static PyObject *
list_create(PyObject *self, PyObject *args) {
	return Py_BuildValue("s", "list create!");
}

static PyObject *
list_append(PyObject *self, PyObject *args) {
	return Py_BuildValue("s", "list append!");
}

static PyObject *
list_extend(PyObject *self, PyObject *args) {
	return Py_BuildValue("s", "list extend!");
}

// Module functions table.

static PyMethodDef
module_functions[] = {
    { "list_create", list_create, METH_VARARGS, "Create list." },
    { "list_append", list_append, METH_VARARGS, "list append" },
    { "list_extend", list_extend, METH_VARARGS, "list extend" },
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
    PyModule_Create(&moduledef);
}
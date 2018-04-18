
/* Gilless List object interface */


#ifndef Py_GLLOBJECT_H
#define Py_GLLOBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef Py_LIMITED_API
typedef struct {
    PyObject_VAR_HEAD
    /* Vector of pointers to list elements.  list[0] is ob_item[0], etc. */
    PyObject **ob_item;

    Py_ssize_t allocated;
} PyGllObject;
#endif

PyAPI_DATA(PyTypeObject) PyGll_Type;
// PyAPI_DATA(PyTypeObject) PyGllIter_Type;
// PyAPI_DATA(PyTypeObject) PyGllRevIter_Type;
// PyAPI_DATA(PyTypeObject) PySortWrapper_Type;

#define PyGll_Check(op) \
    PyType_FastSubclass(Py_TYPE(op), Py_TPFLAGS_GLL_SUBCLASS)
#define PyGll_CheckExact(op) (Py_TYPE(op) == &PyGll_Type)


/* Macro, trading safety for speed */
#ifndef Py_LIMITED_API
#define PyGll_GET_ITEM(op, i) (((PyGllObject *)(op))->ob_item[i])
#define PyGll_SET_ITEM(op, i, v) (((PyGllObject *)(op))->ob_item[i] = (v))
#define PyGll_GET_SIZE(op)    (assert(PyGll_Check(op)),Py_SIZE(op))
#define _PyGll_ITEMS(op)      (((PyGllObject *)(op))->ob_item)
#endif

#ifdef __cplusplus
}
#endif
#endif /* !Py_GLLOBJECT_H */

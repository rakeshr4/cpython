
/* Gilless List object interface */


#ifndef Py_GFLOBJECT_H
#define Py_GFLOBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef Py_LIMITED_API
typedef struct {
    PyObject_VAR_HEAD
    /* Vector of pointers to list elements.  list[0] is ob_item[0], etc. */
    PyObject **ob_item;

    Py_ssize_t allocated;
} PyGflObject;
#endif


// PyAPI_DATA(PyTypeObject) PyGflIter_Type;
// PyAPI_DATA(PyTypeObject) PyGflRevIter_Type;
// PyAPI_DATA(PyTypeObject) PySortWrapper_Type;

#define PyGfl_Check(op) \
    PyType_FastSubclass(Py_TYPE(op), Py_TPFLAGS_DEFAULT)
#define PyGfl_CheckExact(op) (Py_TYPE(op) == &PyGfl_Type)


/* Macro, trading safety for speed */
#ifndef Py_LIMITED_API
#define PyGfl_GET_ITEM(op, i) (((PyGflObject *)(op))->ob_item[i])
#define PyGfl_SET_ITEM(op, i, v) (((PyGflObject *)(op))->ob_item[i] = (v))
#define PyGfl_GET_SIZE(op)    (assert(PyGfl_Check(op)),Py_SIZE(op))
#define _PyGfl_ITEMS(op)      (((PyGflObject *)(op))->ob_item)
#endif

#ifdef __cplusplus
}
#endif
#endif /* !Py_GFLOBJECT_H */

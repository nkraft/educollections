/* Lists for demonstrating order notation.
 *
 * Copyright (c) 2013, Nicholas A. Kraft
 * All rights reserved.
 *
 * Distributed under a BSD-style license. (See accompanying file LICENSE or
 * copy at http://github.com/nkraft/educollections/LICENSE)
 */

#include <Python.h>
#include <structmember.h>
#include "_educollectionsmodule.h"


PyDoc_STRVAR(List_append_doc,
  "Adds the given item to the end of this List.");

PyDoc_STRVAR(List_clear_doc,
  "Clears this List.");

PyDoc_STRVAR(List_get_doc,
  "Returns the item at the given index in this List.");

PyDoc_STRVAR(List_insert_doc,
  "Inserts the given item at the given index in this List.");

PyDoc_STRVAR(List_prepend_doc,
  "Adds the given item to the front of this List.");

PyDoc_STRVAR(List_remove_doc,
  "Removes and returns the item at the given index in this List.");

PyDoc_STRVAR(List_set_doc,
  "Assigns the given item at the given index in this List.");

PyDoc_STRVAR(List_size_doc,
  "Returns the size of this List.");


/* ArrayList
 * Fixed-size-array-based implementation of the List interface. */

typedef struct {
  PyObject_HEAD
  Py_ssize_t capacity;
  Py_ssize_t size;
  PyObject   **data;
} ArrayList;

PyDoc_STRVAR(ArrayList_doc,
  "Fixed-size-array-based implementation of the List interface.");

/* ArrayListType.tp_new */
static PyObject *
ArrayList_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  ArrayList *self;

  self = (ArrayList *)type->tp_alloc(type, 0);
  if (self == NULL) {
    return NULL;
  }
  self->capacity = -1;
  self->size = -1;
  self->data = NULL;
  return (PyObject *)self;
}

/* ArrayListType.tp_init */
static int
ArrayList_init(ArrayList *self, PyObject *args, PyObject *kwds)
{
  PyObject *capacityobj = NULL;
  PyObject **data = NULL;
  PyObject **tmp = NULL;
  Py_ssize_t capacity = -1;
  static char *kwlist[] = {"capacity", NULL};
  int i;

  if (! PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist, &capacityobj)) {
    return -1;
  }
  if (capacityobj != NULL && capacityobj != Py_None) {
    capacity = PyLong_AsSsize_t(capacityobj);
    if (capacity == -1 && PyErr_Occurred()) {
      return -1;
    }
  }
  if (capacity < 1) {
    PyErr_SetString(PyExc_ValueError, "capacity must be greater than zero");
    return -1;
  }
  self->capacity = capacity;
  self->size = 0;
  data = PyMem_New(PyObject *, self->capacity);
  if (data == NULL) {
    PyErr_SetString(PyExc_MemoryError, "memory allocation request failed");
    return -1;
  }
  tmp = self->data;
  Py_INCREF(data);
  self->data = data;
  Py_XDECREF(tmp);
  for (i = 0; i < self->capacity; ++i) {
    Py_INCREF(Py_None);
    self->data[i] = Py_None;
  }
  return 0;
}

/* ArrayListType.tp_dealloc */
static void
ArrayList_dealloc(ArrayList *self)
{
  Py_XDECREF(self->data);
  PyMem_Del(self->data);
  Py_TYPE(self)->tp_free((PyObject*)self);
}

/* ArrayList.append(item) */
static PyObject *
ArrayList_append(ArrayList *self, PyObject *item)
{
#if defined(NDEBUG)
  if (self->size == self->capacity) {
    PyErr_SetString(PyExc_RuntimeError, "ArrayList is full (capacity == size)");
    return NULL;
  }
  if (item == NULL) {
    PyErr_SetString(PyExc_ValueError, "item == NULL");
    return NULL;
  }
#endif
  Py_XDECREF(Py_None);
  Py_INCREF(item);
  self->data[self->size] = item;
  self->size += 1;
  Py_RETURN_NONE;
}

PyDoc_STRVAR(ArrayList_capacity_doc,
  "Returns the capacity of this ArrayList.");

/* ArrayList.capacity() */
static PyObject *
ArrayList_capacity(ArrayList *self)
{
  return PyLong_FromSsize_t(self->capacity);
}

/* ArrayList.clear() */
static PyObject *
ArrayList_clear(ArrayList *self)
{
  PyObject *item;
  int i;

  for (i = 0; i < self->size; ++i) {
      item = self->data[i];
      Py_INCREF(Py_None);
      self->data[i] = Py_None;
      Py_XDECREF(item);
  }
  self->size = 0;
  Py_RETURN_NONE;
}

/* ArrayList.get(index) */
static PyObject *
ArrayList_get(ArrayList *self, PyObject *indexobj)
{
  Py_ssize_t index = -1;
  PyObject *item;

  if (indexobj != NULL && indexobj != Py_None) {
  index = PyLong_AsSsize_t(indexobj);
  if (index == -1 && PyErr_Occurred())
    return NULL;
  }
#if defined(NDEBUG)
  if (index < 0 || index >= self->capacity || index > self->size - 1) {
    PyErr_SetString(PyExc_IndexError, "ArrayList index out of range");
    return NULL;
  }
#endif
  item = self->data[index];
  Py_INCREF(item);
  return item;
}

/* ArrayList.insert(index, item) */
static PyObject *
ArrayList_insert(ArrayList *self, PyObject *args)
{
  PyObject *indexobj = NULL, *itemobj = NULL;
  Py_ssize_t index = -1;
  int i;

  if (self->size == self->capacity) {
    PyErr_SetString(PyExc_RuntimeError, "ArrayList is full (capacity == size)");
    return NULL;
  }
  if (!PyArg_ParseTuple(args, "OO", &indexobj, &itemobj)) {
    PyErr_SetString(PyExc_RuntimeError, "WTF");
    return NULL;
  }
  if (indexobj != NULL && indexobj != Py_None) {
    index = PyLong_AsSsize_t(indexobj);
    if (index == -1 && PyErr_Occurred()) {
      PyErr_SetString(PyExc_RuntimeError, "WTF");
      return NULL;
    }
  }
  if (index < 0 || index >= self->capacity || index > self->size - 1) {
    PyErr_SetString(PyExc_IndexError, "ArrayList index out of range");
    return NULL;
  }
  if (itemobj == NULL) {
    PyErr_SetString(PyExc_ValueError, "item == NULL");
    return NULL;
  }

  Py_XDECREF(Py_None);
  for (i = self->size; i > index; --i) {
  self->data[i] = self->data[i-1];
  }
  Py_INCREF(itemobj);
  self->data[index] = itemobj;
  self->size += 1;
  Py_RETURN_NONE;
}

/* ArrayList.prepend(item) */
static PyObject *
ArrayList_prepend(ArrayList *self, PyObject *item)
{
  int i;

  if (self->size == self->capacity) {
    PyErr_SetString(PyExc_RuntimeError, "ArrayList is full (capacity == size)");
    return NULL;
  }

  if (item == NULL) {
    PyErr_SetString(PyExc_ValueError, "item == NULL");
    return NULL;
  }

  Py_XDECREF(Py_None);
  for (i = self->size; i > 0; --i) {
  self->data[i] = self->data[i-1];
  }
  Py_INCREF(item);
  self->data[0] = item;
  self->size += 1;
  Py_RETURN_NONE;
}

/* ArrayList.remove(index) */
static PyObject *
ArrayList_remove(ArrayList *self, PyObject *indexobj)
{
  PyObject *old_item;
  Py_ssize_t index = -1;
  int j;

  if (indexobj != NULL && indexobj != Py_None) {
    index = PyLong_AsSsize_t(indexobj);
    if (index == -1 && PyErr_Occurred()) {
      PyErr_SetString(PyExc_RuntimeError, "WTF");
      return NULL;
    }
  }
  if (index < 0 || index >= self->capacity || index > self->size - 1) {
    PyErr_SetString(PyExc_IndexError, "ArrayList index out of range");
    return NULL;
  }

  old_item = self->data[index];
  for (j = index; j < self->size - 1; ++j) {
  self->data[j] = self->data[j+1];
  }
  Py_INCREF(Py_None);
  self->data[self->size-1] = Py_None;
  self->size -= 1;

  return old_item;
}

/* ArrayList.set(index, item) */
static PyObject *
ArrayList_set(ArrayList *self, PyObject *args)
{
  PyObject *indexobj = NULL;
  PyObject *itemobj = NULL;
  Py_ssize_t index = -1;
  PyObject *old_value;

  if (!PyArg_ParseTuple(args, "OO", &indexobj, &itemobj)) {
    PyErr_SetString(PyExc_IndexError, "WTF");
    return NULL;
  }
  if (indexobj != NULL && indexobj != Py_None) {
    index = PyLong_AsSsize_t(indexobj);
    if (index == -1 && PyErr_Occurred()) {
      PyErr_SetString(PyExc_IndexError, "WTF");
      return NULL;
    }
  }
#if defined(NDEBUG)
  if (index < 0 || index >= self->capacity || index > self->size - 1) {
    PyErr_SetString(PyExc_IndexError, "ArrayList index out of range");
    return NULL;
  }
  if (itemobj == NULL) {
    /* TODO(nkraft): del the item instead of error? */
    PyErr_SetString(PyExc_ValueError, "item == NULL");
    return NULL;
  }
#endif
  Py_INCREF(itemobj);
  old_value = self->data[index];
  self->data[index] = itemobj;
  Py_DECREF(old_value);
  Py_RETURN_NONE;
}

/* ArrayList.size() */
static PyObject *
ArrayList_size(ArrayList *self)
{
  return PyLong_FromSsize_t(self->size);
}

/* ArrayListType.tp_repr */
static PyObject *
ArrayList_repr(PyObject *self)
{
  /* PyObject *aslist; */
  /* PyObject *result; */
  /* int i; */

  /* i = Py_ReprEnter(self); */
  /* if (i != 0) { */
  /*   if (i < 0) { */
  /*     return NULL; */
  /*   } */
  /*   return PyUnicode_FromString("[...]"); */
  /* } */
  /* aslist = PySequence_List(self); */
  /* if (aslist == NULL) { */
  /*   Py_ReprLeave(self); */
  /*   return NULL; */
  /* } */
  /* result = PyUnicode_FromFormat("%R", aslist); */
  /* Py_DECREF(aslist); */
  /* Py_ReprLeave(self); */
  /* return result; */
  return PyUnicode_FromString("[...]");
}

/* ArrayListType.tp_str */
static PyObject *
ArrayList_str(PyObject *self)
{
  /* PyObject *aslist; */
  /* PyObject *result; */
  /* int i; */

  /* i = Py_ReprEnter(self); */
  /* if (i != 0) { */
  /*     if (i < 0) */
  /*         return NULL; */
  /*     return PyUnicode_FromString("[...]"); */
  /* } */
  /* aslist = PySequence_List(self); */
  /* if (aslist == NULL) { */
  /*     Py_ReprLeave(self); */
  /*     return NULL; */
  /* } */
  /* result = PyUnicode_FromFormat("%R", aslist); */
  /* Py_DECREF(aslist); */
  /* Py_ReprLeave(self); */
  /* return result; */
  return PyUnicode_FromString("[...]");
}

/* ArrayListType.tp_methods */
static PyMethodDef ArrayList_methods[] = {
  {"append",                  (PyCFunction)ArrayList_append,
      METH_O,                  List_append_doc},
  {"capacity",                (PyCFunction)ArrayList_capacity,
      METH_NOARGS,             ArrayList_capacity_doc},
  {"clear",                   (PyCFunction)ArrayList_clear,
      METH_NOARGS,             List_clear_doc},
  {"get",                     (PyCFunction)ArrayList_get,
      METH_O,                  List_get_doc},
  {"insert",                  (PyCFunction)ArrayList_insert,
      METH_VARARGS,            List_insert_doc},
  {"prepend",                 (PyCFunction)ArrayList_prepend,
      METH_O,                  List_prepend_doc},
  {"remove",                  (PyCFunction)ArrayList_remove,
      METH_O,                  List_remove_doc},
  {"set",                     (PyCFunction)ArrayList_set,
      METH_VARARGS,            List_set_doc},
  {"size",                    (PyCFunction)ArrayList_size,
      METH_NOARGS,             List_size_doc},
  {NULL,                      NULL}
};

PyTypeObject ArrayListType = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "_educollections.ArrayList",          /* tp_name */
  sizeof(ArrayList),                    /* tp_basicsize */
  0,                                    /* tp_itemsize */
  (destructor)ArrayList_dealloc,        /* tp_dealloc */
  0,                                    /* tp_print */
  0,                                    /* tp_getattr */
  0,                                    /* tp_setattr */
  0,                                    /* tp_reserved */
  ArrayList_repr,                       /* tp_repr */
  0,                                    /* tp_as_number */
  0,                                    /* tp_as_sequence */
  0,                                    /* tp_as_mapping */
  PyObject_HashNotImplemented,          /* tp_hash  */
  0,                                    /* tp_call */
  ArrayList_str,                        /* tp_str */
  0,                                    /* tp_getattro */
  0,                                    /* tp_setattro */
  0,                                    /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT |
    Py_TPFLAGS_BASETYPE,                /* tp_flags */
  ArrayList_doc,                        /* tp_doc */
  0,                                    /* tp_traverse */
  0,                                    /* tp_clear */
  0,                                    /* tp_richcompare */
  0,                                    /* tp_weaklistoffset */
  0,                                    /* tp_iter */
  0,                                    /* tp_iternext */
  ArrayList_methods,                    /* tp_methods */
  0,                                    /* tp_members */
  0,                                    /* tp_getset */
  0,                                    /* tp_base */
  0,                                    /* tp_dict */
  0,                                    /* tp_descr_get */
  0,                                    /* tp_descr_set */
  0,                                    /* tp_dictoffset */
  (initproc)ArrayList_init,             /* tp_init */
  PyType_GenericAlloc,                  /* tp_alloc */
  ArrayList_new,                        /* tp_new */
};


/* SinglyLinkedListNode */
typedef struct SinglyLinkedListNodeType {
  PyObject *data;
  struct SinglyLinkedListNodeType *next;
} SinglyLinkedListNode;


/* SinglyLinkedList1
 * Resizable singly-linked-node-based implementation of the List interface. */
typedef struct {
  PyObject_HEAD
  SinglyLinkedListNode *head;
  Py_ssize_t size;
  long state;
} SinglyLinkedList1;

PyDoc_STRVAR(SinglyLinkedList1_doc,
  "Resizable singly-linked-node-based implementation of the List interface.");

/* SinglyLinkedList1Type.tp_new */
static PyObject *
SinglyLinkedList1_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  SinglyLinkedList1 *self;

  self = (SinglyLinkedList1 *)type->tp_alloc(type, 0);
  if (self == NULL)
      return NULL;

  self->head = NULL;
  self->size = 0;
  self->state = 0;

  return (PyObject *)self;
}

/* SinglyLinkedList1Type.tp_init */
static int
SinglyLinkedList1_init(SinglyLinkedList1 *self, PyObject *args, PyObject *kwds)
{
  return 0;
}

/* SinglyLinkedList1Type.tp_dealloc */
static void
SinglyLinkedList1_dealloc(SinglyLinkedList1 *self)
{
  SinglyLinkedListNode *n, *tmp;

  n = self->head;
  while (n) {
      Py_XDECREF(n->data);
      tmp = n;
      n = n->next;
      PyMem_Free(tmp);
  }
  Py_TYPE(self)->tp_free((PyObject*)self);
}

/* SinglyLinkedList1.append(item) */
static PyObject *
SinglyLinkedList1_append(SinglyLinkedList1 *self, PyObject *item)
{
  SinglyLinkedListNode *n;

  ++self->state;
  n = PyMem_Malloc(sizeof(SinglyLinkedListNode));
  if (n == NULL) {
      PyErr_NoMemory();
      return NULL;
  }
  n->next = NULL;
  Py_INCREF(item);
  ++self->size;
  n->data = item;
  if (self->head == NULL) {
    self->head = n;
  }
  else {
    /* TODO(nkraft): Walk list and append. */
    /*self->tail->next = n;
    self->tail = n;*/
  }
  Py_RETURN_NONE;
}

/* SinglyLinkedList1.clear() */
static PyObject *
SinglyLinkedList1_clear(SinglyLinkedList1 *self)
{
  SinglyLinkedListNode *n, *tmp;

  n = self->head;
  while (n) {
      Py_XDECREF(n->data);
      tmp = n;
      n = n->next;
      PyMem_Free(tmp);
  }
  self->head = NULL;
  self->size = 0;
  self->state = 0;
  Py_RETURN_NONE;
}

/* SinglyLinkedList1.get(index) */
static PyObject *
SinglyLinkedList1_get(SinglyLinkedList1 *self, PyObject *indexobj)
{
  SinglyLinkedListNode *n;
  PyObject *item;
  Py_ssize_t index = -1;
  int i;

  if (indexobj != NULL && indexobj != Py_None) {
  index = PyLong_AsSsize_t(indexobj);
  if (index == -1 && PyErr_Occurred())
    return NULL;
  }
  if (index < 0 || index > self->size - 1) {
  PyErr_SetString(PyExc_IndexError, "LinkedList index out of range");
  return NULL;
  }

  n = self->head;
  for (i = 0; i < index; ++i)
  n = n->next;
  item = n->data;
  Py_INCREF(item);
  return item;
}

static PyObject * SinglyLinkedList1_prepend(SinglyLinkedList1 *self, PyObject *item);

/* SinglyLinkedList1.insert(index, item) */
static PyObject *
SinglyLinkedList1_insert(SinglyLinkedList1 *self, PyObject *args)
{
  SinglyLinkedListNode *n, *tmp;
  PyObject *indexobj = NULL, *itemobj = NULL;
  Py_ssize_t index = -1;
  int i;

  if (!PyArg_ParseTuple(args, "OO", &indexobj, &itemobj)) {
  return NULL;
  }
  if (indexobj != NULL && indexobj != Py_None) {
  index = PyLong_AsSsize_t(indexobj);
  if (index == -1 && PyErr_Occurred())
    return NULL;
  }
  if (index < 0 || index > self->size - 1) {
  PyErr_SetString(PyExc_IndexError, "SinglyLinkedList1 index out of range");
  return NULL;
  }
  if (itemobj == NULL) {
  PyErr_SetString(PyExc_ValueError, "item == NULL");
  return NULL;
  }

  if (index == 0)
  return SinglyLinkedList1_prepend(self, itemobj);
  n = self->head;
  for (i = 0; i < index - 1; ++i)
  n = n->next;
  tmp = PyMem_Malloc(sizeof(SinglyLinkedListNode));
  if (tmp == NULL) {
    PyErr_NoMemory();
    return NULL;
  }
  ++self->state;
  ++self->size;
  Py_INCREF(itemobj);
  tmp->data = itemobj;
  tmp->next = n->next;
  n->next = tmp;
  Py_RETURN_NONE;
}

/* SinglyLinkedList1.prepend(item) */
static PyObject *
SinglyLinkedList1_prepend(SinglyLinkedList1 *self, PyObject *item)
{
  SinglyLinkedListNode *n;

  ++self->state;
  n = PyMem_Malloc(sizeof(SinglyLinkedListNode));
  if (n == NULL) {
      PyErr_NoMemory();
      return NULL;
  }
  n->next = self->head;
  Py_INCREF(item);
  ++self->size;
  n->data = item;
  self->head = n;
  Py_RETURN_NONE;
}

/* SinglyLinkedList1.remove(index) */
static PyObject *
SinglyLinkedList1_remove(SinglyLinkedList1 *self, PyObject *indexobj)
{
  PyObject *item;
  SinglyLinkedListNode *n, *remove;
  Py_ssize_t index = -1;
  int i;

  if (indexobj != NULL && indexobj != Py_None) {
  index = PyLong_AsSsize_t(indexobj);
  if (index == -1 && PyErr_Occurred())
    return NULL;
  }
  if (index < 0 || index > self->size - 1) {
  PyErr_SetString(PyExc_IndexError, "SinglyLinkedList1 index out of range");
  return NULL;
  }

  if (index == 0) {
    if (self->size == 0) {
      PyErr_SetString(PyExc_IndexError, "remove from an empty LinkedList");
      return NULL;
    }
    item = self->head->data;
    --self->size;
    ++self->state;
    n = self->head;
    self->head = self->head->next;
    PyMem_Free(n);
    return item;
  }
  if (index == self->size - 1) {
    if (self->size == 0) {
      PyErr_SetString(PyExc_IndexError, "remove from an empty LinkedList");
      return NULL;
    }
    /* TODO(nkraft): Initialize item correctly. */
    item = self->head->data;

    n = self->head;
    for (i = 0; i < self->size - 2; ++i) {
      n = n->next;
    }
    PyMem_Free(n->next);
    --self->size;
    ++self->state;
    if (self->size == 0) {
      self->head = NULL;
    } else {
    /* TODO(nkraft): Walk list and append. */
      /* self->tail = n; */
      /* self->tail->next = NULL; */
    }
    return item;
  }

  n = self->head;
  for (i = 0; i < index - 1; ++i) {
    n = n->next;
  }
  remove = n->next;
  item = remove->data;
  n->next = remove->next;
  PyMem_Free(remove);
  --self->size;
  ++self->state;
  return item;
}

/* SinglyLinkedList1.set(index, item) */
static PyObject *
SinglyLinkedList1_set(SinglyLinkedList1 *self, PyObject *args)
{
  SinglyLinkedListNode *n;
  PyObject *indexobj = NULL, *itemobj = NULL, *old_item = NULL;
  Py_ssize_t index = -1;
  int i;

  if (!PyArg_ParseTuple(args, "OO", &indexobj, &itemobj)) {
    return NULL;
  }
  if (indexobj != NULL && indexobj != Py_None) {
  index = PyLong_AsSsize_t(indexobj);
  if (index == -1 && PyErr_Occurred())
    return NULL;
  }
  if (index < 0 || index > self->size - 1) {
    PyErr_SetString(PyExc_IndexError, "SinglyLinkedList1 index out of range");
    return NULL;
  }
  if (itemobj == NULL) {
    PyErr_SetString(PyExc_ValueError, "item == NULL");
    return NULL;
  }

  n = self->head;
  for (i = 0; i < index; ++i) {
    n = n->next;
  }
  ++self->state;
  old_item = n->data;
  Py_XDECREF(old_item);
  Py_INCREF(itemobj);
  n->data = itemobj;
  Py_RETURN_NONE;
}

/* SinglyLinkedList1.size() */
static PyObject *
SinglyLinkedList1_size(SinglyLinkedList1 *self)
{
  return PyLong_FromSsize_t(self->size);
}

/* SinglyLinkedList1Type.tp_repr */
static PyObject *
SinglyLinkedList1_repr(PyObject *self)
{
  /* PyObject *aslist, *result; */
  /* int i; */

  /* i = Py_ReprEnter(self); */
  /* if (i != 0) { */
  /*     if (i < 0) */
  /*         return NULL; */
  /*     return PyUnicode_FromString("[...]"); */
  /* } */

  /* aslist = PySequence_List(self); */
  /* if (aslist == NULL) { */
  /*     Py_ReprLeave(self); */
  /*     return NULL; */
  /* } */
  /* result = PyUnicode_FromFormat("%R", aslist); */
  /* Py_DECREF(aslist); */
  /* Py_ReprLeave(self); */
  /* return result; */
  return PyUnicode_FromString("[...]");
}

/* SinglyLinkedList1Type.tp_str */
static PyObject *
SinglyLinkedList1_str(PyObject *self)
{
  /* PyObject *aslist, *result; */
  /* int i; */

  /* i = Py_ReprEnter(self); */
  /* if (i != 0) { */
  /*     if (i < 0) */
  /*         return NULL; */
  /*     return PyUnicode_FromString("[...]"); */
  /* } */

  /* aslist = PySequence_List(self); */
  /* if (aslist == NULL) { */
  /*     Py_ReprLeave(self); */
  /*     return NULL; */
  /* } */
  /* result = PyUnicode_FromFormat("%R", aslist); */
  /* Py_DECREF(aslist); */
  /* Py_ReprLeave(self); */
  /* return result; */
  return PyUnicode_FromString("[...]");
}

static PyMethodDef SinglyLinkedList1_methods[] = {
  {"append",                  (PyCFunction)SinglyLinkedList1_append,
      METH_O,                  List_append_doc},
  {"clear",                   (PyCFunction)SinglyLinkedList1_clear,
      METH_NOARGS,             List_clear_doc},
  {"get",                     (PyCFunction)SinglyLinkedList1_get,
      METH_O,                  List_get_doc},
  {"insert",                  (PyCFunction)SinglyLinkedList1_insert,
      METH_VARARGS,            List_insert_doc},
  {"prepend",                 (PyCFunction)SinglyLinkedList1_prepend,
      METH_O,                  List_prepend_doc},
  {"remove",                  (PyCFunction)SinglyLinkedList1_remove,
      METH_O,                  List_remove_doc},
  {"set",                     (PyCFunction)SinglyLinkedList1_set,
      METH_VARARGS,            List_set_doc},
  {"size",                    (PyCFunction)SinglyLinkedList1_size,
      METH_NOARGS,             List_size_doc},
  {NULL,                      NULL}
};

PyTypeObject SinglyLinkedListType1 = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "_educollections.SinglyLinkedList1",  /* tp_name */
  sizeof(SinglyLinkedList1),            /* tp_basicsize */
  0,                                    /* tp_itemsize */
  (destructor)SinglyLinkedList1_dealloc,/* tp_dealloc */
  0,                                    /* tp_print */
  0,                                    /* tp_getattr */
  0,                                    /* tp_setattr */
  0,                                    /* tp_reserved */
  SinglyLinkedList1_repr,               /* tp_repr */
  0,                                    /* tp_as_number */
  0,                                    /* tp_as_sequence */
  0,                                    /* tp_as_mapping */
  PyObject_HashNotImplemented,          /* tp_hash  */
  0,                                    /* tp_call */
  SinglyLinkedList1_str,                /* tp_str */
  0,                                    /* tp_getattro */
  0,                                    /* tp_setattro */
  0,                                    /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT |
    Py_TPFLAGS_BASETYPE,                /* tp_flags */
  SinglyLinkedList1_doc,                /* tp_doc */
  0,                                    /* tp_traverse */
  0,                                    /* tp_clear */
  0,                                    /* tp_richcompare */
  0,                                    /* tp_weaklistoffset */
  0,                                    /* tp_iter */
  0,                                    /* tp_iternext */
  SinglyLinkedList1_methods,            /* tp_methods */
  0,                                    /* tp_members */
  0,                                    /* tp_getset */
  0,                                    /* tp_base */
  0,                                    /* tp_dict */
  0,                                    /* tp_descr_get */
  0,                                    /* tp_descr_set */
  0,                                    /* tp_dictoffset */
  (initproc)SinglyLinkedList1_init,     /* tp_init */
  PyType_GenericAlloc,                  /* tp_alloc */
  SinglyLinkedList1_new,                /* tp_new */
};


/* SinglyLinkedList2
 * Resizable singly-linked-node-based implementation of the List interface.
 * Uses a tail pointer to make appending more efficient. */
typedef struct {
  PyObject_HEAD
  SinglyLinkedListNode *head;
  SinglyLinkedListNode *tail;
  Py_ssize_t size;
  long state;
} SinglyLinkedList2;

PyDoc_STRVAR(SinglyLinkedList2_doc,
  "Resizable singly-linked-node-based implementation of the List interface.\n"
  "Uses a tail pointer to make appending more efficient.");

/* SinglyLinkedList2Type.tp_new */
static PyObject *
SinglyLinkedList2_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
  SinglyLinkedList2 *self;

  self = (SinglyLinkedList2 *)type->tp_alloc(type, 0);
  if (self == NULL)
      return NULL;

  self->head = NULL;
  self->tail = NULL;
  self->size = 0;
  self->state = 0;

  return (PyObject *)self;
}

/* SinglyLinkedList2Type.tp_init */
static int
SinglyLinkedList2_init(SinglyLinkedList2 *self, PyObject *args, PyObject *kwds)
{
  return 0;
}

/* SinglyLinkedList2Type.tp_dealloc */
static void
SinglyLinkedList2_dealloc(SinglyLinkedList2 *self)
{
  SinglyLinkedListNode *n, *tmp;

  n = self->head;
  while (n) {
      Py_XDECREF(n->data);
      tmp = n;
      n = n->next;
      PyMem_Free(tmp);
  }
  Py_TYPE(self)->tp_free((PyObject*)self);
}

/* SinglyLinkedList2.append(item) */
static PyObject *
SinglyLinkedList2_append(SinglyLinkedList2 *self, PyObject *item)
{
  SinglyLinkedListNode *n;

  ++self->state;
  n = PyMem_Malloc(sizeof(SinglyLinkedListNode));
  if (n == NULL) {
      PyErr_NoMemory();
      return NULL;
  }
  n->next = NULL;
  Py_INCREF(item);
  ++self->size;
  n->data = item;
  if (self->head == NULL) {
    self->head = self->tail = n;
  }
  else {
    self->tail->next = n;
    self->tail = n;
  }
  Py_RETURN_NONE;
}

/* SinglyLinkedList2.clear() */
static PyObject *
SinglyLinkedList2_clear(SinglyLinkedList2 *self)
{
  SinglyLinkedListNode *n, *tmp;

  n = self->head;
  while (n) {
      Py_XDECREF(n->data);
      tmp = n;
      n = n->next;
      PyMem_Free(tmp);
  }
  self->head = NULL;
  self->tail = NULL;
  self->size = 0;
  self->state = 0;
  Py_RETURN_NONE;
}

/* SinglyLinkedList2.get(index) */
static PyObject *
SinglyLinkedList2_get(SinglyLinkedList2 *self, PyObject *indexobj)
{
  SinglyLinkedListNode *n;
  PyObject *item;
  Py_ssize_t index = -1;
  int i;

  if (indexobj != NULL && indexobj != Py_None) {
  index = PyLong_AsSsize_t(indexobj);
  if (index == -1 && PyErr_Occurred())
    return NULL;
  }
  if (index < 0 || index > self->size - 1) {
  PyErr_SetString(PyExc_IndexError, "LinkedList index out of range");
  return NULL;
  }

  n = self->head;
  for (i = 0; i < index; ++i)
  n = n->next;
  item = n->data;
  Py_INCREF(item);
  return item;
}

static PyObject * SinglyLinkedList2_prepend(SinglyLinkedList2 *self, PyObject *item);

/* SinglyLinkedList2.insert(index, item) */
static PyObject *
SinglyLinkedList2_insert(SinglyLinkedList2 *self, PyObject *args)
{
  SinglyLinkedListNode *n, *tmp;
  PyObject *indexobj = NULL, *itemobj = NULL;
  Py_ssize_t index = -1;
  int i;

  if (!PyArg_ParseTuple(args, "OO", &indexobj, &itemobj)) {
  return NULL;
  }
  if (indexobj != NULL && indexobj != Py_None) {
  index = PyLong_AsSsize_t(indexobj);
  if (index == -1 && PyErr_Occurred())
    return NULL;
  }
  if (index < 0 || index > self->size - 1) {
  PyErr_SetString(PyExc_IndexError, "SinglyLinkedList2 index out of range");
  return NULL;
  }
  if (itemobj == NULL) {
  PyErr_SetString(PyExc_ValueError, "item == NULL");
  return NULL;
  }

  if (index == 0)
  return SinglyLinkedList2_prepend(self, itemobj);
  n = self->head;
  for (i = 0; i < index - 1; ++i)
  n = n->next;
  tmp = PyMem_Malloc(sizeof(SinglyLinkedListNode));
  if (tmp == NULL) {
    PyErr_NoMemory();
    return NULL;
  }
  ++self->state;
  ++self->size;
  Py_INCREF(itemobj);
  tmp->data = itemobj;
  tmp->next = n->next;
  n->next = tmp;
  Py_RETURN_NONE;
}

/* SinglyLinkedList2.prepend(item) */
static PyObject *
SinglyLinkedList2_prepend(SinglyLinkedList2 *self, PyObject *item)
{
  SinglyLinkedListNode *n;

  ++self->state;
  n = PyMem_Malloc(sizeof(SinglyLinkedListNode));
  if (n == NULL) {
      PyErr_NoMemory();
      return NULL;
  }
  n->next = self->head;
  Py_INCREF(item);
  ++self->size;
  n->data = item;
  if (self->head == self->tail)
    self->tail = n;
  self->head = n;
  Py_RETURN_NONE;
}

/* SinglyLinkedList2.remove(index) */
static PyObject *
SinglyLinkedList2_remove(SinglyLinkedList2 *self, PyObject *indexobj)
{
  PyObject *item;
  SinglyLinkedListNode *n, *remove;
  Py_ssize_t index = -1;
  int i;

  if (indexobj != NULL && indexobj != Py_None) {
  index = PyLong_AsSsize_t(indexobj);
  if (index == -1 && PyErr_Occurred())
    return NULL;
  }
  if (index < 0 || index > self->size - 1) {
  PyErr_SetString(PyExc_IndexError, "SinglyLinkedList2 index out of range");
  return NULL;
  }

  if (index == 0) {
    if (self->size == 0) {
      PyErr_SetString(PyExc_IndexError, "remove from an empty LinkedList");
      return NULL;
    }
    item = self->head->data;
    --self->size;
    ++self->state;
    n = self->head;
    self->head = self->head->next;
    if (self->size == 0) {
      self->tail = NULL;
    }
    PyMem_Free(n);
    return item;
  }
  if (index == self->size - 1) {
    if (self->size == 0) {
      PyErr_SetString(PyExc_IndexError, "remove from an empty LinkedList");
      return NULL;
    }
    item = self->tail->data;
    n = self->head;
    for (i = 0; i < self->size - 2; ++i) {
      n = n->next;
    }
    PyMem_Free(n->next);
    --self->size;
    ++self->state;
    if (self->size == 0) {
      self->head = self->tail = NULL;
    } else {
      self->tail = n;
      self->tail->next = NULL;
    }
    return item;
  }

  n = self->head;
  for (i = 0; i < index - 1; ++i) {
    n = n->next;
  }
  remove = n->next;
  item = remove->data;
  n->next = remove->next;
  PyMem_Free(remove);
  --self->size;
  ++self->state;
  return item;
}

/* SinglyLinkedList2.set(index, item) */
static PyObject *
SinglyLinkedList2_set(SinglyLinkedList2 *self, PyObject *args)
{
  SinglyLinkedListNode *n;
  PyObject *indexobj = NULL, *itemobj = NULL, *old_item = NULL;
  Py_ssize_t index = -1;
  int i;

  if (!PyArg_ParseTuple(args, "OO", &indexobj, &itemobj)) {
  return NULL;
  }
  if (indexobj != NULL && indexobj != Py_None) {
  index = PyLong_AsSsize_t(indexobj);
  if (index == -1 && PyErr_Occurred())
    return NULL;
  }
  if (index < 0 || index > self->size - 1) {
  PyErr_SetString(PyExc_IndexError, "SinglyLinkedList2 index out of range");
  return NULL;
  }
  if (itemobj == NULL) {
  PyErr_SetString(PyExc_ValueError, "item == NULL");
  return NULL;
  }

  n = self->head;
  for (i = 0; i < index; ++i)
  n = n->next;
  ++self->state;
  old_item = n->data;
  Py_XDECREF(old_item);
  Py_INCREF(itemobj);
  n->data = itemobj;
  Py_RETURN_NONE;
}

/* SinglyLinkedList2.size() */
static PyObject *
SinglyLinkedList2_size(SinglyLinkedList2 *self)
{
  return PyLong_FromSsize_t(self->size);
}

/* SinglyLinkedList2Type.tp_repr */
static PyObject *
SinglyLinkedList2_repr(PyObject *self)
{
  /* PyObject *aslist, *result; */
  /* int i; */

  /* i = Py_ReprEnter(self); */
  /* if (i != 0) { */
  /*     if (i < 0) */
  /*         return NULL; */
  /*     return PyUnicode_FromString("[...]"); */
  /* } */

  /* aslist = PySequence_List(self); */
  /* if (aslist == NULL) { */
  /*     Py_ReprLeave(self); */
  /*     return NULL; */
  /* } */
  /* result = PyUnicode_FromFormat("%R", aslist); */
  /* Py_DECREF(aslist); */
  /* Py_ReprLeave(self); */
  /* return result; */
  return PyUnicode_FromString("[...]");
}

/* SinglyLinkedList2Type.tp_str */
static PyObject *
SinglyLinkedList2_str(PyObject *self)
{
  /* PyObject *aslist, *result; */
  /* int i; */

  /* i = Py_ReprEnter(self); */
  /* if (i != 0) { */
  /*     if (i < 0) */
  /*         return NULL; */
  /*     return PyUnicode_FromString("[...]"); */
  /* } */

  /* aslist = PySequence_List(self); */
  /* if (aslist == NULL) { */
  /*     Py_ReprLeave(self); */
  /*     return NULL; */
  /* } */
  /* result = PyUnicode_FromFormat("%R", aslist); */
  /* Py_DECREF(aslist); */
  /* Py_ReprLeave(self); */
  /* return result; */
  return PyUnicode_FromString("[...]");
}

static PyMethodDef SinglyLinkedList2_methods[] = {
  {"append",                  (PyCFunction)SinglyLinkedList2_append,
      METH_O,                  List_append_doc},
  {"clear",                   (PyCFunction)SinglyLinkedList2_clear,
      METH_NOARGS,             List_clear_doc},
  {"get",                     (PyCFunction)SinglyLinkedList2_get,
      METH_O,                  List_get_doc},
  {"insert",                  (PyCFunction)SinglyLinkedList2_insert,
      METH_VARARGS,            List_insert_doc},
  {"prepend",                 (PyCFunction)SinglyLinkedList2_prepend,
      METH_O,                  List_prepend_doc},
  {"remove",                  (PyCFunction)SinglyLinkedList2_remove,
      METH_O,                  List_remove_doc},
  {"set",                     (PyCFunction)SinglyLinkedList2_set,
      METH_VARARGS,            List_set_doc},
  {"size",                    (PyCFunction)SinglyLinkedList2_size,
      METH_NOARGS,             List_size_doc},
  {NULL,                      NULL}
};

PyTypeObject SinglyLinkedListType2 = {
  PyVarObject_HEAD_INIT(NULL, 0)
  "_educollections.SinglyLinkedList2",  /* tp_name */
  sizeof(SinglyLinkedList2),            /* tp_basicsize */
  0,                                    /* tp_itemsize */
  (destructor)SinglyLinkedList2_dealloc,/* tp_dealloc */
  0,                                    /* tp_print */
  0,                                    /* tp_getattr */
  0,                                    /* tp_setattr */
  0,                                    /* tp_reserved */
  SinglyLinkedList2_repr,               /* tp_repr */
  0,                                    /* tp_as_number */
  0,                                    /* tp_as_sequence */
  0,                                    /* tp_as_mapping */
  PyObject_HashNotImplemented,          /* tp_hash  */
  0,                                    /* tp_call */
  SinglyLinkedList2_str,                /* tp_str */
  0,                                    /* tp_getattro */
  0,                                    /* tp_setattro */
  0,                                    /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT |
    Py_TPFLAGS_BASETYPE,                /* tp_flags */
  SinglyLinkedList2_doc,                /* tp_doc */
  0,                                    /* tp_traverse */
  0,                                    /* tp_clear */
  0,                                    /* tp_richcompare */
  0,                                    /* tp_weaklistoffset */
  0,                                    /* tp_iter */
  0,                                    /* tp_iternext */
  SinglyLinkedList2_methods,            /* tp_methods */
  0,                                    /* tp_members */
  0,                                    /* tp_getset */
  0,                                    /* tp_base */
  0,                                    /* tp_dict */
  0,                                    /* tp_descr_get */
  0,                                    /* tp_descr_set */
  0,                                    /* tp_dictoffset */
  (initproc)SinglyLinkedList2_init,     /* tp_init */
  PyType_GenericAlloc,                  /* tp_alloc */
  SinglyLinkedList2_new,                /* tp_new */
};

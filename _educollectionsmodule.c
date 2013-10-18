/* Collections for demonstrating order notation.
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

PyDoc_STRVAR(module_doc,
"Collection Interfaces:\n"
"  List --- An ordered collection (also known as a sequence).\n"
"\n"
"Collection Implementations:\n"
"  ArrayList --- Fixed-size-array-based implementation of the List interface.\n"
"  SinglyLinkedList1 --- Resizable singly-linked-node-based implementation of the List interface.\n"
"  SinglyLinkedList2 --- Uses a tail pointer to make appending more efficient.\n"
);

static PyModuleDef _educollections_module = {
  PyModuleDef_HEAD_INIT,
  "_educollections",
  module_doc,
  -1,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};

PyMODINIT_FUNC
PyInit__educollections(void) 
{
  PyObject *m;

  m = PyModule_Create(&_educollections_module);
  if (m == NULL) {
      return NULL;
  }

#define ADD_TYPE(type, name) \
  if (PyType_Ready(&type) < 0) { \
    goto fail; \
  } \
  Py_INCREF(&type); \
  if (PyModule_AddObject(m, name, (PyObject *)&type) < 0) { \
    Py_DECREF(&type); \
    goto fail; \
  }

  ADD_TYPE(ArrayListType, "ArrayList");
  ADD_TYPE(SinglyLinkedListType1, "SinglyLinkedList1");
  ADD_TYPE(SinglyLinkedListType2, "SinglyLinkedList2");

  return m;

fail:
  Py_DECREF(m);
  return NULL;
}

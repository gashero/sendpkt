/*
 * File: linux/exlibnet.c
 * Date: 2007-09-18
 * Author: gashero
 * Copyright@1999-2007, Harry Gashero Liu.
 */

#include <Python.h>
#include <libnet.h>

static PyObject* sendpacket(PyObject* self, PyObject* args, PyObject* keywds) {
    PyObject* packet=NULL;
    static char* kwlist[]={"packet",NULL};
    if (!PyArg_ParseTupleAndKeywords(args,keywds,"O",kwlist,packet)) {
        return NULL;
    }
    printf("Address: 0x%08x\n",(unsigned long)packet);
    Py_RETURN_NONE;
};

static PyMethodDef SendPktMethods[]={
    {"sendpacket",sendpacket,METH_VARARGS|METH_KEYWORDS,""},
    {NULL,NULL,0,NULL}
};

PyMODINIT_FUNC initsendpkt(void) {
    Py_InitModule("sendpkt",SendPktMethods);
};

/*
 * File: linux/exlibnet.c
 * Date: 2007-09-18
 * Author: gashero
 * Copyright@1999-2007, Harry Gashero Liu.
 */

#include <Python.h>
#include <libnet.h>

//----------------------实体函数--------------------------------------------

unsigned int exlibnet_sendpacket(unsigned char* packet) {
    return 0;
}

//----------------------Python接口函数--------------------------------------

static PyObject* sendpacket(PyObject* self, PyObject* args, PyObject* keywds) {
    PyObject* packet=NULL;
    printf("hello");
    static char* kwlist[]={"packet",NULL};
    if (!PyArg_ParseTupleAndKeywords(args,keywds,"O",kwlist,packet)) {
        return NULL;
    }
    printf("Address: 0x%08x\n",(unsigned int)packet);
    Py_RETURN_NONE;
}

static PyMethodDef SendPktMethods[]={
    {"sendpacket",(PyCFunction)sendpacket,METH_VARARGS|METH_KEYWORDS,"Send a packet to ethernet"},
    {NULL,NULL,0,NULL}
};

PyMODINIT_FUNC initsendpkt(void) {
    Py_InitModule("sendpkt",SendPktMethods);
}

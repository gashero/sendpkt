/*
 * File: exwinpcap.c
 * Date: 2007-09-22
 * Author: gashero
 * Copyright@1999-2007, Harry Gashero Liu.
 */

#include <Python.h>
#include <pcap.h>

static PyObject* sendpacket(PyObject* self, PyObject* args, PyObject* kwargs) {
    //获取输入参数
    PyObject* packetobj=NULL;
    unsigned char* packet=NULL;
    unsigned int packet_s=0;
    static char* kwlist[]={"packet",NULL};
    if (!PyArg_ParseTupleAndKeywords(args,kwargs,"O",kwlist,&packetobj)) {
        return NULL;
    }
    //printf("Address: 0x%08x\n",(unsigned int)packetobj);
    if (!PyString_Check(packetobj)) {
        PyErr_SetString(PyExc_TypeError,"packet must be string object");
        return NULL;
    }
    packet=(unsigned char*)PyString_AsString(packetobj);
    packet_s=PyString_Size(packetobj);
    //初始化Winpcap
    pcap_t* p;
    char errbuf[PCAP_ERRBUF_SIZE];
    //int pcap_sendpacket(pcap_t* p, u_char* buffer, int size);
    return NULL;
}

static PyMethodDef SendPktMethods[]={
    {"sendpacket",(PyCFunction)sendpacket,METH_VARARGS | METH_KEYWORDS,"Send a packet to ethernet"},
    {NULL,NULL,0,NULL}
};

PyMODINIT_FUNC initsendpkt(void) {
    Py_InitModule("sendpkt",SendPktMethods);
}

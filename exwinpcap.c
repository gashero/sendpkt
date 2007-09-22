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
    pcap_t* p=NULL;
    char* device=NULL;
    char errbuf[PCAP_ERRBUF_SIZE];
    p=pcap_open_live(device,BUFSIZ,1,0,errbuf);
    if (p==NULL) {
        PyErr_SetString(PyExc_RuntimeError,errbuf);
        return NULL;
    }
    //发送数据包
    int bytes=0;
    bytes=pcap_sendpacket(p,packet,packet_s);
    if (bytes<0) {
        PyErr_SetString(PyExc_RuntimeError,pcap_geterr(p));
        return NULL;
    }else {
        printf("Bytes=%d\n",bytes);
    }
    //销毁winpcap句柄
    pcap_close(p);
    Py_RETURN_NONE;
}

static PyMethodDef SendPktMethods[]={
    {"sendpacket",(PyCFunction)sendpacket,METH_VARARGS | METH_KEYWORDS,"Send a packet to ethernet"},
    {NULL,NULL,0,NULL}
};

PyMODINIT_FUNC initsendpkt(void) {
    Py_InitModule("sendpkt",SendPktMethods);
}

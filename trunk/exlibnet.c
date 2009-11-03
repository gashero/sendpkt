/*
 * File: linux/exlibnet.c
 * Date: 2007-09-18
 * Author: gashero
 * Copyright@1999-2009, Harry Gashero Liu.
 */

#include <Python.h>
#include <libnet.h>
#include <pcap.h>

#define MAX_DEVICE_NAME_LENGTH 255

static PyObject* sendpacket(PyObject* self, PyObject* args, PyObject* kwargs) {
    //Get input parameters
    unsigned char* packet=NULL;
    unsigned int packet_s=0;
    char* device=NULL;
    static char* kwlist[]={"packet","device",NULL};
    if (!PyArg_ParseTupleAndKeywords(args,kwargs,"s#s",kwlist,&packet,&packet_s,&device)) {
        return NULL;
    }
    //printf("Address: 0x%08x\n",(unsigned int)packetobj);
    //printf("Device: %s\n",device);
    //initial libnet
    libnet_t* l;
    char errbuf[LIBNET_ERRBUF_SIZE];
    l=libnet_init(LIBNET_LINK_ADV,device,errbuf);
    if (l==NULL) {
        PyErr_SetString(PyExc_RuntimeError,errbuf);
        return NULL;
    }
    //send packet
    int bytes=0;
    bytes=libnet_adv_write_link(l,packet,packet_s);
    if (bytes<0) {
        PyErr_SetString(PyExc_RuntimeError,libnet_geterror(l));
        return NULL;
    }
    //release libnet
    libnet_destroy(l);
    Py_RETURN_NONE;
}

static PyObject* findalldevs(PyObject* self, PyObject* args) {
    //make sure no parameters
    if (!PyArg_ParseTuple(args,""))
        return NULL;
    //get device list
    int c=0;
    pcap_if_t* first;
    pcap_if_t* now;
    char errbuf[PCAP_ERRBUF_SIZE];
    c=pcap_findalldevs(&first,errbuf);
    if (c!=0) {
        PyErr_SetString(PyExc_RuntimeError,errbuf);
        return NULL;
    }
    //insert device name into list
    PyObject* devlist=NULL;
    PyObject* devname=NULL;
    devlist=PyList_New(0);
    now=first;
    while (1) {
        if (now==NULL)
            break;
        //printf("DEV: %s\n",now->name);
        devname=PyString_FromString(now->name);
        PyList_Append(devlist,devname);
        now=now->next;
    }
    //release pcap_if_t space
    pcap_freealldevs(first);
    return devlist;
}

static PyMethodDef SendPktMethods[]={
    {"sendpacket",(PyCFunction)sendpacket,METH_VARARGS | METH_KEYWORDS,"Send a packet to ethernet"},
    {"findalldevs",findalldevs,METH_VARARGS,"Find all net interfaces"},
    {NULL,NULL,0,NULL}
};

PyMODINIT_FUNC initsendpkt(void) {
    Py_InitModule("sendpkt",SendPktMethods);
}

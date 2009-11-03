/*
 * File: exlibpcap.c
 * Date: 2009-11-03
 * Author: gashero
 * Copyright@1999-2009, Harry Gashero Liu.
 */

#include <Python.h>
#include <pcap.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX_DEVICE_NAME_LENGTH 256

char sendpkt_finddevs_help[]="Find all network interfaces.\n\
iflist=finddevs()\n\
>>> finddevs()\n\
[(ifname,description,addresses,flags),('lo0',None,[],1)]";
static PyObject* sendpkt_finddevs(PyObject* self, PyObject* args) {
    if (!PyArg_ParseTuple(args,""))
        return NULL;
    int c=0;
    pcap_if_t *first, *now;
    //pcap_addr_t *pa_first, *pa_now;
    char errbuf[PCAP_ERRBUF_SIZE];
    //new test
    /*char *netname;
    netname=pcap_lookupdev(errbuf);
    printf("netname: %s\n",netname);*/
    //new test
    if (pcap_findalldevs(&first,errbuf)) {
        PyErr_SetString(PyExc_RuntimeError,errbuf);
        return NULL;
    }
    PyObject *devlist=PyList_New(0);
    PyObject *devname=NULL;
    PyObject *devtuple=NULL;
    PyObject *addresslist=NULL;
    now=first;
    //printf("first pointer=%10p\n",first);
    while (1) {
        if (now==NULL)
            break;
        devtuple=PyTuple_New(4);
        devname=PyString_FromString(now->name);
        //printf("DEV: %s\n",now->name);
        //insert more
        PyTuple_SetItem(devtuple,0,PyString_FromString(now->name));
        if (now->description)
            PyTuple_SetItem(devtuple,1,PyString_FromString(now->description));
        else
            PyTuple_SetItem(devtuple,1,Py_None);
        PyTuple_SetItem(devtuple,3,PyInt_FromLong((long)now->flags));
        addresslist=PyList_New(0);
        //pa_first=now->addresses;
        //pa_now=pa_first;
        /*while (1) {
            printf("pa_now=%10p\n",pa_now);
            if (pa_now==NULL)
                break;
            pa_now=pa_now->next;
            struct sockaddr_in *addr;
            addr=(struct sockaddr_in)pa_now->addr;
            printf("addr: %s\n",inet_ntoa(addr.sin_len));
        }*/
        PyTuple_SetItem(devtuple,2,addresslist);
        PyList_Append(devlist,devtuple);
        now=now->next;
    }
    pcap_freealldevs(first);
    return devlist;
}

char sendpkt_pcap_help[]="Create pcap object.\n\
pcapobj=pcap(ifname=None,bpfstr="",snaplen=4096,promisc=1,to_ms=0)\n\
>>> pc=pcap()";
static PyObject* sendpkt_pcap(PyObject* self, PyObject* args, PyObject* kwargs) {
    Py_RETURN_NONE;
}

/*
 * Close pcap object
 */
static PyObject* sendpkt_close(PyObject* self, PyObject* args, PyObject* kwargs) {
    Py_RETURN_NONE;
}

/*
 * Capture packet with timeout
 */
static PyObject* sendpkt_capture(PyObject* self, PyObject* args, PyObject* kwargs) {
    Py_RETURN_NONE;
}

/*
 * Send packet
 */
static PyObject* sendpkt_sendpacket(PyObject* self, PyObject* args, PyObject* kwargs) {
    Py_RETURN_NONE;
}

static PyMethodDef SendPktMethods[]={
    {"finddevs",    (PyCFunction)sendpkt_finddevs,      METH_VARARGS,                   sendpkt_finddevs_help},
    {"pcap",        (PyCFunction)sendpkt_pcap,          METH_VARARGS | METH_KEYWORDS,   sendpkt_pcap_help},
    {"close",       (PyCFunction)sendpkt_close,         METH_VARARGS | METH_KEYWORDS,   "Close pcap object"},
    {"capture",     (PyCFunction)sendpkt_capture,       METH_VARARGS | METH_KEYWORDS,   "Capture packet from ethernet"},
    {"sendpacket",  (PyCFunction)sendpkt_sendpacket,    METH_VARARGS | METH_KEYWORDS,   "Send packet to ethernet"},
    {NULL,NULL,0,NULL}
};

PyMODINIT_FUNC initsendpkt(void) {
    Py_InitModule("sendpkt",SendPktMethods);
}

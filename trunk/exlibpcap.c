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
    char errbuf[PCAP_ERRBUF_SIZE];
    static char *kwlist[]={"ifname","bpfstr","snaplen","promisc","to_ms",NULL};
    static char *ifname=NULL, *bpfstr=NULL;
    int snaplen=4096,promisc=1,to_ms=0;
    pcap_t* pc;
    struct bpf_program bpfprogram;
    if (!PyArg_ParseTupleAndKeywords(args,kwargs,"|ssiii",kwlist,&ifname,&bpfstr,&snaplen,&promisc,&to_ms))
        return NULL;
    if (! ifname) {
        ifname=pcap_lookupdev(errbuf);
        if (! ifname) {
            PyErr_SetString(PyExc_RuntimeError,errbuf);
            return NULL;
        }
    }
    if (! bpfstr)
        bpfstr="";
    pc=pcap_open_live(ifname,snaplen,promisc,to_ms,errbuf);
    if (! pc) {
        PyErr_SetString(PyExc_RuntimeError,errbuf);
        return NULL;
    }
    if (pcap_compile(pc,&bpfprogram,bpfstr,0,-1)) {
        PyErr_SetString(PyExc_RuntimeError,"pcap_compile() return not 0");
        return NULL;
    }
    pcap_setfilter(pc,&bpfprogram);
    pcap_freecode(&bpfprogram);     //XXX: is there wrong?
    return Py_BuildValue("l",(long)pc);
}

char sendpkt_close_help[]="Close pcap object\n\
close(pcapobj)\n\
>>> close(pcapobj)";
static PyObject* sendpkt_close(PyObject* self, PyObject* args, PyObject* kwargs) {
    static char *kwlist[]={"pcapobj",NULL};
    pcap_t *pc;
    if (!PyArg_ParseTupleAndKeywords(args,kwargs,"l",kwlist,(pcap_t*)&pc))
        return NULL;
    pcap_close(pc);
    Py_RETURN_NONE;
}

char sendpkt_capture_help[]="Capture packet from ethernet.\n\
ptime,pdata=capture(pcapobj)";
static PyObject* sendpkt_capture(PyObject* self, PyObject* args, PyObject* kwargs) {
    static char *kwlist[]={"pcapobj","to_ms",NULL};
    pcap_t *pc;
    int to_ms;
    if (!PyArg_ParseTupleAndKeywords(args,kwargs,"l|i",kwlist,(pcap_t*)&pc,&to_ms))
        return NULL;
    Py_RETURN_NONE;
}

char sendpkt_sendpacket_help[]="Send packet to ethernet.\n\
sendpacket(pcapobj,packet)";
static PyObject* sendpkt_sendpacket(PyObject* self, PyObject* args, PyObject* kwargs) {
    static char *kwlist[]={"pcapobj","packet",NULL};
    pcap_t *pc;
    unsigned char *buf;
    int buf_s;
    if (!PyArg_ParseTupleAndKeywords(args,kwargs,"ls#",kwlist,(pcap_t*)&pc,&buf,&buf_s))
        return NULL;
    //printf("pcap=%p, buf=%s, size=%d\n",pc,buf,buf_s);
    if (pcap_sendpacket(pc,buf,buf_s)) {
        PyErr_SetString(PyExc_RuntimeError,pcap_geterr(pc));
        return NULL;
    }
    Py_RETURN_NONE;
}

static PyMethodDef SendPktMethods[]={
    {"finddevs",    (PyCFunction)sendpkt_finddevs,      METH_VARARGS,                   sendpkt_finddevs_help},
    {"pcap",        (PyCFunction)sendpkt_pcap,          METH_VARARGS | METH_KEYWORDS,   sendpkt_pcap_help},
    {"close",       (PyCFunction)sendpkt_close,         METH_VARARGS | METH_KEYWORDS,   sendpkt_close_help},
    {"capture",     (PyCFunction)sendpkt_capture,       METH_VARARGS | METH_KEYWORDS,   sendpkt_capture_help},
    {"sendpacket",  (PyCFunction)sendpkt_sendpacket,    METH_VARARGS | METH_KEYWORDS,   sendpkt_sendpacket_help},
    {NULL,NULL,0,NULL}
};

PyMODINIT_FUNC initsendpkt(void) {
    Py_InitModule("sendpkt",SendPktMethods);
}

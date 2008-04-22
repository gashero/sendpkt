#! /usr/bin/env python
# -*- coding: UTF-8 -*-
# File: listener.py
# Date: 2007-09-18
# Author: gashero
# Copyright@1999-2007, Harry Gashero Liu.

import time

import pcap

DATETIME_FORMAT="%Y-%m-%d %H:%M:%S"
PCAP_FILTER=""

def main():
    pc=pcap.pcap()
    pc.setfilter(PCAP_FILTER)
    try:
        #pc.setnonblock(True)
        print "Start..."
        for ptime,pdata in pc:
            print time.strftime(DATETIME_FORMAT)
            print repr(str(pdata))
        #while True:
        #    ptime,pdata=pc.next()
        #    print time.strftime(DATETIME_FORMAT)
        #    print repr(str(pdata))
    except KeyboardInterrupt:
        print "EXIT"
    return

if __name__=="__main__":
    main()

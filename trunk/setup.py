#! /usr/bin/env python
# -*- coding: UTF-8 -*-
# File: setup.py
# Date: 2007-09-18
# Author: gashero
# Copyright@1999-2007, Harry Gashero Liu.

"""
通用的setup.py文件，自动判断操作系统类型并调用特定的文件
"""

import os
from distutils.core import setup,Extension

if os.name=="nt":
    module_exwinpcap=Extension('sendpkt',
            sources=['exwinpcap.c',],
            libraries=['wpcap',],
            include_dirs=['../WpdPack/Include',],
            library_dirs=['../WpdPack/Lib',],
            )
    extmods=[module_exwinpcap,]
elif os.name=="posix":
    module_exlibpcap=Extension('sendpkt',
            sources=['exlibpcap.c',],
            libraries=['pcap'],
            include_dirs=['/usr/local/include',],
            library_dirs=['/usr/local/lib',],
            )
    extmods=[module_exlibpcap,]
else:
    raise OSError,"Unknown OS: %s"%os.name

setup(name='sendpkt',
        version='0.3',
        description='Send ethernet frame in Linux or Win32',
        ext_modules=extmods,
        author='gashero',
        author_email='harry.python@gmail.com',
        url='http://sendpkt.googlecode.com/',
        )

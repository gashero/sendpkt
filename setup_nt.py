# -*- coding: UTF-8 -*-
# File: setup.py
# Date: 2007-09-18
# Author: gashero
# Copyright@1999-2007, Harry Gashero Liu.

"""
win32构建脚本
"""

from distutils.core import setup,Extension

module_exwinpcap=Extension('sendpkt',
        sources=['exwinpcap.c',],
        libraries=['wpcap',],
        include_dirs=['../WpdPack/Include',],
        library_dirs=['../WpdPack/Lib',]
        )

setup(name="sendpkt",
        version="0.2",
        description="Send ethernet frame in Linux or Win32",
        ext_modules=[module_exwinpcap,],
        author="gashero",
        author_email="harry.python@gmail.com",
        url="http://code.google.com/p/sendpkt/"
        )

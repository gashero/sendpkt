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

if os.name=="nt":
    from setup_nt import *
elif os.name=="posix":
    from setup_posix import *
else:
    raise OSError,"Unknown OS: %s"%os.name

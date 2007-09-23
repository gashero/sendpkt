#! /usr/bin/env python
# -*- coding: UTF-8 -*-
# File: linux/test.py
# Date: 2007-09-18
# Author: gashero
# Copyright@1999-2007, Harry Gashero Liu.

"""
总体的单元测试
"""

import unittest
import os

import dpkt

import sendpkt

class TestSendPkt(unittest.TestCase):
    """测试发包函数"""

    def setUp(self):
        self.packet="a"*80
        devlist=sendpkt.findalldevs()
        if devlist:
            if os.name=="nt":
                self.device=devlist[-1] #\\NPF\\....
            elif os.name=="posix":
                self.device=devlist[0] #eth0
            else:
                self.device=devlist[0]
        else:
            self.device=""
            raise OSError,"You must run in root mode"
        return

    def tearDown(self):
        del self.packet
        return

    def test_has_sendpacket(self):
        """测试sendpacket这个函数是否存在"""
        hassendpacket=hasattr(sendpkt,"sendpacket")
        self.assertEqual(hassendpacket,True)
        return

    def test_call_sendpacket(self):
        """尝试调用一下sendpacket函数"""
        sendpkt.sendpacket(self.packet,self.device)
        return

    def test_call_sendpacket_keyword(self):
        """尝试以关键字方式传递包"""
        sendpkt.sendpacket(packet=self.packet,device=self.device)
        return

    def test_sendpacket_typeerror(self):
        """尝试其他类型的参数以产生类型异常"""
        try:
            sendpkt.sendpacket(25)
        except TypeError,e:
            pass
        #self.assertEqual(isinstance(e,TypeError),True)
        return

class TestFindAllDevs(unittest.TestCase):
    """测试获取设备列表"""

    def test_findalldevs(self):
        """测试获取设备列表函数"""
        devlist=sendpkt.findalldevs()
        self.assertEqual((len(devlist)>=1),True)
        return

    def test_notparam(self):
        """测试对findalldevs()函数传递参数时抛出异常"""
        e=None
        try:
            devlist=sendpkt.findalldevs('faeij')
        except Exception,e:
            pass
        self.assertEqual(isinstance(e,TypeError),True)
        return

    def test_nt_generic(self):
        """测试nt类操作系统下，肯定有个网口是通用设备"""
        if os.name=='nt':
            default_devname="\\Device\\NPF_GenericDialupAdapter"
            devlist=sendpkt.findalldevs()
            self.assertEqual((default_devname in devlist),True)
        return

if __name__=="__main__":
    unittest.main()

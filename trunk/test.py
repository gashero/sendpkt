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

import dpkt

import sendpkt

class TestSendPkt(unittest.TestCase):
    """测试用例类"""

    def setUp(self):
        self.packet="fjaiewjigawojgewagjoewgjeaowjgoewogjeawijfoewogawepg"
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
        sendpkt.sendpacket(self.packet)
        return

if __name__=="__main__":
    unittest.main()

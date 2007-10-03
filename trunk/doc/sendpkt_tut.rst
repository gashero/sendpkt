=========================
SendPkt快速入门
=========================

:作者: gashero
:电邮: harry.python@gmail.com
:原文地址: http://gashero.yeax.com/?p=26
:项目主页: http://sendpkt.googlecode.com
:日期: 2007-10-03
:版本: sendpkt 0.2
:修订版: 1

.. contents:: 目录
.. sectnum::

简介
------

摘自 SendPkt_ 项目wiki。

.. _SendPkt: http://sendpkt.googlecode.com/

SendPkt是一套同时可以运行在Linux/Win32的发送链路层数据包的Python扩展库。 

SendPkt在Linux和Win32下实现方式是不同的，但是提供对Python的相同接口，确保Python程序可以不经过修改就可以移植。 

在Win32下SendPkt使用WinPcap?中的 `pcap_sendpacket()` 函数发送数据包。 

在Linux下SendPkt使用libnet的 `libnet_adv_write_link()` 函数发送数据包。 

同时SendPkt还提供了查找网络接口设备的功能，以供发送数据包时选择。在Linux下虽然可以使用libnet自动选择的网络接口设备，但是在Win32下，默认的网络设备总会导致发包失败。所以为了可移植性，你必须在发送数据包时指定使用哪个网络设备接口。

使用SendPkt的流程
-------------------

#. 选择合适的网络接口以发送数据包，因为Win32于Linux的不同，需要专门选择。

    对于Win32，通过libpcap取得的第一个网络设备接口(网卡)是一个虚拟网卡，无法发送数据包，而一般选择第二个网口，可以通过os.name判断操作系统并选择合适的网口。

    得到的网络设备接口列表，是一个字符串列表，最终提供给发送函数的网口标志也是字符串。

#. 构造数据包。推荐使用dpkt。

    dpkt是python界通用的数据包组包和解包库，在分层构造数据包时从高层(应用层)向底层依次构造数据包。并且把高层数据包的数据作为下一层数据包的data字段来使用。

    对于高性能应用，手工构造数据包也未尝不可，因为哪种方式都需要对数据包格式的深入了解。

#. 发送数据包，使用 `sendpkt.sendpacket()` 。

    例子见下面的代码。调用方式为 ``sendpkt.sendpacket(packet,device)`` 。第一个参数为数据包，为字符串对象，以太网帧。第二个参数为网络设备名，通过第一步获得。

SendPkt发送数据包的简单例子
-----------------------------

参见SendPkt项目的单元测试 ``test.py`` 。::

    import sendpkt
    #构造一个需要发送的数据包
    packet="a"*80
    #查找所有可用网络设备接口
    devlist=sendpkt.findalldevs()
    #根据操作系统选择合适的网络设备
    if devlist:
        if os.name=="nt":
            device=devlist[-1] #\\NPF\\....
        elif os.name=="posix":
            device=devlist[0] #eth0
        else:
            device=devlist[0]
    else:
        device=""
        raise OSError,"You must run in root mode"
    #发送数据包
    sendpkt.sendpacket(packet,device)

SendPkt发送ARP查询包的例子
----------------------------

ARP查询包的op字段为1，表示查询一个IP地址对应的MAC地址。如下是查询过程。::

    #! /usr/bin/env python
    # -*- coding: UTF-8 -*-
    # File: arpquery.py
    # Date: 2007-09-21
    # Author: gashero
    # Copyright@1999-2007, Harry Gashero Liu.

    """
    ARP查询的测试
    """

    from socket import inet_aton,inet_ntoa

    import dpkt
    import sendpkt

    LOCALMAC="111111"
    LOCALIP=inet_aton("192.168.1.104")
    REMOTEMAC=""
    REMOTEIP=inet_aton("192.168.1.1")

    def main():
        global LOCALMAC
        global LOCALIP
        device=sendpkt.findalldevs()[0] #for linux is eth0
        arp=dpkt.arp.ARP(
                hrd=1,
                pro=2048,
                hln=6,
                pln=4,
                op=1,
                sha=LOCALMAC,
                spa=LOCALIP,
                tha="\x00"*6,
                tpa=inet_aton("192.168.1.1")
                )
        ether=dpkt.ethernet.Ethernet(
                dst="\xff"*6,
                src=LOCALMAC,
                type=0x0806,
                data=arp
                )
        #print ether
        #print repr(dpkt.ethernet.Ethernet(str(ether)))
        sendpkt.sendpacket(str(ether),device)
        return

    if __name__=="__main__":
        main()

参考
------

详细问题请发邮件到 harry.python@gmail.com 询问。

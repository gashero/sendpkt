# 什么是SendPkt #

SendPkt是一套同时可以运行在Linux/Win32的发送链路层数据包的Python扩展库。

SendPkt在Linux和Win32下实现方式是不同的，但是提供对Python的相同接口，确保Python程序可以不经过修改就可以移植。

在Win32下SendPkt使用WinPcap中的 `pcap_sendpacket()` 函数发送数据包。

在Linux下SendPkt使用libnet的 `libnet_adv_write_link()` 函数发送数据包。

同时SendPkt还提供了查找网络接口设备的功能，以供发送数据包时选择。在Linux下虽然可以使用libnet自动选择的网络接口设备，但是在Win32下，默认的网络设备总会导致发包失败。所以为了可移植性，你必须在发送数据包时指定使用哪个网络设备接口。

SendPkt项目的开发者是gashero，有相关问题可以发邮件给这厮( harry.python@gmail.com )。另外，他还有个博客( http://gashero.yeax.com/ )，上面会包含一些SendPkt的使用例子。




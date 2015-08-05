# 使用MinGW构建Python扩展包 #

在包目录下建立文件setup.cfg，内容如下：
```
[build]
compiler=mingw32
```
之后重新使用标准的distutils方式构建和安装即可。当然，确保你的PATH路径可以找到MinGW的gcc.exe。
```
python setup.py build
python setup.py install
```
你还可以自己构造Windows安装包，如下：

python setup.py bdist\_wininst

另外需要小心的一点就是，在Win32下编译SendPkt需要用到WinPcap的开发包WpdPack。现在的版本使用的WinPcap 3.1和对应的WpdPack。这个开发包应该跟SendPkt的svn目录放在同一个路径下，这样才方便setup\_nt.py找到路径。如：

some\_dir

> + sendpkt ...

> + WpdPack ...




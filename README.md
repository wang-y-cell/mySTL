# mySTL
## 简介
这是一个基于c++11的STL库,主要是面向c++学习及stl的学习,包含了常用的容器、算法和迭代器。目前是第一个版本,可能会出现许多错误,后续会不断更改,但也只是修复一些bug和错误,少量增加内容,如有发现错误,请在``issues``指出,如有时间,会尽快修复。

作为新手第一个项目,stl的实现主要是参考了sgi stl的源码,也增加了少许自己的理解何实现,你在学习过程中一定会发现许多不完整的地方,那是故意为之,stl中虽有许多容器,但是核心思想非常相似,像构造函数的参数,大多都是一摸一样的,只是实现根据不同的数据结构而有所不同,所以我只在前面的序列容器中实现,后面的关联容器中只实现必要的函数

> 参考书籍: <<stl源码剖析>>
# 项目介绍
本项目用于学习stl的内部实现和编程技巧

使用ai测试代码寻找错误,但也只是简单的测试,并不能完全覆盖所有情况,这也是测试样例混乱的原因

本项目不包括:

``string`` 严格来说不属于stl,没有实现

``slist(forward_list)`` 与list实现方式类似, 没有实现
容器函数只实现重要部分,其余没有实现

本项目保证包含stl大量重要算法实现和编程技巧,放弃一些重复的构造函数等相关函数,主要是为了学习和理解stl的内部实现,而不是为了实际使用(实际使用也是可以的: ) 。

# 支持
---
- 操作系统
  1. linux
  2. windows
  3. macOS
- 编译器
    1. g++
    2. msvc
    3. clang

# 运行
vscode: 
    1. 克隆仓库
``` bash
# 如果你没有ssh密钥
git clone https://github.com/wang-y-cell/mySTL.git
#如果你有密钥
git clone git@github.com:wang-y-cell/mySTL.git
```

2. 构建项目
g++
```bash
# 终端或git bash

# windows powershell: 
$ mkdir build && cd build
$ cmake .. -G "MinGW Makefiles" #其中..是CmakeLists.txt是相对build目录的路径
$ cmake --build .

# linux
# 运行build.sh脚本
cd scripts
sh build.sh

############################################
# 手动构建
$ mkdir build && cd build
$ cmake .. 
$ make

```

生成的可执行文件在bin目录中,可以在终端或git bash中运行。

- msvc
    Download ZIP文件,解压后用msvc打开项目。

---


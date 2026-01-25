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
$ mkdir build && cd build
$ cmake .. 
$ make

```

生成的可执行文件在bin目录中,可以在终端或git bash中运行。

- msvc
    Download ZIP文件,解压后用msvc打开项目。

---

# mySTL

## Introduction
This is an STL library based on C++11, primarily aimed at C++ learners and those studying the STL, including commonly used containers, algorithms, and iterators. This is the first version, so there may be many bugs. We will continuously make updates—mainly fixing bugs and errors, with only minor additions. If you find any issues, please report them in the **Issues** section. If time permits, we will fix them as soon as possible.

As a beginner's first project, this STL implementation is mainly inspired by the SGI STL source code, with some of my own understanding and modifications added. During your learning process, you will certainly notice many incomplete parts—this is intentional. Although the STL includes many containers, their core design principles are very similar. For example, constructor parameters are mostly identical; only the internal implementations differ based on the underlying data structure. Thus, I fully implement these common interfaces in the early sequential containers, while later associative containers only include essential functions.

> Reference book: *The Annotated STL Sources*

## Project Overview
This project is intended for learning the internal implementation of the STL and improving programming skills.

Using AI to test code and find errors is a simple test that cannot fully cover all situations, which is why the test cases are messy.

### What this project does **not** include:
- `string`: Strictly speaking, it is not part of the STL, so it is not implemented.

- `slist` (`forward_list`): Its implementation is similar to `list`, so it is omitted.

- Container member functions: Only the most important ones are implemented; others are left out.


This project guarantees to include many important STL algorithm implementations and programming techniques, while omitting some repetitive constructors and related functions. The main purpose is to learn and understand the internal implementation of STL, rather than for actual use (though use is still allowed: ).

## Supported Platforms
---
- **Operating Systems**
  1. Linux
  2. Windows
  3. macOS

- **Compilers**
  1. g++
  2. MSVC
  3. Clang

## How to Run

### VS Code:
1. Clone the repository
```bash
# If you don't have an SSH key:
git clone https://github.com/wang-y-cell/mySTL.git

# If you have an SSH key:
git clone git@github.com:wang-y-cell/mySTL.git
```
2. build the project

with g++
```bash
# In terminal or Git Bash

 # On Windows (PowerShell):
 $  mkdir build && cd build
 $  cmake .. -G "MinGW Makefiles"  # ".." is the path to CMakeLists.txt relative to the build directory
 $  cmake --build .

# On Linux:
 $  mkdir build && cd build
 $  cmake ..
 $  make
```

With MSVC on windows:
Download the ZIP file, extract it, and open the project directly in Visual Studio (MSVC).
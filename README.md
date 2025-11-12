# MySTL

这是一个C++标准模板库（STL）的自定义实现。

## 简介

本项目旨在实现C++ STL中一些最常见的数据结构和算法。其目的是用于教育和更深入地理解这些组件的工作原理。

## 特性

*   **数据结构**:
    *   `vector`
    *   `list`
    *   `map`
    *   `set`
*   **算法**:
    *   `sort`
    *   `find`
    *   `copy`

## 入门

要使用此库，只需在你的项目中包含 `vector.h` 目录中的头文件即可。

```cpp
#include "vector.h"
#include <iostream>

int main() {
    mystl::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    for (int i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

## 运行测试

要运行测试，你需要一个C++编译器和一个像Google Test这样的测试框架。

```bash
g++ -std=c++11 tests/vector_test.cpp -o vector_test
./vector_test
```

## 贡献

欢迎贡献！请随时提交拉取请求或开启一个问题。
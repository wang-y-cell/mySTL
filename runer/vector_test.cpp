#include "../include/vector.h"
#include <iostream>
#include <string>
#include <utility>

void test_basic_operations() {
    std::cout << "=== 测试基本操作 ===" << std::endl;
    
    // 默认构造函数
    mystl::vector<int> v1;
    std::cout << "默认构造: size=" << v1.size() << ", capacity=" << v1.capacity() << std::endl;
    
    // 指定大小构造函数
    mystl::vector<int> v2(5);
    std::cout << "指定大小构造(5): size=" << v2.size() << ", capacity=" << v2.capacity() << std::endl;
    
    // 指定大小和值构造函数
    mystl::vector<int> v3(3, 42);
    std::cout << "指定大小和值构造(3, 42): ";
    for (size_t i = 0; i < v3.size(); ++i) {
        std::cout << v3[i] << " ";
    }
    std::cout << std::endl;
}

void test_push_back_and_pop() {
    std::cout << "\n=== 测试push_back和pop_back ===" << std::endl;
    
    mystl::vector<int> v;
    
    // 测试push_back
    std::cout << "push_back操作: ";
    for (int i = 1; i <= 5; ++i) {
        v.push_back(i * 10);
        std::cout << "添加" << (i * 10) << "后: size=" << v.size() << ", capacity=" << v.capacity() << "; ";
    }
    std::cout << std::endl;
    
    // 测试pop_back
    std::cout << "pop_back操作: ";
    while (!v.empty()) {
        std::cout << "删除" << v[v.size()-1] << "后: size=" << v.size()-1 << "; ";
        v.pop_back();
    }
    std::cout << std::endl;
}

void test_iterators() {
    std::cout << "\n=== 测试迭代器 ===" << std::endl;
    
    mystl::vector<int> v;
    for (int i = 1; i <= 5; ++i) {
        v.push_back(i * 10);
    }
    
    std::cout << "使用迭代器遍历: ";
    for (mystl::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::cout << "使用at()方法访问: ";
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << v.at(i) << " ";
    }
    std::cout << std::endl;
}

void test_copy_and_assignment() {
    std::cout << "\n=== 测试拷贝构造和赋值 ===" << std::endl;
    
    mystl::vector<int> v1;
    for (int i = 1; i <= 3; ++i) {
        v1.push_back(i * 10);
    }
    
    // 拷贝构造
    mystl::vector<int> v2(v1);
    std::cout << "拷贝构造后v2: ";
    for (size_t i = 0; i < v2.size(); ++i) {
        std::cout << v2[i] << " ";
    }
    std::cout << std::endl;
    
    // 赋值操作
    mystl::vector<int> v3;
    v3 = v1;
    std::cout << "赋值后v3: ";
    for (size_t i = 0; i < v3.size(); ++i) {
        std::cout << v3[i] << " ";
    }
    std::cout << std::endl;
}

void test_move_semantics() {
    std::cout << "\n=== 测试移动语义 ===" << std::endl;
    
    mystl::vector<std::string> v1;
    v1.push_back("Hello");
    v1.push_back("World");
    
    std::cout << "移动构造前v1大小: " << v1.size() << std::endl;
    
    // 移动构造（如果支持）
    #ifdef MYSTL_HAS_MOVE_SEMANTICS
    mystl::vector<std::string> v2(std::move(v1));
    std::cout << "移动构造后v2大小: " << v2.size() << ", v1大小: " << v1.size() << std::endl;
    
    mystl::vector<std::string> v3;
    v3 = std::move(v2);
    std::cout << "移动赋值后v3大小: " << v3.size() << ", v2大小: " << v2.size() << std::endl;
    #else
    std::cout << "编译器不支持移动语义" << std::endl;
    #endif
}

void test_capacity_management() {
    std::cout << "\n=== 测试容量管理 ===" << std::endl;
    
    mystl::vector<int> v;
    
    std::cout << "初始状态: size=" << v.size() << ", capacity=" << v.capacity() << std::endl;
    
    // 测试reserve
    v.reserve(10);
    std::cout << "reserve(10)后: size=" << v.size() << ", capacity=" << v.capacity() << std::endl;
    
    // 测试resize
    v.resize(5);
    std::cout << "resize(5)后: size=" << v.size() << ", capacity=" << v.capacity() << std::endl;
    
    // 测试clear
    v.clear();
    std::cout << "clear()后: size=" << v.size() << ", capacity=" << v.capacity() << std::endl;
}

void test_exception_handling() {
    std::cout << "\n=== 测试异常处理 ===" << std::endl;
    
    mystl::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    
    // 测试越界访问
    MYSTL_TRY {
        std::cout << "尝试访问越界元素..." << std::endl;
        std::cout << "v.at(10) = " << v.at(10) << std::endl;
    }
    MYSTL_CATCH_ALL {
        std::cout << "捕获到异常: 索引超出范围" << std::endl;
    }
    
    // 测试空vector的pop_back
    mystl::vector<int> empty_v;
    MYSTL_TRY {
        std::cout << "尝试从空vector中pop_back..." << std::endl;
        empty_v.pop_back();
    }
    MYSTL_CATCH_ALL {
        std::cout << "捕获到异常: vector为空" << std::endl;
    }
}

void test_swap() {
    std::cout << "\n=== 测试swap操作 ===" << std::endl;

    mystl::vector<int> v1;
    v1.push_back(1);
    v1.push_back(2);
    
    mystl::vector<int> v2;
    v2.push_back(10);
    v2.push_back(20);
    v2.push_back(30);
    
    std::cout << "交换前: v1大小=" << v1.size() << ", v2大小=" << v2.size() << std::endl;
    
    v1.swap(v2);
    
    std::cout << "交换后: v1大小=" << v1.size() << ", v2大小=" << v2.size() << std::endl;
    std::cout << "v1内容: ";
    for (size_t i = 0; i < v1.size(); ++i) {
        std::cout << v1[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "=== MySTL Vector 综合测试 ===" << std::endl;
    
    test_basic_operations();
    test_push_back_and_pop();
    test_iterators();
    test_copy_and_assignment();
    test_move_semantics();
    test_capacity_management();
    test_exception_handling();
    test_swap();
    
    std::cout << "\n=== 所有测试完成 ===" << std::endl;
    
    return 0;
}

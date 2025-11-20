#include"../include/memory.h"
#include <iostream>

struct MyResource {
    MyResource() { std::cout << "MyResource 已创建\n"; }
    ~MyResource() { std::cout << "MyResource 已销毁\n"; }
    void say_hello() { std::cout << "来自 MyResource 的问候\n"; }
};

void test_ownership_transfer() {
    std::cout << "=== 测试所有权转移 ===\n";
    mystl::auto_ptr<MyResource> p1(new MyResource());
    p1->say_hello();

    std::cout << "p1 指针: " << p1.get() << std::endl;

    mystl::auto_ptr<MyResource> p2(p1); // p1 将所有权转移给 p2
    std::cout << "p2 指针: " << p2.get() << std::endl;
    std::cout << "转移后 p1 指针: " << p1.get() << std::endl; // p1 现在为空

    p2->say_hello();

    // p1->say_hello(); // 这将导致程序崩溃，因为 p1 为空

    std::cout << "test_ownership_transfer 作用域结束，p2 即将被销毁。\n";
}

void test_assignment() {
    std::cout << "\n=== 测试赋值操作 ===\n";
    mystl::auto_ptr<MyResource> p1(new MyResource());
    mystl::auto_ptr<MyResource> p2;

    p2 = p1; // p1 将所有权转移给 p2

    std::cout << "p2 pointer: " << p2.get() << std::endl;
    std::cout << "赋值后 p1 指针: " << p1.get() << std::endl; // p1 现在为空

    p2->say_hello();
    std::cout << "test_assignment 作用域结束，p2 即将被销毁。\n";
}

void test_reset_and_release() {
    std::cout << "\n=== 测试 Reset 和 Release ===\n";
    mystl::auto_ptr<MyResource> p1(new MyResource());
    p1->say_hello();

    MyResource* raw_ptr = p1.release(); // p1 释放所有权
    std::cout << "释放后 p1 指针: " << p1.get() << std::endl;

    std::cout << "手动使用原始指针...\n";
    raw_ptr->say_hello();

    p1.reset(raw_ptr); // p1 重新获得所有权
    std::cout << "reset 后 p1 指针: " << p1.get() << std::endl;

    p1.reset(new MyResource()); // p1 删除旧资源并获取新资源的所有权

    std::cout << "test_reset_and_release 作用域结束，p1 即将被销毁。\n";
}

int main() {
    test_ownership_transfer();
    test_assignment();
    test_reset_and_release();
    return 0;
}
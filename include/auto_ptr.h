#ifndef MYSTL_AUTO_PTR_H
#define MYSTL_AUTO_PTR_H

#include "stl_config.h"

//这个类的存在使得auto_ptr可以指向临时变量,但是这也是auto_ptr最终的设计缺陷.
//auto_ptr在c++17中被废弃,因为他的设计缺陷使得他的使用十分复杂,导致了很多错误.
//在c++11中引入的shared_ptr可以解决这个问题,因为有了右值引用的存在
//#if MYSTL_CPP_VERSION < 17

namespace mystl {

// auto_ptr_ref 的前置声明
template<class T>
struct auto_ptr_ref;

template<class T>
class auto_ptr {
private:
    T* pointee;

public:
    typedef T elem_type; // 元素类型

    // 构造函数：获取指针 p 的所有权
    explicit auto_ptr(T* p = 0) : pointee(p) {}

    // 拷贝构造函数：转移所有权
    auto_ptr(auto_ptr& rhs) : pointee(rhs.release()) {}

    // 兼容类型的拷贝构造函数：转移所有权
    template<class U>
    auto_ptr(auto_ptr<U>& rhs) : pointee(rhs.release()) {}

    // 赋值操作符：转移所有权
    auto_ptr& operator=(auto_ptr& rhs) {
        if (this != &rhs) {
            delete pointee;
            pointee = rhs.release();
        }
        return *this;
    }

    // 兼容类型的赋值操作符：转移所有权
    template<class U>
    auto_ptr& operator=(auto_ptr<U>& rhs) {
        if (this->get() != rhs.get()) {
            delete pointee;
            pointee = rhs.release();
        }
        return *this;
    }

    // 析构函数：销毁所拥有的对象
    ~auto_ptr() {delete pointee;}

    // 解引用操作符
    T& operator*() const {return *pointee;}
    T* operator->() const {return pointee;}

    // 获取原始指针
    T* get() const {return pointee;}

    // 释放指针的所有权
    T* release() {
        T* tmp = pointee;
        pointee = 0;
        return tmp;
    }

    // 获取一个新指针的所有权
    void reset(T* p = 0) {
        if (pointee != p) {
            delete pointee;
            pointee = p;
        }
    }

    // 特殊构造函数，用于从 auto_ptr_ref 构建，以支持从临时 auto_ptr 赋值
    auto_ptr(auto_ptr_ref<T> ref) : pointee(ref.ptr) {}

    // 从 auto_ptr_ref 赋值
    auto_ptr& operator=(auto_ptr_ref<T> ref) {
        if (pointee != ref.ptr) {
            delete pointee;
            pointee = ref.ptr;
        }
        return *this;
    }

    // 转换到 auto_ptr_ref，用于处理赋值过程中的临时 auto_ptr 对象
    template<class U>
    operator auto_ptr_ref<U>() {
        return auto_ptr_ref<U>(this->release());
    }

    template<class U>
    operator auto_ptr<U>() {
        return auto_ptr<U>(this->release());
    }
};

// 用于从临时 auto_ptr 对象转移所有权的辅助结构体
template<class T>
struct auto_ptr_ref {
    T* ptr;
    explicit auto_ptr_ref(T* p) : ptr(p) {}
};

} // namespace mystl

//#endif // MYSTL_CPP_VERSION < 17

#endif // MYSTL_AUTO_PTR_H

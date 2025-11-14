// mystl 第一级内存配置器（基于 malloc/free），支持可配置的 OOM 回调
#ifndef MYSTL_ALLOC_H
#define MYSTL_ALLOC_H

// 头文件依赖：基础大小类型、C 风格分配接口、异常类型
#include <cstddef>
#include <cstdlib>
#include <new>

// OOM 行为宏：控制在内存不足时的默认动作
// - 若定义为抛异常：建议写为 throw bad_alloc
// - 若未定义：默认打印错误并退出（不可恢复，适用于演示/临时场景）
#if 1                                                                                                     
#   include <new>
#   define __THROW_BAD_ALLOC throw std::bad_alloc()
#elif !defined(__THROW_BAD_ALLOC)
#   include<iostream>
#   define __THROW_BAD_ALLOC std::cerr << "out of memory" << std::endl;exit(1);
#endif

// 所有实现位于 mystl 命名空间中，便于与标准库或其他库隔离
namespace mystl {




// 第一级内存配置器模板：直接使用 malloc/free/realloc
// - 非类型模板参数 inst 用于隔离静态 OOM 处理器（不同 inst 拥有各自的处理器状态）
template <int inst>
class malloc_alloc_template {
private:
    // 在 malloc 失败时的回退逻辑：调用处理器并重试分配
    static void* oom_malloc(size_t);
    // 在 realloc 失败时的回退逻辑：调用处理器并重试（当前仅声明，尚未定义）
    static void* oom_realloc(void* p, size_t new_sz);
    // OOM 处理器：类型为无参、返回 void 的函数指针
    static void (* __malloc_alloc_oom_handler)();

public:
    // 字节级分配：成功返回非空指针，失败进入 OOM 回退
    static void* allocate(std::size_t n) {
        void* p = malloc(n);
        if (!p) p = oom_malloc(n);
        return p;
    }

    // 字节级释放：第二参数保留以保持接口一致（未使用）
    static void deallocate(void* p, std::size_t) {
        std::free(p);
    }

    // 字节级重分配：失败进入 OOM 回退（注意：oom_realloc 未在本文件中定义）
    static void* reallocate(void* p, std::size_t, std::size_t new_sz) {
        void* r = realloc(p, new_sz);
        if (!r) r = oom_realloc(p, new_sz);
        return r;
    }

    // 设置 OOM 处理器：返回旧处理器指针，便于还原
    // 返回值为函数指针的成员函数
    static void (* set_malloc_handler(void (*f)()))() {
        void (* old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return old;
    }
    //如果不好理解,可以换看下面的,但是我的stl为了适配多个c++标准库,所以不能使用using,因为using是c++11的特性
    // using handler_type = void (*)();
    // static handler_type set_malloc_handler(handler_type f) {
    //     handler_type old = __malloc_alloc_oom_handler;
    //     __malloc_alloc_oom_handler = f;
    //     return old;
    // }
    //是同样的道理

};

// 为每个 inst 专门化定义并初始化静态 OOM 处理器指针（此处使用 0，可改为 nullptr）
template <int inst>
void (*malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;//c++11可以使用nullptr

// OOM 回退实现：循环调用处理器并重试分配，直到成功或处理器未设置时触发默认 OOM 行为
template<int inst>
void* malloc_alloc_template<inst>::oom_malloc(size_t n) {
    void (* handler)();
    void* ret;
    for(;;){
        handler = __malloc_alloc_oom_handler;
        if(!handler) __THROW_BAD_ALLOC;
        (*handler)();
        ret = malloc(n);
        if(ret) return ret;
    }
}

template<int inst>
void* malloc_alloc_template<inst>::oom_realloc(void*p, size_t new_sz) {
    void (* handler)();
    void* ret;
    for(;;){
        handler = __malloc_alloc_oom_handler;
        if(!handler) __THROW_BAD_ALLOC;
        (*handler)();//调用处理器
        ret = realloc(p, new_sz);
        if(ret) return ret;
    }
}

// 默认实例别名：使用 inst=0 的配置器；如需隔离处理器状态可选择其他 inst 值
typedef malloc_alloc_template<0> malloc_alloc;

// 元素级适配器：将字节级配置器适配为按元素个数进行分配/释放
// 用法示例：auto* arr = simple_alloc<int, malloc_alloc>::allocate(10);
template <class T, class Alloc>
struct simple_alloc {
    // 分配 n 个元素的存储空间；当 n==0 时返回空指针
    static T* allocate(std::size_t n) {
        return n ? static_cast<T*>(Alloc::allocate(n * sizeof(T))) : 0;
    }

    // 分配单个元素的存储空间
    static T* allocate() {
        return static_cast<T*>(Alloc::allocate(sizeof(T)));
    }

    // 释放 n 个元素的存储空间
    static void deallocate(T* p, std::size_t n) {
        Alloc::deallocate(p, n * sizeof(T));
    }

    // 释放单个元素的存储空间
    static void deallocate(T* p) {
        Alloc::deallocate(p, sizeof(T));
    }
};


} // namespace mystl

#endif
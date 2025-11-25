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
namespace msl {




// 第一级内存配置器模板：直接使用 malloc/free/realloc
// - 非类型模板参数 inst 用于隔离静态 OOM 处理器（不同 inst 拥有各自的处理器状态）
template <int inst> //似乎没有派上用场
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
    // 循环调用 OOM 处理器，直到分配成功
    // 风险：若 OOM 处理器无法释放内存也未终止程序，将导致死循环
    for(;;){
        handler = __malloc_alloc_oom_handler;
        if(!handler) __THROW_BAD_ALLOC;
        (*handler)(); // OOM 处理器必须能释放内存或终止程序
        ret = malloc(n);
        if(ret) return ret;
    }
}

template<int inst>
void* malloc_alloc_template<inst>::oom_realloc(void*p, size_t new_sz) {
    void (* handler)();
    void* ret;
    // 循环调用 OOM 处理器，直到重分配成功
    // 风险：若 OOM 处理器无法释放内存也未终止程序，将导致死循环
    for(;;){
        handler = __malloc_alloc_oom_handler;
        if(!handler) __THROW_BAD_ALLOC;
        (*handler)(); // OOM 处理器必须能释放内存或终止程序
        ret = realloc(p, new_sz);
        if(ret) return ret;
    }
}

// 默认实例别名：使用 inst=0 的配置器；如需隔离处理器状态可选择其他 inst 值
typedef malloc_alloc_template<0> malloc_alloc;



//第二级内存配置器模板：内存池 + 自由链表
enum { ALIGN = 8 };
enum { MAX_BYTES = 128 };
enum { NFREELISTS = MAX_BYTES / ALIGN };


//单线程内存配置器
template <int inst>
class default_alloc_template {
private:
    union obj {
        union obj* free_list_link; 
        char client_data[1]; 
    };
    
        // volatile 不保证原子性，此内存池严格单线程，移除 volatile 避免误解
    static obj* free_list[NFREELISTS];
    static char* start_free;
    static char* end_free;
    static size_t heap_size;
private:
    static size_t ROUND_UP(size_t bytes) { return (bytes + ALIGN - 1) & ~(ALIGN - 1); }
    static size_t FREELIST_INDEX(size_t bytes) { return (bytes + ALIGN - 1) / ALIGN - 1; }
    static void* refill(size_t n);
    static char* chunk_alloc(size_t size, int& nobjs);
public:
    static void* allocate(std::size_t n) {
        if (n > MAX_BYTES) return malloc_alloc::allocate(n);
        size_t index = FREELIST_INDEX(n);
        obj** my_list = free_list + index;
        obj* result = *my_list;
        if (result) { *my_list = result->free_list_link; return result; }
        return refill(ROUND_UP(n));
    }
    static void deallocate(void* p, std::size_t n) {
        if (!p || n == 0) return;
        obj *q = (obj*)p;
        obj ** my_free_list;
        if(n > (size_t)MAX_BYTES) {
            malloc_alloc::deallocate(p, n);
            return;
        }
        my_free_list = free_list + FREELIST_INDEX(n);
        q->free_list_link = *my_free_list;
        *my_free_list = q;
    }
    static void* reallocate(void* p, std::size_t old_sz, std::size_t new_sz) {
        if (old_sz > MAX_BYTES && new_sz > MAX_BYTES) return malloc_alloc::reallocate(p, old_sz, new_sz);
        if (ROUND_UP(old_sz) == ROUND_UP(new_sz)) return p;
        void* result = allocate(new_sz);
        size_t copy_sz = old_sz < new_sz ? old_sz : new_sz;
        unsigned char* r = (unsigned char*)result;
        unsigned char* s = (unsigned char*)p;
        for (size_t i = 0; i < copy_sz; ++i) r[i] = s[i];
        deallocate(p, old_sz);
        return result;
    }
};

template <int inst>
typename default_alloc_template<inst>::obj*
default_alloc_template<inst>::free_list[NFREELISTS] = {0};

template <int inst>
char* default_alloc_template<inst>::start_free = 0;

template <int inst>
char* default_alloc_template<inst>::end_free = 0;

template <int inst>
size_t default_alloc_template<inst>::heap_size = 0;

template <int inst>
void* default_alloc_template<inst>::refill(size_t n) {
    int nobjs = 20;
    char* chunk = chunk_alloc(n, nobjs);
    if (nobjs == 1) return (void*)chunk;
    size_t index = FREELIST_INDEX(n);
    obj** my_list = free_list + index;
    char* cur = chunk + n;
    *my_list = (obj*)cur;
    obj* current = (obj*)cur;
    for (int i = 1; i < nobjs - 1; ++i) {
        char* next = cur + n;
        current->free_list_link = (obj*)next;
        current = (obj*)next;
        cur = next;
    }
    current->free_list_link = 0;
    return (void*)chunk;
}


template <int inst>
char* default_alloc_template<inst>::chunk_alloc(size_t size, int& nobjs) {
    size_t total_bytes = size * nobjs;
    size_t bytes_left = end_free - start_free;
    char* result;
    if (bytes_left >= total_bytes) {
        result = start_free;
        start_free += total_bytes;
        return result;
    } else if (bytes_left >= size) {
        nobjs = (int)(bytes_left / size);
        total_bytes = size * nobjs;
        result = start_free;
        start_free += total_bytes;
        return result;
    } else {
        if (bytes_left > 0) {
            size_t index = FREELIST_INDEX(bytes_left);
            obj** my_list = free_list + index;
            ((obj*)start_free)->free_list_link = *my_list;
            *my_list = (obj*)start_free;
            // 把池尾的这段剩余空间视为一个自由块： (obj*)start_free
            // 将其 free_list_link 指向原表头： ((obj*)start_free)->free_list_link = *my_list
            // 更新表头为该块： *my_list = (obj*)start_free 
        }
        size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4); // 申请两倍于需求量 + 额外量的内存
        start_free = (char*)malloc(bytes_to_get);
        if (start_free == 0) {
            int i;
            obj** me_free_list, *p;
            for(i = size; i <= MAX_BYTES; i += ALIGN) {
                me_free_list = free_list + FREELIST_INDEX(i);
                p = *me_free_list;
                if(p) {
                    *me_free_list = p->free_list_link;
                    start_free = (char*)p;
                    end_free = start_free + i;
                    return(chunk_alloc(size,nobjs));
                }
            }      
            end_free = 0;    
            start_free = (char*)malloc_alloc::allocate(bytes_to_get);
        }
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        return chunk_alloc(size, nobjs);
    }
}


typedef default_alloc_template<0> default_alloc;

#ifdef __USE_SECOND_ALLOC //默认是没有定义这个宏的
    typedef malloc_alloc alloc;
#else
    typedef default_alloc alloc;
#endif


// 元素级适配器：将字节级配置器适配为按元素个数进行分配/释放
// 用法示例：auto* arr = simple_alloc<int, malloc_alloc>::allocate(10);
template <typename T, typename Alloc>
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
        if (!p || n == 0) return;
        Alloc::deallocate(p, n * sizeof(T));
    }

    // 释放单个元素的存储空间
    static void deallocate(T* p) {
        if (!p) return;
        Alloc::deallocate(p, sizeof(T));
    }
};


} // namespace msl

#endif

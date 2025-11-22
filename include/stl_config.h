#ifndef MYSTL_CONFIG_H
#define MYSTL_CONFIG_H

// 编译器检测
#if defined(__GNUC__)
#   define MYSTL_COMPILER_GNUC
#elif defined(_MSC_VER)
#   define MYSTL_COMPILER_MSVC
#elif defined(__clang__)
#   define MYSTL_COMPILER_CLANG
#else
#   define MYSTL_COMPILER_UNKNOWN
#endif

// 平台检测
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#   define MYSTL_PLATFORM_WINDOWS
#elif defined(__linux__) || defined(__linux)
#   define MYSTL_PLATFORM_LINUX
#elif defined(__APPLE__)
#   define MYSTL_PLATFORM_APPLE
#endif

// 处理器架构检测
#if defined(__x86_64__) || defined(_M_X64)
#   define MYSTL_ARCH_X86_64
#elif defined(__i386__) || defined(_M_IX86)
#   define MYSTL_ARCH_X86
#elif defined(__aarch64__)
#   define MYSTL_ARCH_ARM64
#elif defined(__arm__)
#   define MYSTL_ARCH_ARM
#endif

// C++标准版本检测
#if __cplusplus >= 201703L
#   define MYSTL_CPP_VERSION 17
#elif __cplusplus >= 201402L
#   define MYSTL_CPP_VERSION 14
#elif __cplusplus >= 201103L
#   define MYSTL_CPP_VERSION 11
#else
#   define MYSTL_CPP_VERSION 98
#endif

// 特性检测
#if MYSTL_CPP_VERSION >= 11
#   define MYSTL_HAS_MOVE_SEMANTICS
#   define MYSTL_HAS_VARIADIC_TEMPLATES
#   define MYSTL_HAS_RVALUE_REFERENCES
#endif

// 内存对齐支持
#if defined(__GNUC__) && (__GNUC__ >= 5 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9))
#   define MYSTL_HAS_ALIGNAS
#endif

// 类模板偏特化支持
#if !defined(_PARTIAL_SPECIALIZATION_OF_CLASS_TEMPLATES)
#   if defined(__GNUC__) && (__GNUC__ >= 3)
#       define _PARTIAL_SPECIALIZATION_OF_CLASS_TEMPLATES
#   endif
#endif

#ifdef _PARTIAL_SPECIALIZATION_OF_CLASS_TEMPLATES
#   define __STL_CLASS_PARTIAL_SPECIALIZATION
#endif

// 线程支持检测
#if defined(_PTHREADS) || defined(__PTHREADS) || \
    defined(__POSIX_THREADS) || defined(_POSIX_THREADS)
#   define __STL_PTHREADS
#endif

// Windows线程支持
#if defined(MYSTL_PLATFORM_WINDOWS)
#   define __STL_WIN32THREADS
#endif

// STL特性检测
#if defined(__GNUC__) && (__GNUC__ >= 3)
#   define __STL_MEMBER_TEMPLATES
#endif

#if defined(__GNUC__) && (__GNUC__ >= 3)
#   define __STL_FRIEND_TEMPLATES
#endif

// 异常处理支持
#ifndef MYSTL_NO_EXCEPTIONS
#   define MYSTL_USE_EXCEPTIONS
#   define MYSTL_TRY try
#   define MYSTL_CATCH_ALL catch(...)
#   define MYSTL_THROW(x) throw x
#   define MYSTL_RETHROW throw
#else
#   define MYSTL_TRY
#   define MYSTL_CATCH_ALL if(false)
#   define MYSTL_THROW(x)
#   define MYSTL_RETHROW
#endif

// RTTI 支持
#ifndef MYSTL_NO_RTTI
#   define MYSTL_HAS_RTTI
#endif

// 常用类型定义
namespace mystl {
    typedef unsigned int size_t;
    
    // nullptr 实现（如果编译器不支持）
    #if MYSTL_CPP_VERSION < 11
        const class {
        public:
            template<class T>
            operator T*() const { return 0; }
            
            template<class C, class T>
            operator T C::*() const { return 0; }
        
        private:
            void operator&() const;
        } nullptr = {};
    #endif
    
    // 静态断言实现（如果编译器不支持）
    #if MYSTL_CPP_VERSION < 11
        template<bool>
        struct StaticAssert;
        
        template<>
        struct StaticAssert<true> {};
        
        #define static_assert(expr, msg) \
            { mystl::StaticAssert<(expr)> static_assert_failed; (void)static_assert_failed; }
    #endif
}

#endif // MYSTL_CONFIG_H

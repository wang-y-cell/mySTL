# mystl配置文件详解

本文档详细解释了`stl_config.h`文件中每个部分的功能和作用。

## 1. 文件保护

```cpp
#ifndef MYSTL_CONFIG_H
#define MYSTL_CONFIG_H
```

这是标准的头文件保护机制，防止头文件被重复包含，避免重复定义错误。

## 2. 编译器检测

```cpp
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
```

这部分用于检测当前使用的编译器：
- `__GNUC__`：GNU GCC 编译器
- `_MSC_VER`：Microsoft Visual C++ 编译器
- `__clang__`：Clang 编译器
- 如果都不是，则标记为未知编译器

## 3. 平台检测

```cpp
// 平台检测
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#   define MYSTL_PLATFORM_WINDOWS
#elif defined(__linux__) || defined(__linux)
#   define MYSTL_PLATFORM_LINUX
#elif defined(__APPLE__)
#   define MYSTL_PLATFORM_APPLE
#endif
```

检测操作系统平台：
- Windows 系统
- Linux 系统
- Apple/MacOS 系统

## 4. 处理器架构检测

```cpp
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
```

检测处理器架构：
- x86_64 (64位Intel/AMD)
- x86 (32位Intel/AMD)
- ARM64 (64位ARM)
- ARM (32位ARM)

## 5. C++标准版本检测

```cpp
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
```

检测C++标准版本：
- C++17 (201703L)
- C++14 (201402L)
- C++11 (201103L)
- C++98 (其他情况)

## 6. 特性检测

```cpp
// 特性检测
#if defined(__GNUC__) && (__GNUC__ >= 5)
#   define MYSTL_HAS_MOVE_SEMANTICS
#   define MYSTL_HAS_VARIADIC_TEMPLATES
#   define MYSTL_HAS_RVALUE_REFERENCES
#endif
```

检测编译器是否支持现代C++特性：
- 移动语义支持
- 可变参数模板支持
- 右值引用支持

## 7. 内存对齐支持

```cpp
// 内存对齐支持
#if defined(__GNUC__) && (__GNUC__ >= 5 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9))
#   define MYSTL_HAS_ALIGNAS
#endif
```

检测编译器是否支持内存对齐特性（alignas关键字）。

## 8. 类模板偏特化支持

```cpp
// 类模板偏特化支持
#if !defined(_PARTIAL_SPECIALIZATION_OF_CLASS_TEMPLATES)
#   if defined(__GNUC__) && (__GNUC__ >= 3)
#       define _PARTIAL_SPECIALIZATION_OF_CLASS_TEMPLATES
#   endif
#endif

#ifdef _PARTIAL_SPECIALIZATION_OF_CLASS_TEMPLATES
#   define __STL_CLASS_PARTIAL_SPECIALIZATION
#endif
```

检测编译器是否支持类模板偏特化，并定义相应的宏。

## 9. 线程支持检测

### POSIX线程支持

```cpp
// 线程支持检测
#if defined(_PTHREADS) || defined(__PTHREADS) || \
    defined(__POSIX_THREADS) || defined(_POSIX_THREADS)
#   define __STL_PTHREADS
#endif
```

检测系统是否支持POSIX线程。

### Windows线程支持

```cpp
// Windows线程支持
#if defined(MYSTL_PLATFORM_WINDOWS)
#   define __STL_WIN32THREADS
#endif
```

在Windows平台上启用Windows线程支持。

## 10. STL特性检测

```cpp
// STL特性检测
#if defined(__GNUC__) && (__GNUC__ >= 3)
#   define __STL_MEMBER_TEMPLATES
#endif

#if defined(__GNUC__) && (__GNUC__ >= 3)
#   define __STL_FRIEND_TEMPLATES
#endif
```

检测编译器是否支持STL相关特性：
- 成员模板支持
- 成员模板友元支持

## 11. 异常处理支持

```cpp
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
```

提供异常处理的宏定义：
- 如果不禁用异常，则使用标准异常处理语法
- 如果禁用异常，则将异常处理宏定义为空，避免编译错误

## 12. RTTI 支持

```cpp
// RTTI 支持
#ifndef MYSTL_NO_RTTI
#   define MYSTL_HAS_RTTI
#endif
```

检测是否启用了运行时类型信息 (RTTI)。

## 13. 常用类型定义

```cpp
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
```

定义STL实现中需要的常用类型和特性：
- `size_t` 类型定义
- 如果编译器不支持C++11的 `nullptr`，则提供自定义实现
- 如果编译器不支持 `static_assert`，则提供自定义实现

## 总结

`stl_config.h` 文件的主要作用是：

1. **编译器兼容性**：检测不同的编译器并提供相应的配置
2. **平台适配**：根据不同平台提供合适的实现
3. **特性检测**：检测编译器支持的C++特性并启用相应功能
4. **向后兼容**：为不支持新特性的编译器提供替代实现
5. **可配置性**：通过宏定义控制功能的启用和禁用

这个配置文件使得您的STL实现可以在不同的编译器和平台上正确编译和运行，提高了代码的可移植性和兼容性。
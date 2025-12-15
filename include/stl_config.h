#ifndef MYSTL_CONFIG_H
#define MYSTL_CONFIG_H


#if defined(__GNUC__)
#   define MYSTL_COMPILER_GNUC
#elif defined(_MSC_VER)
#   define MYSTL_COMPILER_MSVC
#elif defined(__clang__)
#   define MYSTL_COMPILER_CLANG
#else
#   define MYSTL_COMPILER_UNKNOWN
#endif


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#   define MYSTL_PLATFORM_WINDOWS
#elif defined(__linux__) || defined(__linux)
#   define MYSTL_PLATFORM_LINUX
#elif defined(__APPLE__)
#   define MYSTL_PLATFORM_APPLE
#endif


#if defined(__x86_64__) || defined(_M_X64)
#   define MYSTL_ARCH_X86_64
#elif defined(__i386__) || defined(_M_IX86)
#   define MYSTL_ARCH_X86
#elif defined(__aarch64__)
#   define MYSTL_ARCH_ARM64
#elif defined(__arm__)
#   define MYSTL_ARCH_ARM
#endif


#if defined(_MSC_VER)
#   if defined(_MSVC_LANG)
#       if _MSVC_LANG >= 201703L
#           define MYSTL_CPP_VERSION 17
#       elif _MSVC_LANG >= 201402L
#           define MYSTL_CPP_VERSION 14
#       elif _MSVC_LANG >= 201103L
#           define MYSTL_CPP_VERSION 11
#       else
#           define MYSTL_CPP_VERSION 3
#       endif
#   else
        // fallback for older MSVC or if /Zc:__cplusplus not set
#       if _MSC_VER >= 1900 // VS 2015
#           define MYSTL_CPP_VERSION 11
#       else
#           define MYSTL_CPP_VERSION 3
#       endif
#   endif
#else
#   if __cplusplus >= 201703L
#       define MYSTL_CPP_VERSION 17
#   elif __cplusplus >= 201402L
#       define MYSTL_CPP_VERSION 14
#   elif __cplusplus >= 201103L
#       define MYSTL_CPP_VERSION 11
#   else
#       define MYSTL_CPP_VERSION 3
#   endif
#endif


#if defined(__cpp_rvalue_references)
#   define MYSTL_HAS_RVALUE_REFERENCES
#   define MYSTL_HAS_MOVE_SEMANTICS
#elif MYSTL_CPP_VERSION >= 11
#   define MYSTL_HAS_RVALUE_REFERENCES
#   define MYSTL_HAS_MOVE_SEMANTICS
#endif
#if defined(__cpp_variadic_templates)
#   define MYSTL_HAS_VARIADIC_TEMPLATES
#elif MYSTL_CPP_VERSION >= 11
#   define MYSTL_HAS_VARIADIC_TEMPLATES
#endif
#if defined(__cpp_initializer_lists)
#   define MYSTL_HAS_INITIALIZER_LISTS
#elif MYSTL_CPP_VERSION >= 11
#   define MYSTL_HAS_INITIALIZER_LISTS
#endif


#if defined(__GNUC__) && (__GNUC__ >= 5 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9))
#   define MYSTL_HAS_ALIGNAS
#elif defined(_MSC_VER) && _MSC_VER >= 1900
#   define MYSTL_HAS_ALIGNAS
#endif


#if !defined(_PARTIAL_SPECIALIZATION_OF_CLASS_TEMPLATES)
#   if defined(__GNUC__) && (__GNUC__ >= 3)
#       define _PARTIAL_SPECIALIZATION_OF_CLASS_TEMPLATES
#   elif defined(_MSC_VER) && _MSC_VER >= 1310
#       define _PARTIAL_SPECIALIZATION_OF_CLASS_TEMPLATES
#   endif
#endif

#ifdef _PARTIAL_SPECIALIZATION_OF_CLASS_TEMPLATES
#   define __STL_CLASS_PARTIAL_SPECIALIZATION
#endif


#if defined(_PTHREADS) || defined(__PTHREADS) || \
    defined(__POSIX_THREADS) || defined(_POSIX_THREADS)
#   define __STL_PTHREADS
#endif


#if defined(MYSTL_PLATFORM_WINDOWS)
#   define __STL_WIN32THREADS
#endif


#if defined(__GNUC__) && (__GNUC__ >= 3)
#   define __STL_MEMBER_TEMPLATES
#elif defined(_MSC_VER) && _MSC_VER >= 1300
#   define __STL_MEMBER_TEMPLATES
#endif

#if defined(__GNUC__) && (__GNUC__ >= 3)
#   define __STL_FRIEND_TEMPLATES
#elif defined(_MSC_VER) && _MSC_VER >= 1300
#   define __STL_FRIEND_TEMPLATES
#endif


#ifndef MYSTL_NO_EXCEPTIONS
#   define MYSTL_USE_EXCEPTIONS
#   define MYSTL_TRY try
#   define MYSTL_CATCH_ALL catch(...)
#   define MYSTL_THROW(x) throw x
#   define MYSTL_RETHROW throw
#   define MYSTL_UNWIND(action) catch(...) { action; throw; }
#else
#   define MYSTL_TRY
#   define MYSTL_CATCH_ALL if(false)
#   define MYSTL_THROW(x)
#   define MYSTL_RETHROW
#   define MYSTL_UNWIND(action)
#endif


#ifndef MYSTL_NO_RTTI
#   define MYSTL_HAS_RTTI
#endif

#include <cstddef> // for size_t, ptrdiff_t, nullptr_t



namespace msl {
    
    using size_t = std::size_t;
    using ptrdiff_t = std::ptrdiff_t;

    
    #if MYSTL_CPP_VERSION < 11 && !defined(nullptr)
        const class {
        public:
            template<typename T>
            operator T*() const { return 0; }
            
            template<typename C, typename T>
            operator T C::*() const { return 0; }
        
        private:
            void operator&() const;
        } nullptr = {};

    #endif

   
    #if MYSTL_CPP_VERSION < 11
        template<bool>
        struct StaticAssert;
        
        template<>
        struct StaticAssert<true> {};
        
        #define static_assert(expr, msg) \
            { msl::StaticAssert<(expr)> static_assert_failed; (void)static_assert_failed; }

    #endif
}

    #if MYSTL_CPP_VERSION < 11
        using msl::nullptr;
    #endif

#endif // MYSTL_CONFIG_H

#ifndef MYSTL_TYPE_TRAITS_H
#define MYSTL_TYPE_TRAITS_H

#include "stl_config.h"
// 简易类型萃取：为内建类型与指针提供五个编译期属性
// 用途：配合未初始化算法/构造析构进行静态分派

namespace msl {

// 编译期布尔标记类型
struct true_type {};
struct false_type {};

// 默认模板：全部属性为 false，具体类型通过特化设为 true
template <typename T>
struct type_traits {
    typedef false_type is_pod_type;
    typedef false_type is_trivially_destructible;
    typedef false_type is_trivially_default_constructible;
    typedef false_type is_trivially_copy_constructible;
    typedef false_type is_trivial_assignment_operator;
};

template <> struct type_traits<bool> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};

template <> struct type_traits<char> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};

template <> struct type_traits<signed char> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};

template <> struct type_traits<unsigned char> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};

template <> struct type_traits<wchar_t> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};

template <> struct type_traits<short> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};

template <> struct type_traits<unsigned short> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};

template <> struct type_traits<int> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};

template <> struct type_traits<unsigned int> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};

template <> struct type_traits<long> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};

template <> struct type_traits<unsigned long> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};

template <> struct type_traits<float> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};

template <> struct type_traits<double> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};

template <> struct type_traits<long double> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};

#if MYSTL_CPP_VERSION >= 11
// C++11 及以上：long long/char16_t/char32_t 等特化

template <> struct type_traits<long long> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};

template <> struct type_traits<unsigned long long> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};

template <> struct type_traits<char16_t> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};

template <> struct type_traits<char32_t> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};


#endif

// 指针类型特化：视为 POD，构造/拷贝/赋值均平凡
template<typename T> struct type_traits<T*> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};


} // namespace msl

#endif // MYSTL_TYPE_TRAITS_H

#ifndef MYSTL_TYPE_TRAITS_H
#define MYSTL_TYPE_TRAITS_H

#include "stl_config.h"


namespace msl {

struct true_type {};
struct false_type {};


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
// C++11 long long/char16_t/char32_t 

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

template<typename T> struct type_traits<T*> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
    typedef true_type is_trivially_default_constructible;
    typedef true_type is_trivially_copy_constructible;
    typedef true_type is_trivial_assignment_operator;
};




template<typename T> struct is_integer{
    typedef false_type Integral;
};


template<> struct is_integer<bool>{
    typedef true_type Integral;
};

template<> struct is_integer<char>{
    typedef true_type Integral;
};

template<> struct is_integer<wchar_t>{
    typedef true_type Integral;
};

template<> struct is_integer<signed char>{
    typedef true_type Integral;
};

template<> struct is_integer<unsigned char>{
    typedef true_type Integral;
};

template<> struct is_integer<short>{
    typedef true_type Integral;
};

template<> struct is_integer<unsigned short>{
    typedef true_type Integral;
};

template<> struct is_integer<int>{
    typedef true_type Integral;
};

template<> struct is_integer<unsigned int>{
    typedef true_type Integral;
};

template<> struct is_integer<long>{
    typedef true_type Integral;
};

template<> struct is_integer<unsigned long>{
    typedef true_type Integral;
};

#if MYSTL_CPP_VERSION >= 11
// C++11 long long/char16_t/char32_t

template<> struct is_integer<long long>{
    typedef true_type Integral;
};

template<> struct is_integer<unsigned long long>{
    typedef true_type Integral; 
};

template<> struct is_integer<char16_t>{
    typedef true_type Integral; 
};

template<> struct is_integer<char32_t>{
    typedef true_type Integral;
};




//用来去除左值和右值引用
template<typename T>
struct is_move_constructible {
    typedef typename type_traits<T>::is_pod_type Movable;
};

template<typename T>
struct remove_reference {
    typedef T type;
};

template<typename T>
struct remove_reference<T&> {
    typedef T type;
};

template<typename T>
struct remove_reference<T&&> {
    typedef T type;
};

#endif // MYSTL_REMOVE_REFERENCE_H

} // namespace msl

#endif // MYSTL_TYPE_TRAITS_H

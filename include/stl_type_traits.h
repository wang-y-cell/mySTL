#ifndef MYSTL_TYPE_TRAITS_H
#define MYSTL_TYPE_TRAITS_H

#include "stl_config.h"


namespace msl {

struct true_type {
    static const bool value = true; //用来判断is_integer为true还是false
};
struct false_type {
    static const bool value = false; //用来判断is_integer为true还是false
};


/**
 * @brief  integral_constant是一个类型特征类模板，用于表示一个编译时的常量值。
 * 
 * @tparam Tp 常量值的类型
 * @tparam _v 常量值
 */
template <typename Tp, Tp _v>
struct integral_constant {
    static const Tp value = _v;
    typedef Tp value_type;
    typedef integral_constant<Tp, _v> type;
    operator value_type() const { return value; }
};

/**
 * @brief  is_convertible是一个类型特征类模板，用于判断一个类型是否可以转换为另一个类型。
 * 
 * @tparam From 源类型
 * @tparam To 目标类型
 */
template<typename From, typename To>
struct is_convertible 
    //__is_convertible是一个GCC内置的类型特征宏，用于判断一个类型是否可以转换为另一个类型。
    : public integral_constant<bool, __is_convertible(From, To)> 
{};


template <bool B, class T = void>
struct enable_if {};

template <class T>
struct enable_if<true, T> {
    typedef T type;
};

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



/**
 * @brief 判断一个类型是否是整数类型
 * 
 * @tparam T 要判断的类型
 */
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



//无论传进去的是左值还是右值引用，都返回它的类型
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

#endif

} // namespace msl

#endif // MYSTL_TYPE_TRAITS_H

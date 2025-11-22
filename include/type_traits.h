#ifndef MYSTL_TYPE_TRAITS_H
#define MYSTL_TYPE_TRAITS_H

namespace mystl {

struct true_type {};
struct false_type {};

template <class T>
struct type_traits {
    typedef false_type is_pod_type;
    typedef false_type is_trivially_destructible;
};

template <> struct type_traits<bool> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible; 
};

template <> struct type_traits<char> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
};

template <> struct type_traits<signed char> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
};

template <> struct type_traits<unsigned char> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
};

template <> struct type_traits<wchar_t> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
};

template <> struct type_traits<short> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
};

template <> struct type_traits<unsigned short> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
};

template <> struct type_traits<int> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
};

template <> struct type_traits<unsigned int> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
};

template <> struct type_traits<long> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
};

template <> struct type_traits<unsigned long> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
};

template <> struct type_traits<float> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
};

template <> struct type_traits<double> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
};

template <> struct type_traits<long double> {
    typedef true_type is_pod_type;
    typedef true_type is_trivially_destructible;
};

} // namespace mystl

#endif // MYSTL_TYPE_TRAITS_H

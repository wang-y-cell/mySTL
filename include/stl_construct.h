#ifndef MY_STL_CONSTRUCT_HPP_
#define MY_STL_CONSTRUCT_HPP_

#include <new>          // placement new
#include "iterator.h"
#include "type_traits.h"

namespace msl {

// construct: 在已分配的内存上构造对象
template <typename T1,typename T2>
inline void construct(T1* ptr, const T2& val) {
    new (ptr) T1(val);  
}

// destroy: 调用对象的析构函数
template <typename T>
inline void destroy(T* ptr) {
    ptr->~T();
}

// 针对POD类型优化的批量析构
template<typename forward_iterator>
inline void __destroy_aux(forward_iterator first, forward_iterator last, false_type) {
    for (; first != last; ++first)
        destroy(&*first);
}

template<typename forward_iterator>
inline void __destroy_aux(forward_iterator first, forward_iterator last, true_type) {
    // 什么都不做
}

template<typename forward_iterator, typename T>
inline void __destroy(forward_iterator first, forward_iterator last, T*) {
    typedef typename type_traits<T>::is_trivially_destructible trivial_destructor;
    __destroy_aux(first, last, trivial_destructor());
}

template <typename ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last) {
    __destroy(first, last, value_type(first));
}

inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}
inline void destroy(int*, int*) {}
inline void destroy(long*, long*) {}
inline void destroy(float*, float*) {}
inline void destroy(double*, double*) {}

} // namespace msl

#endif // MY_STL_CONSTRUCT_HPP_

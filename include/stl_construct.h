#ifndef MY_STL_CONSTRUCT_HPP_
#define MY_STL_CONSTRUCT_HPP_

#include <new>          // placement new
#include "stl_iterator.h"
#include "stl_type_traits.h"

namespace msl {

// construct:
template <typename T1,typename T2>
inline void construct(T1* ptr, const T2& val) {
    new (ptr) T1(val);  
}

#if MYSTL_CPP_VERSION >= 11
template <typename T1, typename T2>
inline void construct(T1* ptr, T2&& val) {
    new (ptr) T1(msl::forward<T2>(val));
}
#endif

// destroy:

template <typename T>
inline void destroy(T* ptr) {
    ptr->~T();
}


template<typename forward_iterator>
inline void __destroy_aux(forward_iterator first, forward_iterator last, false_type) {
    for (; first != last; ++first)
        destroy(&*first);
}

template<typename forward_iterator>
inline void __destroy_aux(forward_iterator first, forward_iterator last, true_type) {
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





} // namespace msl

#endif // MY_STL_CONSTRUCT_HPP_

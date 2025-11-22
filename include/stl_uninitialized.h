#ifndef STL_UNINITIALIZED_H
#define STL_UNINITIALIZED_H

#include "iterator.h"
#include "type_traits.h"
#include "stl_construct.h"
#include <algorithm> //copy,fill,fill_n 暂时

namespace mystl {

// uninitialized_copy()
template <class Input_iterator,class Forward_iterator >
inline Forward_iterator uninitialized_copy(Input_iterator first,Input_iterator last,Forward_iterator result) {
    return  __uninitialized_copy(first,last,result,value_type(result));
}

template <class Input_iterator, class Forward_iterator,class T >
inline Forward_iterator __uninitialized_copy(Input_iterator first,Input_iterator last,Forward_iterator result,T*) {
    typedef typename type_traits<T>::is_pod_type is_pod_type;
    return __uninitialized_copy_aux(first,last,result,is_pod_type());
}   

template <class Input_iterator, class Forward_iterator,class T >
inline Forward_iterator __uninitialized_copy_aux(Input_iterator first,Input_iterator last,Forward_iterator result,false_type) {
    for (; first != last; ++first, ++result)
        construct(&*result, *first); //传地址
    return result;
}

template <class Input_iterator, class Forward_iterator,class T >
inline Forward_iterator __uninitialized_copy_aux(Input_iterator first,Input_iterator last,Forward_iterator result,true_type) {
    return std::copy(first, last, result);
}

// uninitialized_fill()
template <class Forward_iterator,class T >
inline void uninitialized_fill(Forward_iterator first,Forward_iterator last,const T& value) {
    __uninitialized_fill(first,last,value,value_type(first));
}

template <class Forward_iterator,class T >
inline void __uninitialized_fill(Forward_iterator first,Forward_iterator last,const T& value,T*) {
    typedef typename type_traits<T>::is_pod_type is_pod_type;
    __uninitialized_fill_aux(first,last,value,is_pod_type());
}

template <class Forward_iterator,class T >
inline void __uninitialized_fill_aux(Forward_iterator first,Forward_iterator last,const T& value,false_type) {
    for (; first != last; ++first)
        construct(&*first, value);
}

template <class Forward_iterator,class T >
inline void __uninitialized_fill_aux(Forward_iterator first,Forward_iterator last,const T& value,true_type) {
    std::fill(first, last, value);
}


// uninitialized_fill_n()
template<class Forward_iterator,class Size,class T >
inline Forward_iterator uninitialized_fill_n(Forward_iterator first,Size n,const T& value) {
    return __uninitialized_fill_n(first,n,value,value_type(first));
}

template <class Forward_iterator,class Size,class T >
inline Forward_iterator __uninitialized_fill_n(Forward_iterator first,Size n,const T& value,T*) {
    typedef typename type_traits<T>::is_pod_type is_pod_type;
    return __uninitialized_fill_n_aux(first,n,value,is_pod_type());
}

template <class Forward_iterator,class Size,class T >
inline Forward_iterator __uninitialized_fill_n_aux(Forward_iterator first,Size n,const T& value,true_type) {
    return std::fill_n(first, n, value);
}

template <class Forward_iterator,class Size,class T >
inline Forward_iterator __uninitialized_fill_n_aux(Forward_iterator first,Size n,const T& value,false_type) {
    for (; n > 0; --n, ++first)
        construct(&*first, value);
    return first;   
}





} // namespace mystl

#endif

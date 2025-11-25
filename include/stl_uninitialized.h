#ifndef STL_UNINITIALIZED_H
#define STL_UNINITIALIZED_H

#include "stl_config.h"
#include "iterator.h"
#include "type_traits.h"
#include "stl_construct.h"
#include <algorithm> //copy,fill,fill_n 暂时

namespace msl {

// uninitialized_copy(),返回新的结束迭代器
template <typename Input_iterator, typename Forward_iterator>
inline Forward_iterator __uninitialized_copy_aux(Input_iterator first,Input_iterator last,Forward_iterator result,false_type) {
    Forward_iterator cur = result;
    MYSTL_TRY {
        for (; first != last; ++first, ++cur)
            msl::construct(&*cur, *first);
        return cur;
    }
    MYSTL_CATCH_ALL {
        msl::destroy(result, cur);
        MYSTL_RETHROW;
    }
}

template <typename Input_iterator, typename Forward_iterator>
inline Forward_iterator __uninitialized_copy_aux(Input_iterator first,Input_iterator last,Forward_iterator result,true_type) {
    return std::copy(first, last, result);
}

template <typename Input_iterator, typename Forward_iterator,typename T >
inline Forward_iterator __uninitialized_copy(Input_iterator first,Input_iterator last,Forward_iterator result,T*) {
    typedef typename type_traits<T>::is_pod_type is_pod_type;
    return __uninitialized_copy_aux(first,last,result,is_pod_type());
}   

template <typename Input_iterator,typename Forward_iterator >
inline Forward_iterator uninitialized_copy(Input_iterator first,Input_iterator last,Forward_iterator result) {
    return  __uninitialized_copy(first,last,result,msl::value_type(result));
}




// uninitialized_fill(),返回新的结束迭代器
template <typename Forward_iterator,typename T >
inline void _uninitialized_fill_aux(Forward_iterator first,Forward_iterator last,const T& value,false_type) {
    Forward_iterator cur = first;
    MYSTL_TRY {
        for (; cur != last; ++cur)
            msl::construct(&*cur, value);
    }
    MYSTL_CATCH_ALL {
        msl::destroy(first, cur);
        MYSTL_RETHROW;
    }
}

template <typename Forward_iterator,typename T >
inline void _uninitialized_fill_aux(Forward_iterator first,Forward_iterator last,const T& value,true_type) {
    std::fill(first, last, value);
}

template <typename Forward_iterator,typename T >
inline void _uninitialized_fill(Forward_iterator first,Forward_iterator last,const T& value,T*) {
    typedef typename type_traits<T>::is_pod_type is_pod_type;
    _uninitialized_fill_aux(first,last,value,is_pod_type());
}

template <typename Forward_iterator,typename T >
inline void uninitialized_fill(Forward_iterator first,Forward_iterator last,const T& value) {
    _uninitialized_fill(first,last,value,msl::value_type(first));
}




// uninitialized_fill_n()
template <typename Forward_iterator,typename Size,typename T >
inline Forward_iterator __uninitialized_fill_n_aux(Forward_iterator first,Size n,const T& value,true_type) {
    return std::fill_n(first, n, value);
}

template <typename Forward_iterator,typename Size,typename T >
inline Forward_iterator __uninitialized_fill_n_aux(Forward_iterator first,Size n,const T& value,false_type) {
    Forward_iterator cur = first;
    MYSTL_TRY {
        for (; n > 0; --n, ++cur)
            msl::construct(&*cur, value);
        return cur;
    }
    MYSTL_CATCH_ALL {
        msl::destroy(first, cur);
        MYSTL_RETHROW;
    }
}
template <typename Forward_iterator,typename Size,typename T >
inline Forward_iterator __uninitialized_fill_n(Forward_iterator first,Size n,const T& value,T*) {
    typedef typename type_traits<T>::is_pod_type is_pod_type;
    return __uninitialized_fill_n_aux(first,n,value,is_pod_type());  
}

template<typename Forward_iterator,typename Size,typename T >
inline Forward_iterator uninitialized_fill_n(Forward_iterator first,Size n,const T& value) {
    return __uninitialized_fill_n(first,n,value,msl::value_type(first));
}



} // namespace msl

#endif

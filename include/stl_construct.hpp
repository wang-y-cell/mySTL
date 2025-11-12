#ifndef MY_STL_CONSTRUCT_HPP_
#define MY_STL_CONSTRUCT_HPP_

#include <new>          // placement new
//暂时
#include <type_traits>  // std::is_trivially_destructible
#include <utility>      // std::forward

namespace mystl {

// construct: 在已分配的内存上构造对象
template <typename T1,typename T2>
inline void construct(T1* ptr, const T2& val) {
    new (ptr) T1(val);  
}

// destroy: 调用对象的析构函数
template <class T>
inline void destroy(T* ptr) {
    ptr->~T();
}

// 针对POD类型优化的批量析构
template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last) {
    __destroy(first, last, value_type(first));
}

//暂时使用c++标准库的is_trivially_destructible来判断是否为POD类型
template<typename forward_iterator, typename T>
inline void __destroy(forward_iterator first, forward_iterator last, T*) {
    typedef typename std::is_trivially_destructible<T> trivial_destructor;
    __destroy_aux(first, last, trivial_destructor());
}

template<typename forward_iterator, typename T>
inline void __destroy_aux(forward_iterator first, forward_iterator last, std::false_type) {
    for (; first != last; ++first)
        destroy(&*first);
}

template<typename forward_iterator, typename T>
inline void __destroy_aux(forward_iterator first, forward_iterator last, std::true_type) {
    // 什么都不做
}

inline void destroy(char* first, char* last) {
}

inline void destroy(wchar_t* first, wchar_t* last) {
}


} // namespace mystl

#endif // MY_STL_CONSTRUCT_HPP_
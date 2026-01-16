#ifndef STL_ALGOBASE_H
#define STL_ALGOBASE_H

#include "iterator.h"
#include "stl_pair.h"
#include <cstring>
#if MYSTL_CPP_VERSION >= 11
#include "move.h"
#endif

namespace msl {


/*---------------------------------------------------------------------------*/
/*max/min*/

template<typename T>
inline const T& max(const T& a, const T& b){
    return a < b ? b : a;
}
template<typename T, typename Compare>
inline const T& max(const T& a, const T& b, Compare comp){
    return comp(a, b) ? b : a;
}

template<typename T>
inline const T& min(const T& a, const T& b){
    return a < b ? a : b;
}
template<typename T, typename Compare>
inline const T& min(const T& a, const T& b, Compare comp){
    return comp(a, b) ? a : b;
}

/*---------------------------------------------------------------------------*/
/*equal*/

template<typename InputIterator1, typename InputIterator2>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2){
    for(; first1 != last1; ++first1, ++first2){
        if(*first1 != *first2){
            return false;
        }
    }
    return true;
}

template<typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate pred){
    for(; first1 != last1; ++first1, ++first2){
        if(!pred(*first1, *first2)){
            return false;
        }
    }
    return true;
}

/*---------------------------------------------------------------------------*/
/*fill*/
template<typename ForwardIterator, typename T>
void fill(ForwardIterator first, ForwardIterator last, const T& value){
    for(; first != last; ++first){
        *first = value;
    }
}
/*---------------------------------------------------------------------------*/
/*fill_n*/
template<typename OutputIterator, typename Size, typename T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value){
    for(; n > 0; --n, ++first){
        *first = value;
    }
    return first;
}

/*---------------------------------------------------------------------------*/
/*iter_swap*/

template<typename ForwardIterator1, typename ForwardIterator2,typename T>
inline void iter_swap(ForwardIterator1 first1, ForwardIterator2 first2, T*){
    T temp = *first1;
    *first1 = *first2;
    *first2 = temp;
}

template<typename ForwardIterator1, typename ForwardIterator2>
inline void iter_swap(ForwardIterator1 first1, ForwardIterator2 first2){
    iter_swap(first1, first2, value_type(first1));
}

/*---------------------------------------------------------------------------*/
/*lexicographical_compare*/

//比较两个序列的大小,按照字典序
template<typename InputIterator1, typename InputIterator2>
inline bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2){
    for(; first1 != last1 && first2 != last2; ++first1, ++first2){
        if(*first1 < *first2)return true;
        if(*first2 < *first1)return false;
    }
    return first1 == last1 && first2 != last2;
}

template<typename InputIterator1, typename InputIterator2,typename Compare>
inline bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2,Compare comp){
    for(; first1 != last1 && first2 != last2; ++first1, ++first2){
        if(comp(*first1, *first2))return true;
        if(comp(*first2, *first1))return false;
    }
    return first1 == last1 && first2 != last2;
}

inline bool lexicographical_compare(const unsigned char* first1, const unsigned char* last1,
                             const unsigned char* first2, const unsigned char* last2){
    const size_t len1 = last1 - first1;
    const size_t len2 = last2 - first2;
    const int result = memcmp(first1, first2, min(len1, len2));
    return result < 0 || (result == 0 && len1 < len2);
}

inline bool lexicographical_compare(const signed char* first1, const signed char* last1,
                             const signed char* first2, const signed char* last2){
    const size_t len1 = last1 - first1;
    const size_t len2 = last2 - first2;
    const int result = memcmp(first1, first2, min(len1, len2));
    return result < 0 || (result == 0 && len1 < len2);
}

inline bool lexicographical_compare(const char* first1, const char* last1,
                             const char* first2, const char* last2){
    const size_t len1 = last1 - first1;
    const size_t len2 = last2 - first2;
    const int result = memcmp(first1, first2, min(len1, len2));
    return result < 0 || (result == 0 && len1 < len2);
}

/*---------------------------------------------------------------------------*/
/*move (range)*/
#if MYSTL_CPP_VERSION >= 11

template<class RandomAccessIterator, class OutputIterator,class Distance>
inline OutputIterator __move_d
(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*){
    for(Distance n = last - first; n > 0; --n, ++first, ++result){
        *result = msl::move(*first);
    }
    return result;
}

template<class RandomAccessIterator, class OutputIterator>
inline OutputIterator __move
(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, 
 random_access_iterator_tag){
    return __move_d(first, last, result, distance_type(first));
}

template<class InputIterator, class OutputIterator>
inline OutputIterator __move
(InputIterator first, InputIterator last, OutputIterator result, 
 input_iterator_tag){
    for(; first != last; ++first, ++result){
        *result = msl::move(*first);
    }
    return result;
}



template< class InputIterator,class OutputIterator>
struct move_dispatcher{
    OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result){
        typedef typename iterator_traits<InputIterator>::iterator_category Category;
        static_assert(is_msl_iterator_tag<Category>::value, "must use msl iterator");
        return __move(first, last, result, Category());
    }
};

template<class T>
inline T* __move_t(const T* first, const T* last, T* result, true_type){
    memmove(result, first, sizeof(T) * (last - first));
    return result + (last - first);
}

template<class InputIterator, class T>
inline T* __move_t(InputIterator first, InputIterator last, T* result, false_type){
    return __move_d(first, last, result,(ptrdiff_t*)0);
}

template<class T>
struct move_dispatcher<T*, T*>{
    typedef typename type_traits<T>::is_trivial_assignment_operator movable_type;
    T* operator()(T* first, T* last, T* result){
        return __move_t(first, last, result, movable_type());
    }
};

template<class T>
struct move_dispatcher<const T*, T*>{
    typedef typename type_traits<T>::is_trivial_assignment_operator movable_type;
    T* operator()(const T* first, const T* last, T* result){
        return __move_t(first, last, result, movable_type());
    }
};


template<class InputIt, class OutputIt>
inline OutputIt move(InputIt first, InputIt last, OutputIt result){
    return move_dispatcher<InputIt, OutputIt>()(first, last, result);
}


inline char* move(const char* first, const char* last, char* result){
    memmove(result, first, last - first);
    return result + (last - first);
}

inline wchar_t* move(const wchar_t* first, const wchar_t* last, wchar_t* result){
    memmove(result, first, sizeof(wchar_t) * (last - first));
    return result + (last - first);
}


template <class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 __move_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
                                              BidirectionalIterator2 result, bidirectional_iterator_tag) {
  while (first != last)
    *--result = msl::move(*--last);
  return result;
}

template <class RandomAccessIterator, class OutputIterator>
inline OutputIterator __move_backward_d(RandomAccessIterator first, RandomAccessIterator last,
                                        OutputIterator result) {
  typedef typename iterator_traits<RandomAccessIterator>::difference_type Distance;
  for (Distance n = last - first; n > 0; --n)
    *--result = msl::move(*--last);
  return result;
}

template <class RandomAccessIterator, class OutputIterator>
inline OutputIterator __move_backward(RandomAccessIterator first, RandomAccessIterator last,
                                      OutputIterator result, random_access_iterator_tag) {
  return __move_backward_d(first, last, result);
}

template <class T>
inline T* __move_backward_t(const T* first, const T* last, T* result, true_type) {
  const ptrdiff_t n = last - first;
  result -= n;
  memmove(result, first, sizeof(T) * n);
  return result;
}

template <class InputIterator, class T>
inline T* __move_backward_t(InputIterator first, InputIterator last, T* result, false_type) {
  return __move_backward_d(first, last, result);
}

template <class BidirectionalIterator1, class BidirectionalIterator2>
struct move_backward_dispatcher {
  BidirectionalIterator2 operator()(BidirectionalIterator1 first, BidirectionalIterator1 last,
                                    BidirectionalIterator2 result) {
    typedef typename iterator_traits<BidirectionalIterator1>::iterator_category Category;
    static_assert(is_msl_iterator_tag<Category>::value, "must use msl iterator");
    return __move_backward(first, last, result, Category());
  }
};

template <class T>
struct move_backward_dispatcher<T*, T*> {
  T* operator()(T* first, T* last, T* result) {
    typedef typename type_traits<T>::is_trivial_assignment_operator Trivial;
    return __move_backward_t(first, last, result, Trivial());
  }
};

template <class T>
struct move_backward_dispatcher<const T*, T*> {
  T* operator()(const T* first, const T* last, T* result) {
    typedef typename type_traits<T>::is_trivial_assignment_operator Trivial;
    return __move_backward_t(first, last, result, Trivial());
  }
};

template<class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 move_backward(BidirectionalIterator1 first, 
                                            BidirectionalIterator1 last, 
                                            BidirectionalIterator2 result){
    return move_backward_dispatcher<BidirectionalIterator1, BidirectionalIterator2>()(first, last, result);
}

inline char* move_backward(const char* first, const char* last, char* result) {
    const ptrdiff_t n = last - first;
    result -= n;
    memmove(result, first, n);
    return result;
}

inline wchar_t* move_backward(const wchar_t* first, const wchar_t* last, wchar_t* result) {
    const ptrdiff_t n = last - first;
    result -= n;
    memmove(result, first, sizeof(wchar_t) * n);
    return result;
}
#endif

/*---------------------------------------------------------------------------*/
/*copy*/


template <class InputIterator, class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last,
                             OutputIterator result, input_iterator_tag) {
  for (; first != last; ++result, ++first)
    *result = *first;
  return result;
}


template <class RandomAccessIterator, class OutputIterator>
inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last,
                               OutputIterator result) {
  typedef typename iterator_traits<RandomAccessIterator>::difference_type Distance;
  for (Distance n = last - first; n > 0; --n, ++result, ++first)
    *result = *first;
  return result;
}


template <class RandomAccessIterator, class OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last,
                             OutputIterator result, random_access_iterator_tag) {
  return __copy_d(first, last, result);
}


template <class T>
inline T* __copy_t(const T* first, const T* last, T* result, true_type) {
  memmove(result, first, sizeof(T) * (last - first));
  return result + (last - first);
}


template <class T>
inline T* __copy_t(const T* first, const T* last, T* result, false_type) {
  return __copy_d(first, last, result);
}


template <class InputIterator, class OutputIterator>
struct __copy_dispatch {
  OutputIterator operator()(InputIterator first, InputIterator last,
                            OutputIterator result) {
    typedef typename iterator_traits<InputIterator>::iterator_category Category;
    static_assert(is_msl_iterator_tag<Category>::value, "must use msl iterator");
    return __copy(first, last, result, Category());
  }
};


template <class T>
struct __copy_dispatch<T*, T*> {
  T* operator()(T* first, T* last, T* result) {
    typedef typename type_traits<T>::is_trivial_assignment_operator Trivial;
    return __copy_t(first, last, result, Trivial());
  }
};


template <class T>
struct __copy_dispatch<const T*, T*> { //第一个const是输入迭代器,第二个const是输出迭代器
  T* operator()(const T* first, const T* last, T* result) {
    typedef typename type_traits<T>::is_trivial_assignment_operator Trivial;
    return __copy_t(first, last, result, Trivial());
  }
};

//主函数
template <class InputIterator, class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last,
                           OutputIterator result) {
  return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}


inline char* copy(const char* first, const char* last, char* result) {
  memmove(result, first, last - first);
  return result + (last - first);
}


inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
  memmove(result, first, sizeof(wchar_t) * (last - first));
  return result + (last - first);
}

/*------------------------------------------------------------------------------*/
//copy_n

template <class InputIterator, class Size, class OutputIterator>
inline OutputIterator copy_n(InputIterator first, Size n, OutputIterator result) {
  for (; n > 0; --n, ++result, ++first)
    *result = *first;
  return result;
}

/*---------------------------------------------------------------------------*/
/*copy_backward */

// 最慢的一种
template <class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
                                              BidirectionalIterator2 result, bidirectional_iterator_tag) {
  while (first != last)
    *--result = *--last;
  return result;
}

// 这个版本可供具有随机迭代器和没有平凡赋值运算符的指针使用
template <class RandomAccessIterator, class OutputIterator>
inline OutputIterator __copy_backward_d(RandomAccessIterator first, RandomAccessIterator last,
                                        OutputIterator result) {
  typedef typename iterator_traits<RandomAccessIterator>::difference_type Distance;
  for (Distance n = last - first; n > 0; --n)
    *--result = *--last;
  return result;
}

// 随机迭代器
template <class RandomAccessIterator, class OutputIterator>
inline OutputIterator __copy_backward(RandomAccessIterator first, RandomAccessIterator last,
                                      OutputIterator result, random_access_iterator_tag) {
  return __copy_backward_d(first, last, result);
}


template <class T>
inline T* __copy_backward_t(const T* first, const T* last, T* result, true_type) {
  const ptrdiff_t n = last - first;
  result -= n;
  memmove(result, first, sizeof(T) * n);
  return result;
}


template <class T>
inline T* __copy_backward_t(const T* first, const T* last, T* result, false_type) {
  return __copy_backward_d(first, last, result);
}

// 如果不是指针类型,就看是否具有随机迭代器
template <class BidirectionalIterator1, class BidirectionalIterator2>
struct __copy_backward_dispatch {
  BidirectionalIterator2 operator()
  (BidirectionalIterator1 first, BidirectionalIterator1 last,
                                    BidirectionalIterator2 result) {
    typedef typename iterator_traits<BidirectionalIterator1>::iterator_category Category;
    static_assert(is_msl_iterator_tag<Category>::value, "must use msl iterator");
    return __copy_backward(first, last, result, Category());
  }
};

// 偏特化
template <class T>
struct __copy_backward_dispatch<T*, T*> {
  T* operator()(T* first, T* last, T* result) {
    typedef typename type_traits<T>::is_trivial_assignment_operator Trivial;
    return __copy_backward_t(first, last, result, Trivial());
  }
};

// 偏特化
template <class T>
struct __copy_backward_dispatch<const T*, T*> {
  T* operator()(const T* first, const T* last, T* result) {
    typedef typename type_traits<T>::is_trivial_assignment_operator Trivial;
    return __copy_backward_t(first, last, result, Trivial());
  }
};

// Main copy_backward function
template <class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 
copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
                                            BidirectionalIterator2 result) {
  return __copy_backward_dispatch<BidirectionalIterator1, BidirectionalIterator2>()
  (first, last, result);
}

// Specialization for char*
inline char* 
copy_backward(const char* first, const char* last, char* result) {
  const ptrdiff_t n = last - first;
  result -= n;
  memmove(result, first, n);
  return result;
}

// Specialization for wchar_t*
inline wchar_t* 
copy_backward(const wchar_t* first, const wchar_t* last, wchar_t* result) {
  const ptrdiff_t n = last - first;
  result -= n;
  memmove(result, first, sizeof(wchar_t) * n);
  return result;
}

/*---------------------------------------------------------------------------*/
/*mismatch*/

// 比较两个序列的第一个不匹配的元素
template <class InputIterator1, class InputIterator2>
inline pair<InputIterator1, InputIterator2>
mismatch(InputIterator1 first1, InputIterator1 last1,
         InputIterator2 first2) {
  while (first1 != last1 && *first1 == *first2) {
    ++first1;
    ++first2;
  }
  return pair<InputIterator1, InputIterator2>(first1, first2);
}

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
inline pair<InputIterator1, InputIterator2>
mismatch(InputIterator1 first1, InputIterator1 last1,
         InputIterator2 first2, BinaryPredicate pred) {
  while (first1 != last1 && pred(*first1, *first2)) {
    ++first1;
    ++first2;
  }
  return pair<InputIterator1, InputIterator2>(first1, first2);
}

/*----------------------------------------------------------------------*/
/*swap*/

// 交换两个对象的值
template <class T>
inline void swap(T& a, T& b) {
  T tmp = a;
  a = b;
  b = tmp;
}

}// namespace msl



#endif // STL_ALGOBASE_H

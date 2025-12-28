 #ifndef STL_NUMERIC_H
#define STL_NUMERIC_H

#include "iterator.h"
#include "stl_functional.h"

namespace msl {

//将迭代器[first, last)中的元素累加起来,初始值为init
template<typename InputIterator, typename T>
T accumulate(InputIterator first, InputIterator last, T init) {
    for (; first != last; ++first) {
        init = init + *first;
    }
    return init;
}

//将迭代器[first, last)中的元素累加起来,初始值为init,使用二元操作op
template<typename InputIterator, typename T, typename BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation op) {
    for (; first != last; ++first) {
        init = op(init, *first); //对每个元素执行二元操作
    }
    return init;
}

template<typename InputIterator, typename OutputIterator, typename T>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, T*) {
    T prev = *first;
    while(++first != last) {
        T val = *first;
        *++result = val - prev;
        prev = val;
    }
    return ++result;
}

//计算迭代器[first, last)中的相邻元素的差值,将结果存储在result中
template<typename InputIterator, typename OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result) {
    if(first == last) return result;
    *result = *first;
    return __adjacent_difference(first, last, result,value_type(first));
}

//版本2
template<typename InputIterator, typename OutputIterator, typename T, typename BinaryOperation>
OutputIterator __adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, T*, BinaryOperation binary_op) {
    T prev = *first;
    while(++first != last) {
        T val = *first;
        *++result = binary_op(val, prev);
        prev = val;
    }
    return ++result;
}

template<typename InputIterator, typename OutputIterator, typename BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
    if(first == last) return result;
    *result = *first;
    return __adjacent_difference(first, last, result, value_type(first), binary_op);
}

template<typename InputIterator1, typename InputIterator2, typename T>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init) {
    for (; first1 != last1; ++first1, ++first2) {
        init = init + (*first1 * *first2);
    }
    return init;
}

//版本2
template<typename InputIterator1, typename InputIterator2, typename T,
         typename BinaryOperation1, typename BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1,
                InputIterator2 first2, T init,
                BinaryOperation1 binary_op1, BinaryOperation2 binary_op2) {
    for (; first1 != last1; ++first1, ++first2) {
        init = binary_op1(init, binary_op2(*first1, *first2));
    }
    return init;
}

template <class InputIterator, class OutputIterator, class T>
OutputIterator __partial_sum(InputIterator first, InputIterator last,
                             OutputIterator result, T*) {
  T value = *first;
  while (++first != last) {
    value = value + *first;
    *++result = value;
  }
  return ++result;
}

template <class InputIterator, class OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last,
                           OutputIterator result) {
  if (first == last) return result;
  *result = *first;
  return __partial_sum(first, last, result, value_type(first));
}

template <class InputIterator, class OutputIterator, class T, class BinaryOperation>
OutputIterator __partial_sum(InputIterator first, InputIterator last,
                             OutputIterator result, T*, BinaryOperation binary_op) {
  T value = *first;
  while (++first != last) {
    value = binary_op(value, *first);
    *++result = value;
  }
  return ++result;
}

template <class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last,
                           OutputIterator result, BinaryOperation binary_op) {
  if (first == last) return result;
  *result = *first;
  return __partial_sum(first, last, result, value_type(first), binary_op);
}




//power
//快速幂: https://oi-wiki.org/math/binary-exponentiation/ 

//版本2, sgi stl源码中的有点麻烦了,这是我自己的实现版本
template<typename T, typename Integer, typename MonoidOperation>
T power(T x, Integer n, MonoidOperation monoid_op) {
    T res = identity_element(monoid_op);
    while(n) {
        if(n & 1) res = monoid_op(res, x);
        x = monoid_op(x, x);
        n >>= 1;
    }
    return res;
}

template<typename T, typename Integer>
T power(T x, Integer n) {
    return power(x, n, msl::multiplies<T>());
}

//iota: 从value开始,将[first, last)中的元素赋值为value, value递增
template<typename ForwardIterator, typename T>
void iota(ForwardIterator first, ForwardIterator last, T value) {
    while(first != last) {
        *first++ = value++;
    }
}


} // namespace msl

#endif // STL_NUMERIC_H
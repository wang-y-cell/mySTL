#ifndef STL_ALGO_H
#define STL_ALGO_H

#include "stl_heap.h"
#include "stl_numeric.h"
#include "stl_algobase.h"
#include "iterator.h"
#include <cstdlib>
#include "stl_heap.h"


namespace msl {





//========================================================================
// set_union

/**
 * @brief 计算两个有序集合的并集
 * 
 * 构建一个有序序列，包含在 [first1, last1) 或 [first2, last2) 中出现的所有元素。
 * 如果一个元素在两个范围内都出现，则结果中包含较大数量的该元素。
 * 
 * @param first1 第一个有序序列的起始迭代器
 * @param last1 第一个有序序列的结束迭代器
 * @param first2 第二个有序序列的起始迭代器
 * @param last2 第二个有序序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @return OutputIterator 指向构建的并集序列末尾的迭代器
 */
template <class InputIterator1, class InputIterator2,
          class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, InputIterator2 last2,
                         OutputIterator result) {
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *first2) {
      *result = *first1;
      ++first1;
    }
    else if (*first2 < *first1) {
      *result = *first2;
      ++first2;
    }
    else {
      *result = *first1;
      ++first1;
      ++first2;
    }
    ++result;
  }
  return msl::copy(first2, last2, msl::copy(first1, last1, result));
}

/**
 * @brief 计算两个有序集合的并集（自定义比较器）
 * 
 * 使用自定义比较函数 comp。
 * 
 * @param first1 第一个有序序列的起始迭代器
 * @param last1 第一个有序序列的结束迭代器
 * @param first2 第二个有序序列的起始迭代器
 * @param last2 第二个有序序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @param comp 比较函数对象
 * @return OutputIterator 指向构建的并集序列末尾的迭代器
 */
template <class InputIterator1, class InputIterator2, 
          class OutputIterator, class Compare>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, InputIterator2 last2,
                         OutputIterator result, Compare comp) {
  while (first1 != last1 && first2 != last2) {
    if (comp(*first1, *first2)) {
      *result = *first1;
      ++first1;
    }
    else if (comp(*first2, *first1)) {
      *result = *first2;
      ++first2;
    }
    else {
      *result = *first1;
      ++first1;
      ++first2;
    }
    ++result;
  }
  return msl::copy(first2, last2, msl::copy(first1, last1, result));
}






//========================================================================
// set_intersection

/**
 * @brief 计算两个有序集合的交集
 * 
 * 构建一个有序序列，仅包含同时出现在 [first1, last1) 和 [first2, last2) 中的元素。
 * 
 * @param first1 第一个有序序列的起始迭代器
 * @param last1 第一个有序序列的结束迭代器
 * @param first2 第二个有序序列的起始迭代器
 * @param last2 第二个有序序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @return OutputIterator 指向构建的交集序列末尾的迭代器
 */
template <class InputIterator1, class InputIterator2, 
        class OutputIterator>
OutputIterator set_intersection
(InputIterator1 first1, InputIterator1 last1,
 InputIterator2 first2, InputIterator2 last2,
 OutputIterator result) {
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *first2)
      ++first1;
    else if (*first2 < *first1)
      ++first2;
    else {
      *result = *first1;
      ++result;
      ++first1;
      ++first2;
    }
  }
  return result;
}

/**
 * @brief 计算两个有序集合的交集（自定义比较器）
 * 
 * 使用自定义比较函数 comp。
 * 
 * @param first1 第一个有序序列的起始迭代器
 * @param last1 第一个有序序列的结束迭代器
 * @param first2 第二个有序序列的起始迭代器
 * @param last2 第二个有序序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @param comp 比较函数对象
 * @return OutputIterator 指向构建的交集序列末尾的迭代器
 */
template <class InputIterator1, class InputIterator2, 
          class OutputIterator, class Compare>
OutputIterator set_intersection
(InputIterator1 first1, InputIterator1 last1,
 InputIterator2 first2, InputIterator2 last2,
 OutputIterator result, Compare comp) {
  while (first1 != last1 && first2 != last2) {
    if (comp(*first1, *first2))
      ++first1;
    else if (comp(*first2, *first1))
      ++first2;
    else {
      *result = *first1;
      ++result;
      ++first1;
      ++first2;
    }
  }
  return result;
}






//========================================================================
// set_difference

/**
 * @brief 计算两个有序集合的差集
 * 
 * 构建一个有序序列，包含在 [first1, last1) 中但不在 [first2, last2) 中的元素。
 * 
 * @param first1 第一个有序序列的起始迭代器
 * @param last1 第一个有序序列的结束迭代器
 * @param first2 第二个有序序列的起始迭代器
 * @param last2 第二个有序序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @return OutputIterator 指向构建的差集序列末尾的迭代器
 */
template <class InputIterator1, class InputIterator2, 
          class OutputIterator>
OutputIterator set_difference
(InputIterator1 first1, InputIterator1 last1,
 InputIterator2 first2, InputIterator2 last2,
 OutputIterator result) {
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *first2) {
      *result = *first1;
      ++first1;
      ++result;
    }
    else if (*first2 < *first1) {
      ++first2;
    }
    else {
      ++first1;
      ++first2;
    }
  }
  return msl::copy(first1, last1, result);
}

/**
 * @brief 计算两个有序集合的差集（自定义比较器）
 * 
 * 使用自定义比较函数 comp。
 * 
 * @param first1 第一个有序序列的起始迭代器
 * @param last1 第一个有序序列的结束迭代器
 * @param first2 第二个有序序列的起始迭代器
 * @param last2 第二个有序序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @param comp 比较函数对象
 * @return OutputIterator 指向构建的差集序列末尾的迭代器
 */
template <class InputIterator1, class InputIterator2, 
          class OutputIterator, class Compare>
OutputIterator set_difference
(InputIterator1 first1, InputIterator1 last1,
 InputIterator2 first2, InputIterator2 last2,
 OutputIterator result, Compare comp) {
  while (first1 != last1 && first2 != last2) {
    if (comp(*first1, *first2)) {
      *result = *first1;
      ++first1;
      ++result;
    }
    else if (comp(*first2, *first1)) {
      ++first2;
    }
    else {
      ++first1;
      ++first2;
    }
  }
  return msl::copy(first1, last1, result);
}




//========================================================================
// set_symmetric_difference

/**
 * @brief 计算两个有序集合的对称差集
 * 
 * 构建一个有序序列，包含出现在 [first1, last1) 但不出现在 [first2, last2) 中的元素，
 * 以及出现在 [first2, last2) 但不出现在 [first1, last1) 中的元素。
 * 
 * @param first1 第一个有序序列的起始迭代器
 * @param last1 第一个有序序列的结束迭代器
 * @param first2 第二个有序序列的起始迭代器
 * @param last2 第二个有序序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @return OutputIterator 指向构建的对称差集序列末尾的迭代器
 */
template <class InputIterator1, class InputIterator2, 
          class OutputIterator>
OutputIterator set_symmetric_difference
(InputIterator1 first1, InputIterator1 last1,
 InputIterator2 first2, InputIterator2 last2,
 OutputIterator result) {
  while (first1 != last1 && first2 != last2) {
    if (*first1 < *first2) {
      *result = *first1;
      ++first1;
      ++result;
    } else if (*first2 < *first1) {
      *result = *first2;
      ++first2;
      ++result;
    } else {
      ++first1;
      ++first2;
    }
  }
  return msl::copy(first2, last2, msl::copy(first1, last1, result));
}


/**
 * @brief 计算两个有序集合的对称差集（自定义比较器）
 * 
 * 使用自定义比较函数 comp。
 * 
 * @param first1 第一个有序序列的起始迭代器
 * @param last1 第一个有序序列的结束迭代器
 * @param first2 第二个有序序列的起始迭代器
 * @param last2 第二个有序序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @param comp 比较函数对象
 * @return OutputIterator 指向构建的对称差集序列末尾的迭代器
 */
template <class InputIterator1, class InputIterator2, 
          class OutputIterator, class Compare>
OutputIterator set_symmetric_difference
(InputIterator1 first1, InputIterator1 last1,
 InputIterator2 first2, InputIterator2 last2,
 OutputIterator result, Compare comp) {
  while (first1 != last1 && first2 != last2) {
    if (comp(*first1, *first2)) {
      *result = *first1;
      ++first1;
      ++result;
    } else if (comp(*first2, *first1)) {
      *result = *first2;
      ++first2;
      ++result;
    } else {
      ++first1;
      ++first2;
    }
  }
  return msl::copy(first2, last2, msl::copy(first1, last1, result));
}


/************************************************************************************* */
//adjacent_find

/**
 * @brief 查找相邻的重复元素
 * 
 * 在范围 [first, last) 中查找第一对相邻且相等的元素。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @return ForwardIterator 指向第一对相等元素中第一个元素的迭代器。如果未找到，返回 last。
 */
template <class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
  if (first == last) return last;
  ForwardIterator next = first;
  while (++next != last) {
    if (*first == *next) return first;
    first = next;
  }
  return last;
}

/**
 * @brief 查找相邻的重复元素（自定义比较器）
 * 
 * 在范围 [first, last) 中查找第一对满足 pred(*it, *(it+1)) 为真的相邻元素。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param pred 二元谓词函数对象
 * @return ForwardIterator 指向第一对满足条件的元素中第一个元素的迭代器。如果未找到，返回 last。
 */
template <class ForwardIterator, class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate pred) {
  if (first == last) return last;
  ForwardIterator next = first;
  while (++next != last) {
    if (pred(*first, *next)) return first;
    first = next;
  }
  return last;
}

/*********************************************************************************************************** */

// count

/**
 * @brief 统计等于给定值的元素个数
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param value 要统计的值
 * @return difference_type 等于 value 的元素个数
 */
template <class InputIterator, class T>
typename iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last, const T& value) {
  typename iterator_traits<InputIterator>::difference_type n = 0;
  for (; first != last; ++first)
    if (*first == value)
      ++n;
  return n;
}

// count_if

/**
 * @brief 统计满足条件的元素个数
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param pred 一元谓词函数对象
 * @return difference_type 满足 pred(*it) 为真的元素个数
 */
template <class InputIterator, class Predicate>
typename iterator_traits<InputIterator>::difference_type
count_if(InputIterator first, InputIterator last, Predicate pred) {
  typename iterator_traits<InputIterator>::difference_type n = 0;
  for (; first != last; ++first)
    if (pred(*first))
      ++n;
  return n;
}

/******************************************************************************** */
// find

/**
 * @brief 查找等于给定值的第一个元素
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param value 要查找的值
 * @return InputIterator 指向第一个等于 value 的元素的迭代器。如果未找到，返回 last。
 */
template <class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T& value) {
  while (first != last && *first != value)
    ++first;
  return first;
}

// find_if

/**
 * @brief 查找满足条件的第一个元素
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param pred 一元谓词函数对象
 * @return InputIterator 指向第一个满足 pred(*it) 为真的元素的迭代器。如果未找到，返回 last。
 */
template <class InputIterator, class Predicate>
InputIterator find_if(InputIterator first, InputIterator last, Predicate pred) {
  while (first != last && !pred(*first))
    ++first;
  return first;
}

/************************************************************************************ */
//search

/**
 * @brief 查找子序列
 * 
 * 在范围 [first1, last1) 中查找第一个与范围 [first2, last2) 匹配的子序列。
 * 
 * @param first1 主序列的起始迭代器
 * @param last1 主序列的结束迭代器
 * @param first2 子序列的起始迭代器
 * @param last2 子序列的结束迭代器
 * @param Distance1* 主序列距离类型指针（用于优化）
 * @param Distance2* 子序列距离类型指针（用于优化）
 * @return ForwardIterator1 指向找到的子序列的起始迭代器。如果未找到，返回 last1。
 */
template<typename ForwardIterator1, typename ForwardIterator2,
         typename Distance1, typename Distance2>
inline ForwardIterator1 __search(ForwardIterator1 first1,ForwardIterator1 last1,
                               ForwardIterator2 first2,ForwardIterator2 last2,
                               Distance1*, Distance2*){
      Distance1 d1 = distance(first1,last1);
      Distance2 d2 = distance(first2,last2);
      if(d2 > d1)return last1; //如果第二个序列大于第一个序列一定找不到
      ForwardIterator1 i = first1;
      ForwardIterator2 j = first2;
      while(j != last2){
        if(*j == *i){
          ++i;
          ++j;
        }else{
          if(d1 == d2)return last1; //如果第一个序列和第二个序列长度相等且不匹配，一定找不到
          i = ++first1;
          j = first2;
          --d1;
        }
      }
      return first1;
}

/**
 * @brief 查找子序列
 * 
 * 在范围 [first1, last1) 中查找第一个与范围 [first2, last2) 匹配的子序列。
 * 
 * @param first1 主序列的起始迭代器
 * @param last1 主序列的结束迭代器
 * @param first2 子序列的起始迭代器
 * @param last2 子序列的结束迭代器
 * @return ForwardIterator1 指向找到的子序列的起始迭代器。如果未找到，返回 last1。
 */
template<typename ForwardIterator1, typename ForwardIterator2>
inline ForwardIterator1 search(ForwardIterator1 first1,ForwardIterator1 last1,
                               ForwardIterator2 first2,ForwardIterator2 last2){
        return __search(first1,last1,first2,last2,distance_type(first1),distance_type(first2));
}

/**
 * @brief 查找子序列（自定义比较器）
 * 
 * 在范围 [first1, last1) 中查找第一个与范围 [first2, last2) 匹配的子序列。
 * 使用 pred(*i, *j) 进行元素比较。
 * 
 * @param first1 主序列的起始迭代器
 * @param last1 主序列的结束迭代器
 * @param first2 子序列的起始迭代器
 * @param last2 子序列的结束迭代器
 * @param pred 二元谓词函数对象
 * @param Distance1* 主序列距离类型指针
 * @param Distance2* 子序列距离类型指针
 * @return ForwardIterator1 指向找到的子序列的起始迭代器。如果未找到，返回 last1。
 */
template<typename ForwardIterator1, typename ForwardIterator2,
         typename BinaryPredicate, typename Distance1, typename Distance2>
inline ForwardIterator1 __search(ForwardIterator1 first1,ForwardIterator1 last1,
                               ForwardIterator2 first2,ForwardIterator2 last2,
                               BinaryPredicate pred,
                               Distance1*, Distance2*){
      Distance1 d1 = distance(first1,last1);
      Distance2 d2 = distance(first2,last2);
      if(d2 > d1)return last1;
      ForwardIterator1 i = first1;
      ForwardIterator2 j = first2;
      while(j != last2){
        if(pred(*i, *j)){
          ++i;
          ++j;
        }else{
          if(d1 == d2)return last1;
          i = ++first1;
          j = first2;
          --d1;
        }
      }
      return first1;
}

/**
 * @brief 查找子序列（自定义比较器）
 * 
 * 在范围 [first1, last1) 中查找第一个与范围 [first2, last2) 匹配的子序列。
 * 使用 pred(*i, *j) 进行元素比较。
 * 
 * @param first1 主序列的起始迭代器
 * @param last1 主序列的结束迭代器
 * @param first2 子序列的起始迭代器
 * @param last2 子序列的结束迭代器
 * @param pred 二元谓词函数对象
 * @return ForwardIterator1 指向找到的子序列的起始迭代器。如果未找到，返回 last1。
 */
template<typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
inline ForwardIterator1 search(ForwardIterator1 first1,ForwardIterator1 last1,
                               ForwardIterator2 first2,ForwardIterator2 last2,
                               BinaryPredicate pred){
  return __search(first1,last1,first2,last2,pred,distance_type(first1),distance_type(first2));
}

/************************************************************************************************** */

// search_n

/**
 * @brief 查找连续重复元素
 * 
 * 在范围 [first, last) 中查找连续 count 个等于 value 的元素序列。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param count 重复次数
 * @param value 要查找的值
 * @return ForwardIterator 指向找到的序列的第一个元素的迭代器。如果未找到，返回 last。
 */
template <class ForwardIterator, class Size, class T>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Size count, const T& value) {
    if (count <= 0) return first;
    while (first != last) {
        first = msl::find(first, last, value);
        if (first == last) return last; //没有找到
        
        ForwardIterator current = first;
        Size n = count - 1;  //当前还需要n个相同的数
        ++current;
        
        while (n > 0 && current != last && *current == value) {
            ++current;
            --n;
        }
        
        if (n == 0) return first;
        if (current == last) return last;
        first = current;
    }
    return last;
}

/**
 * @brief 查找连续重复元素（自定义比较器）
 * 
 * 在范围 [first, last) 中查找连续 count 个满足 pred(*it, value) 为真的元素序列。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param count 重复次数
 * @param value 要查找的值
 * @param pred 二元谓词函数对象
 * @return ForwardIterator 指向找到的序列的第一个元素的迭代器。如果未找到，返回 last。
 */
template <class ForwardIterator, class Size, class T, class BinaryPredicate>
ForwardIterator search_n(ForwardIterator first, 
                         ForwardIterator last, 
                         Size count, const T& value, 
                         BinaryPredicate pred) {
    if (count <= 0) return first;
    while (first != last) {
        while (first != last && !pred(*first, value))
            ++first;
        if (first == last) return last;
        
        ForwardIterator current = first;
        Size n = count - 1;
        ++current;
        
        while (n > 0 && current != last && pred(*current, value)) {
            ++current;
            --n;
        }
        
        if (n == 0) return first;
        if (current == last) return last;
        first = current;
    }
    return last;
}

/*************************************************************************************** */
//find_end

/**
 * @brief 查找最后一次出现的子序列
 * 
 * 在范围 [first1, last1) 中查找最后一次出现的范围 [first2, last2) 匹配的子序列。
 * 
 * @param first1 主序列的起始迭代器
 * @param last1 主序列的结束迭代器
 * @param first2 子序列的起始迭代器
 * @param last2 子序列的结束迭代器
 * @param forward_iterator_tag 前向迭代器标签
 * @return ForwardIterator1 指向找到的子序列的起始迭代器。如果未找到，返回 last1。
 */
template <class ForwardIterator1, class ForwardIterator2>
ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                            ForwardIterator2 first2, ForwardIterator2 last2,
                            forward_iterator_tag, forward_iterator_tag) {
  if (first2 == last2)
    return last1;
  else {
    ForwardIterator1 result = last1;
    while (1) {
      ForwardIterator1 new_result = msl::search(first1, last1, first2, last2);
      if (new_result == last1)
        return result;
      else {
        result = new_result;
        first1 = new_result;
        ++first1;
      }
    }
  }
}

/**
 * @brief 查找最后一次出现的子序列（双向迭代器优化）
 * 
 * 使用反向迭代器从后向前查找。
 * 
 * @param first1 主序列的起始迭代器
 * @param last1 主序列的结束迭代器
 * @param first2 子序列的起始迭代器
 * @param last2 子序列的结束迭代器
 * @param bidirectional_iterator_tag 双向迭代器标签
 * @return BidirectionalIterator1 指向找到的子序列的起始迭代器。如果未找到，返回 last1。
 */
template <class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
                                  BidirectionalIterator2 first2, BidirectionalIterator2 last2,
                                  bidirectional_iterator_tag, bidirectional_iterator_tag) {
  typedef reverse_iterator<BidirectionalIterator1> RevIter1;
  typedef reverse_iterator<BidirectionalIterator2> RevIter2;

  RevIter1 rlast1(first1);
  RevIter2 rlast2(first2);
  RevIter1 rresult = msl::search(RevIter1(last1), rlast1,
                                 RevIter2(last2), rlast2);

  if (rresult == rlast1)
    return last1;
  else {
    BidirectionalIterator1 result = rresult.base();
    msl::advance(result, -msl::distance(first2, last2));
    return result;
  }
}

/**
 * @brief 查找最后一次出现的子序列
 * 
 * 在范围 [first1, last1) 中查找最后一次出现的范围 [first2, last2) 匹配的子序列。
 * 
 * @param first1 主序列的起始迭代器
 * @param last1 主序列的结束迭代器
 * @param first2 子序列的起始迭代器
 * @param last2 子序列的结束迭代器
 * @return ForwardIterator1 指向找到的子序列的起始迭代器。如果未找到，返回 last1。
 */
template <class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                                 ForwardIterator2 first2, ForwardIterator2 last2) {
  typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;
  typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;
  return __find_end(first1, last1, first2, last2, category1(), category2());
}

/**
 * @brief 查找最后一次出现的子序列（自定义比较器）
 * 
 * 在范围 [first1, last1) 中查找最后一次出现的范围 [first2, last2) 匹配的子序列。
 * 
 * @param first1 主序列的起始迭代器
 * @param last1 主序列的结束迭代器
 * @param first2 子序列的起始迭代器
 * @param last2 子序列的结束迭代器
 * @param forward_iterator_tag 前向迭代器标签
 * @param comp 二元谓词函数对象
 * @return ForwardIterator1 指向找到的子序列的起始迭代器。如果未找到，返回 last1。
 */
template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                            ForwardIterator2 first2, ForwardIterator2 last2,
                            forward_iterator_tag, forward_iterator_tag,
                            BinaryPredicate comp) {
  if (first2 == last2)
    return last1;
  else {
    ForwardIterator1 result = last1;
    while (1) {
      ForwardIterator1 new_result = msl::search(first1, last1, first2, last2, comp);
      if (new_result == last1)
        return result;
      else {
        result = new_result;
        first1 = new_result;
        ++first1;
      }
    }
  }
}

/**
 * @brief 查找最后一次出现的子序列（自定义比较器，双向迭代器优化）
 * 
 * 使用反向迭代器从后向前查找。
 * 
 * @param first1 主序列的起始迭代器
 * @param last1 主序列的结束迭代器
 * @param first2 子序列的起始迭代器
 * @param last2 子序列的结束迭代器
 * @param bidirectional_iterator_tag 双向迭代器标签
 * @param comp 二元谓词函数对象
 * @return BidirectionalIterator1 指向找到的子序列的起始迭代器。如果未找到，返回 last1。
 */
template <class BidirectionalIterator1, class BidirectionalIterator2, class BinaryPredicate>
BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1,
                                  BidirectionalIterator2 first2, BidirectionalIterator2 last2,
                                  bidirectional_iterator_tag, bidirectional_iterator_tag,
                                  BinaryPredicate comp) {
  typedef reverse_iterator<BidirectionalIterator1> RevIter1;
  typedef reverse_iterator<BidirectionalIterator2> RevIter2;

  RevIter1 rlast1(first1);
  RevIter2 rlast2(first2);
  RevIter1 rresult = msl::search(RevIter1(last1), rlast1,
                                 RevIter2(last2), rlast2,
                                 comp);

  if (rresult == rlast1)
    return last1;
  else {
    BidirectionalIterator1 result = rresult.base();
    msl::advance(result, -msl::distance(first2, last2));
    return result;
  }
}

/**
 * @brief 查找最后一次出现的子序列（自定义比较器）
 * 
 * 在范围 [first1, last1) 中查找最后一次出现的范围 [first2, last2) 匹配的子序列。
 * 
 * @param first1 主序列的起始迭代器
 * @param last1 主序列的结束迭代器
 * @param first2 子序列的起始迭代器
 * @param last2 子序列的结束迭代器
 * @param comp 二元谓词函数对象
 * @return ForwardIterator1 指向找到的子序列的起始迭代器。如果未找到，返回 last1。
 */
template <class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                                 ForwardIterator2 first2, ForwardIterator2 last2,
                                 BinaryPredicate comp) {
  typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;
  typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;
  return __find_end(first1, last1, first2, last2, category1(), category2(), comp);
}


/********************************************************************************** */
//find_first_of

/**
 * @brief 查找第一个匹配元素
 * 
 * 在范围 [first1, last1) 中查找第一个在范围 [first2, last2) 中出现的元素。
 * 
 * @param first1 主序列的起始迭代器
 * @param last1 主序列的结束迭代器
 * @param first2 匹配值集合的起始迭代器
 * @param last2 匹配值集合的结束迭代器
 * @return InputIterator 指向找到的第一个匹配元素的迭代器。如果未找到，返回 last1。
 */
template <class InputIterator, class ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
                            ForwardIterator first2, ForwardIterator last2) {
  for ( ; first1 != last1; ++first1) {
    for (ForwardIterator iter = first2; iter != last2; ++iter) {
      if (*first1 == *iter)
        return first1;
    }
  }
  return last1;
}

/**
 * @brief 查找第一个匹配元素（自定义比较器）
 * 
 * 在范围 [first1, last1) 中查找第一个在范围 [first2, last2) 中满足 comp(*it1, *it2) 为真的元素。
 * 
 * @param first1 主序列的起始迭代器
 * @param last1 主序列的结束迭代器
 * @param first2 匹配值集合的起始迭代器
 * @param last2 匹配值集合的结束迭代器
 * @param comp 二元谓词函数对象
 * @return InputIterator 指向找到的第一个匹配元素的迭代器。如果未找到，返回 last1。
 */
template <class InputIterator, class ForwardIterator, class BinaryPredicate>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
                            ForwardIterator first2, ForwardIterator last2,
                            BinaryPredicate comp) {
  for ( ; first1 != last1; ++first1) {
    for (ForwardIterator iter = first2; iter != last2; ++iter) {
      if (comp(*first1, *iter))
        return first1;
    }
  }
  return last1;
}
/********************************************************************************** */
//for_each

/**
 * @brief 对序列中的每个元素执行操作
 * 
 * 对范围 [first, last) 中的每个元素应用函数 func。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param func 要应用的函数对象或函数指针
 * @return function 返回传入的函数对象 func
 */
template<class inputIterator,class function>
function for_each(inputIterator first, inputIterator last, function func) {
    for(;first != last; ++first)
    func(*first);
  return func;
}

/************************************************************************************** */
//generate

/**
 * @brief 用生成器的结果填充序列
 * 
 * 用 gen() 的返回值赋值给范围 [first, last) 中的每个元素。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param gen 生成器函数对象
 */
template<class ForwardIterator,class Generator>
void generate(ForwardIterator first, ForwardIterator last, Generator gen) {
    for(;first != last; ++first)
        *first = gen();
}

/****************************************************************************************** */
//generate_n

/**
 * @brief 用生成器的结果填充序列的前 n 个元素
 * 
 * 用 gen() 的返回值赋值给从 first 开始的 n 个元素。
 * 
 * @param first 序列的起始迭代器
 * @param n 要填充的元素个数
 * @param gen 生成器函数对象
 */
template<class ForwardIterator,class Size,class Generator>
void generate_n(ForwardIterator first, Size n, Generator gen) {
    for(;n > 0; --n, ++first)
        *first = gen();
}


/****************************************************************************************** */
//includes

/**
 * @brief 检查一个有序序列是否包含另一个有序序列
 * 
 * 检查范围 [first2, last2) 中的所有元素是否都包含在范围 [first1, last1) 中。
 * 两个序列必须已排序。
 * 
 * @param first1 第一个有序序列的起始迭代器
 * @param last1 第一个有序序列的结束迭代器
 * @param first2 第二个有序序列的起始迭代器
 * @param last2 第二个有序序列的结束迭代器
 * @return true 如果第二个序列的所有元素都在第一个序列中
 * @return false 否则
 */
template <class InputIterator1, class InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, InputIterator2 last2) {
  while (first1 != last1 && first2 != last2) {
    if (*first2 < *first1)
      return false;
    else if (*first1 < *first2)
      ++first1;
    else {
      ++first1;
      ++first2;
    }
  }
  return first2 == last2;
}

/**
 * @brief 检查一个有序序列是否包含另一个有序序列（自定义比较器）
 * 
 * 检查范围 [first2, last2) 中的所有元素是否都包含在范围 [first1, last1) 中。
 * 两个序列必须根据 comp 已排序。
 * 
 * @param first1 第一个有序序列的起始迭代器
 * @param last1 第一个有序序列的结束迭代器
 * @param first2 第二个有序序列的起始迭代器
 * @param last2 第二个有序序列的结束迭代器
 * @param comp 二元谓词函数对象
 * @return true 如果第二个序列的所有元素都在第一个序列中
 * @return false 否则
 */
template <class InputIterator1, class InputIterator2, class Compare>
bool includes(InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, InputIterator2 last2, Compare comp) {
  while (first1 != last1 && first2 != last2) {
    if (comp(*first2, *first1))
      return false;
    else if (comp(*first1, *first2))
      ++first1;
    else {
      ++first1;
      ++first2;
    }
  }
  return first2 == last2;
}


/****************************************************************************************** */
//max_element 

/**
 * @brief 查找最大元素
 * 
 * 在范围 [first, last) 中查找最大元素。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @return ForwardIterator 指向最大元素的迭代器。如果序列为空，返回 first。
 */
template <class ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
  if (first == last) return first;
  ForwardIterator result = first;
  while (++first != last) {
    if (*result < *first)
      result = first;
  }
  return result;
}

/**
 * @brief 查找最大元素（自定义比较器）
 * 
 * 在范围 [first, last) 中查找最大元素。
 * 使用 comp(*i, *j) 进行元素比较。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param comp 二元谓词函数对象
 * @return ForwardIterator 指向最大元素的迭代器。如果序列为空，返回 first。
 */
template <class ForwardIterator, class Compare>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last,
                            Compare comp) {
  if (first == last) return first;
  ForwardIterator result = first;
  while (++first != last) {
    if (comp(*result, *first))
      result = first;
  }
  return result;
}

/****************************************************************************************** */
//min_element

/**
 * @brief 查找最小元素
 * 
 * 在范围 [first, last) 中查找最小元素。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @return ForwardIterator 指向最小元素的迭代器。如果序列为空，返回 first。
 */
template <class ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last) {
  if (first == last) return first;
  ForwardIterator result = first;
  while (++first != last) {
    if (*first < *result)
      result = first;
  }
  return result;
}

/**
 * @brief 查找最小元素（自定义比较器）
 * 
 * 在范围 [first, last) 中查找最小元素。
 * 使用 comp(*i, *j) 进行元素比较。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param comp 二元谓词函数对象
 * @return ForwardIterator 指向最小元素的迭代器。如果序列为空，返回 first。
 */
template <class ForwardIterator, class Compare>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last,
                            Compare comp) {
  if (first == last) return first;
  ForwardIterator result = first;
  while (++first != last) {
    if (comp(*first, *result))
      result = first;
  }
  return result;
} 

//****************************************************************************************** */
//merge

/**
 * @brief 合并两个有序序列
 * 
 * 将两个有序序列 [first1, last1) 和 [first2, last2) 合并到一个有序序列中，结果存储在 result 开始的位置。
 * 
 * @param first1 第一个有序序列的起始迭代器
 * @param last1 第一个有序序列的结束迭代器
 * @param first2 第二个有序序列的起始迭代器
 * @param last2 第二个有序序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @return OutputIterator 指向合并后序列末尾的迭代器
 */
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result) {
  while (first1 != last1 && first2 != last2) {
    if (*first2 < *first1) {
      *result = *first2;
      ++first2;
    } else {
      *result = *first1;
      ++first1;
    }
    ++result;
  }
  return msl::copy(first2, last2, msl::copy(first1, last1, result)); //将另一个剩余的部分添加到result后面
}

/**
 * @brief 合并两个有序序列（自定义比较器）
 * 
 * 将两个有序序列 [first1, last1) 和 [first2, last2) 合并到一个有序序列中，结果存储在 result 开始的位置。
 * 使用 comp(*i, *j) 进行元素比较。
 * 
 * @param first1 第一个有序序列的起始迭代器
 * @param last1 第一个有序序列的结束迭代器
 * @param first2 第二个有序序列的起始迭代器
 * @param last2 第二个有序序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @param comp 二元谓词函数对象
 * @return OutputIterator 指向合并后序列末尾的迭代器
 */
template <class InputIterator1, class InputIterator2, class OutputIterator,
          class Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result, Compare comp) {
  while (first1 != last1 && first2 != last2) {
    if (comp(*first2, *first1)) {
      *result = *first2;
      ++first2;
    } else {
      *result = *first1;
      ++first1;
    }
    ++result;
  }
  return msl::copy(first2, last2, msl::copy(first1, last1, result));//将另一个剩余的部分添加到result后面
}

/******************************************************************************************* */
// partition

/**
 * @brief 划分序列
 * 
 * 重新排列范围 [first, last) 中的元素，使得所有满足 pred(*it) 为真的元素排在不满足的元素之前。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param pred 一元谓词函数对象
 * @param forward_iterator_tag 前向迭代器标签
 * @return ForwardIterator 指向第二组元素（不满足 pred 的元素）的第一个元素的迭代器
 */
template <class ForwardIterator, class Predicate>
ForwardIterator __partition(ForwardIterator first, ForwardIterator last,
                            Predicate pred, forward_iterator_tag) {
  if (first == last) return first;
  while (pred(*first))
    if (++first == last) return first;

  ForwardIterator next = first;
  while (++next != last)
    if (pred(*next)) {
      msl::iter_swap(first, next);
      ++first;
    }
  return first;
}

/**
 * @brief 划分序列（双向迭代器优化）
 * 
 * 重新排列范围 [first, last) 中的元素，使得所有满足 pred(*it) 为真的元素排在不满足的元素之前。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param pred 一元谓词函数对象
 * @param bidirectional_iterator_tag 双向迭代器标签
 * @return BidirectionalIterator 指向第二组元素（不满足 pred 的元素）的第一个元素的迭代器
 */
template <class BidirectionalIterator, class Predicate>
BidirectionalIterator __partition(BidirectionalIterator first,
                                  BidirectionalIterator last,
                                  Predicate pred,
                                  bidirectional_iterator_tag) {
  while (true) {
    while (true) {
      if (first == last) return first;
      if (pred(*first))
        ++first;
      else
        break;
    }
    do {
      if (first == last) return first;
      --last;
    } while (!pred(*last));
    msl::iter_swap(first, last);
    ++first;
  }
}

/**
 * @brief 划分序列
 * 
 * 重新排列范围 [first, last) 中的元素，使得所有满足 pred(*it) 为真的元素排在不满足的元素之前。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param pred 一元谓词函数对象
 * @return ForwardIterator 指向第二组元素（不满足 pred 的元素）的第一个元素的迭代器
 */
template <class ForwardIterator, class Predicate>
ForwardIterator partition(ForwardIterator first, ForwardIterator last,
                          Predicate pred) {
  typedef typename iterator_traits<ForwardIterator>::iterator_category Category;
  static_assert(is_msl_iterator_tag<Category>::value, "must use msl iterator");
  return __partition(first, last, pred, Category());
}

/********************************************************************************** */
//remove_copy

/**
 * @brief 移除元素并复制
 * 
 * 将范围 [first, last) 中不等于 value 的元素复制到 result 开始的序列中。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @param value 要移除的值
 * @return OutputIterator 指向复制后序列末尾的迭代器
 */
template <class InputIterator, class OutputIterator, class T>
OutputIterator remove_copy(InputIterator first, InputIterator last,
                           OutputIterator result, const T& value) {
  for (; first != last; ++first) {
    if (*first != value) {
      *result = *first;
      ++result;
    }
  }
  return result;
}

//remove_copy_if

/**
 * @brief 移除满足条件的元素并复制
 * 
 * 将范围 [first, last) 中满足 pred(*it) 为假的元素复制到 result 开始的序列中。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @param pred 一元谓词函数对象
 * @return OutputIterator 指向复制后序列末尾的迭代器
 */
template <class InputIterator, class OutputIterator, class Predicate>
OutputIterator remove_copy_if(InputIterator first, InputIterator last,
                              OutputIterator result, Predicate pred) {
  for (; first != last; ++first) {
    if (!pred(*first)) {
      *result = *first;
      ++result;
    }
  }
  return result;
}

//remove

/**
 * @brief 移除元素
 * 
 * 移除范围 [first, last) 中等于 value 的元素。
 * 剩余元素保持原有相对顺序，返回新的逻辑末尾迭代器。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param value 要移除的值
 * @return ForwardIterator 指向新的逻辑末尾的迭代器
 */
template <class ForwardIterator, class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last,
                       const T& value) {
  first = msl::find(first, last, value);
  ForwardIterator next = first;
  return first == last ? first : msl::remove_copy(++next, last, first, value);
}

//remove_if

/**
 * @brief 移除满足条件的元素
 * 
 * 移除范围 [first, last) 中满足 pred(*it) 为真的元素。
 * 剩余元素保持原有相对顺序，返回新的逻辑末尾迭代器。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param pred 一元谓词函数对象
 * @return ForwardIterator 指向新的逻辑末尾的迭代器
 */
template <class ForwardIterator, class Predicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last,
                          Predicate pred) {
  first = msl::find_if(first, last, pred);
  ForwardIterator next = first;
  return first == last ? first : msl::remove_copy_if(++next, last, first, pred);
}

/*****************************************************************************/
//replace

/**
 * @brief 替换元素
 * 
 * 将范围 [first, last) 中所有等于 old_value 的元素替换为 new_value。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param old_value 要被替换的值
 * @param new_value 新值
 */
template <class ForwardIterator, class T>
void replace(ForwardIterator first, ForwardIterator last, const T& old_value,
             const T& new_value) {
  for (; first != last; ++first) {
    if (*first == old_value) *first = new_value;
  }
}

/**
 * @brief 替换满足条件的元素
 * 
 * 将范围 [first, last) 中所有满足 pred(*it) 为真的元素替换为 new_value。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param pred 一元谓词函数对象
 * @param new_value 新值
 */
template <class ForwardIterator, class Predicate, class T>
void replace_if(ForwardIterator first, ForwardIterator last, Predicate pred,
                const T& new_value) {
  for (; first != last; ++first) {
    if (pred(*first)) *first = new_value;
  }
}

/**
 * @brief 替换元素并复制
 * 
 * 将范围 [first, last) 中的元素复制到 result 开始的序列中，
 * 其中等于 old_value 的元素替换为 new_value。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @param old_value 要被替换的值
 * @param new_value 新值
 * @return OutputIterator 指向复制后序列末尾的迭代器
 */
template <class InputIterator, class OutputIterator, class T>
OutputIterator replace_copy(InputIterator first, InputIterator last,
                            OutputIterator result, const T& old_value,
                            const T& new_value) {
  for (; first != last; ++first, ++result) {
    *result = *first == old_value ? new_value : *first;
  }
  return result;
}

/**
 * @brief 替换满足条件的元素并复制
 * 
 * 将范围 [first, last) 中的元素复制到 result 开始的序列中，
 * 其中满足 pred(*it) 为真的元素替换为 new_value。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @param pred 一元谓词函数对象
 * @param new_value 新值
 * @return OutputIterator 指向复制后序列末尾的迭代器
 */
template <class Iterator, class OutputIterator, class Predicate, class T>
OutputIterator replace_copy_if(Iterator first, Iterator last,
                               OutputIterator result, Predicate pred,
                               const T& new_value) {
  for (; first != last; ++first, ++result) {
    *result = pred(*first) ? new_value : *first;
  }
  return result;
}

/************************************************************************ */
//reverse

template <class BidirectionalIterator>
void __reverse(BidirectionalIterator first, BidirectionalIterator last,
               bidirectional_iterator_tag) {
  while (true) {
    if (first == last || first == --last) return;
    msl::iter_swap(first++, last);
  }
}

template <class RandomAccessIterator>
void __reverse(RandomAccessIterator first, RandomAccessIterator last,
               random_access_iterator_tag) {
  while (first < last) {
    msl::iter_swap(first++, --last);
  }
}

/**
 * @brief 反转序列中的元素顺序
 * 
 * 反转范围 [first, last) 中元素的顺序。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 */
template <class BidirectionalIterator>
void reverse(BidirectionalIterator first, BidirectionalIterator last) {
  typedef typename iterator_traits<BidirectionalIterator>::iterator_category Category;
  static_assert(is_msl_iterator_tag<Category>::value, "must use msl iterator");
  msl::__reverse(first, last, iterator_category(first));
}

//reverse_copy
/**
 * @brief 反转序列并复制
 * 
 * 将范围 [first, last) 中的元素逆序复制到 result 开始的序列中。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @return OutputIterator 指向复制后序列末尾的迭代器
 */
template <class BidirectionalIterator, class OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last,
                            OutputIterator result) {
  while (first != last) {
    --last;
    *result = *last;
    ++result;
  }
  return result;
}

/** *********************************************************************************** */
//swap_ranges

/**
 * @brief 交换两个序列的元素
 * 
 * 交换范围 [first1, last1) 和从 first2 开始的相同长度范围内的元素。
 * 
 * @param first1 第一个序列的起始迭代器
 * @param last1 第一个序列的结束迭代器
 * @param first2 第二个序列的起始迭代器
 * @return ForwardIterator2 指向第二个序列中最后一个被交换元素之后的位置
 */
template <class ForwardIterator1, class ForwardIterator2>
ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1,
                             ForwardIterator2 first2) {
  for (; first1 != last1; ++first1, ++first2)
    msl::iter_swap(first1, first2);
  return first2;
}

/** *********************************************************************************** */
//__gcd
/**
 * @brief 计算最大公约数
 * 
 * 计算两个数的最大公约数（Greatest Common Divisor）。
 * 
 * @param m 第一个数
 * @param n 第二个数
 * @return EuclideanRingElement 最大公约数
 */
template <class EuclideanRingElement>
EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n) {
  while (n != 0) {
    EuclideanRingElement t = m % n;
    m = n;
    n = t;
  }
  return m;
}

template <class RandomAccessIterator, class Distance>
void __rotate_cycle(RandomAccessIterator first, RandomAccessIterator last,
                    Distance initial, Distance shift) {
  typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
  value_type value = *(first + initial);
  Distance ptr1 = initial;
  Distance ptr2 = initial + shift;
  while (ptr2 != initial) {
    *(first + ptr1) = *(first + ptr2);
    ptr1 = ptr2;
    if (last - first - ptr2 > shift)
      ptr2 += shift;
    else
      ptr2 = shift - (last - first - ptr2);
  }
  *(first + ptr1) = value;
}

template <class BidirectionalIterator>
void __rotate(BidirectionalIterator first, BidirectionalIterator middle,
              BidirectionalIterator last, bidirectional_iterator_tag) {
  msl::reverse(first, middle);
  msl::reverse(middle, last);
  msl::reverse(first, last);
}

template <class RandomAccessIterator>
void __rotate(RandomAccessIterator first, RandomAccessIterator middle,
              RandomAccessIterator last, random_access_iterator_tag) {
  typedef typename iterator_traits<RandomAccessIterator>::difference_type Distance;
  Distance n = last - first;
  Distance k = middle - first;
  Distance l = n - k;
  
  if (k == 0) return; 

  if (k == l) {
    msl::swap_ranges(first, middle, middle);
    return;
  }

  Distance d = __gcd(n, k);

  for (Distance i = 0; i < d; i++) {
   __rotate_cycle(first, last, i, k);
  }
}

/**
 * @brief 旋转序列
 * 
 * 将范围 [first, last) 中的元素进行旋转，使得 middle 指向的元素成为新的第一个元素。
 * 
 * @param first 序列的起始迭代器
 * @param middle 指向新的第一个元素的迭代器
 * @param last 序列的结束迭代器
 */
template <class BidirectionalIterator>
void rotate(BidirectionalIterator first, BidirectionalIterator middle,
            BidirectionalIterator last) {
  if (first == middle || middle == last) return;
  typedef typename iterator_traits<BidirectionalIterator>::iterator_category Category;
  static_assert(is_msl_iterator_tag<Category>::value, "must use msl iterator");
  msl::__rotate(first, middle, last, iterator_category(first));
}


/**
 * @brief 旋转序列并复制
 * 
 * 将范围 [first, last) 中的元素旋转并复制到 result 开始的序列中，
 * 使得 middle 指向的元素成为结果序列的第一个元素。
 * 
 * @param first 序列的起始迭代器
 * @param middle 指向新的第一个元素的迭代器
 * @param last 序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @return OutputIterator 指向复制后序列末尾的迭代器
 */
template <class ForwardIterator, class OutputIterator>
OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle,
                           ForwardIterator last, OutputIterator result) {
  return msl::copy(first, middle, msl::copy(middle, last, result));
}

/** *********************************************************************************** */
//transform

/**
 * @brief 对序列中的每个元素执行操作并存储结果
 * 
 * 对范围 [first, last) 中的每个元素应用一元操作 op，并将结果存储到 result 开始的序列中。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @param op 一元操作函数对象
 * @return OutputIterator 指向输出序列末尾的迭代器
 */
template <class InputIterator, class OutputIterator, class UnaryOperation>
OutputIterator transform(InputIterator first, InputIterator last,
                         OutputIterator result, UnaryOperation op) {
  for (; first != last; ++first, ++result) {
    *result = op(*first);
  }
  return result;
}

/**
 * @brief 对两个序列中的对应元素执行操作并存储结果
 * 
 * 对范围 [first1, last1) 和从 first2 开始的序列中的对应元素应用二元操作 binary_op，
 * 并将结果存储到 result 开始的序列中。
 * 
 * @param first1 第一个序列的起始迭代器
 * @param last1 第一个序列的结束迭代器
 * @param first2 第二个序列的起始迭代器
 * @param result 输出序列的起始迭代器
 * @param binary_op 二元操作函数对象
 * @return OutputIterator 指向输出序列末尾的迭代器
 */
template <class InputIterator1, class InputIterator2, class OutputIterator,
          class BinaryOperation>
OutputIterator transform(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, OutputIterator result,
                         BinaryOperation binary_op) {
  for (; first1 != last1; ++first1, ++first2, ++result) {
    *result = binary_op(*first1, *first2);
  }
  return result;
}


/******************************************************************************** */
//unique

//只是多了一个value,其实可以写成一个函数而不需要判断是否是输出迭代器的
template <class InputIterator, class OutputIterator>
OutputIterator __unique_copy(InputIterator first, InputIterator last,
                             OutputIterator result, output_iterator_tag) {
  typedef typename iterator_traits<InputIterator>::value_type value_type;
  value_type value = *first;
  *result = value;
  while (++first != last) {
    if (value != *first) {
      value = *first;
      *++result = value;
    }
  }
  return ++result;
}

template <class InputIterator, class ForwardIterator>
ForwardIterator __unique_copy(InputIterator first, InputIterator last,
                              ForwardIterator result, forward_iterator_tag) {
  *result = *first;
  while (++first != last) {
    if (*result != *first) {
      *++result = *first;
    }
  }
  return ++result;
}

/**
 * @brief 移除连续重复元素并复制
 * 
 * 将范围 [first, last) 中的元素复制到 result 开始的序列中，
 * 并且移除连续的重复元素（只保留第一个）。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @return OutputIterator 指向复制后序列末尾的迭代器
 */
template <class InputIterator, class OutputIterator>
OutputIterator unique_copy(InputIterator first, InputIterator last,
                           OutputIterator result) {
  if (first == last) return result;
  typedef typename iterator_traits<OutputIterator>::iterator_category Category;
  static_assert(is_msl_iterator_tag<Category>::value, "must use msl iterator");
  return msl::__unique_copy(first, last, result, Category());
}


//只是多使用一个value
template <class InputIterator, class OutputIterator, class BinaryPredicate>
OutputIterator __unique_copy(InputIterator first, InputIterator last,
                             OutputIterator result, BinaryPredicate pred,
                             output_iterator_tag) {
  typedef typename iterator_traits<InputIterator>::value_type value_type;
  value_type value = *first;
  *result = value;
  while (++first != last) {
    if (!pred(value, *first)) {
      value = *first;
      *++result = value;
    }
  }
  return ++result;
}

template <class InputIterator, class ForwardIterator, class BinaryPredicate>
ForwardIterator __unique_copy(InputIterator first, InputIterator last,
                              ForwardIterator result, BinaryPredicate pred,
                              forward_iterator_tag) {
  *result = *first;
  while (++first != last) {
    if (!pred(*result, *first)) {
      *++result = *first;
    }
  }
  return ++result;
}

/**
 * @brief 移除满足条件的连续重复元素并复制
 * 
 * 将范围 [first, last) 中的元素复制到 result 开始的序列中，
 * 并且移除连续的满足 pred(prev, current) 为真的元素（只保留第一个）。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param result 输出序列的起始迭代器
 * @param pred 二元谓词函数对象
 * @return OutputIterator 指向复制后序列末尾的迭代器
 */
template <class InputIterator, class OutputIterator, class BinaryPredicate>
OutputIterator unique_copy(InputIterator first, InputIterator last,
                           OutputIterator result, BinaryPredicate pred) {
  if (first == last) return result;
  typedef typename iterator_traits<OutputIterator>::iterator_category Category;
  static_assert(is_msl_iterator_tag<Category>::value, "must use msl iterator");
  return msl::__unique_copy(first, last, result, pred, Category());
}



/**
 * @brief 移除连续重复元素
 * 
 * 移除范围 [first, last) 中连续的重复元素（只保留第一个）。
 * 剩余元素保持原有相对顺序，返回新的逻辑末尾迭代器。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @return ForwardIterator 指向新的逻辑末尾的迭代器
 */
template <class ForwardIterator>
ForwardIterator unique(ForwardIterator first, ForwardIterator last) {
  first = msl::adjacent_find(first, last);
  return msl::unique_copy(first, last, first);
}

/**
 * @brief 移除满足条件的连续重复元素
 * 
 * 移除范围 [first, last) 中连续的满足 pred(prev, current) 为真的元素（只保留第一个）。
 * 剩余元素保持原有相对顺序，返回新的逻辑末尾迭代器。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param pred 二元谓词函数对象
 * @return ForwardIterator 指向新的逻辑末尾的迭代器
 */
template <class ForwardIterator, class BinaryPredicate>
ForwardIterator unique(ForwardIterator first, ForwardIterator last,
                       BinaryPredicate pred) {
  first = msl::adjacent_find(first, last, pred);
  return msl::unique_copy(first, last, first, pred);
}


/*************************************************************************** */
//lower_bound

/**
 * @brief 在有序序列中查找第一个大于或等于给定值的元素
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param value 要查找的值
 * @param Distance* 距离类型指针（用于标签分发）
 * @param forward_iterator_tag 前向迭代器标签
 * @return ForwardIterator 指向第一个大于或等于 value 的元素的迭代器
 */
template <class ForwardIterator, class T, class Distance>
ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last,
                              const T& value, Distance*, forward_iterator_tag) {
  Distance len = 0;
  msl::distance(first, last, len);
  Distance half;
  ForwardIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    msl::advance(middle, half);
    if (*middle < value) {
      first = middle;
      ++first;
      len = len - half - 1;
    } else {
      len = half;
    }
  }
  return first;
}

/**
 * @brief 在有序序列中查找第一个大于或等于给定值的元素（随机访问迭代器优化版本）
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param value 要查找的值
 * @param Distance* 距离类型指针（用于标签分发）
 * @param random_access_iterator_tag 随机访问迭代器标签
 * @return RandomAccessIterator 指向第一个大于或等于 value 的元素的迭代器
 */
template <class RandomAccessIterator, class T, class Distance>
RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last,
                                   const T& value, Distance*, random_access_iterator_tag) {
  Distance len = last - first;
  Distance half;
  RandomAccessIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (*middle < value) {
      first = middle + 1;
      len = len - half - 1;
    } else {
      len = half;
    }
  }
  return first;
}

/**
 * @brief 在有序序列中查找第一个大于或等于给定值的元素
 * 
 * 返回指向范围 [first, last) 中第一个不小于（即大于或等于）value 的元素的迭代器。
 * 如果所有元素都小于 value，则返回 last。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param value 要查找的值
 * @return ForwardIterator 指向第一个大于或等于 value 的元素的迭代器
 */
template <class ForwardIterator, class T>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
                                   const T& value) {
  typedef typename iterator_traits<ForwardIterator>::iterator_category category;
  static_assert(is_msl_iterator_tag<category>::value,"must use msl iterator");
  return __lower_bound(first, last, value, distance_type(first),
                       category());
}

/**
 * @brief 在有序序列中查找第一个不满足 comp(*it, value) 为真的元素
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param value 要查找的值
 * @param comp 比较函数对象
 * @param Distance* 距离类型指针（用于标签分发）
 * @param forward_iterator_tag 前向迭代器标签
 * @return ForwardIterator 指向第一个不满足 comp(*it, value) 的元素的迭代器
 */
template <class ForwardIterator, class T, class Distance, class Compare>
ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last,
                              const T& value, Compare comp, Distance*, forward_iterator_tag) {
  Distance len = 0;
  msl::distance(first, last, len);
  Distance half;
  ForwardIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    msl::advance(middle, half);
    if (comp(*middle, value)) {
      first = middle;
      ++first;
      len = len - half - 1;
    } else {
      len = half;
    }
  }
  return first;
}

/**
 * @brief 在有序序列中查找第一个不满足 comp(*it, value) 为真的元素（随机访问迭代器优化版本）
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param value 要查找的值
 * @param comp 比较函数对象
 * @param Distance* 距离类型指针（用于标签分发）
 * @param random_access_iterator_tag 随机访问迭代器标签
 * @return RandomAccessIterator 指向第一个不满足 comp(*it, value) 的元素的迭代器
 */
template <class RandomAccessIterator, class T, class Distance, class Compare>
RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last,
                                   const T& value, Compare comp, Distance*, random_access_iterator_tag) {
  Distance len = last - first;
  Distance half;
  RandomAccessIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (comp(*middle, value)) {
      first = middle + 1;
      len = len - half - 1;
    } else {
      len = half;
    }
  }
  return first;
}

/**
 * @brief 在有序序列中查找第一个不满足 comp(*it, value) 为真的元素
 * 
 * 使用自定义比较函数 comp。返回指向范围 [first, last) 中第一个
 * 使得 comp(*it, value) 为假的元素的迭代器。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param value 要查找的值
 * @param comp 比较函数对象
 * @return ForwardIterator 指向第一个符合条件的元素的迭代器
 */
template <class ForwardIterator, class T, class Compare>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
                                   const T& value, Compare comp) {
  typedef typename iterator_traits<ForwardIterator>::iterator_category category;
  static_assert(is_msl_iterator_tag<category>::value, "must use msl iterator");
  return __lower_bound(first, last, value, comp, distance_type(first),
                       category());
}


//========================================================================
// upper_bound

/**
 * @brief 在有序序列中查找第一个大于给定值的元素
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param value 要查找的值
 * @param Distance* 距离类型指针（用于标签分发）
 * @param forward_iterator_tag 前向迭代器标签
 * @return ForwardIterator 指向第一个大于 value 的元素的迭代器
 */
template <class ForwardIterator, class T, class Distance>
ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last,
                              const T& value, Distance*, forward_iterator_tag) {
  Distance len = 0;
  msl::distance(first, last, len);
  Distance half;
  ForwardIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    msl::advance(middle, half);
    if (value < *middle) {
      len = half;
    } else {
      first = middle;
      ++first;
      len = len - half - 1;
    }
  }
  return first;
}

/**
 * @brief 在有序序列中查找第一个大于给定值的元素（随机访问迭代器优化版本）
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param value 要查找的值
 * @param Distance* 距离类型指针（用于标签分发）
 * @param random_access_iterator_tag 随机访问迭代器标签
 * @return RandomAccessIterator 指向第一个大于 value 的元素的迭代器
 */
template <class RandomAccessIterator, class T, class Distance>
RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last,
                                   const T& value, Distance*, random_access_iterator_tag) {
  Distance len = last - first;
  Distance half;
  RandomAccessIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (value < *middle) {
      len = half;
    } else {
      first = middle + 1;
      len = len - half - 1;
    }
  }
  return first;
}

/**
 * @brief 在有序序列中查找第一个大于给定值的元素
 * 
 * 返回指向范围 [first, last) 中第一个大于 value 的元素的迭代器。
 * 如果没有元素大于 value，则返回 last。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param value 要查找的值
 * @return ForwardIterator 指向第一个大于 value 的元素的迭代器
 */
template <class ForwardIterator, class T>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
                                   const T& value) {
  typedef typename iterator_traits<ForwardIterator>::iterator_category category;
  static_assert(is_msl_iterator_tag<category>::value, "must use msl iterator");
  return __upper_bound(first, last, value, distance_type(first),
                       category());
}

/**
 * @brief 在有序序列中查找第一个满足 comp(value, *it) 为真的元素
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param value 要查找的值
 * @param comp 比较函数对象
 * @param Distance* 距离类型指针（用于标签分发）
 * @param forward_iterator_tag 前向迭代器标签
 * @return ForwardIterator 指向第一个满足 comp(value, *it) 的元素的迭代器
 */
template <class ForwardIterator, class T, class Distance, class Compare>
ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last,
                              const T& value, Compare comp, Distance*, forward_iterator_tag) {
  Distance len = 0;
  msl::distance(first, last, len);
  Distance half;
  ForwardIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    msl::advance(middle, half);
    if (comp(value, *middle)) {
      len = half;
    } else {
      first = middle;
      ++first;
      len = len - half - 1;
    }
  }
  return first;
}

/**
 * @brief 在有序序列中查找第一个满足 comp(value, *it) 为真的元素（随机访问迭代器优化版本）
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param value 要查找的值
 * @param comp 比较函数对象
 * @param Distance* 距离类型指针（用于标签分发）
 * @param random_access_iterator_tag 随机访问迭代器标签
 * @return RandomAccessIterator 指向第一个满足 comp(value, *it) 的元素的迭代器
 */
template <class RandomAccessIterator, class T, class Distance, class Compare>
RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last,
                                   const T& value, Compare comp, Distance*, random_access_iterator_tag) {
  Distance len = last - first;
  Distance half;
  RandomAccessIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (comp(value, *middle)) {
      len = half;
    } else {
      first = middle + 1;
      len = len - half - 1;
    }
  }
  return first;
}

/**
 * @brief 在有序序列中查找第一个大于给定值的元素
 * 
 * 使用自定义比较函数 comp。返回指向范围 [first, last) 中第一个
 * 使得 comp(value, *it) 为真的元素的迭代器。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param value 要查找的值
 * @param comp 比较函数对象
 * @return ForwardIterator 指向第一个符合条件的元素的迭代器
 */
template <class ForwardIterator, class T, class Compare>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
                                   const T& value, Compare comp) {
  typedef typename iterator_traits<ForwardIterator>::iterator_category category;
  static_assert(is_msl_iterator_tag<category>::value, "must use msl iterator");
  return __upper_bound(first, last, value, comp, distance_type(first),
                       category());
}

/**************************************************************************** */
//binary_search

/**
 * @brief 二分查找
 * 
 * 在有序序列中检查是否存在等于 value 的元素。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param value 要查找的值
 * @return true 如果找到等于 value 的元素
 * @return false 如果未找到
 */
template<class ForwardIterator, class T >
bool binary_search(ForwardIterator first, ForwardIterator last, const T& value){
  ForwardIterator it = msl::lower_bound(first, last, value);
  return (it != last && !(value < *it));
}

/**
 * @brief 二分查找（自定义比较器）
 * 
 * 在有序序列中检查是否存在与 value 等价的元素（根据 comp）。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param value 要查找的值
 * @param comp 比较函数对象
 * @return true 如果找到等价元素
 * @return false 如果未找到
 */
template<class ForwardIterator, class T, class Compare >
bool binary_search(ForwardIterator first, ForwardIterator last, const T& value, Compare comp){
  ForwardIterator it = msl::lower_bound(first, last, value, comp);
  return (it != last && !(comp(value, *it)));
}

/************************************************************************* */
//next_permutation

/**
 * @brief 生成下一个字典序排列
 * 
 * 将范围 [first, last) 中的元素重新排列为下一个字典序更大的排列。
 * 如果当前排列已经是最大的，则将其重排为最小排列（升序），并返回 false。
 * 否则返回 true。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @return true 如果成功生成下一个排列
 * @return false 如果当前已经是最大排列
 */
template <class BidirectionalIterator>
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last) {
  if (first == last) return false;
  BidirectionalIterator i = last;
  if (first == --i) return false;

  while (true) {
    BidirectionalIterator i1, i2;
    i1 = i;
    if (*--i < *i1) {
      i2 = last;
      while (!(*i < *--i2));
      msl::iter_swap(i, i2);
      msl::reverse(i1, last);
      return true;
    }
    if (i == first) {
      msl::reverse(first, last);
      return false;
    }
  }
}

/**
 * @brief 生成下一个字典序排列（自定义比较器）
 * 
 * 使用自定义比较函数 comp。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param comp 比较函数对象
 * @return true 如果成功生成下一个排列
 * @return false 如果当前已经是最大排列
 */
template <class BidirectionalIterator, class Compare>
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp) {
  if (first == last) return false;
  BidirectionalIterator i = last;
  if (first == --i) return false;

  while (true) {
    BidirectionalIterator i1, i2;
    i1 = i;
    if (comp(*--i, *i1)) {
      i2 = last;
      while (!comp(*i, *--i2));
      msl::iter_swap(i, i2);
      msl::reverse(i1, last);
      return true;
    }
    if (i == first) {
      msl::reverse(first, last);
      return false;
    }
  }
}


/**
 * @brief 生成上一个字典序排列
 * 
 * 将范围 [first, last) 中的元素重新排列为上一个字典序更小的排列。
 * 如果当前排列已经是最小的，则将其重排为最大排列（降序），并返回 false。
 * 否则返回 true。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @return true 如果成功生成上一个排列
 * @return false 如果当前已经是最小排列
 */
template <class BidirectionalIterator>
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last) {
  if (first == last) return false;
  BidirectionalIterator i = last;
  if (first == --i) return false;

  while (true) {
    BidirectionalIterator i1, i2;
    i1 = i;
    if (*i1 < *--i) {
      i2 = last;
      while (!(*--i2 < *i));
      msl::iter_swap(i, i2);
      msl::reverse(i1, last);
      return true;
    }
    if (i == first) {
      msl::reverse(first, last);
      return false;
    }
  }
}


/*************************************************************** */
//prev_permutation

/**
 * @brief 生成上一个字典序排列（自定义比较器）
 * 
 * 使用自定义比较函数 comp。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param comp 比较函数对象
 * @return true 如果成功生成上一个排列
 * @return false 如果当前已经是最小排列
 */
template <class BidirectionalIterator, class Compare>
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp) {
  if (first == last) return false;
  BidirectionalIterator i = last;
  if (first == --i) return false;

  while (true) {
    BidirectionalIterator i1, i2;
    i1 = i;
    if (comp(*i1, *--i)) {
      i2 = last;
      while (!comp(*--i2, *i));
      msl::iter_swap(i, i2);
      msl::reverse(i1, last);
      return true;
    }
    if (i == first) {
      msl::reverse(first, last);
      return false;
    }
  }
}

//========================================================================
// random_shuffle

/**
 * @brief 随机打乱序列中的元素
 * 
 * 使用内置的随机数生成器 (std::rand) 随机重排范围 [first, last) 中的元素。
 * 时间复杂度为 O(N)。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 */
template <class RandomAccessIterator>
inline void random_shuffle(RandomAccessIterator first, RandomAccessIterator last) {
  if (first == last) return;
  for (RandomAccessIterator i = first + 1; i != last; ++i) {
    msl::iter_swap(i, first + (std::rand() % ((i - first) + 1)));
  }
}

/**
 * @brief 随机打乱序列中的元素（自定义随机生成器）
 * 
 * 使用提供的随机数生成器 rand 随机重排范围 [first, last) 中的元素。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param rand 随机数生成器函数对象
 */
template <class RandomAccessIterator, class RandomNumberGenerator>
void random_shuffle(RandomAccessIterator first, RandomAccessIterator last,
                    RandomNumberGenerator& rand) {
  if (first == last) return;
  for (RandomAccessIterator i = first + 1; i != last; ++i) {
    msl::iter_swap(i, first + rand((i - first) + 1));
  }
}

/********************************************************************** */
//partial_sort


/**
 * @brief 部分排序
 * 
 * 重排范围 [first, last) 中的元素，使得范围 [first, middle) 包含最小的 (middle - first) 个元素，
 * 并且这些元素按升序排列。范围 [middle, last) 中的其余元素不保证顺序。
 * 
 * @param first 序列的起始迭代器
 * @param middle 排序结束位置的迭代器（包含前 N 个最小元素）
 * @param last 序列的结束迭代器
 * @param T* 值类型指针（用于推导类型）
 */
template<class RandomAccessIterator, class T>
inline void __partial_sort(RandomAccessIterator first,
                           RandomAccessIterator middle,
                           RandomAccessIterator last,T*) {
  msl::make_heap(first, middle);
  for (RandomAccessIterator i = middle; i != last; ++i) {
    if(*i < *first)
       msl::__pop_heap(first, middle, i, T(*i), distance_type(first));    
  }
  msl::sort_heap(first, middle);
}

/**
 * @brief 部分排序
 * 
 * 对范围 [first, last) 进行部分排序，使得前 (middle - first) 个最小元素
 * 被放置在 [first, middle) 中并有序。
 * 
 * @param first 序列的起始迭代器
 * @param middle 排序结束位置的迭代器
 * @param last 序列的结束迭代器
 */
template<class RandomAccessIterator>
inline void partial_sort(RandomAccessIterator first, 
                         RandomAccessIterator middle, 
                         RandomAccessIterator last){
  __partial_sort(first, middle, last, value_type(first));
}


/**
 * @brief 部分排序（自定义比较器）
 * 
 * 使用自定义比较函数 comp。
 * 重排范围 [first, last) 中的元素，使得范围 [first, middle) 包含根据 comp 定义的最小的 (middle - first) 个元素，
 * 并且这些元素按顺序排列。
 * 
 * @param first 序列的起始迭代器
 * @param middle 排序结束位置的迭代器
 * @param last 序列的结束迭代器
 * @param T* 值类型指针
 * @param comp 比较函数对象
 */
template<class RandomAccessIterator, class T,class Compare>
inline void __partial_sort(RandomAccessIterator first,
                           RandomAccessIterator middle,
                           RandomAccessIterator last,T*,Compare comp) {
  msl::make_heap(first, middle, comp);
  for (RandomAccessIterator i = middle; i != last; ++i) {
    if(comp(*i, *first))
       msl::__pop_heap(first, middle, i, T(*i), distance_type(first), comp);    
  }
  msl::sort_heap(first, middle, comp);
}

/**
 * @brief 部分排序（自定义比较器）
 * 
 * 对范围 [first, last) 进行部分排序，使得前 (middle - first) 个最小元素
 * （根据 comp 比较）被放置在 [first, middle) 中并有序。
 * 
 * @param first 序列的起始迭代器
 * @param middle 排序结束位置的迭代器
 * @param last 序列的结束迭代器
 * @param comp 比较函数对象
 */
template<class RandomAccessIterator, class Compare>
inline void partial_sort(RandomAccessIterator first,
                         RandomAccessIterator middle,
                         RandomAccessIterator last, Compare comp) {
  __partial_sort(first, middle, last, value_type(first), comp);
}


/******************************************************************************** */
//partial_sort_copy

/**
 * @brief 部分排序并复制
 * 
 * 将范围 [first, last) 中最小的 N 个元素复制到范围 [result_first, result_last) 中，
 * 其中 N = min(last - first, result_last - result_first)。
 * 复制后的元素在结果范围中按升序排列。
 * 
 * @param first 输入序列的起始迭代器
 * @param last 输入序列的结束迭代器
 * @param result_first 输出序列的起始迭代器
 * @param result_last 输出序列的结束迭代器
 * @param Distance* 距离类型指针（用于推导类型）
 * @param T* 值类型指针（用于推导类型）
 * @return RandomAccessIterator 指向输出序列中已复制元素的末尾的迭代器
 */
template <class InputIterator, class RandomAccessIterator, class Distance, class T>
RandomAccessIterator __partial_sort_copy(InputIterator first, InputIterator last,
                                         RandomAccessIterator result_first,
                                         RandomAccessIterator result_last, 
                                         Distance*, T*) {
    if (result_first == result_last) return result_last;
    RandomAccessIterator result_real_last = result_first;
    while(first != last && result_real_last != result_last) {
        *result_real_last = *first;
        ++result_real_last;
        ++first;
    }
    msl::make_heap(result_first, result_real_last);
    while (first != last) {
        if (*first < *result_first) {
            msl::__adjust_heap(result_first, Distance(0), Distance(result_real_last - result_first), T(*first));
        }
        ++first;
    }
    msl::sort_heap(result_first, result_real_last);
    return result_real_last;
}

/**
 * @brief 部分排序并复制
 * 
 * 复制并排序最小的 N 个元素。
 * 
 * @param first 输入序列的起始迭代器
 * @param last 输入序列的结束迭代器
 * @param result_first 输出序列的起始迭代器
 * @param result_last 输出序列的结束迭代器
 * @return RandomAccessIterator 指向输出序列末尾的迭代器
 */
template <class InputIterator, class RandomAccessIterator>
inline RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last,
                                              RandomAccessIterator result_first,
                                              RandomAccessIterator result_last) {
    return __partial_sort_copy(first, last, result_first, result_last, 
                               distance_type(result_first), value_type(first));
}

/**
 * @brief 部分排序并复制（自定义比较器）
 * 
 * 使用自定义比较函数 comp。
 * 
 * @param first 输入序列的起始迭代器
 * @param last 输入序列的结束迭代器
 * @param result_first 输出序列的起始迭代器
 * @param result_last 输出序列的结束迭代器
 * @param Distance* 距离类型指针
 * @param T* 值类型指针
 * @param comp 比较函数对象
 * @return RandomAccessIterator 指向输出序列末尾的迭代器
 */
template <class InputIterator, class RandomAccessIterator, class Distance, class T, class Compare>
RandomAccessIterator __partial_sort_copy(InputIterator first, InputIterator last,
                                         RandomAccessIterator result_first,
                                         RandomAccessIterator result_last, 
                                         Distance*, T*, Compare comp) {
    if (result_first == result_last) return result_last;
    RandomAccessIterator result_real_last = result_first;
    while(first != last && result_real_last != result_last) {
        *result_real_last = *first;
        ++result_real_last;
        ++first;
    }
    msl::make_heap(result_first, result_real_last, comp);
    while (first != last) {
        if (comp(*first, *result_first)) {
            msl::__adjust_heap(result_first, Distance(0), Distance(result_real_last - result_first), T(*first), comp);
        }
        ++first;
    }
    msl::sort_heap(result_first, result_real_last, comp);
    return result_real_last;
}

/**
 * @brief 部分排序并复制（自定义比较器）
 * 
 * 复制并排序最小的 N 个元素（根据 comp）。
 * 
 * @param first 输入序列的起始迭代器
 * @param last 输入序列的结束迭代器
 * @param result_first 输出序列的起始迭代器
 * @param result_last 输出序列的结束迭代器
 * @param comp 比较函数对象
 * @return RandomAccessIterator 指向输出序列末尾的迭代器
 */
template <class InputIterator, class RandomAccessIterator, class Compare>
inline RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last,
                                              RandomAccessIterator result_first,
                                              RandomAccessIterator result_last,
                                              Compare comp) {
    return __partial_sort_copy(first, last, result_first, result_last, 
                               distance_type(result_first), value_type(first), comp);
}


/****************************************************************************** */
//__insertion_sort

template<class RandomAccessIterator,class T>
void __unguarder_linear_insert(RandomAccessIterator last, T value){
    RandomAccessIterator next = last;
    --next;
    while(value < *next) {
      *last = *next;
      last = next;
      --next;
    }
    *last = value;
}

/**
 * @brief 无哨兵的线性插入排序
 * 
 * 对序列 [first, last) 进行插入排序。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
 * @param T 序列中元素的类型
 */

template<class RandomAccessIterator, class T>
void __liner_insert(RandomAccessIterator first, RandomAccessIterator last, T*) {
    T value = *last;
    if(value < *first) {
        msl::copy_backward(first, last, last + 1);
        *first = value;
    }else{
        __unguarder_linear_insert(last, value);       
    }
}


/** 
 * 对序列 [first, last) 进行插入排序。
 * 
 * @param first 序列的起始迭代器
 * @param last 序列的结束迭代器
*/
template<class RandomAccessIterator>
void __insert_sort(RandomAccessIterator first, RandomAccessIterator last) {
    if(first == last) return;
    for(RandomAccessIterator i = first + 1; i != last; ++i) {
        __liner_insert(first,i,value_type(first));
    }
}




}// namespace msl




#endif // STL_ALGO_H
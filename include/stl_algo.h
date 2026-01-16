#ifndef STL_ALGO_H
#define STL_ALGO_H

#include "stl_heap.h"
#include "stl_numeric.h"
#include "stl_algobase.h"
#include "iterator.h"

namespace msl {





//========================================================================
// set_union

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
//找到第一个相邻的重复元素

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
template <class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T& value) {
  while (first != last && *first != value)
    ++first;
  return first;
}

// find_if
template <class InputIterator, class Predicate>
InputIterator find_if(InputIterator first, InputIterator last, Predicate pred) {
  while (first != last && !pred(*first))
    ++first;
  return first;
}

/************************************************************************************ */
//search
//可以看出search的效率很慢
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

template<typename ForwardIterator1, typename ForwardIterator2>
inline ForwardIterator1 search(ForwardIterator1 first1,ForwardIterator1 last1,
                               ForwardIterator2 first2,ForwardIterator2 last2){
        return __search(first1,last1,first2,last2,distance_type(first1),distance_type(first2));
}

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

template<typename ForwardIterator1, typename ForwardIterator2, typename BinaryPredicate>
inline ForwardIterator1 search(ForwardIterator1 first1,ForwardIterator1 last1,
                               ForwardIterator2 first2,ForwardIterator2 last2,
                               BinaryPredicate pred){
  return __search(first1,last1,first2,last2,pred,distance_type(first1),distance_type(first2));
}

/************************************************************************************************** */

// search_n
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

template <class ForwardIterator, class Size, class T, class BinaryPredicate>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Size count, const T& value, BinaryPredicate pred) {
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

template <class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                                 ForwardIterator2 first2, ForwardIterator2 last2) {
  typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;
  typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;
  return __find_end(first1, last1, first2, last2, category1(), category2());
}

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

template<class inputIterator,class function>
function for_each(inputIterator first, inputIterator last, function func) {
    for(;first != last; ++first)
    func(*first);
  return func;
}

/************************************************************************************** */
//generate
template<class ForwardIterator,class Generator>
void generate(ForwardIterator first, ForwardIterator last, Generator gen) {
    for(;first != last; ++first)
        *first = gen();
}

/****************************************************************************************** */
//generate_n
template<class ForwardIterator,class Size,class Generator>
void generate_n(ForwardIterator first, Size n, Generator gen) {
    for(;n > 0; --n, ++first)
        *first = gen();
}


/****************************************************************************************** */
//includes
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

template <class ForwardIterator, class Predicate>
ForwardIterator partition(ForwardIterator first, ForwardIterator last,
                          Predicate pred) {
  return __partition(first, last, pred, iterator_category(first));
}

/********************************************************************************** */
//remove_copy
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
template <class ForwardIterator, class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last,
                       const T& value) {
  first = msl::find(first, last, value);
  ForwardIterator next = first;
  return first == last ? first : msl::remove_copy(++next, last, first, value);
}

//remove_if
template <class ForwardIterator, class Predicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last,
                          Predicate pred) {
  first = msl::find_if(first, last, pred);
  ForwardIterator next = first;
  return first == last ? first : msl::remove_copy_if(++next, last, first, pred);
}

/*****************************************************************************/
//replace
template <class ForwardIterator, class T>
void replace(ForwardIterator first, ForwardIterator last, const T& old_value,
             const T& new_value) {
  for (; first != last; ++first) {
    if (*first == old_value) *first = new_value;
  }
}

template <class ForwardIterator, class Predicate, class T>
void replace_if(ForwardIterator first, ForwardIterator last, Predicate pred,
                const T& new_value) {
  for (; first != last; ++first) {
    if (pred(*first)) *first = new_value;
  }
}

template <class InputIterator, class OutputIterator, class T>
OutputIterator replace_copy(InputIterator first, InputIterator last,
                            OutputIterator result, const T& old_value,
                            const T& new_value) {
  for (; first != last; ++first, ++result) {
    *result = *first == old_value ? new_value : *first;
  }
  return result;
}

template <class Iterator, class OutputIterator, class Predicate, class T>
OutputIterator replace_copy_if(Iterator first, Iterator last,
                               OutputIterator result, Predicate pred,
                               const T& new_value) {
  for (; first != last; ++first, ++result) {
    *result = pred(*first) ? new_value : *first;
  }
  return result;
}




}// namespace msl


#endif // STL_ALGO_H
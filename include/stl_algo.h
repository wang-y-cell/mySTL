#ifndef STL_ALGO_H
#define STL_ALGO_H

#include "stl_heap.h"
#include "stl_numeric.h"
#include "stl_algobase.h"

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



}// namespace msl


#endif // STL_ALGO_H
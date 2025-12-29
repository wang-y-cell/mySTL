#ifndef STL_ALGOBASE_H
#define STL_ALGOBASE_H

#include "iterator.h"
#include <cstring>

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



}// namespace msl



#endif // STL_ALGOBASE_H

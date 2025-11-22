#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#include <cstddef>
#include "type_traits.h"

namespace mystl {


struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag : public input_iterator_tag{};
struct bidirectional_iterator_tag : public forward_iterator_tag{};
struct random_access_iterator_tag : public bidirectional_iterator_tag{};


template   <class Category,
            class T,
            class Distance = ptrdiff_t,
            class Pointer = T*,
            class Reference = T&>
struct iterator {
    typedef Category iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
};

template <class InputIterator>
struct iterator_traits {
    typedef typename InputIterator::iterator_category iterator_category;
    typedef typename InputIterator::value_type value_type;
    typedef typename InputIterator::difference_type difference_type;
    typedef typename InputIterator::pointer pointer;
    typedef typename InputIterator::reference reference;
};

//偏特化
template <class T>
struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
    typedef typename type_traits<T>::is_pod_type is_pod_type;
};

//偏特化
template <class T>
struct iterator_traits<const T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef typename type_traits<T>::is_pod_type is_pod_type;
};



template <class InputIterator>
inline typename iterator_traits<InputIterator>::iterator_category
iterator_category(const InputIterator&) {
    typedef typename iterator_traits<InputIterator>::iterator_category category;
    return category();
}
    
template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type*
distance_type(const InputIterator&) {
    return static_cast<typename iterator_traits<InputIterator>::difference_type*>(0);
}

template<class InputIterator>
inline typename iterator_traits<InputIterator>::value_type*
value_type(const InputIterator&) {
    return static_cast<typename iterator_traits<InputIterator>::value_type*>(0);
}

template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last,
           input_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last) {
        ++first;++n;
    }
    return n;
}

template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last,
           forward_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last) {
        ++first;++n;
    }
    return n;
}

template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last,
           bidirectional_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last) {
        ++first;++n;
    }
    return n;
}

template<class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last,
           random_access_iterator_tag) {
    return last - first;
}


template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
    return __distance(first, last, iterator_category(first));
}

template<class InputIterator, class Distance>
inline void
__advance(InputIterator& i, Distance n, input_iterator_tag) {
    while (n--) {
        ++i;
    }
}

template<class BidirectionalIterator, class Distance>
inline void
__advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag) {
    if (n >= 0) {
        while (n--) {
            ++i;
        }
    } else {
        while (n++) {
            --i;
        }
    }
}

template<class RandomAccessIterator, class Distance>
inline void
__advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag) {
    i += n;
}



template<class InputIterator, class Distance>
inline void
advance(InputIterator& i, Distance n) {
    __advance(i, n, iterator_category(i));
}

template <class ForwardIterator>
inline ForwardIterator next(ForwardIterator it, typename iterator_traits<ForwardIterator>::difference_type n = 1) {
    advance(it, n);
    return it;
}

template <class BidirectionalIterator>
inline BidirectionalIterator prev(BidirectionalIterator it, typename iterator_traits<BidirectionalIterator>::difference_type n = 1) {
    advance(it, -n);
    return it;
}

template <class InputIterator>
class reverse_iterator {
public:
    typedef InputIterator iterator_type;
    typedef typename iterator_traits<InputIterator>::iterator_category iterator_category;
    typedef typename iterator_traits<InputIterator>::value_type value_type;
    typedef typename iterator_traits<InputIterator>::difference_type difference_type;
    typedef typename iterator_traits<InputIterator>::pointer pointer;
    typedef typename iterator_traits<InputIterator>::reference reference;

private:
    iterator_type current;

public:
    reverse_iterator() : current() {}
    explicit reverse_iterator(iterator_type x) : current(x) {}
    template <class U>
    reverse_iterator(const reverse_iterator<U>& other) : current(other.base()) {}
    iterator_type base() const { return current; }
    reference operator*() const { iterator_type tmp = current; return *--tmp; }
    pointer operator->() const { return &**this; }
    reverse_iterator& operator++() { --current; return *this; }
    reverse_iterator operator++(int) { reverse_iterator tmp = *this; --current; return tmp; }
    reverse_iterator& operator--() { ++current; return *this; }
    reverse_iterator operator--(int) { reverse_iterator tmp = *this; ++current; return tmp; }
    reverse_iterator operator+(difference_type n) const { return reverse_iterator(current - n); }
    reverse_iterator operator-(difference_type n) const { return reverse_iterator(current + n); }
    reverse_iterator& operator+=(difference_type n) { current -= n; return *this; }
    reverse_iterator& operator-=(difference_type n) { current += n; return *this; }
    reference operator[](difference_type n) const { return *(*this + n); }
};

template <class Iter1, class Iter2>
inline bool operator==(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y) { return x.base() == y.base(); }

template <class Iter1, class Iter2>
inline bool operator!=(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y) { return x.base() != y.base(); }

template <class Iter1, class Iter2>
inline bool operator<(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y) { return y.base() < x.base(); }

template <class Iter1, class Iter2>
inline bool operator>(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y) { return y.base() > x.base(); }

template <class Iter1, class Iter2>
inline bool operator<=(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y) { return !(y < x); }

template <class Iter1, class Iter2>
inline bool operator>=(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y) { return !(x < y); }

template <class Iter>
inline reverse_iterator<Iter> operator+(typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter>& x) { return reverse_iterator<Iter>(x.base() - n); }

template <class Iter1, class Iter2>
inline typename reverse_iterator<Iter1>::difference_type operator-(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y) { return y.base() - x.base(); }





} // namespace mystl

#endif //__ITERATOR_H__

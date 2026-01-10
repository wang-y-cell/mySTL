#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#include <cstddef>
#include "type_traits.h"

namespace msl {


struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag : public input_iterator_tag{};
struct bidirectional_iterator_tag : public forward_iterator_tag{};
struct random_access_iterator_tag : public bidirectional_iterator_tag{};


template   <typename Category,
            typename T,
            typename Distance = ptrdiff_t,
            typename Pointer = T*,
            typename Reference = T&>
struct iterator {
    typedef Category iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
};

template <typename InputIterator>
struct iterator_traits {
    typedef typename InputIterator::iterator_category iterator_category;
    typedef typename InputIterator::value_type value_type;
    typedef typename InputIterator::difference_type difference_type;
    typedef typename InputIterator::pointer pointer;
    typedef typename InputIterator::reference reference;
};

//Partial specialization
template <typename T>
struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
    typedef typename type_traits<T>::is_pod_type is_pod_type;
};

//Partial specialization
template <typename T>
struct iterator_traits<const T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef typename type_traits<T>::is_pod_type is_pod_type;
};


//返回迭代器的类别类型
template <typename InputIterator>
inline typename iterator_traits<InputIterator>::iterator_category
iterator_category(const InputIterator&) {
    typedef typename iterator_traits<InputIterator>::iterator_category category;
    return category();
}

//返回迭代器的距离类型
template<typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type*
distance_type(const InputIterator&) {
    return static_cast<typename iterator_traits<InputIterator>::difference_type*>(0);
}

//返回迭代器的值类型
template<typename InputIterator>
inline typename iterator_traits<InputIterator>::value_type*
value_type(const InputIterator&) {
    return static_cast<typename iterator_traits<InputIterator>::value_type*>(0);
}


//不是stl中的,是我自己定义的,可以不用
template<typename T> struct is_msl_iterator_tag { static const bool value = false; };
template<> struct is_msl_iterator_tag<input_iterator_tag> { static const bool value = true; };
template<> struct is_msl_iterator_tag<output_iterator_tag> { static const bool value = true; };
template<> struct is_msl_iterator_tag<forward_iterator_tag> { static const bool value = true; };
template<> struct is_msl_iterator_tag<bidirectional_iterator_tag> { static const bool value = true; };
template<> struct is_msl_iterator_tag<random_access_iterator_tag> { static const bool value = true; };



template<typename InputIterator, typename Distance>
inline void __distance(InputIterator first, InputIterator last, Distance& n, input_iterator_tag) {
    n = 0;
    while (first != last) {
        ++first;++n;
    }
}


template<typename RandomAccessIterator, typename Distance>
inline void __distance(RandomAccessIterator first, RandomAccessIterator last, Distance& n, 
                      random_access_iterator_tag) {
    n = last - first;
}

template<typename InputIterator, typename Distance>
inline void distance(InputIterator first, InputIterator last, Distance& n) {
    return __distance(first, last,n,iterator_category(first));
}


template<typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last,
           input_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last) {
        ++first;++n;
    }
    return n;
}


template<typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last,
           random_access_iterator_tag) {
    return last - first;
}


template<typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
    return __distance(first, last, iterator_category(first));
}



template<typename InputIterator, typename Distance>
inline void
__advance(InputIterator& i, Distance n, input_iterator_tag) {
    while (n--) {
        ++i;
    }
}

template<typename BidirectionalIterator, typename Distance>
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

template<typename RandomAccessIterator, typename Distance>
inline void
__advance(RandomAccessIterator& i, Distance n, random_access_iterator_tag) {
    i += n;
}



template<typename InputIterator, typename Distance>
inline void
advance(InputIterator& i, Distance n) {
    __advance(i, n, iterator_category(i));
}

template <typename ForwardIterator>
inline ForwardIterator next(ForwardIterator it, typename iterator_traits<ForwardIterator>::difference_type n = 1) {
    advance(it, n);
    return it;
}

template <typename BidirectionalIterator>
inline BidirectionalIterator prev(BidirectionalIterator it, typename iterator_traits<BidirectionalIterator>::difference_type n = 1) {
    advance(it, -n);
    return it;
}


template <typename Iterator>
class reverse_iterator {
public:
    //这里需要在类中定义迭代器类型,因为在类中定义的迭代器类型,可以在类中使用
    typedef Iterator iterator_type;        // 迭代器类型
    typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
    typedef typename iterator_traits<Iterator>::value_type value_type;
    typedef typename iterator_traits<Iterator>::difference_type difference_type;
    typedef typename iterator_traits<Iterator>::pointer pointer;
    typedef typename iterator_traits<Iterator>::reference reference;
    typedef reverse_iterator<iterator_type> self; // 迭代器自身类型

private:
    iterator_type current;

public:
    reverse_iterator() : current() {}
    explicit reverse_iterator(iterator_type x) : current(x) {}
    
    template <typename U>
    reverse_iterator(const reverse_iterator<U>& other) : 
    current(other.base()) {}

    iterator_type base() const 
    { return current; }

    reference operator*() const
     { iterator_type tmp = current; return *--tmp; }

    pointer operator->() const 
    { return &**this; }

    self& operator++() 
    { --current; return *this; }

    self operator++(int)
    { self tmp = *this; --current; return tmp; }

    self& operator--() 
    { ++current; return *this; }

    self operator--(int) 
    { self tmp = *this; ++current; return tmp; }

    self operator+(difference_type n) const 
    { return self(current - n); }

    self operator-(difference_type n) const 
    { return self(current + n); }

    self& operator+=(difference_type n) 
    { current -= n; return *this; }

    self& operator-=(difference_type n) 
    { current += n; return *this; }

    reference operator[](difference_type n) const 
    { return *(*this + n); }
};

template <typename Iter1, typename Iter2>
inline bool operator==
(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
 { return x.base() == y.base(); }

template <typename Iter1, typename Iter2>
inline bool operator!=
(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
 { return x.base() != y.base(); }

template <typename Iter1, typename Iter2>
inline bool operator<
(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
 { return y.base() < x.base(); }

template <typename Iter1, typename Iter2>
inline bool operator>
(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
 { return y.base() > x.base(); }

template <typename Iter1, typename Iter2>
inline bool operator<=
(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
 { return !(y < x); }

template <typename Iter1, typename Iter2>
inline bool operator>=
(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
 { return !(x < y); }

template <typename Iter>
inline reverse_iterator<Iter> operator+
(typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter>& x)
{ return reverse_iterator<Iter>(x.base() - n); }

template <typename Iter1, typename Iter2>
inline typename reverse_iterator<Iter1>::difference_type operator-
(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
 { return y.base() - x.base(); }

//后向插入迭代器
template <typename Container>
class back_insert_iterator :
public iterator<output_iterator_tag, void, void, void, void> {
protected:
    Container* container;
public:
    typedef Container container_type;
    typedef back_insert_iterator<Container> self;

    explicit back_insert_iterator(Container& x) : container(&x) {}
    
    self& operator=(const typename Container::value_type& value) {
        container->push_back(value);
        return *this;
    }

    self& operator*() { return *this; }
    self& operator++() { return *this; }
    self& operator++(int) { return *this; }
};

template <typename Container>
inline back_insert_iterator<Container> back_inserter(Container& x) {
    return back_insert_iterator<Container>(x);
}
//前向插入迭代器
template <typename Container>
class front_insert_iterator : 
public iterator<output_iterator_tag, void, void, void, void> {
protected:
    Container* container;
public:
    typedef Container container_type;
    typedef front_insert_iterator<Container> self;

    explicit front_insert_iterator(Container& x) : container(&x) {}
    
    self& operator=
    (const typename Container::value_type& value) {
        container->push_front(value);
        return *this;
    }

    self& operator*() { return *this; }
    self& operator++() { return *this; }
    self& operator++(int) { return *this; }
};

template <typename Container>
inline front_insert_iterator<Container> front_inserter(Container& x) {
    return front_insert_iterator<Container>(x);
}



} // namespace msl

#endif //__ITERATOR_H__

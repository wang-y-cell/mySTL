#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#include <cstddef>
#include <iostream>
#include "stl_type_traits.h"
#include "utility.h"

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

//我自己定义的,为了区分std和msl
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
    typedef typename iterator_traits<InputIterator>::iterator_category category;
    static_assert(is_msl_iterator_tag<category>::value,
                  "iterator_category must be a msl iterator tag");
    __advance(i, n, category());
}

template <typename ForwardIterator>
inline ForwardIterator next
(ForwardIterator it, 
 typename iterator_traits<ForwardIterator>::difference_type n = 1) {
    advance(it, n);
    return it;
}

template <typename BidirectionalIterator>
inline BidirectionalIterator prev
(BidirectionalIterator it, 
 typename iterator_traits<BidirectionalIterator>::difference_type n = 1) {
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

template<typename Container>
class insert_iterator :
public iterator<output_iterator_tag, void, void, void, void> {
    typedef insert_iterator<Container> self;
    typedef typename Container::iterator iterator;
    typedef typename Container::value_type value_type;
private:
    Container* container;
    iterator iter;
public:
    insert_iterator(Container& x, iterator it) : container(&x), iter(it) {}
    self& operator=(const value_type& value) {
        iter = container->insert(iter, value);
        ++iter;
        return *this;
    }

    self& operator*() { return *this; }
    self& operator++() { return *this; }
    self& operator++(int) { return *this; }
};

template <typename Container,typename Iterator>
inline insert_iterator<Container> inserter(Container& x, Iterator it) {
    return insert_iterator<Container>(x, it);
}



template<typename T, typename CharT = char, typename Traits = std::char_traits<CharT>, typename Distance = ptrdiff_t>
class istream_iterator : public iterator<input_iterator_tag, T, Distance, const T*, const T&> {
    typedef istream_iterator<T, CharT, Traits, Distance> self;
public:
    typedef CharT char_type;
    typedef Traits traits_type;
    typedef std::basic_istream<CharT, Traits> istream_type;

    typedef input_iterator_tag iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef const T* pointer;
    typedef const T& reference;

    friend inline bool operator==(const self& x, const self& y) {
        return x.equal(y);
    }
    friend inline bool operator!=(const self& x, const self& y) {
        return !x.equal(y);
    }

private:
    istream_type* stream;
    T current;
    
    bool equal(const self& y) const 
    { return stream == y.stream; }

    void read(){
        if (stream) {
            if (!(*stream >> current)) {
                stream = nullptr;
            }
        }
    }
public:
    istream_iterator(istream_type& s) : stream(&s) { read(); }
    istream_iterator() : stream(nullptr) {}

    reference operator*() const { return current; }
    pointer operator->() const { return &current; }

    self& operator++(){
        read();
        return *this;
    }

    self& operator++(int){
        self tmp = *this;
        read();
        return tmp;
    }
};

template <typename T, typename CharT = char, typename Traits = std::char_traits<CharT>>
class ostream_iterator : public iterator<output_iterator_tag, void, void, void, void> {
    typedef ostream_iterator<T, CharT, Traits> self;
public:
    typedef CharT char_type;
    typedef Traits traits_type;
    typedef std::basic_ostream<CharT, Traits> ostream_type;

    ostream_iterator(ostream_type& s) : stream(&s), delimiter(nullptr) {}
    ostream_iterator(ostream_type& s, const CharT* c) : stream(&s), delimiter(c) {}
    
    self& operator=(const T& value) {
        *stream << value;
        if (delimiter) *stream << delimiter;
        return *this;
    }
    
    self& operator*() { return *this; }
    self& operator++() { return *this; }
    self& operator++(int) { return *this; }

private:
    ostream_type* stream;
    const CharT* delimiter;
};

#if MYSTL_CPP_VERSION >= 11


// Move Iterator
template <typename Iterator>
class move_iterator {
    typedef Iterator iterator_type;
    typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
    typedef typename iterator_traits<Iterator>::value_type value_type;
    typedef typename iterator_traits<Iterator>::difference_type difference_type;
    typedef Iterator pointer;
    typedef typename iterator_traits<Iterator>::value_type&& reference;
    
    typedef move_iterator<Iterator> self;

protected:
    Iterator current;

public:
    move_iterator() : current() {}
    explicit move_iterator(Iterator i) : current(i) {}
    
    template <typename U>
    move_iterator(const move_iterator<U>& u) : current(u.base()) {}
    
    iterator_type base() const { return current; }
    
    reference operator*() const { return msl::move(*current); }
    pointer operator->() const { return current; }
    
    self& operator++() { ++current; return *this; }
    self operator++(int) { self tmp = *this; ++current; return tmp; }
    self& operator--() { --current; return *this; }
    self operator--(int) { self tmp = *this; --current; return tmp; }
    
    self operator+(difference_type n) const { return self(current + n); }
    self& operator+=(difference_type n) { current += n; return *this; }
    self operator-(difference_type n) const { return self(current - n); }
    self& operator-=(difference_type n) { current -= n; return *this; }
    reference operator[](difference_type n) const { return msl::move(current[n]); }
};

template <typename Iterator>
inline bool operator==(const move_iterator<Iterator>& x, const move_iterator<Iterator>& y) {
    return x.base() == y.base();
}

template <typename Iterator>
inline bool operator!=(const move_iterator<Iterator>& x, const move_iterator<Iterator>& y) {
    return !(x == y);
}

template <typename Iterator>
inline bool operator<(const move_iterator<Iterator>& x, const move_iterator<Iterator>& y) {
    return x.base() < y.base();
}

template <typename Iterator>
inline bool operator>(const move_iterator<Iterator>& x, const move_iterator<Iterator>& y) {
    return y < x;
}

template <typename Iterator>
inline bool operator<=(const move_iterator<Iterator>& x, const move_iterator<Iterator>& y) {
    return !(y < x);
}

template <typename Iterator>
inline bool operator>=(const move_iterator<Iterator>& x, const move_iterator<Iterator>& y) {
    return !(x < y);
}

template <typename Iterator>
inline move_iterator<Iterator> operator+(typename move_iterator<Iterator>::difference_type n, const move_iterator<Iterator>& x) {
    return x + n;
}

template <typename Iterator>
inline typename move_iterator<Iterator>::difference_type operator-(const move_iterator<Iterator>& x, const move_iterator<Iterator>& y) {
    return x.base() - y.base();
}

template <typename Iterator>
inline move_iterator<Iterator> make_move_iterator(Iterator it) {
    return move_iterator<Iterator>(it);
}

// Global begin/end
template <typename Container>
inline auto begin(Container& c) -> decltype(c.begin()) {
    return c.begin();
}

template <typename Container>
inline auto begin(const Container& c) -> decltype(c.begin()) {
    return c.begin();
}

template <typename T, size_t N>
inline T* begin(T (&array)[N]) { //需要引用,不然就会退化成指针,这样函数就不知道数组的大小N了
    return array;
}

template <typename Container>
inline auto end(Container& c) -> decltype(c.end()) {
    return c.end();
}

template <typename Container>
inline auto end(const Container& c) -> decltype(c.end()) {
    return c.end();
}

template <typename T, size_t N>
inline T* end(T (&array)[N]) {
    return array + N;
}

#endif

} // namespace msl

#endif //__ITERATOR_H__
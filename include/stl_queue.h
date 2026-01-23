#ifndef MYSTL_QUEUE_H
#define MYSTL_QUEUE_H

#include "stl_deque.h"
#include "stl_vector.h"
#include "stl_functional.h"
#include "stl_heap.h"
#include "utility.h"

namespace msl {

template <typename T, typename Sequence = msl::deque<T> >
class queue {
    template <typename T1, typename Seq1>
    friend bool operator==(const queue<T1, Seq1>&, const queue<T1, Seq1>&);
    
    template <typename T1, typename Seq1>
    friend bool operator<(const queue<T1, Seq1>&, const queue<T1, Seq1>&);

public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;

protected:
    Sequence c;

public:
    queue() : c() { }
    queue(const Sequence& s) : c(s) { }
    queue(const queue& x) : c(x.c) { }
    queue& operator=(const queue& x) {
        c = x.c;
        return *this;
    }
    #if MYSTL_CPP_VERSION >= 11
    queue(Sequence&& s) : c(msl::move(s)) { }
    queue(queue&& x) : c(msl::move(x.c)) { }
    queue& operator=(queue&& x) {
        c = msl::move(x.c);
        return *this;
    }
    #endif
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    reference front() { return c.front(); }
    const_reference front() const { return c.front(); }
    reference back() { return c.back(); }
    const_reference back() const { return c.back(); }
    void push(const value_type& x) { c.push_back(x); }
    void pop() { c.pop_front(); }
};

template <typename T, typename Sequence>
bool operator==(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
    return x.c == y.c;
}

template <typename T, typename Sequence>
bool operator<(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
    return x.c < y.c;
}

template <typename T, typename Sequence>
bool operator!=(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
    return !(x == y);
}

template <typename T, typename Sequence>
bool operator>(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
    return y < x;
}

template <typename T, typename Sequence>
bool operator<=(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
    return !(y < x);
}

template <typename T, typename Sequence>
bool operator>=(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
    return !(x < y);
}


template<typename T, 
         typename Sequence = msl::vector<T>, 
         typename Compare = msl::less<T>> 
class priority_queue{
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;

protected:
    Sequence c;
    Compare comp;

public:
    explicit priority_queue(const Compare& x = Compare(), const Sequence& s = Sequence()) 
        : c(s), comp(x) {
        msl::make_heap(c.begin(), c.end(), comp);
    }

    template<typename InputIterator>
    priority_queue(InputIterator first, InputIterator last, 
                   const Compare& x = Compare())
                : c(first,last), comp(x) {
                msl::make_heap(c.begin(), c.end(), comp);
    }

    priority_queue(const priority_queue& x) : c(x.c), comp(x.comp) {}

    #if MYSTL_CPP_VERSION >= 11
    priority_queue(priority_queue&& x) noexcept
        : c(msl::move(x.c)), comp(msl::move(x.comp)){ }

    priority_queue& operator=(priority_queue&& x) noexcept {
        c = msl::move(x.c);
        comp = msl::move(x.comp);
        return *this;
    }
    #endif

    priority_queue& operator=(const priority_queue& x) {
        c = x.c;
        comp = x.comp;
        return *this;
    }

    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    const_reference top() const { return c.front(); }
    
    void push(const value_type& x) {
        MYSTL_TRY {
            c.push_back(x);
            msl::push_heap(c.begin(), c.end(), comp);
        }
        MYSTL_UNWIND(c.clear())
    }
    
    void pop() {
        MYSTL_TRY {
            msl::pop_heap(c.begin(), c.end(), comp);
            c.pop_back();
        }
        MYSTL_UNWIND(c.clear())
    }
};




} // namespace msl

#endif // MYSTL_QUEUE_H

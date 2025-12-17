#ifndef MYSTL_QUEUE_H
#define MYSTL_QUEUE_H

#include "deque.h"

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

} // namespace msl

#endif // MYSTL_QUEUE_H

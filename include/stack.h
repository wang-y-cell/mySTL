#ifndef MYSTL_STACK_H
#define MYSTL_STACK_H

#include "deque.h"

namespace msl {

template <typename T, typename Sequence = msl::deque<T> >
class stack {

    template <typename T1, typename Seq1>
    friend bool operator==(const stack<T1, Seq1>&, const stack<T1, Seq1>&);
    
    template <typename T1, typename Seq1>
    friend bool operator<(const stack<T1, Seq1>&, const stack<T1, Seq1>&);

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
    reference top() { return c.back(); }
    const_reference top() const { return c.back(); }
    void push(const value_type& x) { c.push_back(x); }
    void pop() { c.pop_back(); }
};

template <typename T, typename Sequence>
bool operator==(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
    return x.c == y.c;
}

template <typename T, typename Sequence>
bool operator<(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
    return x.c < y.c;
}

template <typename T, typename Sequence>
bool operator!=(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
    return !(x == y);
}

template <typename T, typename Sequence>
bool operator>(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
    return y < x;
}

template <typename T, typename Sequence>
bool operator<=(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
    return !(y < x);
}

template <typename T, typename Sequence>
bool operator>=(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
    return !(x < y);
}

} // namespace msl

#endif // MYSTL_STACK_H

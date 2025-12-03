#ifndef MYSTL_DEQUE_H
#define MYSTL_DEQUE_H

#include "stl_config.h"
#include "iterator.h"
#include <cstddef>

namespace msl {

inline size_t _deque_buf_size(size_t sz) {
    return sz < 512 ? static_cast<size_t>(512 / sz) : static_cast<size_t>(1);
}

template <typename T, typename Ref, typename Ptr>
struct __deque_iterator {
    typedef __deque_iterator<T, T&, T*>              iterator;
    typedef __deque_iterator<T, const T&, const T*>  const_iterator;
    typedef __deque_iterator<T, Ref, Ptr>            self;

    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef T** map_pointer;

    T* cur;
    T* first;
    T* last;
    map_pointer node;

    static size_type buffer_size() { return _deque_buf_size(sizeof(T)); }

    __deque_iterator() : cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {}
    __deque_iterator(T* x, map_pointer y) : cur(x), first(nullptr), last(nullptr), node(nullptr) {
        set_node(y);
    }

    reference operator*() const { return *cur; }
    pointer operator->() const { return &(operator*()); }

    difference_type operator-(const self& x) const {
        return difference_type(buffer_size()) * (node - x.node - 1)
             + (cur - first) + (x.last - x.cur);
    }

    self& operator++() {
        ++cur;
        if (cur == last) { set_node(node + 1); cur = first; }
        return *this;
    }
    self operator++(int) { self tmp = *this; ++*this; return tmp; }

    self& operator--() {
        if (cur == first) { set_node(node - 1); cur = last; }
        --cur;
        return *this;
    }
    self operator--(int) { self tmp = *this; --*this; return tmp; }

    self& operator+=(difference_type n) {
        difference_type offset = n + (cur - first);
        if (offset >= 0 && offset < difference_type(buffer_size())) {
            cur += n;
        } else {
            difference_type node_offset = offset > 0
                ? offset / difference_type(buffer_size())
                : -((difference_type(-offset) - 1) / difference_type(buffer_size())) - 1;
            set_node(node + node_offset);
            cur = first + (offset - node_offset * difference_type(buffer_size()));
        }
        return *this;
    }

    self& operator-=(difference_type n) { return *this += -n; }

    self operator+(difference_type n) const { self tmp = *this; tmp += n; return tmp; }
    self operator-(difference_type n) const { self tmp = *this; tmp -= n; return tmp; }

    reference operator[](difference_type n) const { return *(*this + n); }

    bool operator==(const self& x) const { return cur == x.cur; }
    bool operator!=(const self& x) const { return !(*this == x); }
    bool operator<(const self& x) const { return node == x.node ? (cur < x.cur) : (node < x.node); }
    bool operator>(const self& x) const { return x < *this; }
    bool operator<=(const self& x) const { return !(x < *this); }
    bool operator>=(const self& x) const { return !(*this < x); }

    void set_node(map_pointer new_node) {
        node = new_node;
        first = *new_node;
        last = first + buffer_size();
    }
};

template <typename T, typename Alloc>
class _deque_base {
public:
    typedef Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }

protected:
    typedef __deque_iterator<T, T&, T*> iterator;
    typedef simple_alloc<T, Alloc> data_allocator;
    typedef simple_alloc<T*, Alloc> map_allocator;

    T** map_;
    size_t map_size_;
    iterator start_;
    iterator finish_;

    static size_t buffer_size() { return iterator::buffer_size(); }

    T* allocate_node() { return data_allocator::allocate(buffer_size()); }
    void deallocate_node(T* p) { data_allocator::deallocate(p, buffer_size()); }

    T** allocate_map(size_t n) { return map_allocator::allocate(n); }
    void deallocate_map(T** p, size_t n) { map_allocator::deallocate(p, n); }

    void create_map_and_nodes(size_t num_elements) {
        size_t num_nodes = num_elements / buffer_size() + 1;
        map_size_ = num_nodes + 2;
        if (map_size_ < 8) map_size_ = 8;
        map_ = allocate_map(map_size_);
        T** nstart = map_ + (map_size_ - num_nodes) / 2;
        T** nfinish = nstart + num_nodes - 1;
        for (T** n = nstart; n <= nfinish; ++n) *n = allocate_node();
        start_.set_node(nstart);
        finish_.set_node(nfinish);
        start_.cur = start_.first;
        finish_.cur = finish_.first + (num_elements % buffer_size());
    }

public:
    _deque_base(const allocator_type&) : map_(0), map_size_(0), start_(), finish_() {}
    ~_deque_base() {
        if (map_) {
            for (T** n = start_.node; n <= finish_.node; ++n) {
                if (*n) deallocate_node(*n);
            }
            deallocate_map(map_, map_size_);
        }
    }
};

} // namespace msl

#endif // MYSTL_DEQUE_H


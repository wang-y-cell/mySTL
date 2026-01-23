#ifndef MYSTL_MAP_H
#define MYSTL_MAP_H

#include "stl_alloc.h"
#include "stl_tree.h"
#include "stl_pair.h"
#include "stl_functional.h"


namespace msl {

template <class Key, class T, class Compare = less<Key>, class Alloc = alloc>
class map {
public:
    // typedefs:
    typedef Key key_type;
    typedef T data_type;
    typedef T mapped_type;
    typedef pair<const Key, T> value_type;
    typedef Compare key_compare;

    class value_compare {
        friend class map<Key, T, Compare, Alloc>;
    protected:
        Compare comp;
        value_compare(Compare c) : comp(c) {}
    public:
        bool operator()(const value_type& x, const value_type& y) const {
            return comp(x.first, y.first);
        }
    };

private:
    typedef rb_tree<key_type, value_type, select1st<value_type>, key_compare, Alloc> rep_type;
    rep_type t;  
public:
    typedef typename rep_type::pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::iterator iterator;
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::reverse_iterator reverse_iterator;
    typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    // allocation/deallocation
    map() : t(Compare()) {}
    explicit map(const Compare& comp) : t(comp) {}

    template <class InputIterator>
    map(InputIterator first, InputIterator last)
        : t(Compare()) { t.insert_unique(first, last); }

    template <class InputIterator>
    map(InputIterator first, InputIterator last, const Compare& comp)
        : t(comp) { t.insert_unique(first, last); }

    map(const map<Key, T, Compare, Alloc>& x) : t(x.t) {}
    map<Key, T, Compare, Alloc>& operator=(const map<Key, T, Compare, Alloc>& x) {
        t = x.t;
        return *this;
    }

    // accessors:
    key_compare key_comp() const { return t.key_comp(); }
    value_compare value_comp() const { return value_compare(t.key_comp()); }
    iterator begin() { return t.begin(); }
    const_iterator begin() const { return t.begin(); }
    iterator end() { return t.end(); }
    const_iterator end() const { return t.end(); }
    #if MYSTL_CPP_VERSION >= 11
    const_iterator cbegin() const { return t.cbegin(); }
    const_iterator cend() const { return t.cend(); }
    #endif

    reverse_iterator rbegin() { return t.rbegin(); }
    const_reverse_iterator rbegin() const { return t.rbegin(); }
    reverse_iterator rend() { return t.rend(); }
    const_reverse_iterator rend() const { return t.rend(); }
    #if MYSTL_CPP_VERSION >= 11
    const_reverse_iterator crbegin() const { return t.crbegin(); }
    const_reverse_iterator crend() const { return t.crend(); }
    #endif

    bool empty() const { return t.empty(); }
    size_type size() const { return t.size(); }
    size_type max_size() const { return t.max_size(); }
    void swap(map<Key, T, Compare, Alloc>& x) { t.swap(x.t); }

    // insert/erase
    typedef pair<iterator, bool> pair_iterator_bool; 
    pair<iterator, bool> insert(const value_type& x) {
        return t.insert_unique(x);
    }
    iterator insert(iterator position, const value_type& x) {
        return t.insert_unique(position, x);
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        t.insert_unique(first, last);
    }

    void erase(iterator position) {
        t.erase(position);
    }
    size_type erase(const key_type& x) {
        return t.erase(x);
    }
    void erase(iterator first, iterator last) {
        t.erase(first, last);
    }
    void clear() { t.clear(); }

    // map operations:
    iterator find(const key_type& x) { return t.find(x); }
    const_iterator find(const key_type& x) const { return t.find(x); }
    size_type count(const key_type& x) const { return t.count(x); }
    iterator lower_bound(const key_type& x) { return t.lower_bound(x); }
    const_iterator lower_bound(const key_type& x) const { return t.lower_bound(x); }
    iterator upper_bound(const key_type& x) { return t.upper_bound(x); }
    const_iterator upper_bound(const key_type& x) const { return t.upper_bound(x); }
    pair<iterator, iterator> equal_range(const key_type& x) {
        return t.equal_range(x);
    }
    pair<const_iterator, const_iterator> equal_range(const key_type& x) const {
        return t.equal_range(x);
    }

    // operator[]
    T& operator[](const key_type& k) {
        return (*((insert(value_type(k, T()))).first)).second;
    }

};

} // namespace msl

#endif

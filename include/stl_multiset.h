#ifndef MYSTL_MULTISET_H
#define MYSTL_MULTISET_H

#include "stl_alloc.h"
#include "stl_tree.h"
#include "stl_functional.h"

namespace msl {


template <class Key, class Compare = less<Key>, class Alloc = alloc>
class multiset {
public:
    // typedefs:
    typedef Key key_type;
    typedef Key value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;
private:
    typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;
    rep_type t;  // rb_tree representing set
public:
    typedef typename rep_type::const_pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::const_reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::const_iterator iterator;
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::const_reverse_iterator reverse_iterator;
    typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    // allocation/deallocation
    multiset() : t(Compare()) {}
    explicit multiset(const Compare& comp) : t(comp) {}

    template <class InputIterator>
    multiset(InputIterator first, InputIterator last)
        : t(Compare()) { t.insert_equal(first, last); }

    template <class InputIterator>
    multiset(InputIterator first, InputIterator last, const Compare& comp)
        : t(comp) { t.insert_equal(first, last); }

    multiset(const multiset<Key, Compare, Alloc>& x) : t(x.t) {}
    multiset<Key, Compare, Alloc>& operator=(const multiset<Key, Compare, Alloc>& x) {
        t = x.t;
        return *this;
    }

    // accessors:
    key_compare key_comp() const { return t.key_comp(); }
    value_compare value_comp() const { return t.key_comp(); }
    iterator begin() const { return t.begin(); }
    iterator end() const { return t.end(); }
    #if MYSTL_CPP_VERSION >= 11
    const_iterator cbegin() const { return t.cbegin(); }
    const_iterator cend() const { return t.cend(); }
    #endif

    reverse_iterator rbegin() const { return t.rbegin(); }
    reverse_iterator rend() const { return t.rend(); }
    
    #if MYSTL_CPP_VERSION >= 11
    const_reverse_iterator crbegin() const { return t.crbegin(); }
    const_reverse_iterator crend() const { return t.crend(); }
    #endif
    
    bool empty() const { return t.empty(); }
    size_type size() const { return t.size(); }
    size_type max_size() const { return t.max_size(); }
    void swap(multiset<Key, Compare, Alloc>& x) { t.swap(x.t); }

    // insert/erase
    iterator insert(const value_type& x) {
        iterator p = t.insert_equal(x);
        return p;
    }
    iterator insert(iterator position, const value_type& x) {
        typedef typename rep_type::iterator rep_iterator;
        return t.insert_equal(rep_iterator(position.node), x);
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        t.insert_equal(first, last);
    }

    void erase(iterator position) {
        typedef typename rep_type::iterator rep_iterator;
        t.erase(rep_iterator(position.node));
    }
    size_type erase(const key_type& x) {
        return t.erase(x);
    }
    void erase(iterator first, iterator last) {
        typedef typename rep_type::iterator rep_iterator;
        t.erase(rep_iterator(first.node), rep_iterator(last.node));
    }
    void clear() { t.clear(); }

    // set operations:
    iterator find(const key_type& x) const { return t.find(x); }
    size_type count(const key_type& x) const { return t.count(x); }
    iterator lower_bound(const key_type& x) const { return t.lower_bound(x); }
    iterator upper_bound(const key_type& x) const { return t.upper_bound(x); }
    pair<iterator, iterator> equal_range(const key_type& x) const {
        return t.equal_range(x);
    }

    
    

};

} // namespace msl



#endif  // STL_MULTISET_Hq
#ifndef STL_HASH_MULTISET_H
#define STL_HASH_MULTISET_H

#include "stl_alloc.h"
#include "stl_hash_fun.h"
#include "stl_functional.h"
#include "stl_hashtable.h"

namespace msl {

template <class Value, class HashFcn, class EqualKey, class Alloc>
class hash_multiset;

template <class Value, class HashFcn, class EqualKey, class Alloc>
bool operator== (const hash_multiset<Value, HashFcn, EqualKey, Alloc>& hs1, const hash_multiset<Value, HashFcn, EqualKey, Alloc>& hs2);

template <class Value, class HashFcn = hash<Value>, class EqualKey = equal_to<Value>, class Alloc = alloc>
class hash_multiset {
private:
    typedef hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc> ht;
    ht rep;

public:
    typedef typename ht::key_type key_type;
    typedef typename ht::value_type value_type;
    typedef typename ht::hasher hasher;
    typedef typename ht::key_equal key_equal;
    
    typedef typename ht::size_type size_type;
    typedef typename ht::difference_type difference_type;
    typedef typename ht::const_pointer pointer;
    typedef typename ht::const_pointer const_pointer;
    typedef typename ht::const_reference reference;
    typedef typename ht::const_reference const_reference;

    typedef typename ht::const_iterator iterator;
    typedef typename ht::const_iterator const_iterator;

    hasher hash_funct() const { return rep.hash_funct(); }
    key_equal key_eq() const { return rep.key_eq(); }

public:
    hash_multiset() : rep(100, hasher(), key_equal()) {}
    explicit hash_multiset(size_type n) : rep(n, hasher(), key_equal()) {}
    hash_multiset(size_type n, const hasher& hf) : rep(n, hf, key_equal()) {}
    hash_multiset(size_type n, const hasher& hf, const key_equal& eql) : rep(n, hf, eql) {}

    template <class InputIterator>
    hash_multiset(InputIterator first, InputIterator last) 
        : rep(100, hasher(), key_equal()) {
        rep.insert_equal(first, last);
    }
    
    template <class InputIterator>
    hash_multiset(InputIterator first, InputIterator last, size_type n)
        : rep(n, hasher(), key_equal()) {
        rep.insert_equal(first, last);
    }

    template <class InputIterator>
    hash_multiset(InputIterator first, InputIterator last, size_type n, const hasher& hf)
        : rep(n, hf, key_equal()) {
        rep.insert_equal(first, last);
    }

    size_type size() const { return rep.size(); }
    size_type max_size() const { return rep.max_size(); }
    bool empty() const { return rep.empty(); }
    
    void swap(hash_multiset& hs) { rep.swap(hs.rep); }
    
    friend bool operator== <>(const hash_multiset& hs1, const hash_multiset& hs2);

    iterator begin() const { return rep.begin(); }
    iterator end() const { return rep.end(); }

    iterator insert(const value_type& obj) {
        return rep.insert_equal(obj);
    }
    
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        rep.insert_equal(first, last);
    }
    
    void insert(const value_type* first, const value_type* last) {
        rep.insert_equal(first, last);
    }
    
    iterator insert_noresize(const value_type& obj) {
        return rep.insert_equal_noresize(obj);
    }

    iterator find(const key_type& key) const { return rep.find(key); }

    size_type count(const key_type& key) const { return rep.count(key); }
    
    size_type erase(const key_type& key) { return rep.erase(key); }
    
    void erase(iterator it) { 
        typedef typename ht::iterator mutable_iterator;
        if (it.cur)
            rep.erase(mutable_iterator(it.cur, const_cast<ht*>(&rep))); 
    }
    
    void erase(iterator first, iterator last) { 
        typedef typename ht::iterator mutable_iterator;
        if (first == last) return;
        // 注意：这里需要处理 const_iterator 到 iterator 的转换
        // 为了安全起见，我们还是循环删除吧，虽然效率低一点
        while (first != last) {
            iterator temp = first;
            ++first;
            erase(temp);
        }
    }
    
    void clear() { rep.clear(); }

public:
    void resize(size_type hint) { rep.resize(hint); }
    size_type bucket_count() const { return rep.bucket_count(); }
    size_type max_bucket_count() const { return rep.max_bucket_count(); }
    size_type elems_in_bucket(size_type n) const { return rep.elems_in_bucket(n); }
};

template <class Value, class HashFcn, class EqualKey, class Alloc>
inline void swap(hash_multiset<Value, HashFcn, EqualKey, Alloc>& hs1,
                 hash_multiset<Value, HashFcn, EqualKey, Alloc>& hs2) {
    hs1.swap(hs2);
}

template <class Value, class HashFcn, class EqualKey, class Alloc>
bool operator== (const hash_multiset<Value, HashFcn, EqualKey, Alloc>& hs1, 
                 const hash_multiset<Value, HashFcn, EqualKey, Alloc>& hs2) {
    return hs1.rep == hs2.rep;
}

} // namespace msl

#endif

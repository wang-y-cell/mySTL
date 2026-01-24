#ifndef STL_HASHTABLE_H
#define STL_HASHTABLE_H

#include "stl_alloc.h"
#include "stl_iterator.h"

namespace msl{

//每个桶中的节点
template<typename value>
struct hash_node{
    hash_node* next;
    value val;
};
//声明
template<typename value, typename key, typename hashfcn,
         typename extractkey,class equalkey, typename alloc = alloc>
class hashtable;

template<typename value, typename key, typename hashfcn,
         typename extractkey,class equalkey, typename alloc>
struct hashtable_iterator;

template<typename value, typename key, typename hashfcn,
         typename extractkey,class equalkey, typename alloc>
struct hashtable_const_iterator;

template<typename value, typename key, typename hashfcn,
         typename extractkey,class equalkey, typename alloc>
struct hashtable_iterator{
    typedef hashtable<value, key, hashfcn, extractkey, alloc> hashtable;
    typedef hash_node<value> node;
    typedef hashtable_iterator<value,key,hashfcn,extractkey,equalkey,alloc> 
        iterator;
    typedef hashtable_const_iterator<value,key,hashfcn,extractkey,equalkey,alloc> 
        const_iterator;

    typedef forward_iterator_tag iterator_category;
    typedef value value_type;
    typedef ptrdiff_t difference_type;
    typedef sise_t size_type;
    typedef value& reference;
    typedef value* pointer;

    node* cur;
    hashtable* ht;

    void increment(){ //operator++ 的辅助函数
        const node* old = cur;
        cur = cur->next;
        if(!cur){
            size_type bucket = ht->bkt_num(old->val);
            while(!cur && ++bucket < ht ->buckets.size())
                cur = ht->buckets[bucket];
        }
    }

    hashtable_iterator(node* n, hashtable* h) : cur(n), ht(h) {}
    hashtable_iterator() : cur(0), ht(0) {}
    reference operator*() const { return cur->val;}
    pointer operator->() const { return &(operator*())}
    iterator& operator++(){
        increment();
        return *this;
    }
    iterator& operator++(int){
        hashtable_iterator tmp = *this;
        ++*this;
        return tmp;
    }
    bool operator=(const iterator& it) const {
        return cur == it.cur;
    }
    bool operator!=(const iterator& it) const {
        return cur != it.cur;
    }
};

template<typename value, typename key, typename hashfcn,
         typename extractkey,class equalkey, typename alloc>
struct hashtable_const_iterator{
    typedef hashtable<value, key, hashfcn, extractkey, alloc> hashtable;
    typedef hash_node<value> node;
    typedef hashtable_iterator<value,key,hashfcn,extractkey,equalkey,alloc> 
        iterator;
    typedef hashtable_const_iterator<value,key,hashfcn,extractkey,equalkey,alloc> 
        const_iterator;

    typedef forward_iterator_tag iterator_category;
    typedef value value_type;
    typedef ptrdiff_t difference_type;
    typedef sise_t size_type;
    typedef value& reference;
    typedef value* pointer;

    node* cur;
    hashtable* ht;

    void increment(){
        const node* old = cur;
        cur = cur->next;
        if(!cur){
            size_type bucket = ht->bkt_num(old->val);
            while(!cur && ++bucket < ht ->buckets.size())
                cur = ht->buckets[bucket];
        }
    }

    hashtable_const_iterator(node* n, hashtable* h) : cur(n), ht(h) {}
    hashtable_const_iterator() : cur(0), ht(0) {}
    reference operator*() const { return cur->val;}
    pointer operator->() const { return &(operator*())}
    iterator& operator++(){
        increment();
        return *this;
    }
    iterator& operator++(int){
        hashtable_iterator tmp = *this;
        increment();
        return tmp;
    }
    bool operator=(const const_iterator& it) const {
        return cur == it.cur;
    }
    bool operator!=(const const_iterator& it) const {
        return cur != it.cur;
    }
};




}   // namespace msl


#endif // STL_HASHTABLE_H
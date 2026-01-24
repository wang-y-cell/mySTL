#ifndef STL_HASHTABLE_H
#define STL_HASHTABLE_H

#include "stl_alloc.h"
#include "stl_iterator.h"
#include "stl_algo.h"

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
    typedef hashtable<value, key, hashfcn, extractkey, equalkey, alloc> hashtable;
    typedef hash_node<value> node;
    typedef hashtable_iterator<value,key,hashfcn,extractkey,equalkey,alloc> 
        iterator;
    typedef hashtable_const_iterator<value,key,hashfcn,extractkey,equalkey,alloc> 
        const_iterator;

    typedef forward_iterator_tag iterator_category;
    typedef value value_type;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
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
    pointer operator->() const { return &(operator*()); }
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
    typedef size_t size_type;
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
    pointer operator->() const { return &(operator*()); }
    const_iterator& operator++(){
        increment();
        return *this;
    }
    const_iterator& operator++(int){
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

static const int __stl_num_primes = 28;

static const unsigned long __stl_prime_list[__stl_num_primes] =
{
  53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
  1610612741ul, 3221225473ul, 4294967291ul
};

//寻找第一个大于等于n的素数
inline unsigned long __stl_next_prime(unsigned long n){
    const unsigned long* first = __stl_prime_list;
    const unsigned long* last = __stl_prime_list + __stl_num_primes;
    const unsigned long* pos = msl::lower_bound(first, last, n);
    return pos == last ? *(last - 1) : *pos;
}

/**
 * @brief 哈希表
 * 
 * @tparam value 元素类型
 * @tparam key 键类型
 * @tparam hashfcn 哈希函数类型
 * @tparam extractkey 提取键的函数类型
 * @tparam equalkey 键相等判断函数类型
 * @tparam alloc 分配器类型
 */
template<typename value, typename key, typename hashfcn,
         typename extractkey,class equalkey, typename alloc>
class hashtable {
    typedef key key_type;
    typedef value value_type;
    typedef  hashfcn hasher;

    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef const value_type* const_pointer;

private:
    typedef hash_node<value> node;
public:
    typedef alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }
private:
    typedef simple_alloc<node, allocator_type> node_allocator;
    node* get_node(){ return node_allocator::allocate(1); } 
    void put_node(node* n){ node_allocator::deallocate(n); }

private:
    hasher hash;
    equalkey equals;
    extractkey get_key;
    vector<node*,alloc> buckets;
    size_type num_elements;

public:
      typedef hashtable_iterator<value,key,hashfcn,extractkey,equalkey,alloc>
          iterator;
  typedef hashtable_const_iterator<value,key,hashfcn,extractkey,equalkey,alloc>
          const_iterator;

  friend struct
  hashtable_iterator<value,key,hashfcn,extractkey,equalkey,alloc>;
  friend struct
  hashtable_const_iterator<value,key,hashfcn,extractkey,equalkey,alloc>;

    size_type bucket_count() const { return buckets.size(); }
    size_type max_bucket_count() const 
    { return __stl_prime_list[__stl_num_primes - 1]; }

    node* new_node(const value_type& val) {
        node* n = get_node();
        n->next = 0;
        MYSTL_TRY{
            construct(&n->val, val);
            return n;
        }
        MYSTL_UNWIND(node_allocator::deallocate(n));
    }

    void delete_node(node* n){
        destroy(&n->val);
        node_allocator::deallocate(n);
    }

private:
    size_type next_size(size_type n) const {
        return __stl_next_prime(n);
    }

    size_type initiallize_buckets(size_type n){
        const size_type n_buckets = next_size(n);
        buckets.reserve(n_buckets);
        buckets.insert(buckets.end(), n_buckets, (node*)0);
        return n_buckets;
    }

    void resize(size_type num_elements_hint);
    pair<iterator, bool> insert_unique_noresize(const value_type& val);
    iterator insert_equal_noresize(const value_type& val);

public:
    hashtable(size_type n,
              const hashfcn& hf,
              const equalkey& eql)
        : hash(hf), equals(eql), get_key(extractkey()),
          num_elements(0)
    {
        initiallize_buckets(n);
    }

    hashtable(size_type n,
              const hashfcn& hf,
              const equalkey& eql,
              const extractkey& getk)
        : hash(hf), equals(eql), get_key(getk),
          num_elements(0)
    {
        initiallize_buckets(n);
    }

    /**
     * @brief 插入唯一元素
     * 
     * @param val 要插入的元素
     * @return pair<iterator, bool> 插入结果, 第一个元素是迭代器, 第二个元素是是否插入成功
     */
    pair<iterator, bool> insert_unique(const value_type& val) {
        resize(num_elements + 1);
        return insert_unique_noresize(val);
    }
    
    /**
     * @brief 插入相等元素
     * 
     * @param val 要插入的元素
     * @return iterator 插入位置的迭代器
     */
    iterator insert_equal(const value_type& val) {
        resize(num_elements + 1);
        return insert_equal_noresize(val);
    }

private:
    //以下四个函数用来索引bucket的序号
    //注意: 前两个函数的参数是对象,后两个参数的参数是键
    size_type bkt_num(const value_type& obj,size_t n) const {
        return bkt_num_key(get_key(obj),n);
    }

    size_type bkt_num(const value_type& obj) const {
        return bkt_num_key(get_key(obj));
    }

    size_type bkt_num_key(const key_type& k) const {
        return bkt_num_key(k,bucket_count());
    }

    size_type bkt_num_key(const key_type& k,size_t n) const {
        return hash(k) % n;
    }

};

template<typename v, typename k, 
         typename hf, typename ex, 
         typename eq, typename a>
    void hashtable<v,k,hf,ex,eq,a>::resize(size_type num_elements_hint) {
        const size_type old_size = bucket_count();
        if (num_elements_hint > old_size) {
            const size_type new_size = next_size(num_elements_hint);
            if(new_size > old_size) {
                vector<node*,alloc> tmp(new_size, (node*)0);
                for(size_type bucket = 0; bucket < old_size; ++bucket) {
                    node* first = buckets[bucket];
                    while(first) {
                        size_type new_idx = bkt_num(first->val,new_size);
                        buckets[bucket] = first->next;
                        first->next = tmp[new_idx];
                        tmp[new_idx] = first;
                        first = buckets[bucket];
                    }
                }
                buckets.swap(tmp);
            }
        }
    }


template<typename v, typename k, 
         typename hf, typename ex, 
         typename eq, typename a>
pair<typename hashtable<v,k,hf,ex,eq,a>::iterator,bool>
hashtable<v,k,hf,ex,eq,a>::insert_unique_noresize(const value_type& obj) {
    const size_type bucket = bkt_num(obj);
    node* first = buckets[bucket];
    for(node* cur = first; cur; cur = cur->next) {
        if(equals(get_key(cur->val), get_key(obj)))
            return pair<iterator,bool>(iterator(cur,this), false);
    }
    node* tmp = new_node(obj);
    tmp->next = first;
    buckets[bucket] = tmp;
    ++num_elements;
    return pair<iterator,bool>(iterator(tmp,this), true);
}

template<typename v, typename k, 
         typename hf, typename ex, 
         typename eq, typename a>
typename hashtable<v,k,hf,ex,eq,a>::iterator
hashtable<v,k,hf,ex,eq,a>::insert_equal_noresize(const value_type& obj) {
    const size_type bucket = bkt_num(obj);
    node* first = buckets[bucket];
    for(node* cur = first; cur; cur = cur->next) {
        if(equals(get_key(cur->val), get_key(obj))){
            node* tmp = new_node(obj);
            tmp->next = cur->next;
            cur->next = tmp;
            ++num_elements;
            return iterator(tmp,this);
        }
    }
    node* tmp = new_node(obj);
    tmp->next = first;
    buckets[bucket] = tmp;
    ++num_elements;
    return iterator(tmp,this);
}



}   // namespace msl


#endif // STL_HASHTABLE_H
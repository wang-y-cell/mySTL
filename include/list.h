#ifndef MYSTL_LIST_H
#define MYSTL_LIST_H

#include "stl_config.h"
#include "iterator.h"
#include "memory.h"
#include <cstddef>
#include <iostream>

namespace msl {

// List node structure
struct __node_base{
    __node_base* prev;
    __node_base* next;
};

template<typename T>
struct __list_node : public __node_base{
    T data;
};

struct _iterator_base{
    // don't need typename T or Ref or Ptr,put in base
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef bidirectional_iterator_tag iterator_category;

    __node_base* node;

    _iterator_base(__node_base* n = nullptr) : node(n) {}

    bool operator==(const _iterator_base& x) const { return node == x.node; }
    bool operator!=(const _iterator_base& x) const { return node != x.node; }

    void increment() { node = node->next; }
    void decrement() { node = node->prev; }

};

template<typename T,typename Ref,typename Ptr>
struct __list_iterator : _iterator_base{
    //need typename T or Ref or Ptr
    typedef __list_iterator<T, T&, T*>              iterator;
    typedef __list_iterator<T, const T&, const T*>  const_iterator;
    typedef __list_iterator<T, Ref, Ptr>            self;

    typedef T value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef __list_node<T> list_node;

    __list_iterator(){ }
    __list_iterator(list_node* p) : _iterator_base(p) { }
    __list_iterator(iterator& x) : _iterator_base(x.node) { }

    reference operator*() const {return ((list_node*)node)->data;}
    pointer operator->() const {return &operator*();}

    self& operator++() {
        increment();
        return *this;
    }
    self operator++(int) {
        self tmp = *this;
        increment();
        return tmp;
    }
    self& operator--() {
        decrement();
        return *this;
    }
    self operator--(int) {
        self tmp = *this;
        decrement();
        return tmp;
    }

};

template<typename T,typename Alloc>
class list_base{
public:
    typedef Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }

    list_base(const allocator_type&){
        node_ = get_node();
        node_->next = node_;
        node_->prev = node_;
    }

    ~list_base(){
        clear();
        put_node(node_);
    }

    void clear();
protected:
    typedef simple_alloc<__list_node<T>,Alloc> data_allocator;
    __list_node<T>* get_node() { return data_allocator::allocate(); }
    void put_node(__list_node<T>* p) { data_allocator::deallocate(p); }
protected:
    __list_node<T>* node_;
};

template<typename T,typename Alloc>
void list_base<T,Alloc>::clear(){
    __list_node<T>* cur = static_cast<__list_node<T>*>(node_->next);
    while (cur != (__list_node<T>*)node_) {
        __list_node<T>* tmp = cur;
        cur = static_cast<__list_node<T>*>(cur->next);
        destroy(&tmp->data);
        put_node(tmp);
    }
    node_->next = node_;
    node_->prev = node_;
}

// List class
template <typename T, typename Alloc = alloc>
class list : protected list_base<T, Alloc>{
    typedef list_base<T,Alloc> base;
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef __list_node<T>* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef typename base::allocator_type allocator_type;
    allocator_type get_allocator() const { return base::get_allocator(); }

    typedef __list_iterator<T, T&, T*>             iterator;
    typedef __list_iterator<T, const T&, const T*> const_iterator;
    // Reverse iterators
    typedef msl::reverse_iterator<iterator> reverse_iterator;
    typedef msl::reverse_iterator<const_iterator> const_reverse_iterator;
    
protected:
    using base::node_;
    using base::get_node;
    using base::put_node;


    link_type create_node(const_reference val = T()) {
        link_type p = get_node();
        MYSTL_TRY{
            construct(&p->data, val);
            return p;
        }
        MYSTL_CATCH_ALL{
            put_node(p);
            MYSTL_RETHROW;
        }
    }

    void destroy_node(link_type p) {
        destroy(&p->data);
        put_node(p);
    }

public:
    explicit list(const allocator_type& a = allocator_type()) : base(a) {}
    iterator begin()             { return static_cast<link_type>(node_->next); }
    const_iterator begin() const { return static_cast<const link_type>(node_->next); }
    iterator end()               { return static_cast<link_type>(node_); }
    const_iterator end() const   { return static_cast<const link_type>(node_); }
    #if MYSTL_CPP_VERSION >= 11
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }
    #endif


    reverse_iterator rbegin()             { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator rend()               { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const   { return const_reverse_iterator(begin()); }
    #if MYSTL_CPP_VERSION >= 11
    const_iterator crbegin() const {return end(); }
    const_iterator crend() const { return begin(); }
    #endif

    bool empty() const { return begin() == end(); }
    size_type size() const {return distance(begin(),end());}
    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
    reference back() { return *--end(); }
    const_reference back() const { return *--end(); }

    void insert(iterator pos, const_reference val) {
        link_type p = create_node(val);
        p->next = pos.node;
        p->prev = pos.node->prev;
        pos.node->prev->next = p;
        pos.node->prev = p;
    }

    void push_back(const_reference val) {
        insert(end(), val);
    }

    void push_front(const_reference val) {
        insert(begin(), val);
    }

    iterator erase(iterator pos){
        link_type p = static_cast<link_type>(pos.node);
        link_type next = static_cast<link_type>(p->next);
        p->prev->next = p->next;
        p->next->prev = p->prev;
        destroy_node(p);
        return iterator(next);
    }

    void clear() { base::clear(); }

    void remove(const_reference val) {
        iterator first = begin();
        iterator last = end();
        while(first != last) {
            if (*first == val) {
                first = erase(first);
            } else {
                ++first;
            }
        }
    }

    void unique(){
        iterator first = begin();
        iterator last = end();
        if (first == last) return;
        iterator next = first;
        while (++next != last) {
            if (*first == *next)erase(next);
            else first = next;
            next = first;
        }
    }


    
};

} // namespace msl

#endif // MYSTL_LIST_H                          

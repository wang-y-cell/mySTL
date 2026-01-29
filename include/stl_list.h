#ifndef MYSTL_LIST_H
#define MYSTL_LIST_H

#include "stl_config.h"
#include "stl_iterator.h"
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
    __list_iterator(const iterator& x) : _iterator_base(x.node) { }

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
    /**
     * @brief 获取分配器
     * 
     * @return allocator_type 
     */
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
    /**
     * @brief 获取分配器
     * 
     * @return allocator_type 
     */
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

    #if MYSTL_CPP_VERSION >= 11
    template <typename... Args>
    link_type create_node(Args&&... args) {
        link_type p = get_node();
        MYSTL_TRY{
            construct(&p->data, msl::forward<Args>(args)...);
            return p;
        }
        MYSTL_CATCH_ALL{
            put_node(p);
            MYSTL_RETHROW;
        }
    }
    #endif

    void destroy_node(link_type p) {
        destroy(&p->data);
        put_node(p);
    }


    /**
     * @brief 将[first, last)区间的元素移动到pos位置之前
     * 
     * @param pos 位置迭代器
     * @param first 迭代器开始
     * @param last 迭代器结束
     */
    void transfer(iterator pos, iterator first, iterator last) {
        if (pos != last) {
            last.node->prev->next = pos.node;
            first.node->prev->next = last.node;
            pos.node->prev->next = first.node;
            link_type tmp = static_cast<link_type>(pos.node->prev);
            pos.node->prev = last.node->prev;
            last.node->prev = first.node->prev;
            first.node->prev = tmp;
        }
    }

public:
    /**
     * @brief 构造函数
     * 
     * @param a 分配器
     */
    explicit list(const allocator_type& a = allocator_type()) : base(a) {}

    /**
     * @brief 构造函数
     * 
     * @param n 元素数量
     * @param value 元素值
     */
    list(size_type n, const_reference value) : base(allocator_type()) {
        insert(begin(), n, value);
    }

    /**
     * @brief 构造函数
     * 
     * @param first 迭代器开始
     * @param last 迭代器结束
     */
    template <typename InputIterator, typename = typename msl::enable_if<!msl::is_integer<InputIterator>::Integral::value>::type>
    list(InputIterator first, InputIterator last) : base(allocator_type()) {
        insert(begin(), first, last);
    }
    
    list(const list& x) : base(x.get_allocator()) {
        insert(begin(), x.begin(), x.end());
    }

    #if MYSTL_CPP_VERSION >= 11
    list(list&& x) : base(x.get_allocator()) {
        msl::swap(node_, x.node_);
    }

    list(std::initializer_list<T> ilist) : base(allocator_type()) {
        insert(begin(), ilist.begin(), ilist.end());
    }
    #endif
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
    const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }
    #endif

    bool empty() const { return begin() == end(); }
    size_type size() const {return distance(begin(),end());}
    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
    reference back() { return *--end(); }
    const_reference back() const { return *--end(); }
    void swap(list& x){msl::swap(node_, x.node_);}

    list& operator=(const list& x) {
        if (this != &x) {
            assign(x.begin(), x.end());
        }
        return *this;
    }

    #if MYSTL_CPP_VERSION >= 11
    list& operator=(list&& x) {
        clear();
        msl::swap(node_, x.node_);
        return *this;
    }

    list& operator=(std::initializer_list<T> ilist) {
        assign(ilist.begin(), ilist.end());
        return *this;
    }
    #endif
    
    /**
     * @brief 将链表赋值为n个value
     * 
     * @param n 元素数量
     * @param value 元素值
     */
    void assign(size_type n, const_reference value) {
        iterator first = begin();
        iterator last = end();
        for (; n > 0 && first != last; --n, ++first) {
            *first = value;
        }
        if (n > 0) {
            insert(end(), n, value);
        } else {
            erase(first, last);
        }
    }

    /**
     * @brief 将链表赋值为迭代器范围[first, last)的元素
     * 
     * @param first 迭代器开始
     * @param last 迭代器结束
     */
    template <typename InputIterator, typename = typename msl::enable_if<!msl::is_integer<InputIterator>::Integral::value>::type>
    void assign(InputIterator first, InputIterator last) {
        iterator start = begin();
        iterator finish = end();
        while (start != finish && first != last) {
            *start = *first;
            ++start;
            ++first;
        }
        if (first == last) {
            erase(start, finish);
        } else {
            insert(finish, first, last);
        }
    }

    #if MYSTL_CPP_VERSION >= 11
    /**
     * @brief 将链表赋值为initializer_list ilist的元素
     * 
     * @param ilist initializer_list
     */
    void assign(std::initializer_list<T> ilist) {
        assign(ilist.begin(), ilist.end());
    }
    #endif
    
    /**
     * @brief 在迭代器pos前插入value
     * 
     * @param pos 迭代器位置
     * @param val 元素值
     * @return iterator 插入位置迭代器
     */
    iterator insert(iterator pos, const_reference val) {
        link_type p = create_node(val);
        p->next = pos.node;
        p->prev = pos.node->prev;
        pos.node->prev->next = p;
        pos.node->prev = p;
        return iterator(p);
    }

    /**
     * @brief 在迭代器pos前插入n个value
     * 
     * @param pos 迭代器位置
     * @param n 元素数量
     * @param value 元素值
     */
    void insert(iterator pos, size_type n, const_reference value) {
        for (; n > 0; --n) {
            insert(pos, value);
        }
    }
    
    /**
     * @brief 在迭代器pos前插入迭代器范围[first, last)的元素
     * 
     * @param pos 迭代器位置
     * @param first 迭代器开始
     * @param last 迭代器结束
     */
    template <typename InputIterator, 
    typename = typename msl::enable_if<!msl::is_integer<InputIterator>::Integral::value>::type>
    void insert(iterator pos, InputIterator first, InputIterator last) {
        for (; first != last; ++first) {
            insert(pos, *first);
        }
    }

    /**
     * @brief 在迭代器pos前插入value的移动版本
     * 
     * @param pos 迭代器位置
     * @param value 元素值
     * @return iterator 插入位置迭代器
     */
    #if MYSTL_CPP_VERSION >= 11
    iterator insert(iterator pos, value_type&& value) {
        link_type p = create_node(msl::move(value));
        p->next = pos.node;
        p->prev = pos.node->prev;
        pos.node->prev->next = p;
        pos.node->prev = p;
        return iterator(p);
    }

    /**
     * @brief 在迭代器pos前插入initializer_list ilist的元素
     * 
     * @param pos 迭代器位置
     * @param ilist initializer_list
     */
    void insert(iterator pos, std::initializer_list<T> ilist) {
        insert(pos, ilist.begin(), ilist.end());
    }

    /**
     * @brief 在迭代器pos前插入Args... args的元素
     * 
     * @param pos 迭代器位置
     * @param args 元素值
     * @return iterator 插入位置迭代器
     */
    template <typename... Args>
    iterator emplace(iterator pos, Args&&... args) {
        link_type p = create_node(msl::forward<Args>(args)...);
        p->next = pos.node;
        p->prev = pos.node->prev;
        pos.node->prev->next = p;
        pos.node->prev = p;
        return iterator(p);
    }

    /**
     * @brief 在链表头插入Args... args的元素
     * 
     * @param args 元素值
     */
    template <typename... Args>
    void emplace_front(Args&&... args) {
        emplace(begin(), msl::forward<Args>(args)...);
    }
    
    /**
     * @brief 在链表尾插入Args... args的元素
     * 
     * @param args 元素值
     */
    template <typename... Args>
    void emplace_back(Args&&... args) {
        emplace(end(), msl::forward<Args>(args)...);
    }
    #endif

    /**
     * @brief 在链表尾插入value
     * 
     * @param val 元素值
     */
    void push_back(const_reference val) {
        insert(end(), val);
    }

    /**
     * @brief 在链表头插入value
     * 
     * @param val 元素值
     */
    void push_front(const_reference val) {
        insert(begin(), val);
    }
    
    /**
     * @brief 删除链表头元素
     * 
     */
    void pop_front() {
        erase(begin());
    }

    /**
     * @brief 删除链表尾元素
     * 
     */
    void pop_back() {
        iterator tmp = end();
        erase(--tmp);
    }
    
    /**
     * @brief 删除迭代器pos指向的元素
     * 
     * @param pos 迭代器位置
     * @return iterator 下一个元素迭代器
     */
    iterator erase(iterator pos){
        link_type p = static_cast<link_type>(pos.node);
        link_type next = static_cast<link_type>(p->next);
        p->prev->next = p->next;
        p->next->prev = p->prev;
        destroy_node(p);
        return iterator(next);
    }

    /**
     * @brief 删除迭代器范围[first, last)的元素
     * 
     * @param first 迭代器开始
     * @param last 迭代器结束
     * @return iterator 下一个元素迭代器
     */
    iterator erase(iterator first, iterator last) {
        while (first != last) {
            first = erase(first);
        }
        return last;
    }
    
    /**
     * @brief 改变链表大小为new_size，若new_size大于当前大小，则在链表尾插入value，
     * 若new_size小于当前大小，则删除链表尾元素
     * 
     * @param new_size 新大小
     * @param value 元素值
     */
    void resize(size_type new_size, const_reference value = T()) {
        size_type len = size();
        if (len < new_size) {
            insert(end(), new_size - len, value);
        } else {
            iterator new_end = begin();
            while(new_size--) {
                ++new_end;
            }
            erase(new_end, end());
        }
    }

    /**
     * @brief 返回链表最大大小
     * 
     * @return size_type 最大大小
     */
    size_type max_size() const { return size_type(-1); }

    /**
     * @brief 清空链表
     * 
     */
    void clear() { base::clear(); }

    /**
     * @brief 删除链表中所有等于val的元素
     * 
     * @param val 元素值
     */
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

    /**
     * @brief 删除链表中所有连续重复的元素
     * 
     */
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
    
    /**
     * @brief 将链表x的元素移动到pos位置
     * 
     * @param pos 目标位置
     * @param x 链表x
     */
    void splice(iterator pos, list& x) {
        if (!x.empty()) {
            transfer(pos, x.begin(), x.end());
        }
    }

    /**
     * @brief 将链表x的元素i移动到pos位置
     * 
     * @param pos 目标位置
     * @param x 链表x
     * @param i 元素i
     */
    void splice(iterator pos, list& x, iterator i) {
        iterator j = i;
        ++j;
        if (pos == i || pos == j) return;
        transfer(pos, i, j);
    }

    /**
     * @brief 将链表x合并到当前链表，要求x链表元素有序
     * 
     * @param x 链表x
     */
    void merge(list& x) {
        if (this != &x) {
            iterator first1 = begin();
            iterator last1 = end();
            iterator first2 = x.begin();
            iterator last2 = x.end();
            while (first1 != last1 && first2 != last2) {
                if (*first2 < *first1) {
                    iterator next = first2;
                    transfer(first1, first2, ++next);
                    first2 = next;
                } else {
                    ++first1;
                }
            }
            if (first2 != last2) transfer(last1, first2, last2);
        }
    }

    /**
     * @brief 将链表x合并到当前链表，要求x链表元素有序
     * 
     * @param x 链表x
     * @param comp 比较函数
     */
    template <typename Compare>
    void merge(list& x, Compare comp) {
        if (this != &x) {
            iterator first1 = begin();
            iterator last1 = end();
            iterator first2 = x.begin();
            iterator last2 = x.end();
            while (first1 != last1 && first2 != last2) {
                if (comp(*first2, *first1)) {
                    iterator next = first2;
                    transfer(first1, first2, ++next);
                    first2 = next;
                } else {
                    ++first1;
                }
            }
            if (first2 != last2) transfer(last1, first2, last2);
        }
    }

    /**
     * @brief 反转链表
     * 
     */
    void reverse() {
        if (node_->next != node_ && node_->next->next != node_) {
            iterator first = begin();
            ++first;
            while (first != end()) { // 第一个元素不移动,之后每个元素移到begin()
                iterator next = first;
                ++next;
                transfer(begin(), first, next);
                first = next;
            }
        }
    }
    

    // 归并排序,非递归实现
    void sort(){
        if (node_->next != node_->prev) {
            list carry;
            list counter[64]; // 归并排序的辅助数组
            int fill = 0;
            while (!empty()) {
                carry.splice(carry.begin(), *this, begin());
                int i = 0;
                while (i < fill && !counter[i].empty()) {
                    counter[i].merge(carry);
                    carry.swap(counter[i++]);
                }
                carry.swap(counter[i]);
                if (i == fill) ++fill;
            }
            for (int i = 1; i < fill; ++i) counter[i].merge(counter[i - 1]);
            swap(counter[fill-1]);
        }
    }

    /**
     * @brief 归并排序,非递归实现
     * 
     * @param comp 比较函数
     */
    template <typename Compare>
    void sort(Compare comp){
        if (node_->next != node_->prev) {
            list carry;
            list counter[64]; // 归并排序的辅助数组
            int fill = 0;
            while (!empty()) {
                carry.splice(carry.begin(), *this, begin());
                int i = 0;
                while (i < fill && !counter[i].empty()) {
                    counter[i].merge(carry, comp);
                    carry.swap(counter[i++]);
                }
                carry.swap(counter[i]);
                if (i == fill) ++fill;
            }
            for (int i = 1; i < fill; ++i) counter[i].merge(counter[i - 1], comp);
            swap(counter[fill-1]);
        }
    }
    
};

} // namespace msl

#endif // MYSTL_LIST_H                          

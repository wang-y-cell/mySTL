#ifndef MYSTL_DEQUE_H
#define MYSTL_DEQUE_H

#include "stl_config.h"
#include "stl_iterator.h"
#include "stl_uninitialized.h"
#include "stl_alloc.h"
#include "stl_algobase.h"
#include <cstddef>

namespace msl {

inline size_t _deque_buf_size(size_t BufSiz, size_t sz) {
    return BufSiz == 0 ? sz < 512 ? static_cast<size_t>(512 / sz) : static_cast<size_t>(1)
                        : BufSiz;
}

template <typename T, typename Ref, typename Ptr, size_t BufSiz>
struct __deque_iterator {
    typedef __deque_iterator<T, T&, T*, BufSiz>              iterator;
    typedef __deque_iterator<T, const T&, const T*, BufSiz>  const_iterator;
    typedef __deque_iterator<T, Ref, Ptr, BufSiz>            self;

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

    static size_type buffer_size() { return _deque_buf_size(BufSiz, sizeof(T)); }

    __deque_iterator() : cur(0), first(0), last(0), node(0) {}
    __deque_iterator(T* x, map_pointer y) : cur(x), first(0), last(0), node(0) {
        set_node(y);
    }
    __deque_iterator(const iterator& x) : cur(x.cur), first(x.first), last(x.last), node(x.node) {}

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






template <typename T, typename Alloc, size_t BufSiz>
class _deque_base {
public:
    typedef Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }

protected:
    typedef __deque_iterator<T, T&, T*, BufSiz> iterator;
    typedef __deque_iterator<T, const T&, const T*, BufSiz> const_iterator;

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
        
        MYSTL_TRY {
            for (T** n = nstart; n <= nfinish; ++n) *n = allocate_node();
        }
        MYSTL_CATCH_ALL {
             // clean up allocated nodes
             for (T** n = nstart; n <= nfinish; ++n) {
                 if (*n) deallocate_node(*n);
                 else break;
             }
             deallocate_map(map_, map_size_);
             throw;
        }

        start_.set_node(nstart);
        finish_.set_node(nfinish);
        start_.cur = start_.first;
        finish_.cur = finish_.first + (num_elements % buffer_size());
    }

public:
    _deque_base(const allocator_type&) : map_(0), map_size_(0), start_(), finish_() {}
    _deque_base(const allocator_type&, size_t n) : map_(0), map_size_(0), start_(), finish_() 
    { create_map_and_nodes(n); }

    ~_deque_base() {
        if (map_) {
            for (T** n = start_.node; n <= finish_.node; ++n) {
                deallocate_node(*n);
            }
            deallocate_map(map_, map_size_);
        }
    }
};

template<class T, class Alloc = alloc, size_t BufSiz = 0>
class deque : public _deque_base<T, Alloc, BufSiz> {
    typedef _deque_base<T, Alloc, BufSiz> base;
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T** map_pointer;

    typedef typename base::allocator_type allocator_type;
    typedef typename base::iterator iterator;
    typedef typename base::const_iterator const_iterator;
    typedef msl::reverse_iterator<iterator> reverse_iterator;
    typedef msl::reverse_iterator<const_iterator> const_reverse_iterator;
    allocator_type get_allocator() const { return base::get_allocator(); }


protected:
    using base::buffer_size;
    using base::create_map_and_nodes;
    using base::allocate_node;
    using base::deallocate_node;
    using base::allocate_map;
    using base::deallocate_map;
    
    using base::start_;
    using base::finish_;
    using base::map_;
    using base::map_size_;

public:
    iterator begin() { return start_;}
    const_iterator begin() const { return start_;}

    iterator end() { return finish_;}
    const_iterator end() const { return finish_;}

    #if MYSTL_CPP_VERSION >= 11
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }
    #endif

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
    #if MYSTL_CPP_VERSION >= 11
    const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }
    #endif

    reference front(){return *start_;}
    const_reference front() const { return *start_;}
    reference back(){return *(finish_ - 1);}
    const_reference back() const { return *(finish_ - 1);}

    size_type size() const { return finish_ - start_;}
    size_type max_size() const { return size_type(-1);}
    bool empty() const { return finish_ == start_;}

    reference operator[](difference_type n) const {return start_[difference_type(n)];}

private:
    void fill_initialize(size_type n, const value_type& value);
    void push_back_aux(const value_type& value);
    void push_front_aux(const value_type& value);
    void pop_back_aux();
    void pop_front_aux();
    void reserve_amp_at_back(size_type nodes_to_add = 1);
    void reserve_amp_at_front(size_type nodes_to_add = 1);
    void reallocate_map(size_type nodes_to_add,bool add_at_front);
    iterator insert_aux(iterator pos, const value_type& value);

public:
    deque(allocator_type alloc = allocator_type())
    : base(alloc, 0) {}

    deque(size_type n, const value_type& value, allocator_type alloc = allocator_type())
    : base(alloc, n) { fill_initialize(n, value); }

    void push_back(const value_type& value) {
        if (finish_.cur != finish_.last - 1) {
            msl::construct(finish_.cur, value);
            ++finish_.cur;
        } else {
            push_back_aux(value);
        }
    }

    void push_front(const value_type& value) {
        if (start_.cur != start_.first) {
            msl::construct(start_.cur - 1, value);
            --start_.cur;
        } else {
            push_front_aux(value);
        }
    }

    void pop_back() {
        if (finish_.cur != finish_.first) {
            --finish_.cur;
            msl::destroy(finish_.cur);
        } else {
            pop_back_aux();
        }
    }

    void pop_front() {
        if (start_.cur != start_.last - 1) {
            msl::destroy(start_.cur);
            ++start_.cur;
        } else {
            pop_front_aux();
        }
    }

    void clear(){
        for(map_pointer n = start_.node + 1; n < finish_.node; ++n) {
            destroy(*n,*n + buffer_size());
            deallocate_node(*n);
        }

        if(finish_.node != start_.node){
            destroy(start_.cur, start_.last);
            destroy(finish_.first, finish_.cur);
            deallocate_node(*finish_.node);
        }else{
            destroy(start_.cur, finish_.cur); //如果头尾在一个缓冲区中
        }
        finish_ = start_;  //更新状态
    }

    iterator erase(iterator pos) {
        iterator next = pos;
        ++next;
        difference_type index = pos - start_;
        if (index < size() >> 1) {
            msl::copy_backward(start_, pos, next);
            pop_front();
        } else {
            msl::copy(next, finish_, pos);
            pop_back();
        }
        return start_ + index;
    }

    iterator erase(iterator first, iterator last) {
        if (first == start_ && last == finish_) { //如果是整个区间
            clear();
            return finish_;
        }
        difference_type n = last - first; // 计算删除的元素个数
        difference_type elems_before = first - start_; // 计算删除区间前的元素个数
        if(elems_before < ((size() - n) >> 1)){
            msl::copy_backward(start_, first, last);
            iterator new_start = start_ + n;
            destroy(start_,new_start);// 销毁删除区间前的元素

            // 释放删除区间前的缓冲区
            for(map_pointer n = start_.node; n < new_start.node; ++n) {
                deallocate_node(*n);
            }
            start_ = new_start;
        }else{
            msl::copy(last, finish_, first);
            iterator new_finish = finish_ - n;
            destroy(new_finish, finish_);// 销毁删除区间后的元素
            // 释放删除区间后的缓冲区
            for(map_pointer n = new_finish.node; n < finish_.node; ++n) {
                deallocate_node(*n);
            }
            finish_ = new_finish;
        }
        return start_ + elems_before;
    }

    //insert
    iterator insert(iterator pos, const value_type& value) {
        if(pos == start_) { // 如果插入位置在头部
            push_front(value);
            return start_;
        }
        else if(pos == finish_) { // 如果插入位置在尾部
            push_back(value);
            return finish_ - 1;
        }
        else{
            return insert_aux(pos, value);
        }
    }

};

template<typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::fill_initialize(size_type n, const value_type& value) {
    map_pointer cur;
    MYSTL_TRY{
        for(cur = start_.node; cur < finish_.node; ++cur) {
            uninitialized_fill(*cur, *cur + buffer_size(), value);
        }
        uninitialized_fill(finish_.first, finish_.cur, value);
    }MYSTL_CATCH_ALL{
        for(map_pointer n = start_.node; n < cur; ++n) {
            destroy(*n, *n + buffer_size());
            deallocate_node(*n);
        }
        destroy(finish_.first, finish_.cur);
        deallocate_node(*finish_.node);
        throw;
    }
}

template<typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::push_back_aux(const value_type& value) {
    value_type t_copy = value;
    reserve_amp_at_back();
    *(finish_.node + 1) = allocate_node();
    MYSTL_TRY{
        construct(finish_.cur, t_copy);
        finish_.set_node(finish_.node + 1);
        finish_.cur = finish_.first;
    }
    MYSTL_UNWIND(deallocate_node(*(finish_.node + 1)))
}

template<typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::push_front_aux(const value_type& value) {
    value_type t_copy = value;
    reserve_amp_at_front();
    *(start_.node - 1) = allocate_node();
    MYSTL_TRY{
        start_.set_node(start_.node - 1);
        start_.cur = start_.last - 1;
        msl::construct(start_.cur, t_copy);
    }
    MYSTL_CATCH_ALL{
        start_.set_node(start_.node + 1);
        start_.cur = start_.first;
        deallocate_node(*(start_.node - 1));
        throw;
    }
}

template<typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::reserve_amp_at_back(size_type nodes_to_add) {
    if (nodes_to_add + 1 > map_size_ - (finish_.node - map_)) {
        reallocate_map(nodes_to_add, false);
    }
}

template<typename T,typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::reserve_amp_at_front(size_type nodes_to_add) {
    if (nodes_to_add > start_.node - map_) {
        reallocate_map(nodes_to_add, true);
    }
}

template<typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::reallocate_map(size_type nodes_to_add,bool add_at_front) {
    size_type old_num_nodes = finish_.node - start_.node + 1;
    size_type new_num_nodes = old_num_nodes + nodes_to_add;

    map_pointer new_nstart;
    if(map_size_ > new_num_nodes * 2) {
        new_nstart = map_ + (map_size_ - new_num_nodes) / 2
        + (add_at_front ? nodes_to_add : 0);
        if(new_nstart < start_.node) {
            msl::copy(start_.node, finish_.node + 1, new_nstart);
        } else {
            msl::copy_backward(start_.node, finish_.node + 1, new_nstart + old_num_nodes);
        }
    }else{
        size_type new_map_size = map_size_ + msl::max(map_size_, nodes_to_add) + 2;
        map_pointer new_map = allocate_map(new_map_size);
        new_nstart = new_map + (new_map_size - new_num_nodes) / 2 
        + (add_at_front ? nodes_to_add : 0);
        msl::copy(start_.node, finish_.node + 1, new_nstart);
        deallocate_map(map_, map_size_);
        map_ = new_map;
        map_size_ = new_map_size;
    }
    start_.set_node(new_nstart);
    finish_.set_node(new_nstart + old_num_nodes - 1);
}

template<typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::pop_back_aux() {
    deallocate_node(*finish_.node);
    finish_.set_node(finish_.node - 1);
    finish_.cur = finish_.last - 1;
    msl::destroy(finish_.cur);
}

template<typename T, typename Alloc, size_t BufSiz>
void deque<T, Alloc, BufSiz>::pop_front_aux() {
    destroy(start_.cur);
    deallocate_node(*start_.node);
    start_.set_node(start_.node + 1);
    start_.cur = start_.first;
}

template<typename T, typename Alloc, size_t BufSiz>
typename deque<T, Alloc, BufSiz>::iterator
deque<T, Alloc, BufSiz>::insert_aux(iterator pos, const value_type& value) {
    difference_type index = pos - start_;
    if(index < size() / 2) {
        push_front(front());
        iterator front1 = start_;
        ++front1;
        iterator front2 = front1;
        ++front2;
        pos = start_ + index;
        iterator pos1 = pos;
        ++pos1;
        msl::copy(front2, pos1, front1);
    } else {
        push_back(back());
        iterator back1 = finish_;
        --back1;
        iterator back2 = back1;
        --back2;
        pos = start_ + index;
        msl::copy_backward(pos, back2, back1);
    }
    *pos = value;
    return pos;
}

template <typename T, typename Alloc, size_t BufSiz>
bool operator==(const deque<T, Alloc, BufSiz>& x, const deque<T, Alloc, BufSiz>& y) {
    return x.size() == y.size() && msl::equal(x.begin(), x.end(), y.begin());
}

template <typename T, typename Alloc, size_t BufSiz>
bool operator<(const deque<T, Alloc, BufSiz>& x, const deque<T, Alloc, BufSiz>& y) {
    return msl::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}









} // namespace msl

#endif // MYSTL_DEQUE_H


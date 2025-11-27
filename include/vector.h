#ifndef MYSTL_VECTOR_H
#define MYSTL_VECTOR_H

#include "stl_config.h"
#include "memory.h"
#include "iterator.h"
#include <stdexcept>
#include <algorithm>
#if MYSTL_CPP_VERSION >= 11
#include <initializer_list>
#include <utility>
#endif

namespace msl {







template <typename T, typename Alloc>
class vector_base {
protected:
    typedef simple_alloc<T, Alloc> data_allocator;
    typedef T* iterator;

    iterator start_;
    iterator finish_;
    iterator end_of_storage_;
    typedef Alloc allocator_type;

public:
    vector_base(const allocator_type& ) : 
        start_(0), finish_(0), end_of_storage_(0) {}

    explicit vector_base(msl::size_t n, const allocator_type& ) :
        start_(0), finish_(0), end_of_storage_(0) {
        if (n) {
            start_ = data_allocator::allocate(n);
            finish_ = start_;
            end_of_storage_ = start_ + n;
        }
    }
    ~vector_base() {
        if (start_) {
            data_allocator::deallocate(start_, end_of_storage_ - start_);
        }
    }
    allocator_type get_allocator() const { return allocator_type(); }
};



template <typename T, typename Alloc = alloc>
class vector : protected vector_base<T, Alloc> {
    typedef vector_base<T, Alloc> base;
    using base::start_;
    using base::finish_;
    using base::end_of_storage_;
public:
    typedef typename base::allocator_type allocator_type;
    allocator_type get_allocator() const { return base::get_allocator(); }
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef msl::size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef msl::reverse_iterator<iterator> reverse_iterator;
    typedef msl::reverse_iterator<const_iterator> const_reverse_iterator;

protected:
    typedef simple_alloc<T, Alloc> data_allocator;

    void insert_aux(iterator position, const_reference value);
    #if MYSTL_CPP_VERSION >= 11
    template<typename... Args>
    iterator realloc_emplace(iterator position, Args&&... args);
    void realloc_insert(iterator position, const_reference value);
    #else
    void realloc_insert(iterator position, const_reference value);
    #endif
    

    // 填充初始化：分配 n 个元素并填充值
    void fill_initialize(size_type n, const T& value) {
        start_ = allocate_and_fill(n, value);
        finish_ = start_ + n;
        end_of_storage_ = finish_;
    }

    // 分配 n 个元素并填充值
    iterator allocate_and_fill(size_type n, const T& value) {
        iterator result = data_allocator::allocate(n);
        msl::uninitialized_fill_n(result, n, value);
        return result;
    }

    void deallocate(){
        if(start_){
            data_allocator::deallocate(start_, end_of_storage_ - start_);
        }
    }
public:
    // 构造函数
    vector(const allocator_type& alloc = allocator_type()) : base(alloc) {}

    explicit vector(size_type n, const allocator_type& alloc = allocator_type()) : base(n, alloc) {
        finish_ = msl::uninitialized_fill_n(start_, n, T());
    }

    vector(size_type n, const_reference value, const allocator_type& alloc = allocator_type()) :
     base(n, alloc) {
        finish_ = msl::uninitialized_fill_n(start_, n, value);
    }

    vector(const vector& other, const allocator_type& alloc = allocator_type()) :
     base(other.size(), alloc) {
        finish_ = msl::uninitialized_copy(other.begin(), other.end(), start_);
    }

    template <typename InputIt>
    vector(InputIt first, InputIt last, const allocator_type& alloc = allocator_type()) :
     base(msl::distance(first, last), alloc) {
        finish_ = msl::uninitialized_copy(first, last, start_);
    }

#if MYSTL_CPP_VERSION >= 11
    vector(std::initializer_list<T> ilist, const allocator_type& alloc = allocator_type()) :
     base(static_cast<size_type>(ilist.size()), alloc) {
        finish_ = msl::uninitialized_copy(ilist.begin(), ilist.end(), start_);
    }
#endif

//移动语句
#ifdef MYSTL_HAS_MOVE_SEMANTICS
    vector(vector&& other) noexcept : base(other.get_allocator()) {
        start_ = other.start_;
        finish_ = other.finish_;
        end_of_storage_ = other.end_of_storage_;
        other.start_ = other.finish_ = other.end_of_storage_ = 0;
    }

    vector& operator=(vector&& other) noexcept {
        if (this != &other) {
            clear();
            start_ = other.start_;
            finish_ = other.finish_;
            end_of_storage_ = other.end_of_storage_;
            other.start_ = other.finish_ = other.end_of_storage_ = 0;
        }
        return *this;
    }
#endif

    // 赋值运算符
    vector& operator=(const vector& other) {
        if (this != &other) {
            assign(other.begin(), other.end());
        }
        return *this;
    }

    // 析构函数
    ~vector() { msl::destroy(start_, finish_); }

private:
    void fill_assign(size_type n, const_reference value);
    void assign_dispatch(size_type n, const_reference value, msl::true_type);
    template <typename InputIt>
    void assign_dispatch(InputIt first, InputIt last, msl::false_type);
    template <typename InputIt>
    void assign_range(InputIt first, InputIt last, msl::input_iterator_tag);
    template <typename ForwardIt>
    void assign_range(ForwardIt first, ForwardIt last, msl::forward_iterator_tag);
    template <typename RandomIt>
    void assign_range(RandomIt first, RandomIt last, msl::random_access_iterator_tag);
    

public:
    iterator begin() { return start_; }
    const_iterator begin() const { return start_; }
    iterator end() { return finish_; }
    const_iterator end() const { return finish_; }
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

    
    size_type size() const 
    { return static_cast<size_type>(finish_ - start_); }
    
    size_type capacity() const 
    { return static_cast<size_type>(end_of_storage_ - start_); }
    
    bool empty() const { return start_ == finish_; }
    size_type max_size() const { return static_cast<size_type>(-1) / sizeof(T); }
    
    void assign(size_type n, const_reference value) { fill_assign(n, value); }
    template <typename InputIterator>
    void assign(InputIterator first, InputIterator last);

    #if MYSTL_CPP_VERSION >= 11
    template <typename... Args>
    iterator emplace(iterator position, Args&&... args);
    template <typename... Args>
    void emplace_back(Args&&... args);
    #endif

    void shrink_to_fit();

    void insert(iterator position,size_type n,const_reference value);
    void insert(iterator position,const_iterator first,const_iterator last);

    #if MYSTL_CPP_VERSION >= 11
    void insert(iterator position,std::initializer_list<T> ilist);
    #endif
    
    void insert(iterator position,const_reference value);

    reference operator[](size_type n) { return start_[n]; }
    const_reference operator[](size_type n) const { return start_[n]; }

    reference at(size_type n) {
        if (n >= size()) throw std::out_of_range("Index out of range");
        return start_[n];
    }

    const_reference at(size_type n) const {
        if (n >= size()) throw std::out_of_range("Index out of range");
        return start_[n];
    }

    reference front() { return *start_; }
    const_reference front() const { return *start_; }
    reference back() { return *(finish_ - 1); }
    const_reference back() const { return *(finish_ - 1); }

    pointer data() { return start_; }
    const_pointer data() const { return start_; }


    void push_back(const_reference value) {
        if(finish_ != end_of_storage_) {
            msl::construct(finish_, value);
            ++finish_;
        } else {
            insert_aux(end(), value);
        }
    }

    void pop_back() {
        --finish_;
        msl::destroy(finish_);
    }

    iterator erase(iterator first,iterator last){
        iterator i = std::copy(last,finish_,first);
        msl::destroy(i,finish_);
        finish_ = finish_ - (last - first);
        return first;
    }

    iterator erase(iterator position){
        if(position + 1 != finish_)
             std::copy(position + 1,finish_,position);
             --finish_;
             msl::destroy(finish_);
             return position;
    }
    void reserve(size_type new_capacity) {
        if (new_capacity > capacity()) {
            size_type old_size = size();
            iterator new_start = data_allocator::allocate(new_capacity);
            iterator new_finish = msl::uninitialized_copy(start_, finish_, new_start);
            msl::destroy(start_, finish_);
            data_allocator::deallocate(start_, end_of_storage_ - start_);
            start_ = new_start;
            finish_ = new_finish;
            end_of_storage_ = start_ + new_capacity;
        }
    }

    void resize(size_type new_size, const T& value = T()) {
        if (new_size < size()) {
            msl::destroy(start_ + new_size, finish_);
            finish_ = start_ + new_size;
        } else if (new_size > size()) {
            reserve(new_size);
            finish_ = msl::uninitialized_fill_n(finish_, new_size - size(), value);
        }
    }

    void clear() {
        erase(begin(),end());
    }

    void swap(vector& other) {
        std::swap(start_, other.start_);
        std::swap(finish_, other.finish_);
        std::swap(end_of_storage_, other.end_of_storage_);
    }

};

template<typename T,typename Alloc>
void vector<T,Alloc>::insert_aux(iterator position, const_reference value) {
    if (finish_ != end_of_storage_) {
        msl::construct(finish_, *(finish_ - 1));
        ++finish_;
        std::copy_backward(position, finish_ - 2, finish_ - 1);
        *position = value;
    } else {
        realloc_insert(position, value);
    }
}

template<typename T,typename Alloc>
void vector<T,Alloc>::realloc_insert(iterator position, const_reference value) {
    const size_type old_size = size();
    const size_type new_capacity = old_size ? old_size * 2 : 1;
    iterator new_start = data_allocator::allocate(new_capacity);
    iterator new_finish = new_start;
    #if MYSTL_CPP_VERSION >= 11
    new_finish = msl::uninitialized_move(start_, position, new_start);
    #else
    new_finish = msl::uninitialized_copy(start_, position, new_start);
    #endif
    msl::construct(new_finish, value);
    ++new_finish;
    #if MYSTL_CPP_VERSION >= 11
    new_finish = msl::uninitialized_move(position, finish_, new_finish);
    #else
    new_finish = msl::uninitialized_copy(position, finish_, new_finish);
    #endif
    msl::destroy(start_, finish_);
    data_allocator::deallocate(start_, end_of_storage_ - start_);
    start_ = new_start;
    finish_ = new_finish;
    end_of_storage_ = start_ + new_capacity;
}

template<typename T,typename Alloc>
void vector<T,Alloc>::insert(iterator position,size_type n,const_reference value){
    if(n != 0){
        if(finish_ + n <= end_of_storage_){
            const size_type elems_after = finish_ - position;
            iterator old_finish = finish_;
            if(elems_after > n){
                iterator constructed = finish_;
                MYSTL_TRY{
                    msl::uninitialized_copy(old_finish - n, old_finish, finish_);
                    finish_ += n;
                    std::copy_backward(position, old_finish - n, old_finish);
                    std::fill(position, position + n, value);
                }
                MYSTL_CATCH_ALL{
                    msl::destroy(constructed, finish_);
                    finish_ = old_finish;
                    MYSTL_RETHROW;
                }
            }
            else {
                iterator constructed = finish_;
                MYSTL_TRY{
                    msl::uninitialized_fill_n(finish_,n - elems_after,value);
                    finish_ += n - elems_after;
                    msl::uninitialized_copy(position,old_finish,finish_);
                    finish_ += elems_after;
                    std::fill(position,old_finish,value);
                }
                MYSTL_CATCH_ALL{
                    msl::destroy(constructed, finish_);
                    finish_ = old_finish;
                    MYSTL_RETHROW;
                }
            }
        }
        else {
            const size_type old_size = size();
            const size_type len = old_size + std::max(old_size,n);
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;
            MYSTL_TRY{
                new_finish = msl::uninitialized_copy(start_, position, new_start);
                new_finish = msl::uninitialized_fill_n(new_finish, n, value);
                new_finish = msl::uninitialized_copy(position, finish_, new_finish);
            }
            MYSTL_CATCH_ALL{
                msl::destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, len);
                MYSTL_RETHROW;
            }
            msl::destroy(start_,finish_);
            deallocate();
            start_ = new_start;
            finish_ = new_finish;
            end_of_storage_ = start_ + len;
        }
    }
}

template<typename T,typename Alloc>
void vector<T,Alloc>::insert(iterator position,const_reference value){
    insert_aux(position, value);
}

template<typename T,typename Alloc>
void vector<T,Alloc>::insert(iterator position,const_iterator first,const_iterator last){
    size_type n = static_cast<size_type>(last - first);
    if(n == 0) return;
    if(static_cast<size_type>(end_of_storage_ - finish_) >= n){
        size_type elems_after = static_cast<size_type>(finish_ - position);
        iterator old_finish = finish_;
        if(elems_after > n){
            iterator constructed = finish_;
            MYSTL_TRY {
                msl::uninitialized_copy(old_finish - n, old_finish, finish_);
                finish_ += n;
                std::copy_backward(position, old_finish - n, old_finish);
                std::copy(first, last, position);
            }
            MYSTL_CATCH_ALL {
                msl::destroy(constructed, finish_);
                finish_ = old_finish;
                MYSTL_RETHROW;
            }
        } else {
            const_iterator mid = first + elems_after;
            iterator constructed = finish_;
            MYSTL_TRY {
                msl::uninitialized_copy(mid, last, finish_);
                finish_ += n - elems_after;
                msl::uninitialized_copy(position, old_finish, finish_);
                finish_ += elems_after;
                std::copy(first, mid, position);
            }
            MYSTL_CATCH_ALL {
                msl::destroy(constructed, finish_);
                finish_ = old_finish;
                MYSTL_RETHROW;
            }
        }
    } else {
        size_type old_size = size();
        size_type new_len = old_size + std::max(old_size, n);
        iterator new_start = data_allocator::allocate(new_len);
        iterator new_finish = new_start;
        MYSTL_TRY {
            new_finish = msl::uninitialized_copy(start_, position, new_start);
            new_finish = msl::uninitialized_copy(first, last, new_finish);
            new_finish = msl::uninitialized_copy(position, finish_, new_finish);
        }
        MYSTL_CATCH_ALL {
            msl::destroy(new_start, new_finish);
            data_allocator::deallocate(new_start, new_len);
            MYSTL_RETHROW;
        }
        msl::destroy(start_, finish_);
        data_allocator::deallocate(start_, end_of_storage_ - start_);
        start_ = new_start;
        finish_ = new_finish;
        end_of_storage_ = start_ + new_len;
    }
}



#if MYSTL_CPP_VERSION >= 0
template<typename T, typename Alloc>
template<typename InputIterator>
void vector<T,Alloc>::assign(InputIterator first, InputIterator last) {
    typedef typename msl::is_integer<InputIterator>::Integral Integral;
    assign_dispatch(first, last, Integral());
}

template<typename T, typename Alloc>
void vector<T,Alloc>::assign_dispatch(size_type n, const_reference value, msl::true_type) {
    fill_assign(n, value);
}

template<typename T, typename Alloc>
template<typename InputIt>
void vector<T,Alloc>::assign_dispatch(InputIt first, InputIt last, msl::false_type) {
    typedef typename msl::iterator_traits<InputIt>::iterator_category Cat;
    static_assert(msl::is_msl_iterator_tag<Cat>::value, "msl::vector::assign forbids std iterators");
    assign_range(first, last, msl::iterator_category(first));
}

template<typename T, typename Alloc>
template<typename InputIt>
void vector<T,Alloc>::assign_range(InputIt first, InputIt last, msl::input_iterator_tag) {
    iterator cur = begin();
    iterator e = end();
    for (; cur != e && first != last; ++cur, ++first) {
        *cur = *first;
    }
    if (first == last) {
        erase(cur, e);
    } else {
        for (; first != last; ++first) {
            push_back(*first);
        }
    }
}

template<typename T, typename Alloc>
template<typename ForwardIt>
void vector<T,Alloc>::assign_range(ForwardIt first, ForwardIt last, msl::forward_iterator_tag) {
    size_type n = static_cast<size_type>(msl::distance(first, last));
    if (n > capacity()) {
        iterator new_start = data_allocator::allocate(n);
        iterator new_finish = msl::uninitialized_copy(first, last, new_start);
        msl::destroy(start_, finish_);
        data_allocator::deallocate(start_, end_of_storage_ - start_);
        start_ = new_start;
        finish_ = new_finish;
        end_of_storage_ = start_ + n;
    } else {
        size_type cur_size = size();
        ForwardIt it = first;
        size_type i = 0;
        for (; i < cur_size && it != last; ++i, ++it) {
            start_[i] = *it;
        }
        if (i < cur_size) {
            msl::destroy(start_ + i, finish_);
        }
        finish_ = start_ + i;
        if (it != last) {
            finish_ = msl::uninitialized_copy(it, last, finish_);
        }
    }
}

template<typename T, typename Alloc>
template<typename RandomIt>
void vector<T,Alloc>::assign_range(RandomIt first, RandomIt last, msl::random_access_iterator_tag) {
    assign_range(first, last, forward_iterator_tag());
}

#endif

#if MYSTL_CPP_VERSION >= 11
template<typename T,typename Alloc>
void vector<T,Alloc>::insert(iterator position,std::initializer_list<T> ilist){
    const_iterator first = ilist.begin();
    const_iterator last = ilist.end();
    insert(position, first, last);
}
#endif

template<typename T,typename Alloc>
void vector<T,Alloc>::fill_assign(size_type n, const_reference value){
    if(n > capacity()){
        reserve(n);
    }
    msl::uninitialized_fill_n(start_, n, value);
    if(n < size()){
        msl::destroy(start_ + n, finish_);
    }
    finish_ = start_ + n;
}

template <typename T, typename Alloc>
inline bool operator==(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
    return x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin());
}

template <typename T, typename Alloc>
inline bool operator!=(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
    return !(x == y);
}

template <typename T, typename Alloc>
inline bool operator<(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
    return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <typename T, typename Alloc>
inline bool operator>(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
    return y < x;
}

template <typename T, typename Alloc>
inline bool operator<=(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
    return !(y < x);
}

template <typename T, typename Alloc>
inline bool operator>=(const vector<T, Alloc>& x, const vector<T, Alloc>& y) {
    return !(x < y);
}

template <typename T, typename Alloc>
inline void swap(vector<T, Alloc>& x, vector<T, Alloc>& y) {
    x.swap(y);
}

template<typename T, typename Alloc>
void vector<T,Alloc>::shrink_to_fit() {
    if (finish_ == start_) {
        if (start_) {
            data_allocator::deallocate(start_, end_of_storage_ - start_);
            start_ = 0;
            finish_ = 0;
            end_of_storage_ = 0;
        }
        return;
    }
    if (end_of_storage_ != finish_) {
        size_type new_capacity = size();
        iterator new_start = data_allocator::allocate(new_capacity);
        iterator new_finish = msl::uninitialized_copy(start_, finish_, new_start);
        msl::destroy(start_, finish_);
        data_allocator::deallocate(start_, end_of_storage_ - start_);
        start_ = new_start;
        finish_ = new_finish;
        end_of_storage_ = start_ + new_capacity;
    }
}

#if MYSTL_CPP_VERSION >= 11
template<typename T, typename Alloc>
template<typename... Args>
typename vector<T, Alloc>::iterator
vector<T,Alloc>::emplace(iterator position, Args&&... args) {
    if (finish_ != end_of_storage_) {
        msl::construct(finish_, *(finish_ - 1));
        ++finish_;
        std::copy_backward(position, finish_ - 2, finish_ - 1);
        msl::destroy(position);
        new (position) T(std::forward<Args>(args)...);
        return position;
    } else {
        return realloc_emplace(position, std::forward<Args>(args)...);
    }
}

template<typename T, typename Alloc>
template<typename... Args>
void vector<T,Alloc>::emplace_back(Args&&... args) {
    if (finish_ != end_of_storage_) {
        new (finish_) T(std::forward<Args>(args)...);
        ++finish_;
    } else {
        (void)realloc_emplace(end(), std::forward<Args>(args)...);
    }
}

template<typename T,typename Alloc>
template<typename... Args>
typename vector<T,Alloc>::iterator
vector<T,Alloc>::realloc_emplace(iterator position, Args&&... args) {
    size_type old_size = size();
    size_type new_capacity = old_size ? old_size * 2 : 1;
    iterator new_start = data_allocator::allocate(new_capacity);
    iterator new_finish = new_start;
    size_type pos_index = static_cast<size_type>(position - start_);
    #if MYSTL_CPP_VERSION >= 11
    new_finish = msl::uninitialized_move(start_, position, new_start);
    #else
    new_finish = msl::uninitialized_copy(start_, position, new_start);
    #endif
    new (new_finish) T(std::forward<Args>(args)...);
    ++new_finish;
    #if MYSTL_CPP_VERSION >= 11
    new_finish = msl::uninitialized_move(position, finish_, new_finish);
    #else
    new_finish = msl::uninitialized_copy(position, finish_, new_finish);
    #endif
    msl::destroy(start_, finish_);
    data_allocator::deallocate(start_, end_of_storage_ - start_);
    start_ = new_start;
    finish_ = new_finish;
    end_of_storage_ = start_ + new_capacity;
    return start_ + pos_index;
}

#endif




} // namespace msl

#endif // MYSTL_VECTOR_H

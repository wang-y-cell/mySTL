#ifndef MYSTL_VECTOR_H
#define MYSTL_VECTOR_H

#include "stl_config.h"
#include "memory.h"
#include "iterator.h"
#include <stdexcept>
#include <algorithm>
#if MYSTL_CPP_VERSION >= 11
#include <initializer_list>
#endif

namespace msl {

template <typename T, typename Alloc = alloc>
class vector {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T* iterator;
    typedef const T* const_iterator;

protected:
    iterator start_;
    iterator finish_;
    iterator end_of_storage_;
    typedef simple_alloc<T, Alloc> data_allocator;

    void insert_aux(iterator position, const_reference value);
    

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
    vector() : start_(0), finish_(0), end_of_storage_(0) {}

    explicit vector(size_type n) : start_(0), finish_(0), end_of_storage_(0) {
        fill_initialize(n, T());
    }

    vector(size_type n, const_reference value) : start_(0), finish_(0), end_of_storage_(0) {
        fill_initialize(n, value);
    }

    vector(const vector& other) : start_(0), finish_(0), end_of_storage_(0) {
        size_type n = other.size();
        start_ = data_allocator::allocate(n);
        finish_ = msl::uninitialized_copy(other.begin(), other.end(), start_);
        end_of_storage_ = start_ + n;
    }
//移动语句
#ifdef MYSTL_HAS_MOVE_SEMANTICS
    vector(vector&& other) noexcept : start_(other.start_), finish_(other.finish_), end_of_storage_(other.end_of_storage_) {
        other.start_ = other.finish_ = other.end_of_storage_ = 0;
    }

    vector& operator=(vector&& other) noexcept {
        if (this != &other) {
            clear();
            if (start_) data_allocator::deallocate(start_, end_of_storage_ - start_);
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
            assign_from(other.begin(), other.end());
        }
        return *this;
    }

    // 析构函数
    ~vector() {
        msl::destroy(start_, finish_);
        deallocate();
    }

private:
    template <typename InputIt>
    void assign_from(InputIt first, InputIt last) {
        size_type n = msl::distance(first, last);
        if (n > capacity()) reserve(n);
        iterator cur = start_;
        InputIt it = first;
        for (; it != last; ++it, ++cur) {
            if (cur < finish_) {
                *cur = *it;
            } else {
                msl::construct(cur, *it);
            }
        }
        if (cur < finish_) {
            msl::destroy(cur, finish_);
        }
        finish_ = start_ + n;
    }

    public:
    iterator begin() { return start_; }
    const_iterator begin() const { return start_; }
    iterator end() { return finish_; }
    const_iterator end() const { return finish_; }

    
    size_type size() const 
    { return static_cast<size_type>(finish_ - start_); }
    
    size_type capacity() const 
    { return static_cast<size_type>(end_of_storage_ - start_); }
    
    bool empty() const { return start_ == finish_; }
    
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
        const size_type old_size = size();
        const size_type new_capacity = old_size ? old_size * 2 : 1;
        iterator new_start = data_allocator::allocate(new_capacity);
        iterator new_finish = msl::uninitialized_copy(start_, position, new_start);
        msl::construct(new_finish, value);
        ++new_finish;
        new_finish = msl::uninitialized_copy(position, finish_, new_finish);
        msl::destroy(start_, finish_);
        data_allocator::deallocate(start_, end_of_storage_ - start_);
        start_ = new_start;
        finish_ = new_finish;
        end_of_storage_ = start_ + new_capacity;
    }
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

#if MYSTL_CPP_VERSION >= 11
template<typename T,typename Alloc>
void vector<T,Alloc>::insert(iterator position,std::initializer_list<T> ilist){
    const_iterator first = ilist.begin();
    const_iterator last = ilist.end();
    insert(position, first, last);
}
#endif

} // namespace msl

#endif // MYSTL_VECTOR_H

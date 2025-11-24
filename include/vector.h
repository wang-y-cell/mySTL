#ifndef MYSTL_VECTOR_H
#define MYSTL_VECTOR_H

#include "stl_config.h"
#include "memory.h"
#include "iterator.h"
#include <stdexcept>
#include <algorithm>

namespace mystl {

template <class T, class Alloc = alloc>
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
        mystl::uninitialized_fill_n(result, n, value);
        return result;
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
        finish_ = mystl::uninitialized_copy(other.begin(), other.end(), start_);
        end_of_storage_ = start_ + n;
    }

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
        if (start_) {
            mystl::destroy(start_, finish_);
            data_allocator::deallocate(start_, end_of_storage_ - start_);
        }
    }

private:
    template <class InputIt>
    void assign_from(InputIt first, InputIt last) {
        size_type n = mystl::distance(first, last);
        if (n > capacity()) reserve(n);
        iterator cur = start_;
        InputIt it = first;
        for (; it != last; ++it, ++cur) {
            if (cur < finish_) {
                *cur = *it;
            } else {
                mystl::construct(cur, *it);
            }
        }
        if (cur < finish_) {
            mystl::destroy(cur, finish_);
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

    void push_back(const_reference value) {
        if(finish_ != end_of_storage_) {
            mystl::construct(finish_, value);
            ++finish_;
        } else {
            insert_aux(end(), value);
        }
    }

    void pop_back() {
        --finish_;
        mystl::destroy(finish_);
    }

    void reserve(size_type new_capacity) {
        if (new_capacity > capacity()) {
            size_type old_size = size();
            iterator new_start = data_allocator::allocate(new_capacity);
            iterator new_finish = mystl::uninitialized_copy(start_, finish_, new_start);
            mystl::destroy(start_, finish_);
            data_allocator::deallocate(start_, end_of_storage_ - start_);
            start_ = new_start;
            finish_ = new_finish;
            end_of_storage_ = start_ + new_capacity;
        }
    }

    void resize(size_type new_size, const T& value = T()) {
        if (new_size < size()) {
            mystl::destroy(start_ + new_size, finish_);
            finish_ = start_ + new_size;
        } else if (new_size > size()) {
            reserve(new_size);
            finish_ = mystl::uninitialized_fill_n(finish_, new_size - size(), value);
        }
    }

    void clear() {
        mystl::destroy(start_, finish_);
        finish_ = start_;
    }

    void swap(vector& other) {
        std::swap(start_, other.start_);
        std::swap(finish_, other.finish_);
        std::swap(end_of_storage_, other.end_of_storage_);
    }

};

template<class T,class Alloc>
void vector<T,Alloc>::insert_aux(iterator position, const_reference value) {
    if (finish_ != end_of_storage_) {
        mystl::construct(finish_, *(finish_ - 1));
        ++finish_;
        copy_backward(position, finish_ - 2, finish_ - 1);
        *position = value;
    } else {
        const size_type old_size = size();
        const size_type new_capacity = old_size ? old_size * 2 : 1;
        iterator new_start = data_allocator::allocate(new_capacity);
        iterator new_finish = mystl::uninitialized_copy(start_, position, new_start);
        mystl::construct(new_finish, value);
        ++new_finish;
        new_finish = mystl::uninitialized_copy(position, finish_, new_finish);
        mystl::destroy(start_, finish_);
        data_allocator::deallocate(start_, end_of_storage_ - start_);
        start_ = new_start;
        finish_ = new_finish;
        end_of_storage_ = start_ + new_capacity;
    }
}


} // namespace mystl

#endif // MYSTL_VECTOR_H

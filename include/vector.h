#ifndef MYSTL_VECTOR_H
#define MYSTL_VECTOR_H

#include "stl_config.h"
#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace mystl {

// 前置声明
template<typename T>
class vector;

// 迭代器类
template<typename T>
class vector_iterator {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef std::random_access_iterator_tag iterator_category;

private:
    pointer ptr_;

public:
    vector_iterator(pointer ptr) : ptr_(ptr) {}
    
    reference operator*() const { return *ptr_; }
    pointer operator->() const { return ptr_; }
    
    vector_iterator& operator++() { ++ptr_; return *this; }
    vector_iterator operator++(int) { vector_iterator tmp = *this; ++ptr_; return tmp; }
    
    vector_iterator& operator--() { --ptr_; return *this; }
    vector_iterator operator--(int) { vector_iterator tmp = *this; --ptr_; return tmp; }
    
    vector_iterator operator+(difference_type n) const { return vector_iterator(ptr_ + n); }
    vector_iterator operator-(difference_type n) const { return vector_iterator(ptr_ - n); }
    
    difference_type operator-(const vector_iterator& other) const { return ptr_ - other.ptr_; }
    
    bool operator==(const vector_iterator& other) const { return ptr_ == other.ptr_; }
    bool operator!=(const vector_iterator& other) const { return ptr_ != other.ptr_; }
    
    reference operator[](difference_type n) const { return ptr_[n]; }
};

template<typename T>
class vector {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef vector_iterator<T> iterator;
    typedef const vector_iterator<T> const_iterator;

private:
    pointer data_;
    size_type size_;
    size_type capacity_;

    // 异常安全的内存分配
    pointer allocate(size_type n) {
        MYSTL_TRY {
            return new T[n];
        }
        MYSTL_CATCH_ALL {
            MYSTL_THROW(std::bad_alloc());
        }
    }

    void deallocate(pointer ptr) {
        delete[] ptr;
    }

    void construct(pointer ptr, const_reference value) {
        MYSTL_TRY {
            new (ptr) T(value);
        }
        MYSTL_CATCH_ALL {
            MYSTL_THROW(std::runtime_error("Construction failed"));
        }
    }

    void destroy(pointer ptr) {
        ptr->~T();
    }

public:
    // 构造函数
    vector() : data_(nullptr), size_(0), capacity_(0) {}
    
    explicit vector(size_type n) : size_(n), capacity_(n) {
        data_ = allocate(n);
        MYSTL_TRY {
            for (size_type i = 0; i < n; ++i) {
                construct(data_ + i, T());
            }
        }
        MYSTL_CATCH_ALL {
            deallocate(data_);
            MYSTL_RETHROW;
        }
    }
    
    vector(size_type n, const_reference value) : size_(n), capacity_(n) {
        data_ = allocate(n);
        MYSTL_TRY {
            for (size_type i = 0; i < n; ++i) {
                construct(data_ + i, value);
            }
        }
        MYSTL_CATCH_ALL {
            deallocate(data_);
            MYSTL_RETHROW;
        }
    }

    // 析构函数
    ~vector() {
        if (data_) {
            for (size_type i = 0; i < size_; ++i) {
                destroy(data_ + i);
            }
            deallocate(data_);
        }
    }

    // 拷贝构造函数
    vector(const vector& other) : size_(other.size_), capacity_(other.capacity_) {
        data_ = allocate(capacity_);
        MYSTL_TRY {
            for (size_type i = 0; i < size_; ++i) {
                construct(data_ + i, other.data_[i]);
            }
        }
        MYSTL_CATCH_ALL {
            deallocate(data_);
            MYSTL_RETHROW;
        }
    }

#ifdef MYSTL_HAS_MOVE_SEMANTICS
    // 移动构造函数
    vector(vector&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    // 移动赋值运算符
    vector& operator=(vector&& other) noexcept {
        if (this != &other) {
            if (data_) {
                for (size_type i = 0; i < size_; ++i) {
                    destroy(data_ + i);
                }
                deallocate(data_);
            }
            
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            
            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }
#endif

    // 拷贝赋值运算符
    vector& operator=(const vector& other) {
        if (this != &other) {
            vector tmp(other);
            swap(tmp);
        }
        return *this;
    }

    // 添加元素
    void push_back(const_reference value) {
        if (size_ >= capacity_) {
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        construct(data_ + size_, value);
        ++size_;
    }

    // 删除最后一个元素
    void pop_back() {
        MYSTL_TRY {
            if (empty()) {
                MYSTL_THROW(std::out_of_range("Vector is empty"));
            }
            --size_;
            destroy(data_ + size_);
        }
        MYSTL_CATCH_ALL {
            MYSTL_RETHROW;
        }
    }

    // 访问元素
    reference operator[](size_type index) {
        return data_[index];
    }

    const_reference operator[](size_type index) const {
        return data_[index];
    }

    reference at(size_type index) {
        MYSTL_TRY {
            if (index >= size_) {
                MYSTL_THROW(std::out_of_range("Index out of range"));
            }
            return data_[index];
        }
        MYSTL_CATCH_ALL {
            MYSTL_RETHROW;
        }
    }

    const_reference at(size_type index) const {
        MYSTL_TRY {
            if (index >= size_) {
                MYSTL_THROW(std::out_of_range("Index out of range"));
            }
            return data_[index];
        }
        MYSTL_CATCH_ALL {
            MYSTL_RETHROW;
        }
    }

    // 迭代器
    iterator begin() { return iterator(data_); }
    iterator end() { return iterator(data_ + size_); }
    const_iterator begin() const { return const_iterator(data_); }
    const_iterator end() const { return const_iterator(data_ + size_); }

    // 获取大小
    size_type size() const { return size_; }
    size_type capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }

    // 修改容量
    void reserve(size_type new_capacity) {
        if (new_capacity > capacity_) {
            pointer new_data = allocate(new_capacity);
            size_type new_size = size_;
            
            MYSTL_TRY {
                for (size_type i = 0; i < size_; ++i) {
                    construct(new_data + i, data_[i]);
                }
            }
            MYSTL_CATCH_ALL {
                for (size_type i = 0; i < new_size; ++i) {
                    destroy(new_data + i);
                }
                deallocate(new_data);
                MYSTL_RETHROW;
            }
            
            for (size_type i = 0; i < size_; ++i) {
                destroy(data_ + i);
            }
            deallocate(data_);
            
            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    void resize(size_type new_size) {
        if (new_size < size_) {
            for (size_type i = new_size; i < size_; ++i) {
                destroy(data_ + i);
            }
        } else if (new_size > size_) {
            reserve(new_size);
            for (size_type i = size_; i < new_size; ++i) {
                construct(data_ + i, T());
            }
        }
        size_ = new_size;
    }

    void clear() {
        for (size_type i = 0; i < size_; ++i) {
            destroy(data_ + i);
        }
        size_ = 0;
    }

    void swap(vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
};

} // namespace mystl

#endif // MYSTL_VECTOR_H
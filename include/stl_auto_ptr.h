#ifndef MYSTL_AUTO_PTR_H
#define MYSTL_AUTO_PTR_H

#include "stl_config.h"



//#if MYSTL_CPP_VERSION < 17

namespace msl {

// auto_ptr_ref 
template<typename T>
struct auto_ptr_ref;

template<typename T>
class auto_ptr {
private:
    T* pointee;

public:
    typedef T elem_type; 

 
    explicit auto_ptr(T* p = 0) : pointee(p) {}

 
    auto_ptr(auto_ptr& rhs) : pointee(rhs.release()) {}


    template<typename U>
    auto_ptr(auto_ptr<U>& rhs) : pointee(rhs.release()) {}


    auto_ptr& operator=(auto_ptr& rhs) {
        if (this != &rhs) {
            delete pointee;
            pointee = rhs.release();
        }
        return *this;
    }


    template<typename U>
    auto_ptr& operator=(auto_ptr<U>& rhs) {
        if (this->get() != rhs.get()) {
            delete pointee;
            pointee = rhs.release();
        }
        return *this;
    }


    ~auto_ptr() {delete pointee;}


    T& operator*() const {return *pointee;}
    T* operator->() const {return pointee;}


    T* get() const {return pointee;}


    T* release() {
        T* tmp = pointee;
        pointee = 0;
        return tmp;
    }


    void reset(T* p = 0) {
        if (pointee != p) {
            delete pointee;
            pointee = p;
        }
    }


    auto_ptr(auto_ptr_ref<T> ref) : pointee(ref.ptr) {}


    auto_ptr& operator=(auto_ptr_ref<T> ref) {
        if (pointee != ref.ptr) {
            delete pointee;
            pointee = ref.ptr;
        }
        return *this;
    }


    template<typename U>
    operator auto_ptr_ref<U>() {
        return auto_ptr_ref<U>(this->release());
    }

    template<typename U>
    operator auto_ptr<U>() {
        return auto_ptr<U>(this->release());
    }
};


template<typename T>
struct auto_ptr_ref {
    T* ptr;
    explicit auto_ptr_ref(T* p) : ptr(p) {}
};

} // namespace msl

//#endif // MYSTL_CPP_VERSION < 17

#endif // MYSTL_AUTO_PTR_H

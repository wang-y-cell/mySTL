
#ifndef MYSTL_ALLOC_H
#define MYSTL_ALLOC_H


#include <cstddef>
#include <cstdlib>
#include <new>


#if 1                                                                                                     
#   include <new>
#   define __THROW_BAD_ALLOC throw std::bad_alloc()
#elif !defined(__THROW_BAD_ALLOC)
#   include<iostream>
#   define __THROW_BAD_ALLOC std::cerr << "out of memory" << std::endl;exit(1);
#endif




namespace msl {





template <int inst> 
class malloc_alloc_template {
private:

    static void* oom_malloc(size_t);

    static void* oom_realloc(void* p, size_t new_sz);

    static void (* __malloc_alloc_oom_handler)();

public:

    static void* allocate(size_t n) {
        void* p = malloc(n);
        if (!p) p = oom_malloc(n);
        return p;
    }


    static void deallocate(void* p, size_t) {
        free(p);
    }

    static void* reallocate(void* p, size_t, size_t new_sz) {
        void* r = realloc(p, new_sz);
        if (!r) r = oom_realloc(p, new_sz);
        return r;
    }

    static void (* set_malloc_handler(void (*f)()))() {
        void (* old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return old;
    }


};


template <int inst>
void (*malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;


template<int inst>
void* malloc_alloc_template<inst>::oom_malloc(size_t n) {
    void (* handler)();
    void* ret;

    for(;;){
        handler = __malloc_alloc_oom_handler;
        if(!handler) __THROW_BAD_ALLOC;
        (*handler)();
        ret = malloc(n);
        if(ret) return ret;
    }
}

template<int inst>
void* malloc_alloc_template<inst>::oom_realloc(void*p, size_t new_sz) {
    void (* handler)();
    void* ret;

    for(;;){
        handler = __malloc_alloc_oom_handler;
        if(!handler) __THROW_BAD_ALLOC;
        (*handler)(); 
        ret = realloc(p, new_sz);
        if(ret) return ret;
    }
}

typedef malloc_alloc_template<0> malloc_alloc;



enum { ALIGN = 8 };
enum { MAX_BYTES = 128 };
enum { NFREELISTS = MAX_BYTES / ALIGN };


template <int inst>
class default_alloc_template {
private:
    union obj {
        union obj* free_list_link; 
        char client_data[1]; 
    };
    
    static obj* free_list[NFREELISTS];
    static char* start_free;
    static char* end_free;
    static size_t heap_size;
private:
    static size_t ROUND_UP(size_t bytes) { return (bytes + ALIGN - 1) & ~(ALIGN - 1); }
    static size_t FREELIST_INDEX(size_t bytes) { return (bytes + ALIGN - 1) / ALIGN - 1; }
    static void* refill(size_t n);
    static char* chunk_alloc(size_t size, int& nobjs);
public:
    static void* allocate(std::size_t n) {
        if (n > MAX_BYTES) return malloc_alloc::allocate(n);
        size_t index = FREELIST_INDEX(n);
        obj** my_list = free_list + index;
        obj* result = *my_list;
        if (result) { *my_list = result->free_list_link; return result; }
        return refill(ROUND_UP(n));
    }
    static void deallocate(void* p, size_t n) {
        if (!p || n == 0) return;
        obj *q = (obj*)p;
        obj ** my_free_list;
        if(n > (size_t)MAX_BYTES) {
            malloc_alloc::deallocate(p, n);
            return;
        }
        my_free_list = free_list + FREELIST_INDEX(n);
        q->free_list_link = *my_free_list;
        *my_free_list = q;
    }
    static void* reallocate(void* p, size_t old_sz, size_t new_sz) {
        if (old_sz > MAX_BYTES && new_sz > MAX_BYTES) return malloc_alloc::reallocate(p, old_sz, new_sz);
        if (ROUND_UP(old_sz) == ROUND_UP(new_sz)) return p;
        void* result = allocate(new_sz);
        size_t copy_sz = old_sz < new_sz ? old_sz : new_sz;
        unsigned char* r = (unsigned char*)result;
        unsigned char* s = (unsigned char*)p;
        for (size_t i = 0; i < copy_sz; ++i) r[i] = s[i];
        deallocate(p, old_sz);
        return result;
    }
};

template <int inst>
typename default_alloc_template<inst>::obj*
default_alloc_template<inst>::free_list[NFREELISTS] = {0};

template <int inst>
char* default_alloc_template<inst>::start_free = 0;

template <int inst>
char* default_alloc_template<inst>::end_free = 0;

template <int inst>
size_t default_alloc_template<inst>::heap_size = 0;

template <int inst>
void* default_alloc_template<inst>::refill(size_t n) {
    int nobjs = 20;
    char* chunk = chunk_alloc(n, nobjs);
    if (nobjs == 1) return (void*)chunk;
    size_t index = FREELIST_INDEX(n);
    obj** my_list = free_list + index;
    char* cur = chunk + n;
    *my_list = (obj*)cur;
    obj* current = (obj*)cur;
    for (int i = 1; i < nobjs - 1; ++i) {
        char* next = cur + n;
        current->free_list_link = (obj*)next;
        current = (obj*)next;
        cur = next;
    }
    current->free_list_link = 0;
    return (void*)chunk;
}


template <int inst>
char* default_alloc_template<inst>::chunk_alloc(size_t size, int& nobjs) {
    size_t total_bytes = size * nobjs;
    size_t bytes_left = end_free - start_free;
    char* result;
    if (bytes_left >= total_bytes) {
        result = start_free;
        start_free += total_bytes;
        return result;
    } else if (bytes_left >= size) {
        nobjs = (int)(bytes_left / size);
        total_bytes = size * nobjs;
        result = start_free;
        start_free += total_bytes;
        return result;
    } else {
        if (bytes_left > 0) {
            size_t index = FREELIST_INDEX(bytes_left);
            obj** my_list = free_list + index;
            ((obj*)start_free)->free_list_link = *my_list;
            *my_list = (obj*)start_free;
        }
        size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4); 
        start_free = (char*)malloc(bytes_to_get);
        if (start_free == 0) {
            int i;
            obj** me_free_list, *p;
            for(i = size; i <= MAX_BYTES; i += ALIGN) {
                me_free_list = free_list + FREELIST_INDEX(i);
                p = *me_free_list;
                if(p) {
                    *me_free_list = p->free_list_link;
                    start_free = (char*)p;
                    end_free = start_free + i;
                    return(chunk_alloc(size,nobjs));
                }
            }      
            end_free = 0;    
            start_free = (char*)malloc_alloc::allocate(bytes_to_get);
        }
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        return chunk_alloc(size, nobjs);
    }
}


typedef default_alloc_template<0> default_alloc;

#ifdef __NOT_USE_SECOND_ALLOC 
    typedef malloc_alloc alloc;
#else
    typedef default_alloc alloc;
#endif


template <typename T, typename Alloc>
struct simple_alloc {
    static T* allocate(std::size_t n) {
        return n ? static_cast<T*>(Alloc::allocate(n * sizeof(T))) : 0;
    }


    static T* allocate() {
        return static_cast<T*>(Alloc::allocate(sizeof(T)));
    }

    static void deallocate(T* p, std::size_t n) {
        if (!p || n == 0) return;
        Alloc::deallocate(p, n * sizeof(T));
    }


    static void deallocate(T* p) {
        if (!p) return;
        Alloc::deallocate(p, sizeof(T));
    }
};


} // namespace msl

#endif

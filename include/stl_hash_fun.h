/*
当前哈希函数实现的都是内置的整数类型
用来给hashtable使用,实现的基本上都是整数本身



*/
#ifndef STL_HASH_FUN_H
#define STL_HASH_FUN_H


#include "stl_config.h"
#include <string>

namespace msl{

template<typename key> struct hash{ };
/**************************************************************** */
//非源码,但是我认为比较常用所以添加
inline size_t __stl_hash_string(const std::string& str) {
    unsigned long h = 0;
    for(size_t i = 0; i < str.size(); ++i){
        h = h * 5 + str[i];
    }
    return size_t(h);
} 
/***************************************************************** */
inline size_t __stl_hash_string(const char* str) {
    unsigned long h = 0;
    for(; *str ; ++str){
        h = h * 5 + *str;
    }
    return size_t(h);
}

template<> struct hash<std::string> {
    size_t operator()(const std::string& str) const {
        return __stl_hash_string(str);
    }
};

template<> struct hash<char*> {
    size_t operator()(const char* str) const {
        return __stl_hash_string(str);
    }
};

template<> struct hash<const char*> {
    size_t operator()(const char* str) const {
        return __stl_hash_string(str);
    }
};

template<> struct hash<char> {
    size_t operator()(char c) const {
        return size_t(c);
    }
};

template<> struct hash<unsigned char> {
    size_t operator()(unsigned char c) const {
        return size_t(c);
    }
};

template<> struct hash<signed char> {
    size_t operator()(signed char c) const {
        return size_t(c);
    }
};

template<> struct hash<short> {
    size_t operator()(short s) const {
        return size_t(s);
    }
};

template<> struct hash<unsigned short> {
    size_t operator()(unsigned short s) const {
        return size_t(s);
    }
};

template<> struct hash<unsigned int> {
    size_t operator()(unsigned int i) const {
        return size_t(i);
    }
};

template<> struct hash<int> {
    size_t operator()(int i) const {
        return size_t(i);
    }
};

template<> struct hash<unsigned long> {
    size_t operator()(unsigned long l) const {
        return size_t(l);
    }
};

template<> struct hash<long> {
    size_t operator()(long l) const {
        return size_t(l);
    }
};


#if MYSTL_CPP_VERSION >= 11

template<> struct hash<long long> {
    size_t operator()(long long l) const {
        return size_t(l);
    }
};

template<> struct hash<unsigned long long> {
    size_t operator()(unsigned long long l) const {
        return size_t(l);
    }
};

#endif

}


#endif  // STL_HASH_FUN_H
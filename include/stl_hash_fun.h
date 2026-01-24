/*
可以看出源码的哈希函数实现的十分简单,
只是堆数值本身作为哈希值进行一次封装
由此看出,在使用hashtable的时候更推荐使用提供自己的哈希函数
*/
#ifndef STL_HASH_FUN_H
#define STL_HASH_FUN_H


#include "stl_config.h"
#include <string>

namespace msl{

template<typename key> struct hash{ };

inline size_t __stl_hash_string(const char* str) {
    unsigned long h = 0;
    for(; *str ; ++str){
        h = h * 5 + *str;
    }
    return size_t(h);
}

/**************************************************************** */
//非源码,但是我认为比较常用所以添加
inline size_t __stl_hash_string(const std::string& str) {
    unsigned long h = 0;
    for(size_t i = 0; i < str.size(); ++i){
        h = h * 5 + str[i];
    }
    return size_t(h);
} 

template<> struct hash<double> {
    size_t operator()(const double& d) const {
        return size_t(d * 100);
    }
};

template<> struct hash<float> {
    size_t operator()(const float& f) const {
        return size_t(f * 100);
    }
};

/***************************************************************** */


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
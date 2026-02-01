#ifndef MYSTL_FUNCTIONAL_H
#define MYSTL_FUNCTIONAL_H

namespace msl {

// 一元函数基类
template <class Arg, class Result>
struct unary_function {
    typedef Arg argument_type;
    typedef Result result_type;
};

// 二元函数基类
template <class Arg1, class Arg2, class Result>
struct binary_function {
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
};

// 算术类仿函数
template <class T>
struct plus : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x + y; }
};

template <class T>
struct minus : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x - y; }
};

template <class T>
struct multiplies : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x * y; }
};

template <class T>
struct divides : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x / y; }
};

template <class T>
struct modulus : public binary_function<T, T, T> {
    T operator()(const T& x, const T& y) const { return x % y; }
};

template <class T>
struct negate : public unary_function<T, T> {
    T operator()(const T& x) const { return -x; }
};

// 证同元素
template <class T>
T identity_element(const multiplies<T>&) { return T(1); }
template <class T>
T identity_element(const plus<T>&) { return T(0); }



// 关系运算类仿函数
template <class T>
struct equal_to : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x == y; }
};

template <class T>
struct not_equal_to : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x != y; }
};

template <class T>
struct greater : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x > y; }
};

template <class T>
struct less : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

template <class T>
struct greater_equal : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x >= y; }
};

template <class T>
struct less_equal : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x <= y; }
};

// 逻辑运算类仿函数
template <class T>
struct logical_and : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x && y; }
};

template <class T>
struct logical_or : public binary_function<T, T, bool> {
    bool operator()(const T& x, const T& y) const { return x || y; }
};

template <class T>
struct logical_not : public unary_function<T, bool> {
    bool operator()(const T& x) const { return !x; }
};

// 证同（Identity）、选择（Select）、投射（Project）
template <class T>
struct identity : public unary_function<T, T> {
    const T& operator()(const T& x) const { return x; }
};

template <class Pair>
struct select1st : public unary_function<Pair, typename Pair::first_type> {
    const typename Pair::first_type& operator()(const Pair& x) const {
        return x.first;
    }
};

template <class Pair>
struct select2nd : public unary_function<Pair, typename Pair::second_type> {
    const typename Pair::second_type& operator()(const Pair& x) const {
        return x.second;
    }
};

template <class Arg1, class Arg2>
struct project1st : public binary_function<Arg1, Arg2, Arg1> {
    Arg1 operator()(const Arg1& x, const Arg2&) const { return x; }
};

template <class Arg1, class Arg2>
struct project2nd : public binary_function<Arg1, Arg2, Arg2> {
    Arg2 operator()(const Arg1&, const Arg2& y) const { return y; }
};

/**
 * @brief 一元否定仿函数
 * 
 * @tparam Predicate 谓词类型, 
 */
template <class Predicate>
class unary_negate : 
public unary_function<typename Predicate::argument_type, bool> {
protected:
    Predicate pred;
public:
    explicit unary_negate(const Predicate& p) : pred(p) {}
    bool operator()(const typename Predicate::argument_type& x) const {
        return !pred(x);
    }
};

template <class Predicate>
inline unary_negate<Predicate> not1(const Predicate& pred) {
    return unary_negate<Predicate>(pred);
}

template <class Predicate>
class binary_negate :
public binary_function<typename Predicate::first_argument_type,
                       typename Predicate::second_argument_type, bool> {
protected:
    Predicate pred;
public:
    explicit binary_negate(const Predicate& p) : pred(p) {}
    bool operator()(const typename Predicate::first_argument_type& x,
                    const typename Predicate::second_argument_type& y) const {
        return !pred(x, y);
    }
}; 

template<class Predicate>
inline binary_negate<Predicate> not2(const Predicate& pred) {
    return binary_negate<Predicate>(pred);
}

template <class Operation>
class binder1st :
public unary_function<typename Operation::second_argument_type,
                       typename Operation::result_type> {
protected:
    Operation op;
    typename Operation::first_argument_type first;
public:
    binder1st(const Operation& o, const typename Operation::first_argument_type& f)
        : op(o), first(f) {}
    typename Operation::result_type operator()(const typename Operation::second_argument_type& y) const {
        return op(first, y);
    }
};  


/**
 * @brief 绑定第一个参数的仿函数
 * 
 * @tparam Operation 操作类型
 * @param o 操作对象
 * @param f 第一个参数
 * @return binder1st<Operation> 绑定第一个参数的仿函数对象
 */
template <class Operation>
inline binder1st<Operation> bind1st
(const Operation& o, const typename Operation::first_argument_type& f) {
    return binder1st<Operation>(o, f);
}

template <class Operation>
class binder2nd :
public unary_function<typename Operation::first_argument_type,
                      typename Operation::result_type> {
protected:
    Operation op;
    typename Operation::second_argument_type second;
public:
    binder2nd(const Operation& o, const typename Operation::second_argument_type& s)
        : op(o), second(s) {}
    typename Operation::result_type operator()(const typename Operation::first_argument_type& x) const {
        return op(x, second);
    }
};

template <class Operation>
inline binder2nd<Operation> bind2nd
(const Operation& o, const typename Operation::second_argument_type& s) {
    return binder2nd<Operation>(o, s);
}

template <class Operation1, class Operation2>
class unary_compose :
public unary_function<typename Operation2::argument_type,
                      typename Operation1::result_type>{
protected:
    Operation1 op1;
    Operation2 op2;
public:
    unary_compose(const Operation1& p1,const Operation2& p2) : 
    op1(p1), op2(p2) { }
    typename Operation1::result_type operator()
    (const typename Operation2::argument_type& x) const {
        return op1(op2(x));
    }
};

template<class Operation1,class Operation2>
inline unary_compose<Operation1,Operation2> 
compose1(const Operation1& p1,const Operation2& p2) {
    return unary_compose<Operation1,Operation2>(p1,p2);
}

template<class Operation1,class Operation2,class Operation3>
class binary_compose :
public unary_function<typename Operation2::argument_type,
                      typename Operation1::result_type>{
protected:
    Operation1 op1;
    Operation2 op2;
    Operation3 op3;
public:
    binary_compose(const Operation1& p1,const Operation2& p2,const Operation3& p3) : 
    op1(p1), op2(p2), op3(p3) { }
    typename Operation1::result_type operator()
    (const typename Operation2::argument_type& x1) const {
        return op1(op2(x1),op3(x1));
    }
    
};

template<class Operation1,class Operation2,class Operation3>
inline binary_compose<Operation1,Operation2,Operation3> 
compose2(const Operation1& p1,const Operation2& p2,const Operation3& p3) {
    return binary_compose<Operation1,Operation2,Operation3>(p1,p2,p3);
}

template<class Arg,class Result>
class pointer_to_unary_function :
public unary_function<Arg,Result>{
protected:
    Result (*func)(Arg);
public:
    pointer_to_unary_function() : func(0) { }
    pointer_to_unary_function(Result (*f)(Arg)) : func(f) { }
    Result operator()(Arg x) const {
        return func(x);
    }
};

template<class Arg,class Result>
inline pointer_to_unary_function<Arg,Result>
ptr_fun(Result (*f)(Arg)) {
    return pointer_to_unary_function<Arg,Result>(f);
}

template<class Arg1,class Arg2,class Result>
class pointer_to_binary_function :
public binary_function<Arg1,Arg2,Result>{
protected:
    Result (*func)(Arg1,Arg2);
public:
    pointer_to_binary_function() : func(0) { }
    pointer_to_binary_function(Result (*f)(Arg1,Arg2)) : func(f) { }
    Result operator()(Arg1 x,Arg2 y) const {
        return func(x,y);
    }
};

template<class Arg1,class Arg2,class Result>
inline pointer_to_binary_function<Arg1,Arg2,Result>
ptr_fun(Result (*f)(Arg1,Arg2)) {
    return pointer_to_binary_function<Arg1,Arg2,Result>(f);
}

//成员函数适配器需要根据成员函数的参数类型和返回值类型定义
//根据是否为const,有一个或无参数,指针还是引用调用,定义不同的适配器
//一共有8中情况


// 1. no const, no arg, pointer call
template<class S, class T> //S是返回类型,T是类类型
class mem_fun_t :public unary_function<T*,S>{
protected:
    S (T::*func)();
public:
    mem_fun_t(S (T::*f)()) : func(f) { }
    S operator()(T* p) const {
        return (p->*func)();
    }
};

//2.const, no arg, pointer call
template<class S, class T> //S是返回类型,T是类类型
class const_mem_fun_t :public unary_function<const T*,S>{
protected:
    S (T::*func)() const;
public:
    const_mem_fun_t(S (T::*f)() const) : func(f) { }
    S operator()(const T* p) const {
        return (p->*func)();
    }
};

// 3. no const, 1 arg, pointer call
template <class S, class T, class A>
class mem_fun1_t : public binary_function<T*, A, S> {
protected:
    S (T::*func)(A);
public:
    mem_fun1_t(S (T::*f)(A)) : func(f) {}
    S operator()(T* p, A x) const {
        return (p->*func)(x);
    }
};

// 4. const, 1 arg, pointer call
template <class S, class T, class A>
class const_mem_fun1_t : public binary_function<const T*, A, S> {
protected:
    S (T::*func)(A) const;
public:
    const_mem_fun1_t(S (T::*f)(A) const) : func(f) {}
    S operator()(const T* p, A x) const {
        return (p->*func)(x);
    }
};

// 5. no const, no arg, ref call
template <class S, class T>
class mem_fun_ref_t : public unary_function<T, S> {
protected:
    S (T::*func)();
public:
    mem_fun_ref_t(S (T::*f)()) : func(f) {}
    S operator()(T& p) const {
        return (p.*func)();
    }
};

// 6. const, no arg, ref call
template <class S, class T>
class const_mem_fun_ref_t : public unary_function<T, S> {
protected:
    S (T::*func)() const;
public:
    const_mem_fun_ref_t(S (T::*f)() const) : func(f) {}
    S operator()(const T& p) const {
        return (p.*func)();
    }
};

// 7. no const, 1 arg, ref call
template <class S, class T, class A>
class mem_fun1_ref_t : public binary_function<T, A, S> {
protected:
    S (T::*func)(A);
public:
    mem_fun1_ref_t(S (T::*f)(A)) : func(f) {}
    S operator()(T& p, A x) const {
        return (p.*func)(x);
    }
};

// 8. const, 1 arg, ref call
template <class S, class T, class A>
class const_mem_fun1_ref_t : public binary_function<T, A, S> {
protected:
    S (T::*func)(A) const;
public:
    const_mem_fun1_ref_t(S (T::*f)(A) const) : func(f) {}
    S operator()(const T& p, A x) const {
        return (p.*func)(x);
    }
};


// mem_fun helper functions
template <class S, class T>
inline mem_fun_t<S, T> mem_fun(S (T::*f)()) {
    return mem_fun_t<S, T>(f);
}

template <class S, class T>
inline const_mem_fun_t<S, T> mem_fun(S (T::*f)() const) {
    return const_mem_fun_t<S, T>(f);
}

template <class S, class T, class A>
inline mem_fun1_t<S, T, A> mem_fun(S (T::*f)(A)) {
    return mem_fun1_t<S, T, A>(f);
}

template <class S, class T, class A>
inline const_mem_fun1_t<S, T, A> mem_fun(S (T::*f)(A) const) {
    return const_mem_fun1_t<S, T, A>(f);
}

// mem_fun_ref helper functions
template <class S, class T>
inline mem_fun_ref_t<S, T> mem_fun_ref(S (T::*f)()) {
    return mem_fun_ref_t<S, T>(f);
}

template <class S, class T>
inline const_mem_fun_ref_t<S, T> mem_fun_ref(S (T::*f)() const) {
    return const_mem_fun_ref_t<S, T>(f);
}

template <class S, class T, class A>
inline mem_fun1_ref_t<S, T, A> mem_fun_ref(S (T::*f)(A)) {
    return mem_fun1_ref_t<S, T, A>(f);
}

template <class S, class T, class A>
inline const_mem_fun1_ref_t<S, T, A> mem_fun_ref(S (T::*f)(A) const) {
    return const_mem_fun1_ref_t<S, T, A>(f);
}



} // namespace msl

#endif // MYSTL_FUNCTIONAL_H

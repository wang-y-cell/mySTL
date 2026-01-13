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


} // namespace msl

#endif // MYSTL_FUNCTIONAL_H

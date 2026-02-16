#include <iostream>
#include <vector>
#include <algorithm>
#include "stl_functional.h"

void print(){
        std::cout << "=================================================" << std::endl;
}

void test_not(){
    print();
    std::cout << "Testing unary_negate (not1) and binary_negate (not2)..." << std::endl;

    struct IsEven : public msl::unary_function<int, bool> {
        bool operator()(int x) const { return x % 2 == 0; }
    };

    msl::unary_negate<IsEven> is_odd = msl::not1(IsEven());
    
    if (is_odd(3)) {
        std::cout << "not1(IsEven)(3) is true (Correct)" << std::endl;
    } else {
        std::cout << "not1(IsEven)(3) is false (Incorrect)" << std::endl;
    }

    if (!is_odd(4)) {
        std::cout << "not1(IsEven)(4) is false (Correct)" << std::endl;
    } else {
        std::cout << "not1(IsEven)(4) is true (Incorrect)" << std::endl;
    }

    // Test not2 with less<int>
    msl::less<int> less_func;
    msl::binary_negate<msl::less<int> > not_less = msl::not2(less_func);

    // not_less(a, b) should be equivalent to !(a < b), i.e., a >= b
    if (not_less(5, 3)) { // 5 >= 3 is true
        std::cout << "not2(less)(5, 3) is true (Correct)" << std::endl;
    } else {
        std::cout << "not2(less)(5, 3) is false (Incorrect)" << std::endl;
    }

    if (!not_less(2, 4)) { // 2 >= 4 is false
        std::cout << "not2(less)(2, 4) is false (Correct)" << std::endl;
    } else {
        std::cout << "not2(less)(2, 4) is true (Incorrect)" << std::endl;
    }

    // Test logical_not
    msl::logical_not<bool> logical_not_func;
    if (logical_not_func(false)) {
        std::cout << "logical_not(false) is true (Correct)" << std::endl;
    } else {
        std::cout << "logical_not(false) is false (Incorrect)" << std::endl;
    }

}
void test_binders(){
    print();
    std::cout << "Testing bind1st and bind2nd..." << std::endl;

    // Test bind1st with less<int>
    // bind1st(less, 10)(x) <=> 10 < x <=> x > 10
    msl::binder1st<msl::less<int>> greater_than_10 = msl::bind1st(msl::less<int>(), 10);
    
    if (greater_than_10(20)) {
        std::cout << "bind1st(less, 10)(20) is true (Correct)" << std::endl;
    } else {
        std::cout << "bind1st(less, 10)(20) is false (Incorrect)" << std::endl;
    }

    if (!greater_than_10(5)) {
        std::cout << "bind1st(less, 10)(5) is false (Correct)" << std::endl;
    } else {
        std::cout << "bind1st(less, 10)(5) is true (Incorrect)" << std::endl;
    }

    // Test bind2nd with less<int>
    // bind2nd(less, 10)(x) <=> x < 10
    msl::binder2nd<msl::less<int> > less_than_10 = msl::bind2nd(msl::less<int>(), 10);

    if (less_than_10(5)) {
        std::cout << "bind2nd(less, 10)(5) is true (Correct)" << std::endl;
    } else {
        std::cout << "bind2nd(less, 10)(5) is false (Incorrect)" << std::endl;
    }

    if (!less_than_10(20)) {
        std::cout << "bind2nd(less, 10)(20) is false (Correct)" << std::endl;
    } else {
        std::cout << "bind2nd(less, 10)(20) is true (Incorrect)" << std::endl;
    }

    // Test with vector iteration
    int arr[] = { 1, 5, 10, 15, 20 };
    std::vector<int> v(arr, arr + 5);
    
    int count_lt_10 = 0;
    for(size_t i = 0; i < v.size(); ++i) {
        if(less_than_10(v[i])) count_lt_10++;
    }
    
    if (count_lt_10 == 2) { // 1, 5
        std::cout << "Vector iteration with bind2nd success" << std::endl;
    } else {
        std::cout << "Vector iteration with bind2nd failed: " << count_lt_10 << std::endl;
    }

}

void test_compose() {
    print();
    std::cout << "Testing compose1 and compose2..." << std::endl;

    // Test compose1
    // f(x) = -(x + 10)
    // op1 = negate<int>, op2 = bind2nd(plus<int>, 10)
    // compose1(negate, bind2nd(plus, 10))(x) -> negate(plus(x, 10))
    msl::unary_compose<msl::negate<int>, msl::binder2nd<msl::plus<int> > > 
        comp1 = msl::compose1(msl::negate<int>(), msl::bind2nd(msl::plus<int>(), 10));

    if (comp1(20) == -30) {
        std::cout << "compose1(negate, bind2nd(plus, 10))(20) == -30 (Correct)" << std::endl;
    } else {
        std::cout << "compose1 failed: " << comp1(20) << std::endl;
    }

    // Test compose2
    // f(x) = (x + 10) * (x - 5)
    // op1 = multiplies<int>
    // op2 = bind2nd(plus<int>, 10)
    // op3 = bind2nd(minus<int>, 5)
    msl::binary_compose<msl::multiplies<int>, msl::binder2nd<msl::plus<int> >, msl::binder2nd<msl::minus<int> > >
        comp2 = msl::compose2(msl::multiplies<int>(), 
                              msl::bind2nd(msl::plus<int>(), 10), 
                              msl::bind2nd(msl::minus<int>(), 5));

    // x = 20 -> (20+10) * (20-5) = 30 * 15 = 450
    if (comp2(20) == 450) {
        std::cout << "compose2(mult, plus10, minus5)(20) == 450 (Correct)" << std::endl;
    } else {
        std::cout << "compose2 failed: " << comp2(20) << std::endl;
    }
}

// Global functions for ptr_fun test
int square(int x) {
    return x * x;
}

int sum(int x, int y) {
    return x + y;
}

void test_ptr_fun() {
    print();
    std::cout << "Testing ptr_fun..." << std::endl;

    // Test unary ptr_fun
    msl::pointer_to_unary_function<int, int> ptr_square = msl::ptr_fun(square);
    if (ptr_square(5) == 25) {
        std::cout << "ptr_fun(square)(5) == 25 (Correct)" << std::endl;
    } else {
        std::cout << "ptr_fun(square)(5) failed: " << ptr_square(5) << std::endl;
    }

    // Test with compose1 to verify adaptability
    // compose1(negate, ptr_fun(square)) -> -(x*x)
    msl::unary_compose<msl::negate<int>, msl::pointer_to_unary_function<int, int> > 
        neg_square = msl::compose1(msl::negate<int>(), msl::ptr_fun(square));
    
    if (neg_square(5) == -25) {
        std::cout << "compose1(negate, ptr_fun(square))(5) == -25 (Correct)" << std::endl;
    } else {
        std::cout << "compose1(negate, ptr_fun(square))(5) failed: " << neg_square(5) << std::endl;
    }

    // Test binary ptr_fun
    msl::pointer_to_binary_function<int, int, int> ptr_sum = msl::ptr_fun(sum);
    if (ptr_sum(10, 20) == 30) {
        std::cout << "ptr_fun(sum)(10, 20) == 30 (Correct)" << std::endl;
    } else {
        std::cout << "ptr_fun(sum)(10, 20) failed: " << ptr_sum(10, 20) << std::endl;
    }

    // Test with bind1st to verify adaptability
    // bind1st(ptr_fun(sum), 10)(x) -> 10 + x
    msl::binder1st<msl::pointer_to_binary_function<int, int, int> > 
        add10 = msl::bind1st(msl::ptr_fun(sum), 10);
    
    if (add10(20) == 30) {
        std::cout << "bind1st(ptr_fun(sum), 10)(20) == 30 (Correct)" << std::endl;
    } else {
        std::cout << "bind1st(ptr_fun(sum), 10)(20) failed: " << add10(20) << std::endl;
    }
}

class TestClass {
public:
    int val;
    TestClass(int v) : val(v) {}

    // 1. no const, no arg
    int func() { return val; }

    // 2. const, no arg
    int const_func() const { return val; }

    // 3. no const, 1 arg
    int func1(int x) { return val + x; }

    // 4. const, 1 arg
    int const_func1(int x) const { return val + x; }
};
void test_mem_fun() {
    print();
    std::cout << "Testing mem_fun..." << std::endl;
    TestClass obj(10);
    TestClass* pObj = &obj;

    // 1. mem_fun_t
    // msl::mem_fun(&TestClass::func) returns mem_fun_t
    // operator()(TestClass*)
    if (msl::mem_fun(&TestClass::func)(pObj) == 10) {
        std::cout << "mem_fun_t passed" << std::endl;
    } else {
        std::cout << "mem_fun_t failed" << std::endl;
    }

    // 2. const_mem_fun_t
    const TestClass constObj(20);
    const TestClass* pConstObj = &constObj;
    if (msl::mem_fun(&TestClass::const_func)(pConstObj) == 20) {
        std::cout << "const_mem_fun_t passed" << std::endl;
    } else {
        std::cout << "const_mem_fun_t failed" << std::endl;
    }

    // 3. mem_fun1_t
    if (msl::mem_fun(&TestClass::func1)(pObj, 5) == 15) {
        std::cout << "mem_fun1_t passed" << std::endl;
    } else {
        std::cout << "mem_fun1_t failed" << std::endl;
    }

    // 4. const_mem_fun1_t
    if (msl::mem_fun(&TestClass::const_func1)(pConstObj, 5) == 25) {
        std::cout << "const_mem_fun1_t passed" << std::endl;
    } else {
        std::cout << "const_mem_fun1_t failed" << std::endl;
    }
}
void test_mem_fun_ref() {
    print();
    std::cout << "Testing mem_fun_ref..." << std::endl;
    TestClass obj(10);

    // 5. mem_fun_ref_t
    if (msl::mem_fun_ref(&TestClass::func)(obj) == 10) {
        std::cout << "mem_fun_ref_t passed" << std::endl;
    } else {
        std::cout << "mem_fun_ref_t failed" << std::endl;
    }

    // 6. const_mem_fun_ref_t
    const TestClass constObj(20);
    if (msl::mem_fun_ref(&TestClass::const_func)(constObj) == 20) {
        std::cout << "const_mem_fun_ref_t passed" << std::endl;
    } else {
        std::cout << "const_mem_fun_ref_t failed" << std::endl;
    }

    // 7. mem_fun1_ref_t
    if (msl::mem_fun_ref(&TestClass::func1)(obj, 5) == 15) {
        std::cout << "mem_fun1_ref_t passed" << std::endl;
    } else {
        std::cout << "mem_fun1_ref_t failed" << std::endl;
    }

    // 8. const_mem_fun1_ref_t
    if (msl::mem_fun_ref(&TestClass::const_func1)(constObj, 5) == 25) {
        std::cout << "const_mem_fun1_ref_t passed" << std::endl;
    } else {
        std::cout << "const_mem_fun1_ref_t failed" << std::endl;
    }
}



int main() {
    test_not();
    test_binders();
    test_compose();
    test_ptr_fun();
    test_mem_fun();
    test_mem_fun_ref();
    return 0;
}

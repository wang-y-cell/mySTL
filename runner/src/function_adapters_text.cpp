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

int main() {
    test_not();
    test_binders();
    test_compose();
    return 0;
}

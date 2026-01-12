#include <iostream>
#include <vector>
#include <algorithm>
#include "stl_functional.h"

void test_not(){
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


int main() {
    test_not();
    return 0;
}

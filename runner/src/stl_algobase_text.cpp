#include "stl_algobase.h"
#include <iostream>
#include <vector>
#include <algorithm> // For comparison if needed, but we use msl

void test_equal() {
    int a[] = {1, 2, 3, 4, 5};
    int b[] = {1, 2, 3, 4, 5};
    int c[] = {1, 2, 3, 4, 6};

    std::cout << "Test equal: ";
    if (msl::equal(a, a + 5, b)) {
        std::cout << "PASS ";
    } else {
        std::cout << "FAIL ";
    }

    if (!msl::equal(a, a + 5, c)) {
        std::cout << "PASS" << std::endl;
    } else {
        std::cout << "FAIL" << std::endl;
    }
}

void test_fill() {
    int a[5];
    msl::fill(a, a + 5, 9);
    
    std::cout << "Test fill: ";
    bool pass = true;
    for (int i = 0; i < 5; ++i) {
        if (a[i] != 9) pass = false;
    }
    if (pass) std::cout << "PASS" << std::endl;
    else std::cout << "FAIL" << std::endl;
}

void test_fill_n() {
    int a[5] = {0};
    msl::fill_n(a, 3, 7);

    std::cout << "Test fill_n: ";
    if (a[0] == 7 && a[1] == 7 && a[2] == 7 && a[3] == 0) {
        std::cout << "PASS" << std::endl;
    } else {
        std::cout << "FAIL" << std::endl;
    }
}

void test_iter_swap() {
    int a = 10, b = 20;
    msl::iter_swap(&a, &b);

    std::cout << "Test iter_swap: ";
    if (a == 20 && b == 10) {
        std::cout << "PASS" << std::endl;
    } else {
        std::cout << "FAIL" << std::endl;
    }
}

void test_lexicographical_compare() {
    char s1[] = "abc";
    char s2[] = "abd";
    char s3[] = "abc";
    char s4[] = "aba";

    std::cout << "Test lexicographical_compare: ";
    
    // s1 < s2 should be true
    bool res1 = msl::lexicographical_compare(s1, s1 + 3, s2, s2 + 3);
    // s1 < s3 should be false (equal)
    bool res2 = msl::lexicographical_compare(s1, s1 + 3, s3, s3 + 3);
    // s1 < s4 should be false (greater)
    bool res3 = msl::lexicographical_compare(s1, s1 + 3, s4, s4 + 3);

    if (res1 && !res2 && !res3) {
        std::cout << "PASS" << std::endl;
    } else {
        std::cout << "FAIL (res1=" << res1 << ", res2=" << res2 << ", res3=" << res3 << ")" << std::endl;
    }
}

void test_max_min() {
    int a = 10, b = 20;
    std::cout << "Test max/min: ";
    if (msl::max(a, b) == 20 && msl::min(a, b) == 10) {
        std::cout << "PASS" << std::endl;
    } else {
        std::cout << "FAIL" << std::endl;
    }
}

int main() {
    test_equal();
    test_fill();
    test_fill_n();
    test_iter_swap();
    test_lexicographical_compare();
    test_max_min();
    return 0;
}

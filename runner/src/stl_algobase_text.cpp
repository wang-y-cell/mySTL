#include "stl_algobase.h"
#include <iostream>
#include <vector>
#include <algorithm> // For comparison if needed, but we use msl

void print(){
    std::cout << "==========================================" << std::endl;
}
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

void test_copy() {
    std::cout << "Test copy: ";
    int a[] = {1, 2, 3, 4, 5};
    int b[5];
    msl::copy(a, a + 5, b);
    if (msl::equal(a, a + 5, b)) std::cout << "PASS ";
    else std::cout << "FAIL ";

    char c[] = "hello";
    char d[6];
    msl::copy(c, c + 6, d);
    if (msl::equal(c, c + 6, d)) std::cout << "PASS ";
    else std::cout << "FAIL ";

    wchar_t wc[] = L"hello";
    wchar_t wd[6];
    msl::copy(wc, wc + 6, wd);
    if (msl::equal(wc, wc + 6, wd)) std::cout << "PASS" << std::endl;
    else std::cout << "FAIL" << std::endl;
}

void test_copy_backward() {
    std::cout << "Test copy_backward: ";
    int a[] = {1, 2, 3, 4, 5};
    int b[5];
    msl::copy_backward(a, a + 5, b + 5);
    if (msl::equal(a, a + 5, b)) std::cout << "PASS ";
    else std::cout << "FAIL ";

    // Overlapping test: 1 2 3 4 5 -> 1 1 2 3 4 5 (in c+1 to c+6)
    int c[] = {1, 2, 3, 4, 5, 0};
    // Copy [c, c+5) to ending at c+6. i.e. c+1...c+6
    // Source: 1 2 3 4 5
    // Dest range end: c+6. Dest range start: c+1.
    // Result should be 1 1 2 3 4 5
    msl::copy_backward(c, c + 5, c + 6);
    int expected[] = {1, 1, 2, 3, 4, 5};
    if (msl::equal(c, c + 6, expected)) std::cout << "PASS ";
    else std::cout << "FAIL ";

    char s[] = "hello";
    char d[6];
    msl::copy_backward(s, s + 6, d + 6);
    if (msl::equal(s, s + 6, d)) std::cout << "PASS ";
    else std::cout << "FAIL ";

    wchar_t ws[] = L"world";
    wchar_t wd[6];
    msl::copy_backward(ws, ws + 6, wd + 6);
    if (msl::equal(ws, ws + 6, wd)) std::cout << "PASS" << std::endl;
    else std::cout << "FAIL" << std::endl;
}

int main() {
    print();
    test_equal();
    test_fill();
    test_fill_n();
    test_iter_swap();
    test_lexicographical_compare();
    test_max_min();
    test_copy();
    test_copy_backward();
    return 0;
}

#include <iostream>
#include <vector>
#include <algorithm> // 用于 std::sort
#include "stl_algo.h"
#include "vector.h"
#include "set.h"

bool compare_desc(int a, int b) {
    return a > b;
}

void test_set_union() {
    // ==========================================
    // 测试 set_union
    // ==========================================
    std::cout << "==========================================" << std::endl;
    std::cout << "Testing set_union..." << std::endl;

    // 测试用例 1: 默认比较
    {
        int a1[] = {1, 3, 5, 7, 9};
        int a2[] = {2, 3, 4, 5, 6};
        msl::set<int> v1(a1, a1 + 5);
        msl::set<int> v2(a2, a2 + 5);
        msl::vector<int> result(10); 

        auto it = msl::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());
        result.resize(it - result.begin());

        std::cout << "Union of {1,3,5,7,9} and {2,3,4,5,6}: ";
        for (auto x : result) std::cout << x << " ";
        std::cout << std::endl;

        int expected[] = {1, 2, 3, 4, 5, 6, 7, 9};
        bool pass = (result.size() == 8);
        if (pass) {
            for (size_t i = 0; i < 8; ++i) {
                if (result[i] != expected[i]) {
                    pass = false;
                    break;
                }
            }
        }
        std::cout << "Test Case 1: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // 测试用例 2: 自定义比较
    {
        int a1[] = {9, 7, 5, 3, 1};
        int a2[] = {6, 5, 4, 3, 2};
        msl::set<int, msl::greater<int>> v1(a1, a1 + 5);
        msl::set<int, msl::greater<int>> v2(a2, a2 + 5);
        msl::vector<int> result(10);

        auto it = msl::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin(), compare_desc);
        result.resize(it - result.begin());

        std::cout << "Union of {9,7,5,3,1} and {6,5,4,3,2} (desc): ";
        for (auto x : result) std::cout << x << " ";
        std::cout << std::endl;

        int expected[] = {9, 7, 6, 5, 4, 3, 2, 1};
        bool pass = (result.size() == 8);
        if (pass) {
            for (size_t i = 0; i < 8; ++i) {
                if (result[i] != expected[i]) {
                    pass = false;
                    break;
                }
            }
        }
        std::cout << "Test Case 2: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
    
    // 测试用例 3: 空范围
    {
         msl::set<int> v1;
         int a2[] = {1, 2, 3};
         msl::set<int> v2(a2, a2 + 3);
         msl::vector<int> result(5);
         
         auto it = msl::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());
         result.resize(it - result.begin());
         
         bool pass = (result.size() == 3 && result[0] == 1 && result[1] == 2 && result[2] == 3);
         std::cout << "Test Case 3 (Empty v1): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_set_intersection() {
    // ==========================================
    // 测试 set_intersection
    // ==========================================
    std::cout << "==========================================" << std::endl;
    std::cout << "Testing set_intersection..." << std::endl;

    // 测试用例 4: 默认比较
    {
        int a1[] = {1, 3, 5, 7, 9};
        int a2[] = {2, 3, 4, 5, 6};
        msl::set<int> v1(a1, a1 + 5);
        msl::set<int> v2(a2, a2 + 5);
        msl::vector<int> result(10);

        auto it = msl::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());
        result.resize(it - result.begin());

        std::cout << "Intersection of {1,3,5,7,9} and {2,3,4,5,6}: ";
        for (auto x : result) std::cout << x << " ";
        std::cout << std::endl;

        // 预期结果: 3 5
        int expected[] = {3, 5};
        bool pass = (result.size() == 2);
        if (pass) {
            for (size_t i = 0; i < 2; ++i) {
                if (result[i] != expected[i]) {
                    pass = false;
                    break;
                }
            }
        }
        std::cout << "Test Case 4: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // 测试用例 5: 自定义比较
    {
        int a1[] = {9, 7, 5, 3, 1};
        int a2[] = {6, 5, 4, 3, 2};
        msl::set<int, msl::greater<int>> v1(a1, a1 + 5);
        msl::set<int, msl::greater<int>> v2(a2, a2 + 5);
        msl::vector<int> result(10);

        auto it = msl::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin(), compare_desc);
        result.resize(it - result.begin());

        std::cout << "Intersection of {9,7,5,3,1} and {6,5,4,3,2} (desc): ";
        for (auto x : result) std::cout << x << " ";
        std::cout << std::endl;

        // 预期结果: 5 3
        int expected[] = {5, 3};
        bool pass = (result.size() == 2);
        if (pass) {
            for (size_t i = 0; i < 2; ++i) {
                if (result[i] != expected[i]) {
                    pass = false;
                    break;
                }
            }
        }
        std::cout << "Test Case 5: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // 测试用例 6: 无交集
    {
        int a1[] = {1, 2, 3};
        int a2[] = {4, 5, 6};
        msl::set<int> v1(a1, a1 + 3);
        msl::set<int> v2(a2, a2 + 3);
        msl::vector<int> result(10);

        auto it = msl::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());
        result.resize(it - result.begin());

        std::cout << "Intersection of {1,2,3} and {4,5,6}: ";
        for (auto x : result) std::cout << x << " ";
        std::cout << std::endl;

        bool pass = (result.empty());
        std::cout << "Test Case 6: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_set_difference() {
    // ==========================================
    // 测试 set_difference
    // ==========================================
    std::cout << "==========================================" << std::endl;
    std::cout << "Testing set_difference..." << std::endl;

    // 测试用例 7: 默认比较
    {
        int a1[] = {1, 3, 5, 7, 9};
        int a2[] = {2, 3, 4, 5, 6};
        msl::set<int> v1(a1, a1 + 5);
        msl::set<int> v2(a2, a2 + 5);
        msl::vector<int> result(10);

        auto it = msl::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());
        result.resize(it - result.begin());

        std::cout << "Difference of {1,3,5,7,9} and {2,3,4,5,6}: ";
        for (auto x : result) std::cout << x << " ";
        std::cout << std::endl;

        // 预期结果: 1 7 9
        int expected[] = {1, 7, 9};
        bool pass = (result.size() == 3);
        if (pass) {
            for (size_t i = 0; i < 3; ++i) {
                if (result[i] != expected[i]) {
                    pass = false;
                    break;
                }
            }
        }
        std::cout << "Test Case 7: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // 测试用例 8: 自定义比较
    {
        int a1[] = {9, 7, 5, 3, 1};
        int a2[] = {6, 5, 4, 3, 2};
        msl::set<int, msl::greater<int>> v1(a1, a1 + 5);
        msl::set<int, msl::greater<int>> v2(a2, a2 + 5);
        msl::vector<int> result(10);

        auto it = msl::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin(), compare_desc);
        result.resize(it - result.begin());

        std::cout << "Difference of {9,7,5,3,1} and {6,5,4,3,2} (desc): ";
        for (auto x : result) std::cout << x << " ";
        std::cout << std::endl;

        // 预期结果: 9 7 1
        int expected[] = {9, 7, 1};
        bool pass = (result.size() == 3);
        if (pass) {
            for (size_t i = 0; i < 3; ++i) {
                if (result[i] != expected[i]) {
                    pass = false;
                    break;
                }
            }
        }
        std::cout << "Test Case 8: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // 测试用例 9: 所有元素都在 v2 中 (结果为空)
    {
        int a1[] = {1, 3};
        int a2[] = {1, 2, 3, 4};
        msl::set<int> v1(a1, a1 + 2);
        msl::set<int> v2(a2, a2 + 4);
        msl::vector<int> result(10);

        auto it = msl::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());
        result.resize(it - result.begin());

        std::cout << "Difference of {1,3} and {1,2,3,4}: ";
        for (auto x : result) std::cout << x << " ";
        std::cout << std::endl;

        bool pass = (result.empty());
        std::cout << "Test Case 9: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_set_symmetric_difference() {
    // ==========================================
    // 测试 set_symmetric_difference
    // ==========================================
    std::cout << "==========================================" << std::endl;
    std::cout << "Testing set_symmetric_difference..." << std::endl;

    // 测试用例 10: 默认比较
    {
        int a1[] = {1, 3, 5, 7, 9};
        int a2[] = {2, 3, 4, 5, 6};
        msl::set<int> v1(a1, a1 + 5);
        msl::set<int> v2(a2, a2 + 5);
        msl::vector<int> result(20);

        auto it = msl::set_symmetric_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());
        result.resize(it - result.begin());

        std::cout << "Symmetric Difference of {1,3,5,7,9} and {2,3,4,5,6}: ";
        for (auto x : result) std::cout << x << " ";
        std::cout << std::endl;

        // 预期结果: 1 2 4 6 7 9
        int expected[] = {1, 2, 4, 6, 7, 9};
        bool pass = (result.size() == 6);
        if (pass) {
            for (size_t i = 0; i < 6; ++i) {
                if (result[i] != expected[i]) {
                    pass = false;
                    break;
                }
            }
        }
        std::cout << "Test Case 10: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // 测试用例 11: 自定义比较
    {
        int a1[] = {9, 7, 5, 3, 1};
        int a2[] = {6, 5, 4, 3, 2};
        msl::set<int, msl::greater<int>> v1(a1, a1 + 5);
        msl::set<int, msl::greater<int>> v2(a2, a2 + 5);
        msl::vector<int> result(20);

        auto it = msl::set_symmetric_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin(), compare_desc);
        result.resize(it - result.begin());

        std::cout << "Symmetric Difference of {9,7,5,3,1} and {6,5,4,3,2} (desc): ";
        for (auto x : result) std::cout << x << " ";
        std::cout << std::endl;

        // 预期结果: 9 7 6 4 2 1
        int expected[] = {9, 7, 6, 4, 2, 1};
        bool pass = (result.size() == 6);
        if (pass) {
            for (size_t i = 0; i < 6; ++i) {
                if (result[i] != expected[i]) {
                    pass = false;
                    break;
                }
            }
        }
        std::cout << "Test Case 11: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // 测试用例 12: 空范围
    {
        msl::set<int> v1;
        int a2[] = {1, 2, 3};
        msl::set<int> v2(a2, a2 + 3);
        msl::vector<int> result(10);

        auto it = msl::set_symmetric_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());
        result.resize(it - result.begin());

        std::cout << "Symmetric Difference of {} and {1,2,3}: ";
        for (auto x : result) std::cout << x << " ";
        std::cout << std::endl;

        int expected[] = {1, 2, 3};
        bool pass = (result.size() == 3);
        if (pass) {
            for (size_t i = 0; i < 3; ++i) {
                if (result[i] != expected[i]) {
                    pass = false;
                    break;
                }
            }
        }
        std::cout << "Test Case 12: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

int main() {
    test_set_union();
    std::cout << std::endl;
    test_set_intersection();
    std::cout << std::endl;
    test_set_difference();
    std::cout << std::endl;
    test_set_symmetric_difference();

    return 0;
}

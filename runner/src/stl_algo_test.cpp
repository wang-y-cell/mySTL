#include <iostream>
#include <vector>
#include <algorithm> // 用于 std::sort
#include <cmath>
#include <cctype>
#include "stl_algo.h"
#include "vector.h"
#include "set.h"

bool compare_desc(int a, int b) {
    return a > b;
}

void print(){
    std::cout << "==========================================" << std::endl;
}

void test_set_union() {
    // ==========================================
    // 测试 set_union
    // ==========================================
    print();
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
    print();
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
    print();
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
    print();
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

void test_adjacent_find() {
    // ==========================================
    // 测试 adjacent_find
    // ==========================================
    print();
    std::cout << "Testing adjacent_find..." << std::endl;

    // 测试用例 13: 默认比较
    {
        int a[] = {1, 2, 3, 3, 4, 5, 6};
        msl::vector<int> v(a, a + 7);

        auto it = msl::adjacent_find(v.begin(), v.end());
        std::cout << "Adjacent Find of {1,2,3,3,4,5,6}: " << *it << std::endl;

        bool pass = (it == v.begin() + 2);
        std::cout << "Test Case 13: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_count() {
    // ==========================================
    // 测试 count 和 count_if
    // ==========================================
    print();
    std::cout << "Testing count and count_if..." << std::endl;

    // 测试 count
    {
        int a[] = {1, 2, 3, 3, 4, 3, 5};
        msl::vector<int> v(a, a + 7);

        auto c = msl::count(v.begin(), v.end(), 3);
        std::cout << "Count of 3 in {1,2,3,3,4,3,5}: " << c << std::endl;

        bool pass = (c == 3);
        std::cout << "Test Case count: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // 测试 count_if
    {
        int a[] = {1, 2, 3, 4, 5, 6};
        msl::vector<int> v(a, a + 6);

        // 统计偶数个数
        struct IsEven {
            bool operator()(int x) const { return x % 2 == 0; }
        };
        
        auto c = msl::count_if(v.begin(), v.end(), IsEven());
        std::cout << "Count of even numbers in {1,2,3,4,5,6}: " << c << std::endl;

        bool pass = (c == 3);
        std::cout << "Test Case count_if: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_find() {
    // ==========================================
    // 测试 find 和 find_if
    // ==========================================
    print();
    std::cout << "Testing find and find_if..." << std::endl;

    // 测试 find
    {
        int a[] = {10, 20, 30, 40};
        msl::vector<int> v(a, a + 4);

        auto it = msl::find(v.begin(), v.end(), 30);
        if (it != v.end()) {
            std::cout << "Found 30 at index: " << (it - v.begin()) << std::endl;
        } else {
            std::cout << "30 not found" << std::endl;
        }
        
        bool pass = (it != v.end() && *it == 30);
        std::cout << "Test Case find (exists): " << (pass ? "PASSED" : "FAILED") << std::endl;

        auto it2 = msl::find(v.begin(), v.end(), 50);
        if (it2 == v.end()) {
            std::cout << "50 not found (correct)" << std::endl;
        }

        bool pass2 = (it2 == v.end());
        std::cout << "Test Case find (not exists): " << (pass2 ? "PASSED" : "FAILED") << std::endl;
    }

    // 测试 find_if
    {
        int a[] = {1, 3, 5, 8, 9};
        msl::vector<int> v(a, a + 5);

        // 查找第一个偶数
        struct IsEven {
            bool operator()(int x) const { return x % 2 == 0; }
        };

        auto it = msl::find_if(v.begin(), v.end(), IsEven());
        if (it != v.end()) {
            std::cout << "Found first even number: " << *it << std::endl;
        }

        bool pass = (it != v.end() && *it == 8);
        std::cout << "Test Case find_if: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_search() {
    // ==========================================
    // 测试 search
    // ==========================================
    print();
    std::cout << "Testing search..." << std::endl;

    // Test case 1: Normal match
    {
        int a[] = {1, 2, 3, 4, 5, 6};
        int sub[] = {3, 4, 5};
        msl::vector<int> v(a, a + 6);
        msl::vector<int> s(sub, sub + 3);

        auto it = msl::search(v.begin(), v.end(), s.begin(), s.end());
        bool pass = (it != v.end() && *it == 3 && *(it + 1) == 4);
        std::cout << "Test Case search (normal match): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 2: No match
    {
        int a[] = {1, 2, 3, 4, 5, 6};
        int sub[] = {3, 5};
        msl::vector<int> v(a, a + 6);
        msl::vector<int> s(sub, sub + 2);

        auto it = msl::search(v.begin(), v.end(), s.begin(), s.end());
        bool pass = (it == v.end());
        std::cout << "Test Case search (no match): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 3: Match at beginning
    {
        int a[] = {1, 2, 3};
        int sub[] = {1, 2};
        msl::vector<int> v(a, a + 3);
        msl::vector<int> s(sub, sub + 2);

        auto it = msl::search(v.begin(), v.end(), s.begin(), s.end());
        bool pass = (it == v.begin());
        std::cout << "Test Case search (match at beginning): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 4: Match at end
    {
        int a[] = {1, 2, 3};
        int sub[] = {2, 3};
        msl::vector<int> v(a, a + 3);
        msl::vector<int> s(sub, sub + 2);

        auto it = msl::search(v.begin(), v.end(), s.begin(), s.end());
        bool pass = (it != v.end() && *it == 2);
        std::cout << "Test Case search (match at end): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 5: Empty subsequence (should return first1)
    {
        int a[] = {1, 2, 3};
        int sub[] = {};
        msl::vector<int> v(a, a + 3);
        msl::vector<int> s(sub, sub + 0);

        auto it = msl::search(v.begin(), v.end(), s.begin(), s.end());
        bool pass = (it == v.begin());
        std::cout << "Test Case search (empty subsequence): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 6: Longer subsequence than sequence (should return last1)
    {
        int a[] = {1, 2};
        int sub[] = {1, 2, 3};
        msl::vector<int> v(a, a + 2);
        msl::vector<int> s(sub, sub + 3);

        auto it = msl::search(v.begin(), v.end(), s.begin(), s.end());
        bool pass = (it == v.end());
        std::cout << "Test Case search (longer subsequence): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
    
    // Test case 7: Partial match then fail then match
    {
        int a[] = {1, 2, 1, 2, 3};
        int sub[] = {1, 2, 3};
        msl::vector<int> v(a, a + 5);
        msl::vector<int> s(sub, sub + 3);

        auto it = msl::search(v.begin(), v.end(), s.begin(), s.end());
        bool pass = (it != v.end() && (it - v.begin()) == 2);
        std::cout << "Test Case search (partial match retry): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 8: Predicate search (case insensitive or similar, here using modulo)
    {
        int a[] = {10, 21, 32, 43, 54};
        int sub[] = {1, 2, 3}; // Matches 21, 32, 43 mod 10 -> 1, 2, 3
        msl::vector<int> v(a, a + 5);
        msl::vector<int> s(sub, sub + 3);

        struct Mod10Equal {
            bool operator()(int x, int y) const {
                return (x % 10) == (y % 10);
            }
        };

        auto it = msl::search(v.begin(), v.end(), s.begin(), s.end(), Mod10Equal());
        bool pass = (it != v.end() && *it == 21);
        std::cout << "Test Case search (predicate): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_search_n() {
    // ==========================================
    // 测试 search_n
    // ==========================================
    print();
    std::cout << "Testing search_n..." << std::endl;

    // Test case 1: Normal match
    {
        int a[] = {1, 2, 3, 3, 3, 4};
        msl::vector<int> v(a, a + 6);

        auto it = msl::search_n(v.begin(), v.end(), 3, 3);
        bool pass = (it != v.end() && *it == 3 && *(it + 1) == 3 && *(it + 2) == 3);
        std::cout << "Test Case search_n (normal match): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 2: No match
    {
        int a[] = {1, 2, 3, 3, 4};
        msl::vector<int> v(a, a + 5);

        auto it = msl::search_n(v.begin(), v.end(), 3, 3);
        bool pass = (it == v.end());
        std::cout << "Test Case search_n (no match): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 3: Count is 0
    {
        int a[] = {1, 2, 3};
        msl::vector<int> v(a, a + 3);

        auto it = msl::search_n(v.begin(), v.end(), 0, 3);
        bool pass = (it == v.begin());
        std::cout << "Test Case search_n (count 0): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 4: Match at beginning
    {
        int a[] = {3, 3, 1, 2};
        msl::vector<int> v(a, a + 4);

        auto it = msl::search_n(v.begin(), v.end(), 2, 3);
        bool pass = (it == v.begin());
        std::cout << "Test Case search_n (match at beginning): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 5: Match at end
    {
        int a[] = {1, 2, 3, 3};
        msl::vector<int> v(a, a + 4);

        auto it = msl::search_n(v.begin(), v.end(), 2, 3);
        bool pass = (it != v.end() && *it == 3);
        std::cout << "Test Case search_n (match at end): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 6: Predicate
    {
        int a[] = {10, 20, 30, 31, 32, 40};
        msl::vector<int> v(a, a + 6);

        // Find 3 consecutive elements > 25
        struct GreaterThan {
            bool operator()(int x, int val) const {
                return x > val;
            }
        };

        auto it = msl::search_n(v.begin(), v.end(), 3, 25, GreaterThan());
        bool pass = (it != v.end() && *it == 30);
        std::cout << "Test Case search_n (predicate): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_find_end() {
    // ==========================================
    // 测试 find_end
    // ==========================================
    print();
    std::cout << "Testing find_end..." << std::endl;

    // Test case 1: Normal match (ForwardIterator logic mostly, but vector is RandomAccess)
    {
        int a[] = {1, 2, 3, 1, 2, 3, 4};
        int sub[] = {1, 2, 3};
        msl::vector<int> v(a, a + 7);
        msl::vector<int> s(sub, sub + 3);

        auto it = msl::find_end(v.begin(), v.end(), s.begin(), s.end());
        bool pass = (it == v.begin() + 3);
        std::cout << "Test Case find_end (normal match): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 2: No match
    {
        int a[] = {1, 2, 3, 4};
        int sub[] = {5, 6};
        msl::vector<int> v(a, a + 4);
        msl::vector<int> s(sub, sub + 2);

        auto it = msl::find_end(v.begin(), v.end(), s.begin(), s.end());
        bool pass = (it == v.end());
        std::cout << "Test Case find_end (no match): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 3: Predicate
    {
        int a[] = {1, 2, 3, -1, -2, -3, 4};
        int sub[] = {1, 2, 3};
        msl::vector<int> v(a, a + 7);
        msl::vector<int> s(sub, sub + 3);

        struct AbsEqual {
            bool operator()(int x, int y) const {
                return std::abs(x) == std::abs(y);
            }
        };

        auto it = msl::find_end(v.begin(), v.end(), s.begin(), s.end(), AbsEqual());
        bool pass = (it == v.begin() + 3); // Should match -1, -2, -3
        std::cout << "Test Case find_end (predicate): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_find_first_of() {
    // ==========================================
    // 测试 find_first_of
    // ==========================================
    print();
    std::cout << "Testing find_first_of..." << std::endl;

    // Test case 1: Normal match
    {
        int a[] = {0, 1, 2, 3, 4, 5};
        int targets[] = {100, 5, 3}; // 3 appears first in 'a' (index 3), 5 appears later (index 5)
        msl::vector<int> v(a, a + 6);
        msl::vector<int> t(targets, targets + 3);

        auto it = msl::find_first_of(v.begin(), v.end(), t.begin(), t.end());
        bool pass = (it != v.end() && *it == 3);
        std::cout << "Test Case find_first_of (normal match): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 2: No match
    {
        int a[] = {1, 2};
        int targets[] = {3, 4};
        msl::vector<int> v(a, a + 2);
        msl::vector<int> t(targets, targets + 2);

        auto it = msl::find_first_of(v.begin(), v.end(), t.begin(), t.end());
        bool pass = (it == v.end());
        std::cout << "Test Case find_first_of (no match): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 3: Predicate
    {
        char text[] = "Hello World";
        char vowels[] = "aeiou";
        
        // 'e' is the first vowel in "Hello World"
        char* it = msl::find_first_of(text, text + 11, vowels, vowels + 5);
        bool pass1 = (*it == 'e');
        
        char text2[] = "HELLO WORLD";
        // Case insensitive match for 'E'
        struct CaseInsensitiveEqual {
            bool operator()(char a, char b) const {
                return std::tolower(a) == std::tolower(b);
            }
        };
        
        char* it2 = msl::find_first_of(text2, text2 + 11, vowels, vowels + 5, CaseInsensitiveEqual());
        bool pass2 = (*it2 == 'E');

        std::cout << "Test Case find_first_of (predicate): " << ((pass1 && pass2) ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_includes() {
    // ==========================================
    // 测试 includes
    // ==========================================
    print();
    std::cout << "Testing includes..." << std::endl;

    // Test case 1: Subset (true)
    {
        int a1[] = {1, 2, 3, 4, 5};
        int a2[] = {1, 3, 5};
        msl::vector<int> v1(a1, a1 + 5);
        msl::vector<int> v2(a2, a2 + 3);

        bool result = msl::includes(v1.begin(), v1.end(), v2.begin(), v2.end());
        std::cout << "Test Case includes (subset): " << (result ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 2: Not a subset (false)
    {
        int a1[] = {1, 2, 3, 4, 5};
        int a2[] = {1, 6};
        msl::vector<int> v1(a1, a1 + 5);
        msl::vector<int> v2(a2, a2 + 2);

        bool result = msl::includes(v1.begin(), v1.end(), v2.begin(), v2.end());
        std::cout << "Test Case includes (not subset): " << (!result ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 3: Empty subset (true)
    {
        int a1[] = {1, 2};
        msl::vector<int> v1(a1, a1 + 2);
        msl::vector<int> v2;

        bool result = msl::includes(v1.begin(), v1.end(), v2.begin(), v2.end());
        std::cout << "Test Case includes (empty subset): " << (result ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 4: Identical sets (true)
    {
        int a1[] = {1, 2};
        msl::vector<int> v1(a1, a1 + 2);
        msl::vector<int> v2(a1, a1 + 2);

        bool result = msl::includes(v1.begin(), v1.end(), v2.begin(), v2.end());
        std::cout << "Test Case includes (identical): " << (result ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 5: Superset check (false)
    // v2 is larger than v1, cannot be included
    {
        int a1[] = {1, 2};
        int a2[] = {1, 2, 3};
        msl::vector<int> v1(a1, a1 + 2);
        msl::vector<int> v2(a2, a2 + 3);

        bool result = msl::includes(v1.begin(), v1.end(), v2.begin(), v2.end());
        std::cout << "Test Case includes (superset): " << (!result ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 6: Predicate check (descending)
    {
        int a1[] = {5, 4, 3, 2, 1};
        int a2[] = {5, 3, 1};
        msl::vector<int> v1(a1, a1 + 5);
        msl::vector<int> v2(a2, a2 + 3);

        bool result = msl::includes(v1.begin(), v1.end(), v2.begin(), v2.end(), msl::greater<int>());
        std::cout << "Test Case includes (predicate): " << (result ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_min_max_element() {
    // ==========================================
    // 测试 min_element 和 max_element
    // ==========================================
    print();
    std::cout << "Testing min_element and max_element..." << std::endl;

    // Test case 1: Normal integers
    {
        int a[] = {3, 1, 4, 1, 5, 9, 2, 6};
        msl::vector<int> v(a, a + 8);

        auto min_it = msl::min_element(v.begin(), v.end());
        auto max_it = msl::max_element(v.begin(), v.end());

        bool pass = (*min_it == 1 && *max_it == 9);
        // Note: min_element returns the first smallest element, so it should be the first '1' at index 1
        pass &= (min_it == v.begin() + 1);
        
        std::cout << "Test Case min/max (normal): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 2: Empty range
    {
        msl::vector<int> v;
        auto min_it = msl::min_element(v.begin(), v.end());
        auto max_it = msl::max_element(v.begin(), v.end());

        bool pass = (min_it == v.end() && max_it == v.end());
        std::cout << "Test Case min/max (empty): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 3: Predicate (abs value)
    {
        int a[] = {-10, 5, -2, 8};
        msl::vector<int> v(a, a + 4);

        struct AbsCompare {
            bool operator()(int x, int y) const {
                return std::abs(x) < std::abs(y);
            }
        };

        auto min_it = msl::min_element(v.begin(), v.end(), AbsCompare());
        auto max_it = msl::max_element(v.begin(), v.end(), AbsCompare());

        // Min abs: -2 (abs 2)
        // Max abs: -10 (abs 10)
        bool pass = (*min_it == -2 && *max_it == -10);
        std::cout << "Test Case min/max (predicate): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_merge() {
    print();
    std::cout << "Testing merge..." << std::endl;

    // Test case 1: Normal merge of two sorted sequences
    {
        int a[] = {1, 3, 5, 7};
        int b[] = {2, 4, 6, 8};
        msl::vector<int> v1(a, a + 4);
        msl::vector<int> v2(b, b + 4);
        msl::vector<int> res(8);

        msl::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), res.begin());
        
        int expected[] = {1, 2, 3, 4, 5, 6, 7, 8};
        bool pass = true;
        for (int i = 0; i < 8; ++i) {
            if (res[i] != expected[i]) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case merge (normal): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 2: One empty sequence
    {
        int a[] = {1, 2, 3};
        msl::vector<int> v1(a, a + 3);
        msl::vector<int> v2;
        msl::vector<int> res(3);

        msl::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), res.begin());
        
        int expected[] = {1, 2, 3};
        bool pass = true;
        for (int i = 0; i < 3; ++i) {
            if (res[i] != expected[i]) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case merge (one empty): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 3: Both empty
    {
        msl::vector<int> v1;
        msl::vector<int> v2;
        msl::vector<int> res;

        msl::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), res.begin());
        bool pass = res.empty();
        std::cout << "Test Case merge (both empty): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 4: Predicate (descending order)
    {
        int a[] = {7, 5, 3, 1};
        int b[] = {8, 6, 4, 2};
        msl::vector<int> v1(a, a + 4);
        msl::vector<int> v2(b, b + 4);
        msl::vector<int> res(8);

        struct Greater {
            bool operator()(int x, int y) const { return x > y; }
        };

        msl::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), res.begin(), Greater());
        
        int expected[] = {8, 7, 6, 5, 4, 3, 2, 1};
        bool pass = true;
        for (int i = 0; i < 8; ++i) {
            if (res[i] != expected[i]) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case merge (predicate): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_partition() {
    print();
    std::cout << "Testing partition..." << std::endl;

    // Test case 1: Partition even/odd
    {
        int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        msl::vector<int> v(a, a + 10);

        struct IsEven {
            bool operator()(int x) const { return x % 2 == 0; }
        };

        auto it = msl::partition(v.begin(), v.end(), IsEven());
        
        // Verify all elements before 'it' are even
        bool pass = true;
        for (auto i = v.begin(); i != it; ++i) {
            if (*i % 2 != 0) {
                pass = false;
                break;
            }
        }
        // Verify all elements after 'it' are odd
        for (auto i = it; i != v.end(); ++i) {
            if (*i % 2 == 0) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case partition (even/odd): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 2: All true
    {
        int a[] = {2, 4, 6, 8};
        msl::vector<int> v(a, a + 4);
        struct IsEven { bool operator()(int x) const { return x % 2 == 0; } };
        auto it = msl::partition(v.begin(), v.end(), IsEven());
        bool pass = (it == v.end());
        std::cout << "Test Case partition (all true): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 3: All false
    {
        int a[] = {1, 3, 5, 7};
        msl::vector<int> v(a, a + 4);
        struct IsEven { bool operator()(int x) const { return x % 2 == 0; } };
        auto it = msl::partition(v.begin(), v.end(), IsEven());
        bool pass = (it == v.begin());
        std::cout << "Test Case partition (all false): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 4: Empty
    {
        msl::vector<int> v;
        struct IsEven { bool operator()(int x) const { return x % 2 == 0; } };
        auto it = msl::partition(v.begin(), v.end(), IsEven());
        bool pass = (it == v.begin());
        std::cout << "Test Case partition (empty): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_remove() {
    print();
    std::cout << "Testing remove and remove_if..." << std::endl;

    // Test case 1: remove
    {
        int a[] = {1, 2, 3, 2, 4, 2, 5};
        msl::vector<int> v(a, a + 7);
        auto end = msl::remove(v.begin(), v.end(), 2);
        
        bool pass = (end - v.begin() == 4); // Should have 4 elements left: 1, 3, 4, 5
        if (pass) {
            int expected[] = {1, 3, 4, 5};
            for (int i = 0; i < 4; ++i) {
                if (v[i] != expected[i]) {
                    pass = false;
                    break;
                }
            }
        }
        std::cout << "Test Case remove: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 2: remove_if
    {
        int a[] = {1, 2, 3, 4, 5, 6};
        msl::vector<int> v(a, a + 6);
        struct IsEven { bool operator()(int x) const { return x % 2 == 0; } };
        
        auto end = msl::remove_if(v.begin(), v.end(), IsEven());
        
        bool pass = (end - v.begin() == 3); // Should have 3 elements left: 1, 3, 5
        if (pass) {
            int expected[] = {1, 3, 5};
            for (int i = 0; i < 3; ++i) {
                if (v[i] != expected[i]) {
                    pass = false;
                    break;
                }
            }
        }
        std::cout << "Test Case remove_if: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_remove_copy() {
    print();
    std::cout << "Testing remove_copy and remove_copy_if..." << std::endl;

    // Test case 1: remove_copy
    {
        int a[] = {1, 2, 3, 2, 4};
        msl::vector<int> v(a, a + 5);
        msl::vector<int> res(5);
        
        auto end = msl::remove_copy(v.begin(), v.end(), res.begin(), 2);
        
        bool pass = (end - res.begin() == 3); // 1, 3, 4
        if (pass) {
            int expected[] = {1, 3, 4};
            for (int i = 0; i < 3; ++i) {
                if (res[i] != expected[i]) {
                    pass = false;
                    break;
                }
            }
        }
        std::cout << "Test Case remove_copy: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 2: remove_copy_if
    {
        int a[] = {1, 2, 3, 4, 5};
        msl::vector<int> v(a, a + 5);
        msl::vector<int> res(5);
        struct IsEven { bool operator()(int x) const { return x % 2 == 0; } };
        
        auto end = msl::remove_copy_if(v.begin(), v.end(), res.begin(), IsEven());
        
        bool pass = (end - res.begin() == 3); // 1, 3, 5
        if (pass) {
            int expected[] = {1, 3, 5};
            for (int i = 0; i < 3; ++i) {
                if (res[i] != expected[i]) {
                    pass = false;
                    break;
                }
            }
        }
        std::cout << "Test Case remove_copy_if: " << (pass ? "PASSED" : "FAILED") << std::endl;
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
    std::cout << std::endl;
    test_adjacent_find();
    std::cout << std::endl;
    test_count();
    std::cout << std::endl;
    test_find();
    std::cout << std::endl;
    test_search();
    std::cout << std::endl;
    test_search_n();
    std::cout << std::endl;
    test_find_end();
    std::cout << std::endl;
    test_find_first_of();
    std::cout << std::endl;
    test_includes();
    std::cout << std::endl;
    test_min_max_element();
    std::cout << std::endl;
    test_merge();
    std::cout << std::endl;
    test_partition();
    std::cout << std::endl;
    test_remove();
    std::cout << std::endl;
    test_remove_copy();
    return 0;
}

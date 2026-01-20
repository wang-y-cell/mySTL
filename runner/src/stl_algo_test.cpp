#include <iostream>
#include <vector>
#include <algorithm> // 用于 std::sort
#include <cmath>
#include <cctype>
#include "stl_algo.h"
#include "vector.h"
#include "set.h"
#include "list.h"


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

bool are_equal(int a, int b) {
    return a == b;
}

void test_unique() {
    print();
    std::cout << "Testing unique..." << std::endl;
    // Test Case 1: Standard unique
    {
        int a[] = {1, 2, 2, 3, 3, 3, 4, 5, 5};
        msl::vector<int> v(a, a + 9);
        msl::vector<int>::iterator it = msl::unique(v.begin(), v.end());
        
        int expected[] = {1, 2, 3, 4, 5};
        bool pass = (it == v.begin() + 5);
        for (int i = 0; i < 5; ++i) {
            if (v[i] != expected[i]) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case unique: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
    
    // Test Case 2: Predicate unique
    {
        int a[] = {1, 2, 2, 3, 3, 3, 4, 5, 5};
        msl::vector<int> v(a, a + 9);
        msl::vector<int>::iterator it = msl::unique(v.begin(), v.end(), are_equal);
        
        int expected[] = {1, 2, 3, 4, 5};
        bool pass = (it == v.begin() + 5);
        for (int i = 0; i < 5; ++i) {
            if (v[i] != expected[i]) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case unique (predicate): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_unique_copy() {
    print();
    std::cout << "Testing unique_copy..." << std::endl;
    // Test Case 1: Standard unique_copy
    {
        int a[] = {1, 2, 2, 3, 3, 3, 4, 5, 5};
        msl::vector<int> v(a, a + 9);
        msl::vector<int> result(9);
        
        msl::vector<int>::iterator it = msl::unique_copy(v.begin(), v.end(), result.begin());
        
        int expected[] = {1, 2, 3, 4, 5};
        bool pass = (it == result.begin() + 5);
        for (int i = 0; i < 5; ++i) {
            if (result[i] != expected[i]) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case unique_copy: " << (pass ? "PASSED" : "FAILED") << std::endl;
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

void test_replace() {
    print();
    std::cout << "Testing replace and replace_if..." << std::endl;

    // Test case 1: replace
    {
        int a[] = {1, 2, 3, 2, 4, 2, 5};
        msl::vector<int> v(a, a + 7);
        msl::replace(v.begin(), v.end(), 2, 0);
        
        int expected[] = {1, 0, 3, 0, 4, 0, 5};
        bool pass = true;
        for (int i = 0; i < 7; ++i) {
            if (v[i] != expected[i]) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case replace: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 2: replace_if
    {
        int a[] = {1, 2, 3, 4, 5, 6};
        msl::vector<int> v(a, a + 6);
        struct IsEven { bool operator()(int x) const { return x % 2 == 0; } };
        
        msl::replace_if(v.begin(), v.end(), IsEven(), 0);
        
        int expected[] = {1, 0, 3, 0, 5, 0};
        bool pass = true;
        for (int i = 0; i < 6; ++i) {
            if (v[i] != expected[i]) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case replace_if: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_replace_copy() {
    print();
    std::cout << "Testing replace_copy and replace_copy_if..." << std::endl;

    // Test case 1: replace_copy
    {
        int a[] = {1, 2, 3, 2, 4};
        msl::vector<int> v(a, a + 5);
        msl::vector<int> res(5);
        
        msl::replace_copy(v.begin(), v.end(), res.begin(), 2, 0);
        
        int expected[] = {1, 0, 3, 0, 4};
        bool pass = true;
        for (int i = 0; i < 5; ++i) {
            if (res[i] != expected[i]) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case replace_copy: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 2: replace_copy_if
    {
        int a[] = {1, 2, 3, 4, 5};
        msl::vector<int> v(a, a + 5);
        msl::vector<int> res(5);
        struct IsEven { bool operator()(int x) const { return x % 2 == 0; } };
        
        msl::replace_copy_if(v.begin(), v.end(), res.begin(), IsEven(), 0);
        
        int expected[] = {1, 0, 3, 0, 5};
        bool pass = true;
        for (int i = 0; i < 5; ++i) {
            if (res[i] != expected[i]) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case replace_copy_if: " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_reverse() {
    print();
    std::cout << "Testing reverse..." << std::endl;

    // Test case 1: Random Access Iterator (vector)
    {
        int a[] = {1, 2, 3, 4, 5};
        msl::vector<int> v(a, a + 5);
        msl::reverse(v.begin(), v.end());
        
        int expected[] = {5, 4, 3, 2, 1};
        bool pass = true;
        for (int i = 0; i < 5; ++i) {
            if (v[i] != expected[i]) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case reverse (random access - vector): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 2: Bidirectional Iterator (list)
    {
        msl::list<int> l;
        l.push_back(1);
        l.push_back(2);
        l.push_back(3);
        l.push_back(4);
        l.push_back(5);

        msl::reverse(l.begin(), l.end());

        int expected[] = {5, 4, 3, 2, 1};
        bool pass = true;
        int i = 0;
        for (auto it = l.begin(); it != l.end(); ++it, ++i) {
            if (*it != expected[i]) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case reverse (bidirectional - list): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 3: Empty range
    {
        msl::vector<int> v;
        msl::reverse(v.begin(), v.end());
        bool pass = v.empty();
        std::cout << "Test Case reverse (empty): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 4: Single element
    {
        msl::vector<int> v;
        v.push_back(1);
        msl::reverse(v.begin(), v.end());
        bool pass = (v.size() == 1 && v[0] == 1);
        std::cout << "Test Case reverse (single element): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_reverse_copy() {
    print();
    std::cout << "Testing reverse_copy..." << std::endl;

    // Test case 1: Vector copy to another vector
    {
        int a[] = {1, 2, 3, 4, 5};
        msl::vector<int> v(a, a + 5);
        msl::vector<int> res(5);
        
        msl::reverse_copy(v.begin(), v.end(), res.begin());
        
        int expected[] = {5, 4, 3, 2, 1};
        bool pass = true;
        for (int i = 0; i < 5; ++i) {
            if (res[i] != expected[i]) {
                pass = false;
                break;
            }
        }
        // Verify original is unchanged
        for (int i = 0; i < 5; ++i) {
            if (v[i] != a[i]) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case reverse_copy (vector): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 2: List copy to vector
    {
        msl::list<int> l;
        l.push_back(10);
        l.push_back(20);
        l.push_back(30);
        
        msl::vector<int> res(3);
        msl::reverse_copy(l.begin(), l.end(), res.begin());
        
        int expected[] = {30, 20, 10};
        bool pass = true;
        for (int i = 0; i < 3; ++i) {
            if (res[i] != expected[i]) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case reverse_copy (list to vector): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_rotate() {
    print();
    std::cout << "Testing rotate..." << std::endl;

    // Test case 1: Random Access Iterator (vector) - Cycle Algorithm
    {
        int a[] = {1, 2, 3, 4, 5, 6};
        msl::vector<int> v(a, a + 6);
        // Rotate at index 2 (value 3): {1, 2, [3], 4, 5, 6} -> {3, 4, 5, 6, 1, 2}
        msl::rotate(v.begin(), v.begin() + 2, v.end());
        
        int expected[] = {3, 4, 5, 6, 1, 2};
        bool pass = true;
        for (int i = 0; i < 6; ++i) {
            if (v[i] != expected[i]) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case rotate (random access - vector): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 2: Bidirectional Iterator (list) - Reverse Algorithm
    {
        msl::list<int> l;
        for (int i = 1; i <= 6; ++i) l.push_back(i);
        
        // Rotate at 3rd element: {1, 2, [3], 4, 5, 6} -> {3, 4, 5, 6, 1, 2}
        auto middle = l.begin();
        ++middle; ++middle; // Points to 3
        
        msl::rotate(l.begin(), middle, l.end());
        
        int expected[] = {3, 4, 5, 6, 1, 2};
        bool pass = true;
        int i = 0;
        for (auto it = l.begin(); it != l.end(); ++it, ++i) {
            if (*it != expected[i]) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case rotate (bidirectional - list): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }

    // Test case 3: Empty or single element
    {
        msl::vector<int> v;
        msl::rotate(v.begin(), v.end(), v.end());
        bool pass = v.empty();
        
        v.push_back(1);
        msl::rotate(v.begin(), v.begin(), v.end());
        pass = pass && (v[0] == 1);
        
        std::cout << "Test Case rotate (trivial): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

void test_rotate_copy() {
    print();
    std::cout << "Testing rotate_copy..." << std::endl;
    // Test case 1: Vector
    {
        int a[] = {1, 2, 3, 4, 5, 6};
        msl::vector<int> v(a, a + 6);
        msl::vector<int> result(6);
        
        // Rotate at 3 (index 2)
        msl::rotate_copy(v.begin(), v.begin() + 2, v.end(), result.begin());
        
        int expected[] = {3, 4, 5, 6, 1, 2};
        bool pass = true;
        for (int i = 0; i < 6; ++i) {
            if (result[i] != expected[i]) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case rotate_copy (vector): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}

int increment(int i) { return i + 1; }
int sum(int a, int b) { return a + b; }

void test_transform() {
    print();
    std::cout << "Testing transform..." << std::endl;
    
    // Test Unary
    {
        int a[] = {1, 2, 3, 4, 5};
        msl::vector<int> v(a, a + 5);
        msl::vector<int> result(5);
        
        msl::transform(v.begin(), v.end(), result.begin(), increment);
        
        int expected[] = {2, 3, 4, 5, 6};
        bool pass = true;
        for (int i = 0; i < 5; ++i) {
            if (result[i] != expected[i]) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case transform (unary): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
    
    // Test Binary
    {
        int a[] = {1, 2, 3, 4, 5};
        int b[] = {5, 4, 3, 2, 1};
        msl::vector<int> v1(a, a + 5);
        msl::vector<int> v2(b, b + 5);
        msl::vector<int> result(5);
        
        msl::transform(v1.begin(), v1.end(), v2.begin(), result.begin(), sum);
        
        bool pass = true;
        for (int i = 0; i < 5; ++i) {
            if (result[i] != 6) {
                pass = false;
                break;
            }
        }
        std::cout << "Test Case transform (binary): " << (pass ? "PASSED" : "FAILED") << std::endl;
    }
}


void test_upper_bound() {
    print();
    std::cout << "Testing upper_bound..." << std::endl;

    // Test with Random Access Iterator (vector)
    {
        int arr[] = {1, 2, 4, 4, 5, 6};
        msl::vector<int> v(arr, arr + 6);

        // upper_bound(4) should return first element > 4, which is 5 (index 4)
        auto it = msl::upper_bound(v.begin(), v.end(), 4);
        if (it != v.end() && *it == 5 && (it - v.begin()) == 4) {
            std::cout << "Vector upper_bound(4) PASSED" << std::endl;
        } else {
            std::cout << "Vector upper_bound(4) FAILED" << std::endl;
            if (it != v.end()) std::cout << "Got: " << *it << " at index " << (it - v.begin()) << std::endl;
            else std::cout << "Got end()" << std::endl;
        }

        // upper_bound(3) should return first element > 3, which is 4 (index 2)
        it = msl::upper_bound(v.begin(), v.end(), 3);
        if (it != v.end() && *it == 4 && (it - v.begin()) == 2) {
            std::cout << "Vector upper_bound(3) PASSED" << std::endl;
        } else {
            std::cout << "Vector upper_bound(3) FAILED" << std::endl;
        }
        
        // upper_bound(7) should return end()
        it = msl::upper_bound(v.begin(), v.end(), 7);
        if (it == v.end()) {
            std::cout << "Vector upper_bound(7) PASSED" << std::endl;
        } else {
            std::cout << "Vector upper_bound(7) FAILED" << std::endl;
        }
    }

    // Test with Bidirectional Iterator (list) - falls back to forward_iterator implementation
    {
        int arr[] = {1, 2, 4, 4, 5, 6};
        msl::list<int> l;
        for(int i : arr) l.push_back(i);

        // upper_bound(4) -> 5
        auto it = msl::upper_bound(l.begin(), l.end(), 4);
        if (it != l.end() && *it == 5) {
             std::cout << "List upper_bound(4) PASSED" << std::endl;
        } else {
            std::cout << "List upper_bound(4) FAILED" << std::endl;
        }
    }

    // Test with custom comparator
    {
        int arr[] = {6, 5, 4, 4, 2, 1};
        msl::vector<int> v(arr, arr + 6);
        // upper_bound with greater<int>
        // comp(val, *it) -> val > *it
        // upper_bound finds first element where comp(val, *it) is true.
        // i.e. 4 > *it is true.
        // 6: 4 > 6 False
        // 5: 4 > 5 False
        // 4: 4 > 4 False
        // 4: 4 > 4 False
        // 2: 4 > 2 True! -> Returns 2.
        
        auto it = msl::upper_bound(v.begin(), v.end(), 4, [](int a, int b){ return a > b; });
        if (it != v.end() && *it == 2) {
             std::cout << "Vector custom compare upper_bound(4) PASSED" << std::endl;
        } else {
             std::cout << "Vector custom compare upper_bound(4) FAILED" << std::endl;
             if (it != v.end()) std::cout << "Got: " << *it << std::endl;
        }
    }
}



void test_binary_search() {
    print();
    std::cout << "Testing binary_search..." << std::endl;

    // Test with vector (Random Access Iterator)
    {
        int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        msl::vector<int> v(arr, arr + 9);

        if (msl::binary_search(v.begin(), v.end(), 5)) {
            std::cout << "Vector binary_search(5) PASSED" << std::endl;
        } else {
            std::cout << "Vector binary_search(5) FAILED" << std::endl;
        }

        if (!msl::binary_search(v.begin(), v.end(), 10)) {
            std::cout << "Vector binary_search(10) PASSED" << std::endl;
        } else {
            std::cout << "Vector binary_search(10) FAILED" << std::endl;
        }
    }

    // Test with list (Forward Iterator)
    {
        int arr[] = {1, 3, 5, 7, 9};
        msl::list<int> l;
        for(int i : arr) l.push_back(i);

        if (msl::binary_search(l.begin(), l.end(), 3)) {
            std::cout << "List binary_search(3) PASSED" << std::endl;
        } else {
            std::cout << "List binary_search(3) FAILED" << std::endl;
        }

        if (!msl::binary_search(l.begin(), l.end(), 4)) {
            std::cout << "List binary_search(4) PASSED" << std::endl;
        } else {
            std::cout << "List binary_search(4) FAILED" << std::endl;
        }
    }

    // Test with custom comparator
    {
        int arr[] = {9, 7, 5, 3, 1};
        msl::vector<int> v(arr, arr + 5);
        // Descending order, use greater<int> like comparator
        auto comp = [](int a, int b) { return a > b; };

        if (msl::binary_search(v.begin(), v.end(), 5, comp)) {
            std::cout << "Custom comparator binary_search(5) PASSED" << std::endl;
        } else {
            std::cout << "Custom comparator binary_search(5) FAILED" << std::endl;
        }

        if (!msl::binary_search(v.begin(), v.end(), 6, comp)) {
            std::cout << "Custom comparator binary_search(6) PASSED" << std::endl;
        } else {
            std::cout << "Custom comparator binary_search(6) FAILED" << std::endl;
        }
    }
}

void test_next_permutation() {
    print();
    std::cout << "Testing next_permutation..." << std::endl;

    // Test 1: Simple permutation {1, 2, 3}
    {
        int arr[] = {1, 2, 3};
        msl::vector<int> v(arr, arr + 3);
        
        // 1 2 3 -> 1 3 2
        bool has_next = msl::next_permutation(v.begin(), v.end());
        if (has_next && v[0] == 1 && v[1] == 3 && v[2] == 2) {
            std::cout << "next_permutation {1,2,3}->{1,3,2} PASSED" << std::endl;
        } else {
            std::cout << "next_permutation {1,2,3}->{1,3,2} FAILED" << std::endl;
            for(int x : v) std::cout << x << " "; std::cout << std::endl;
        }

        // Cycle through all
        int count = 1; // already did one
        while (msl::next_permutation(v.begin(), v.end())) {
            count++;
        }
        // Total permutations of 3 elements is 3! = 6. 
        // Since we started at 1,2,3 and did one step to 1,3,2, we expect 5 more steps to return true?
        // No, loop runs until it returns false (back to 1,2,3).
        // Sequence: 123 -> 132 -> 213 -> 231 -> 312 -> 321 -> 123(false)
        // count should be 6? Wait.
        // 1. (start) 123
        // 2. 132 (count=1, has_next=true)
        // loop:
        // 3. 213 (count=2)
        // 4. 231 (count=3)
        // 5. 312 (count=4)
        // 6. 321 (count=5)
        // 7. 123 (returns false, loop terminates)
        
        if (count == 5 && v[0] == 1 && v[1] == 2 && v[2] == 3) {
             std::cout << "next_permutation cycle PASSED" << std::endl;
        } else {
             std::cout << "next_permutation cycle FAILED, count=" << count << std::endl;
             for(int x : v) std::cout << x << " "; std::cout << std::endl;
        }
    }

    // Test 2: Custom comparator (descending)
    {
        // For descending order, "next" permutation means "next lexicographically smaller" if we use > as comparator?
        // Actually, next_permutation with comp uses comp(a, b) to check if a < b.
        // If we use greater<int>, we are defining an order where 3 < 2 (3 comes before 2).
        // So sorted sequence is 3, 2, 1.
        // If we start with 3, 2, 1, next_permutation should return false.
        // If we start with 1, 2, 3 (which is "last" in descending order), it should go to... wait.
        
        // Let's stick to simple logic:
        // Comparator defines "less than".
        // Sequence: 3, 2, 1. comp(3, 2) is true (3 > 2). So 3 < 2 in this ordering.
        // Wait, greater(a,b) returns a > b.
        // greater(3, 2) -> true. So 3 is "before" 2? No, STL comparators usually define "strict weak ordering".
        // sort(v, greater) -> 3, 2, 1.
        // So 3 is the "smallest" element, 1 is the "largest".
        // So 3, 2, 1 is the sorted sequence (first permutation).
        // 1, 2, 3 is the last permutation.
        
        int arr[] = {3, 2, 1};
        msl::vector<int> v(arr, arr + 3);
        // comp: a > b. 
        // 3, 2, 1 is sorted.
        // next should be 3, 1, 2.
        
        bool has_next = msl::next_permutation(v.begin(), v.end(), [](int a, int b){ return a > b; });
        if (has_next && v[0] == 3 && v[1] == 1 && v[2] == 2) {
             std::cout << "Custom comparator next_permutation {3,2,1}->{3,1,2} PASSED" << std::endl;
        } else {
             std::cout << "Custom comparator next_permutation {3,2,1}->{3,1,2} FAILED" << std::endl;
             for(int x : v) std::cout << x << " "; std::cout << std::endl;
        }
    }
}

void test_prev_permutation() {
    print();
    std::cout << "Testing prev_permutation..." << std::endl;

    // Test 1: Simple permutation {3, 2, 1}
    {
        int arr[] = {3, 2, 1};
        msl::vector<int> v(arr, arr + 3);
        
        // 3 2 1 -> 3 1 2
        bool has_prev = msl::prev_permutation(v.begin(), v.end());
        if (has_prev && v[0] == 3 && v[1] == 1 && v[2] == 2) {
            std::cout << "prev_permutation {3,2,1}->{3,1,2} PASSED" << std::endl;
        } else {
            std::cout << "prev_permutation {3,2,1}->{3,1,2} FAILED" << std::endl;
            for(int x : v) std::cout << x << " "; std::cout << std::endl;
        }

        // Cycle through all
        int count = 1; 
        while (msl::prev_permutation(v.begin(), v.end())) {
            count++;
        }
        // Sequence: 321 -> 312 -> 231 -> 213 -> 132 -> 123 -> 321(false)
        
        if (count == 5 && v[0] == 3 && v[1] == 2 && v[2] == 1) {
             std::cout << "prev_permutation cycle PASSED" << std::endl;
        } else {
             std::cout << "prev_permutation cycle FAILED, count=" << count << std::endl;
             for(int x : v) std::cout << x << " "; std::cout << std::endl;
        }
    }

    // Test 2: Custom comparator (descending logic inverted)
    {
        // comp: a > b. 
        // Logic: "prev" permutation based on ordering >.
        // Sorted: 3, 2, 1 (first).
        // Reverse sorted: 1, 2, 3 (last).
        
        // Start with 1, 2, 3.
        int arr[] = {1, 2, 3};
        msl::vector<int> v(arr, arr + 3);
        
        // prev of {1, 2, 3} with > should be {1, 3, 2} ?
        // Let's trace:
        // comp(i1, --i) => *i1 > *i.
        // 1 2 3: 3 > 2 (true). i=1 (val 2).
        // i2=last. Find comp(i2, i) => *i2 > 2.
        // 3 > 2. i2=2 (val 3).
        // Swap 2 and 3 -> 1 3 2.
        // Reverse after i -> 1 3 2.
        // Yes.
        
        bool has_prev = msl::prev_permutation(v.begin(), v.end(), [](int a, int b){ return a > b; });
        if (has_prev && v[0] == 1 && v[1] == 3 && v[2] == 2) {
             std::cout << "Custom comparator prev_permutation {1,2,3}->{1,3,2} PASSED" << std::endl;
        } else {
             std::cout << "Custom comparator prev_permutation {1,2,3}->{1,3,2} FAILED" << std::endl;
             for(int x : v) std::cout << x << " "; std::cout << std::endl;
        }
    }
}

int main() {
    test_prev_permutation();
    std::cout << std::endl;
    test_next_permutation();
    std::cout << std::endl;
    test_binary_search();
    std::cout << std::endl;
    test_upper_bound();
    std::cout << std::endl;
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
    std::cout << std::endl;
    test_replace();
    std::cout << std::endl;
    test_replace_copy();
    std::cout << std::endl;
    test_reverse();
    std::cout << std::endl;
    test_reverse_copy();
    std::cout << std::endl;
    test_rotate();
    std::cout << std::endl;
    test_rotate_copy();
    std::cout << std::endl;
    test_transform();
    std::cout << std::endl;
    test_unique();
    std::cout << std::endl;
    test_unique_copy();
    return 0;

}

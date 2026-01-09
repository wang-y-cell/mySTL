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

void test_adjacent_find() {
    // ==========================================
    // 测试 adjacent_find
    // ==========================================
    std::cout << "==========================================" << std::endl;
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
    std::cout << "==========================================" << std::endl;
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
    std::cout << "==========================================" << std::endl;
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
    std::cout << "==========================================" << std::endl;
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
    std::cout << "==========================================" << std::endl;
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
    return 0;
}

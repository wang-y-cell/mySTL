#include "vector.h"
#include <iostream>
#include <cassert>
#include <string>
#include <stdexcept>

// Global shared vector object as requested
msl::vector<int> g_vec;

// Helper to print vector state
void print_vec(const std::string& msg) {
    std::cout << msg << ": ";
    for (size_t i = 0; i < g_vec.size(); ++i) {
        std::cout << g_vec[i] << " ";
    }
    std::cout << "(size=" << g_vec.size() << ", cap=" << g_vec.capacity() << ")" << std::endl;
}

// 1. Test push_back
void test_push_back() {
    std::cout << "\n[Running test_push_back]" << std::endl;
    g_vec.push_back(1);
    g_vec.push_back(2);
    g_vec.push_back(3);
    g_vec.push_back(4);
    g_vec.push_back(5);
    print_vec("After push_back 1-5");
    assert(g_vec.size() == 5);
    assert(g_vec.back() == 5);
}

// 2. Test access (operator[], at, front, back, data)
void test_access() {
    std::cout << "\n[Running test_access]" << std::endl;
    assert(g_vec[0] == 1);
    assert(g_vec.at(1) == 2);
    assert(g_vec.front() == 1);
    assert(g_vec.back() == 5);
    assert(*g_vec.data() == 1);
    
    bool caught = false;
    try {
        g_vec.at(100);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);
    std::cout << "Access checks passed." << std::endl;
}

// 3. Test pop_back
void test_pop_back() {
    std::cout << "\n[Running test_pop_back]" << std::endl;
    g_vec.pop_back();
    print_vec("After pop_back");
    assert(g_vec.size() == 4);
    assert(g_vec.back() == 4);
}

// 4. Test insert
void test_insert() {
    std::cout << "\n[Running test_insert]" << std::endl;
    // Insert single element
    g_vec.insert(g_vec.begin() + 1, 10);
    print_vec("After insert 10 at index 1");
    assert(g_vec[1] == 10);
    
    // Insert count
    g_vec.insert(g_vec.end(), 2, 20);
    print_vec("After insert two 20s at end");
    assert(g_vec.size() == 7);
    assert(g_vec[5] == 20);
    assert(g_vec[6] == 20);

    // Insert range
    int arr[] = {30, 31};
    g_vec.insert(g_vec.begin(), arr, arr + 2);
    print_vec("After insert range {30, 31} at begin");
    assert(g_vec[0] == 30);
    assert(g_vec[1] == 31);
    
    // Insert initializer list (C++11)
    g_vec.insert(g_vec.end(), {40, 41});
    print_vec("After insert init list {40, 41} at end");
    assert(g_vec.back() == 41);
}

// 5. Test erase
void test_erase() {
    std::cout << "\n[Running test_erase]" << std::endl;
    // Erase single element (the 30 at begin)
    g_vec.erase(g_vec.begin());
    print_vec("After erase begin");
    assert(g_vec[0] == 31);
    
    // Erase range (last two elements: 40, 41)
    g_vec.erase(g_vec.end() - 2, g_vec.end());
    print_vec("After erase last 2");
    assert(g_vec.back() == 20);
}

// 6. Test resize
void test_resize() {
    std::cout << "\n[Running test_resize]" << std::endl;
    size_t old_size = g_vec.size();
    g_vec.resize(old_size + 2, 99);
    print_vec("After resize +2 with 99");
    assert(g_vec.back() == 99);
    assert(g_vec.size() == old_size + 2);
    
    g_vec.resize(3);
    print_vec("After resize to 3");
    assert(g_vec.size() == 3);
}

// 7. Test reserve
void test_reserve() {
    std::cout << "\n[Running test_reserve]" << std::endl;
    size_t old_cap = g_vec.capacity();
    g_vec.reserve(old_cap + 20);
    print_vec("After reserve +20");
    assert(g_vec.capacity() >= old_cap + 20);
}

// 8. Test shrink_to_fit
void test_shrink_to_fit() {
    std::cout << "\n[Running test_shrink_to_fit]" << std::endl;
    g_vec.shrink_to_fit(); // Note: implementation might be no-op or binding to std, check vector.h
    // In strict C++11 vector, this requests reduction. Our implementation might not fully support it 
    // or it's just a request. We just check it compiles and runs.
    print_vec("After shrink_to_fit");
}

// 9. Test iterators
void test_iterators() {
    std::cout << "\n[Running test_iterators]" << std::endl;
    std::cout << "Forward: ";
    for (auto it = g_vec.begin(); it != g_vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\nReverse: ";
    for (auto rit = g_vec.rbegin(); rit != g_vec.rend(); ++rit) {
        std::cout << *rit << " ";
    }
    std::cout << "\nConst Forward: ";
    for (auto it = g_vec.cbegin(); it != g_vec.cend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

// 10. Test emplace and emplace_back (C++11)
void test_emplace() {
    std::cout << "\n[Running test_emplace]" << std::endl;
    g_vec.emplace_back(100);
    print_vec("After emplace_back 100");
    assert(g_vec.back() == 100);
    
    g_vec.emplace(g_vec.begin(), 0);
    print_vec("After emplace 0 at begin");
    assert(g_vec.front() == 0);
}

// 11. Test swap
void test_swap() {
    std::cout << "\n[Running test_swap]" << std::endl;
    msl::vector<int> other;
    other.push_back(999);
    
    g_vec.swap(other);
    print_vec("After swap with {999}");
    
    assert(g_vec.size() == 1);
    assert(g_vec[0] == 999);
    
    // Swap back
    g_vec.swap(other);
    print_vec("After swapping back");
}

// 12. Test clear
void test_clear() {
    std::cout << "\n[Running test_clear]" << std::endl;
    g_vec.clear();
    print_vec("After clear");
    assert(g_vec.empty());
    assert(g_vec.size() == 0);
}

// 13. Test assign
void test_assign() {
    std::cout << "\n[Running test_assign]" << std::endl;
    g_vec.assign(5, 7);
    print_vec("After assign 5 7s");
    assert(g_vec.size() == 5);
    assert(g_vec[0] == 7);
    
    // g_vec.assign({1, 2, 3}); // Not implemented
    // print_vec("After assign init list {1, 2, 3}"); 

    // Wait, vector.h has assign(InputIterator first, InputIterator last)
    // and assign(size_type n, const_reference value).
    // Does it have assign(initializer_list)? Standard has it.
    // Let's check vector.h... it seems MISSING in the Read output.
    // It has: void assign(size_type n, const_reference value)
    // and template <typename InputIterator> void assign(InputIterator first, InputIterator last)
    // No assign(initializer_list). I'll skip that or use iterator version.
    
    auto il = {8, 9};
    g_vec.assign(il.begin(), il.end());
    print_vec("After assign range {8, 9}");
    assert(g_vec.size() == 2);
    assert(g_vec[0] == 8);
}

// 14. Test max_size and get_allocator
void test_utils() {
    std::cout << "\n[Running test_utils]" << std::endl;
    std::cout << "Max size: " << g_vec.max_size() << std::endl;
    g_vec.get_allocator();
}

int main() {
    std::cout << "Starting Vector Tests (C++11)..." << std::endl;
    
    test_push_back();
    test_access();
    test_pop_back();
    test_insert();
    test_erase();
    test_resize();
    test_reserve();
    test_shrink_to_fit();
    test_iterators();
    test_emplace();
    test_swap();
    test_clear();
    test_assign();
    test_utils();

    std::cout << "\nAll tests passed successfully!" << std::endl;
    return 0;
}

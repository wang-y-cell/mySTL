#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include "list.h"


void print(){
    std::cout << "==========================================" << std::endl;
}



// Helper to print list
template <typename T>
void print_list(const msl::list<T>& l, const std::string& name) {
    std::cout << name << " (" << l.size() << "): ";
    for (const auto& x : l) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

void test_constructors() {
    std::cout << "\n[Test Constructors]" << std::endl;
    
    // Fill - use size_t to avoid ambiguity if SFINAE fails
    msl::list<int> l1(5, 10);
    print_list(l1, "l1(5, 10)");
    assert(l1.size() == 5);
    for(auto x : l1) assert(x == 10);

    // Range
    int arr[] = {1, 2, 3, 4, 5};
    msl::list<int> l2(arr, arr + 5);
    print_list(l2, "l2(range)");
    assert(l2.size() == 5);
    int i = 1;
    for(auto x : l2) assert(x == i++);

    // Copy
    msl::list<int> l3(l2);
    print_list(l3, "l3(copy l2)");
    assert(l3.size() == 5);
    assert(l3.front() == 1);

    #if MYSTL_CPP_VERSION >= 11
    // Move
    msl::list<int> l4(std::move(l3));
    print_list(l4, "l4(move l3)");
    print_list(l3, "l3(after move)");
    assert(l4.size() == 5);
    assert(l3.empty());

    // Initializer list
    msl::list<int> l5 = {10, 20, 30};
    print_list(l5, "l5(init_list)");
    assert(l5.size() == 3);
    assert(l5.front() == 10);
    assert(l5.back() == 30);
    #endif
}

void test_assignment() {
    std::cout << "\n[Test Assignment]" << std::endl;
    msl::list<int> l1;
    l1.push_back(1); l1.push_back(2); l1.push_back(3);
    
    msl::list<int> l2;
    l2 = l1;
    print_list(l2, "l2 = l1");
    assert(l2.size() == 3);
    
    #if MYSTL_CPP_VERSION >= 11
    l2 = std::move(l1);
    print_list(l2, "l2 = move(l1)");
    assert(l2.size() == 3);
    assert(l1.empty());

    l2 = {4, 5, 6, 7};
    print_list(l2, "l2 = {4,5,6,7}");
    assert(l2.size() == 4);
    assert(l2.front() == 4);
    #endif

    l2.assign(3, 100);
    print_list(l2, "l2.assign(3, 100)");
    assert(l2.size() == 3);
    for(auto x : l2) assert(x == 100);
    
    int arr[] = {8, 9};
    l2.assign(arr, arr + 2);
    print_list(l2, "l2.assign(range)");
    assert(l2.size() == 2);
    assert(l2.front() == 8);
}

void test_modifiers() {
    std::cout << "\n[Test Modifiers]" << std::endl;
    msl::list<int> l;
    l.push_back(1);
    
    #if MYSTL_CPP_VERSION >= 11
    l.emplace_back(2);
    l.emplace_front(0);
    print_list(l, "emplace 0, 1, 2");
    assert(l.front() == 0);
    assert(l.back() == 2);
    
    auto it = l.begin();
    ++it; // at 1
    l.emplace(it, 10); // 0, 10, 1, 2
    print_list(l, "emplace 10 at 1");
    assert(l.size() == 4);
    
    l.insert(l.begin(), {8, 9}); // 8, 9, 0, 10, 1, 2
    print_list(l, "insert {8, 9}");
    assert(l.front() == 8);
    #else
    l.push_back(2);
    l.push_front(0);
    l.insert(++l.begin(), 10);
    l.insert(l.begin(), 8);
    l.insert(l.begin(), 9); // 9, 8, 0, 10, 1, 2
    #endif

    l.resize(2);
    print_list(l, "resize(2)");
    assert(l.size() == 2);
    
    l.resize(5, 99); 
    print_list(l, "resize(5, 99)");
    assert(l.size() == 5);
    assert(l.back() == 99);
    
    // Erase range
    auto first = l.begin();
    ++first;
    auto last = l.end();
    --last;
    l.erase(first, last); // keep first and last
    print_list(l, "erase range (1, end-1)");
    assert(l.size() == 2);
}

int main() {
    test_constructors();
    test_assignment();
    test_modifiers();
    
    std::cout << "\nAll list tests passed!" << std::endl;
    return 0;
}

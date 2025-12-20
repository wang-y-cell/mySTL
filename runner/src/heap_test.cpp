#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include "stl_heap.h"

bool greater_cmp(int a, int b) {
    return a > b;
}

int main() {
    // Test 1: Standard Max Heap
    std::vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    
    // Manually make heap for 10, 20, 30 -> 30, 10, 20
    std::make_heap(v.begin(), v.end()); 
    
    v.push_back(40);
    msl::push_heap(v.begin(), v.end());
    
    assert(v.front() == 40);
    assert(std::is_heap(v.begin(), v.end()));

    // Test pop_heap
    msl::pop_heap(v.begin(), v.end());
    assert(v.back() == 40); // Max element moved to back
    v.pop_back(); // Remove it
    assert(std::is_heap(v.begin(), v.end()));
    assert(v.front() == 30); // Next max

    // Test 2: Custom Comparator (Min Heap)
    std::vector<int> v2;
    v2.push_back(30);
    v2.push_back(20);
    v2.push_back(10);
    
    // Make min heap: 10, 20, 30
    std::make_heap(v2.begin(), v2.end(), greater_cmp);
    assert(v2.front() == 10);

    v2.push_back(5);
    msl::push_heap(v2.begin(), v2.end(), greater_cmp);

    assert(v2.front() == 5);
    assert(std::is_heap(v2.begin(), v2.end(), greater_cmp));

    // Test pop_heap with comparator
    msl::pop_heap(v2.begin(), v2.end(), greater_cmp);
    assert(v2.back() == 5); // Min element moved to back
    v2.pop_back();
    assert(std::is_heap(v2.begin(), v2.end(), greater_cmp));
    assert(v2.front() == 10);

    std::cout << "heap test passed" << std::endl;
    return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <random>
#include "stl_heap.h"

// Helper to print vector
void print_vec(const std::vector<int>& v) {
    for (int x : v) std::cout << x << " ";
    std::cout << std::endl;
}

bool greater_cmp(int a, int b) {
    return a > b;
}

int main() {
    // Test 1: Standard Max Heap -> Ascending Sort
    std::cout << "Test 1: Standard Max Heap -> Ascending Sort" << std::endl;
    std::vector<int> v;
    for(int i=0; i<10; ++i) v.push_back(i);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(v.begin(), v.end(), g);
    
    // Make heap first (using std::make_heap or msl::make_heap if we had it, but pop_heap relies on valid heap)
    // Since we only implemented push/pop, let's build heap using push_heap
    std::vector<int> heap_v;
    for(int x : v) {
        heap_v.push_back(x);
        msl::push_heap(heap_v.begin(), heap_v.end());
    }
    
    // Check if it is a heap
    assert(std::is_heap(heap_v.begin(), heap_v.end()));
    
    // Sort heap
    msl::sort_heap(heap_v.begin(), heap_v.end());
    
    // Check if sorted
    assert(std::is_sorted(heap_v.begin(), heap_v.end()));
    print_vec(heap_v);


    // Test 2: Custom Comparator (Min Heap) -> Descending Sort
    std::cout << "Test 2: Min Heap -> Descending Sort" << std::endl;
    std::vector<int> v2;
    for(int i=0; i<10; ++i) v2.push_back(i);
    std::shuffle(v2.begin(), v2.end(), g);
    
    std::vector<int> heap_v2;
    for(int x : v2) {
        heap_v2.push_back(x);
        msl::push_heap(heap_v2.begin(), heap_v2.end(), greater_cmp);
    }
    
    // Check if it is a heap (with comparator)
    assert(std::is_heap(heap_v2.begin(), heap_v2.end(), greater_cmp));
    
    // Sort heap
    msl::sort_heap(heap_v2.begin(), heap_v2.end(), greater_cmp);
    
    // Check if sorted descending
    assert(std::is_sorted(heap_v2.begin(), heap_v2.end(), greater_cmp));
    // Verify manually
    for(size_t i=0; i<heap_v2.size()-1; ++i) {
        assert(heap_v2[i] > heap_v2[i+1]);
    }
    print_vec(heap_v2);

    // Test 3: Edge cases
    std::cout << "Test 3: Edge cases" << std::endl;
    std::vector<int> empty_v;
    msl::sort_heap(empty_v.begin(), empty_v.end());
    assert(empty_v.empty());

    std::vector<int> single_v;
    single_v.push_back(1);
    msl::sort_heap(single_v.begin(), single_v.end());
    assert(single_v.size() == 1);
    assert(single_v[0] == 1);

    std::cout << "sort_heap test passed" << std::endl;
    return 0;
}

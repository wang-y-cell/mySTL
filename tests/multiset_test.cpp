#include "stl_multiset.h"
#include <iostream>
#include <cassert>
void print(){
    std::cout << "==========================================" << std::endl;
}
// Simple test for multiset
int main() {
    print();
    std::cout << "Testing multiset..." << std::endl;
    msl::multiset<int> ms;
    
    // Insert
    std::cout << "1. Testing insert..." << std::endl;
    ms.insert(10);
    ms.insert(20);
    ms.insert(10); // Duplicate
    ms.insert(30);
    ms.insert(20); // Duplicate
    
    // Verify size
    std::cout << "Size: " << ms.size() << std::endl;
    assert(ms.size() == 5);

    // Traversal
    std::cout << "2. Testing traversal..." << std::endl;
    std::cout << "Elements: ";
    for (msl::multiset<int>::iterator it = ms.begin(); it != ms.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    // Expected: 10 10 20 20 30 (sorted)

    // Count
    std::cout << "3. Testing count..." << std::endl;
    assert(ms.count(10) == 2);
    assert(ms.count(20) == 2);
    assert(ms.count(30) == 1);
    assert(ms.count(40) == 0);

    // Find
    std::cout << "4. Testing find..." << std::endl;
    msl::multiset<int>::iterator it = ms.find(20);
    assert(it != ms.end());
    assert(*it == 20);
    
    it = ms.find(40);
    assert(it == ms.end());

    // Lower/Upper bound
    std::cout << "5. Testing bounds..." << std::endl;
    auto lb = ms.lower_bound(20);
    auto ub = ms.upper_bound(20);
    // Should be range of two 20s
    assert(lb != ms.end() && *lb == 20);
    assert(msl::distance(lb, ub) == 2);

    // Erase
    std::cout << "6. Testing erase..." << std::endl;
    size_t erased_count = ms.erase(20);
    std::cout << "Erased " << erased_count << " elements with key 20" << std::endl;
    assert(erased_count == 2);
    assert(ms.size() == 3);
    assert(ms.count(20) == 0);

    std::cout << "All multiset tests passed!" << std::endl;
    return 0;
}

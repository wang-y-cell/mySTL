#include "stl_hash_multiset.h"
#include <iostream>
#include <cassert>
#include <string>

using namespace msl;

void test_hash_multiset() {
    std::cout << "Testing hash_multiset..." << std::endl;

    // Test constructor
    hash_multiset<int> hs;
    assert(hs.empty());
    assert(hs.size() == 0);
    std::cout << "Construction successful." << std::endl;

    // Test insert (allow duplicates)
    hs.insert(10);
    hs.insert(20);
    hs.insert(30);
    hs.insert(10); // Duplicate
    hs.insert(10); // Triplicate
    
    assert(hs.size() == 5);
    assert(hs.count(10) == 3);
    assert(hs.count(20) == 1);
    assert(hs.count(30) == 1);
    assert(hs.count(40) == 0);
    std::cout << "Insert successful." << std::endl;

    // Test iteration
    std::cout << "Iterating: ";
    int sum = 0;
    int count10 = 0;
    for (hash_multiset<int>::iterator it = hs.begin(); it != hs.end(); ++it) {
        std::cout << *it << " ";
        sum += *it;
        if (*it == 10) count10++;
    }
    std::cout << std::endl;
    // 10*3 + 20 + 30 = 80
    assert(sum == 80);
    assert(count10 == 3);
    std::cout << "Iteration successful." << std::endl;

    // Test find
    hash_multiset<int>::iterator it = hs.find(20);
    assert(it != hs.end());
    assert(*it == 20);
    
    it = hs.find(10);
    assert(it != hs.end());
    assert(*it == 10);
    
    it = hs.find(40);
    assert(it == hs.end());
    std::cout << "Find successful." << std::endl;

    // Test erase by iterator
    it = hs.find(20);
    hs.erase(it);
    assert(hs.size() == 4);
    assert(hs.count(20) == 0);
    std::cout << "Erase by iterator successful." << std::endl;

    // Test erase by key (should remove all copies)
    size_t n = hs.erase(10);
    assert(n == 3);
    assert(hs.size() == 1); // Only 30 left
    assert(hs.count(10) == 0);
    assert(hs.count(30) == 1);
    std::cout << "Erase by key successful." << std::endl;

    // Test clear
    hs.clear();
    assert(hs.empty());
    assert(hs.size() == 0);
    std::cout << "Clear successful." << std::endl;

    // Test swap
    hash_multiset<int> hs1;
    hs1.insert(100);
    hs1.insert(100);
    
    hash_multiset<int> hs2;
    hs2.insert(200);
    
    hs1.swap(hs2);
    assert(hs1.size() == 1);
    assert(*hs1.begin() == 200);
    assert(hs2.size() == 2);
    assert(hs2.count(100) == 2);
    std::cout << "Swap successful." << std::endl;

    // Test range constructor
    int arr[] = { 1, 2, 3, 1, 2 };
    hash_multiset<int> hs3(arr, arr + 5);
    assert(hs3.size() == 5);
    assert(hs3.count(1) == 2);
    assert(hs3.count(2) == 2);
    assert(hs3.count(3) == 1);
    std::cout << "Range constructor successful." << std::endl;

    std::cout << "All hash_multiset tests passed!" << std::endl;
}

int main() {
    test_hash_multiset();
    return 0;
}

#include <iostream>
#include <cassert>
#include "stl_hash_set.h"
#include "stl_hash_fun.h"
#include "stl_functional.h"

using namespace msl;

void test_hash_set() {
    std::cout << "Testing hash_set..." << std::endl;

    // Test constructor
    hash_set<int, hash<int>, equal_to<int>> hs;
    assert(hs.empty());
    assert(hs.size() == 0);
    std::cout << "Construction successful." << std::endl;

    // Test insert
    hs.insert(10);
    hs.insert(20);
    hs.insert(30);
    hs.insert(10); // Duplicate
    
    assert(hs.size() == 3);
    assert(hs.count(10) == 1);
    assert(hs.count(20) == 1);
    assert(hs.count(30) == 1);
    assert(hs.count(40) == 0);
    std::cout << "Insert successful." << std::endl;

    // Test iterator
    std::cout << "Iterating: ";
    int sum = 0;
    for (auto it = hs.begin(); it != hs.end(); ++it) {
        std::cout << *it << " ";
        sum += *it;
    }
    std::cout << std::endl;
    assert(sum == 60);
    std::cout << "Iteration successful." << std::endl;

    // Test find
    auto it = hs.find(20);
    assert(it != hs.end());
    assert(*it == 20);
    assert(hs.find(40) == hs.end());
    std::cout << "Find successful." << std::endl;

    // Test erase by key
    hs.erase(10);
    assert(hs.size() == 2);
    assert(hs.count(10) == 0);
    std::cout << "Erase by key successful." << std::endl;

    // Test erase by iterator
    it = hs.find(20);
    hs.erase(it);
    assert(hs.size() == 1);
    assert(hs.count(20) == 0);
    std::cout << "Erase by iterator successful." << std::endl;

    // Test clear
    hs.clear();
    assert(hs.empty());
    assert(hs.size() == 0);
    std::cout << "Clear successful." << std::endl;

    // Test swap
    hash_set<int> hs1;
    hs1.insert(100);
    hash_set<int> hs2;
    hs2.insert(200);
    
    hs1.swap(hs2);
    assert(hs1.size() == 1);
    assert(*hs1.begin() == 200);
    assert(hs2.size() == 1);
    assert(*hs2.begin() == 100);
    std::cout << "Swap successful." << std::endl;

    // Test range insert/constructor
    int arr[] = {1, 2, 3, 4, 5};
    hash_set<int> hs3(arr, arr + 5);
    assert(hs3.size() == 5);
    assert(hs3.count(1) == 1);
    assert(hs3.count(5) == 1);
    std::cout << "Range constructor successful." << std::endl;

    std::cout << "All hash_set tests passed!" << std::endl;
}

int main() {
    test_hash_set();
    return 0;
}

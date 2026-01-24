#include "stl_hash_multimap.h"
#include <iostream>
#include <cassert>
#include <string>

using namespace msl;

void test_hash_multimap() {
    std::cout << "Testing hash_multimap..." << std::endl;

    // Test constructor
    hash_multimap<std::string, int> hm;
    assert(hm.empty());
    assert(hm.size() == 0);
    std::cout << "Construction successful." << std::endl;

    // Test insert (allow duplicates)
    hm.insert(msl::make_pair(std::string("apple"), 1));
    hm.insert(msl::make_pair(std::string("banana"), 2));
    hm.insert(msl::make_pair(std::string("cherry"), 3));
    hm.insert(msl::make_pair(std::string("apple"), 4)); // Duplicate key
    hm.insert(msl::make_pair(std::string("apple"), 5)); // Triplicate key
    
    assert(hm.size() == 5);
    assert(hm.count("apple") == 3);
    assert(hm.count("banana") == 1);
    assert(hm.count("cherry") == 1);
    assert(hm.count("durian") == 0);
    std::cout << "Insert successful." << std::endl;

    // Test iteration
    std::cout << "Iterating: ";
    int sum = 0;
    int appleSum = 0;
    for (hash_multimap<std::string, int>::iterator it = hm.begin(); it != hm.end(); ++it) {
        std::cout << it->first << ":" << it->second << " ";
        sum += it->second;
        if (it->first == "apple") appleSum += it->second;
    }
    std::cout << std::endl;
    // 1+2+3+4+5 = 15
    assert(sum == 15);
    // 1+4+5 = 10
    assert(appleSum == 10);
    std::cout << "Iteration successful." << std::endl;

    // Test find
    hash_multimap<std::string, int>::iterator it = hm.find("banana");
    assert(it != hm.end());
    assert(it->second == 2);
    
    it = hm.find("apple");
    assert(it != hm.end());
    assert(it->first == "apple");
    // Could be 1, 4, or 5 depending on implementation order
    
    it = hm.find("elderberry");
    assert(it == hm.end());
    std::cout << "Find successful." << std::endl;

    // Test erase by key
    size_t n = hm.erase("apple");
    assert(n == 3);
    assert(hm.size() == 2);
    assert(hm.count("apple") == 0);
    std::cout << "Erase by key successful." << std::endl;

    // Test erase by iterator
    it = hm.find("banana");
    hm.erase(it);
    assert(hm.size() == 1);
    assert(hm.count("banana") == 0);
    std::cout << "Erase by iterator successful." << std::endl;

    // Test clear
    hm.clear();
    assert(hm.empty());
    assert(hm.size() == 0);
    std::cout << "Clear successful." << std::endl;

    // Test swap
    hash_multimap<int, int> hm1;
    hm1.insert(msl::make_pair(1, 10));
    hm1.insert(msl::make_pair(1, 11));
    
    hash_multimap<int, int> hm2;
    hm2.insert(msl::make_pair(2, 20));
    
    hm1.swap(hm2);
    assert(hm1.size() == 1);
    assert(hm1.begin()->first == 2);
    assert(hm2.size() == 2);
    assert(hm2.count(1) == 2);
    std::cout << "Swap successful." << std::endl;

    // Test range constructor
    msl::pair<int, int> arr[] = { msl::make_pair(1, 1), msl::make_pair(1, 2), msl::make_pair(2, 3) };
    hash_multimap<int, int> hm3(arr, arr + 3);
    assert(hm3.size() == 3);
    assert(hm3.count(1) == 2);
    assert(hm3.count(2) == 1);
    std::cout << "Range constructor successful." << std::endl;

    std::cout << "All hash_multimap tests passed!" << std::endl;
}

int main() {
    test_hash_multimap();
    return 0;
}

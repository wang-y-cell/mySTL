#include "stl_hash_map.h"
#include <iostream>
#include <cassert>
#include <string>

using namespace msl;

void print(){
    std::cout << "==========================================" << std::endl;
}

void test_hash_map() {
    std::cout << "Testing hash_map..." << std::endl;

    // Test constructor
    hash_map<std::string, int> hm;
    assert(hm.empty());
    assert(hm.size() == 0);
    std::cout << "Construction successful." << std::endl;

    // Test insert
    hm.insert(msl::make_pair(std::string("apple"), 1));
    hm.insert(msl::make_pair(std::string("banana"), 2));
    hm.insert(msl::make_pair(std::string("cherry"), 3));
    
    // Test duplicate insert (should fail)
    msl::pair<hash_map<std::string, int>::iterator, bool> p = hm.insert(msl::make_pair(std::string("apple"), 4));
    assert(p.second == false);
    assert(p.first->second == 1); // Value should not change

    assert(hm.size() == 3);
    assert(hm.count("apple") == 1);
    assert(hm.count("banana") == 1);
    assert(hm.count("cherry") == 1);
    assert(hm.count("durian") == 0);
    std::cout << "Insert successful." << std::endl;

    // Test operator[]
    hm["durian"] = 4;
    assert(hm.size() == 4);
    assert(hm["durian"] == 4);
    
    hm["apple"] = 100; // Update existing
    assert(hm["apple"] == 100);
    std::cout << "operator[] successful." << std::endl;

    // Test iteration
    std::cout << "Iterating: ";
    int sum = 0;
    for (hash_map<std::string, int>::iterator it = hm.begin(); it != hm.end(); ++it) {
        std::cout << it->first << ":" << it->second << " ";
        sum += it->second;
    }
    std::cout << std::endl;
    // apple:100, banana:2, cherry:3, durian:4 -> sum = 109
    assert(sum == 109);
    std::cout << "Iteration successful." << std::endl;

    // Test find
    hash_map<std::string, int>::iterator it = hm.find("banana");
    assert(it != hm.end());
    assert(it->second == 2);
    
    it = hm.find("elderberry");
    assert(it == hm.end());
    std::cout << "Find successful." << std::endl;

    // Test erase by key
    size_t n = hm.erase("apple");
    assert(n == 1);
    assert(hm.size() == 3);
    assert(hm.count("apple") == 0);
    std::cout << "Erase by key successful." << std::endl;

    // Test erase by iterator
    it = hm.find("banana");
    hm.erase(it);
    assert(hm.size() == 2);
    assert(hm.count("banana") == 0);
    std::cout << "Erase by iterator successful." << std::endl;

    // Test clear
    hm.clear();
    assert(hm.empty());
    assert(hm.size() == 0);
    std::cout << "Clear successful." << std::endl;

    // Test swap
    hash_map<int, int> hm1;
    hm1.insert(msl::make_pair(1, 10));
    
    hash_map<int, int> hm2;
    hm2.insert(msl::make_pair(2, 20));
    
    hm1.swap(hm2);
    assert(hm1.size() == 1);
    assert(hm1.begin()->first == 2);
    assert(hm2.size() == 1);
    assert(hm2.begin()->first == 1);
    std::cout << "Swap successful." << std::endl;

    // Test range constructor
    msl::pair<int, int> arr[] = { msl::make_pair(1, 1), msl::make_pair(2, 4), msl::make_pair(3, 9) };
    hash_map<int, int> hm3(arr, arr + 3);
    assert(hm3.size() == 3);
    assert(hm3[1] == 1);
    assert(hm3[2] == 4);
    assert(hm3[3] == 9);
    std::cout << "Range constructor successful." << std::endl;

    std::cout << "All hash_map tests passed!" << std::endl;
}

int main() {
    print();
    test_hash_map();
    return 0;
}

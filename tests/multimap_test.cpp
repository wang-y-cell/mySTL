#include "stl_multimap.h"
#include <iostream>
#include <string>
#include <cassert>

void print(){
    std::cout << "==========================================" << std::endl;
}
// Simple test for multimap
int main() {
    print();
    std::cout << "Testing multimap..." << std::endl;
    msl::multimap<int, std::string> mm;
    
    // Insert using pair
    std::cout << "1. Testing insert..." << std::endl;
    mm.insert(msl::make_pair(1, std::string("one")));
    mm.insert(msl::make_pair(2, std::string("two")));
    mm.insert(msl::make_pair(1, std::string("one_again"))); // Duplicate key
    mm.insert(msl::make_pair(3, std::string("three")));
    
    // Verify size
    std::cout << "Size: " << mm.size() << std::endl;
    assert(mm.size() == 4);

    // Traversal
    std::cout << "2. Testing traversal..." << std::endl;
    std::cout << "Elements:" << std::endl;
    for (msl::multimap<int, std::string>::iterator it = mm.begin(); it != mm.end(); ++it) {
        std::cout << it->first << " -> " << it->second << std::endl;
    }
    
    // Count
    std::cout << "3. Testing count..." << std::endl;
    size_t count1 = mm.count(1);
    std::cout << "Count of key 1: " << count1 << std::endl;
    assert(count1 == 2);
    assert(mm.count(2) == 1);
    assert(mm.count(4) == 0);

    // Find
    std::cout << "4. Testing find..." << std::endl;
    msl::multimap<int, std::string>::iterator it = mm.find(2);
    assert(it != mm.end());
    assert(it->second == "two");
    
    it = mm.find(4);
    assert(it == mm.end());

    // Erase
    std::cout << "5. Testing erase..." << std::endl;
    size_t erased_count = mm.erase(1);
    std::cout << "Erased " << erased_count << " elements with key 1" << std::endl;
    assert(erased_count == 2);
    assert(mm.size() == 2);
    
    assert(mm.count(1) == 0);
    assert(mm.find(1) == mm.end());

    std::cout << "All multimap tests passed!" << std::endl;
    return 0;
}

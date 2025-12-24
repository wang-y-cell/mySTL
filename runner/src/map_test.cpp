#include "../include/map.h"
#include <iostream>
#include <string>

// Simple test for map
int main() {
    msl::map<int, std::string> m;
    
    // Insert using pair
    m.insert(msl::pair<const int, std::string>(1, "one"));
    m.insert(msl::make_pair(2, std::string("two")));
    
    // Insert using operator[]
    m[3] = "three";
    m[1] = "ONE"; // Update existing

    std::cout << "Size: " << m.size() << std::endl; // Should be 3
    if (m.size() != 3) {
        std::cout << "Error: Size is " << m.size() << ", expected 3" << std::endl;
        return 1;
    }

    // Traversal
    std::cout << "Elements:" << std::endl;
    for (msl::map<int, std::string>::iterator it = m.begin(); it != m.end(); ++it) {
        std::cout << it->first << " -> " << it->second << std::endl;
    }
    // Expected: 1->ONE, 2->two, 3->three

    // Find
    msl::map<int, std::string>::iterator it = m.find(2);
    if (it != m.end()) std::cout << "Found 2: " << it->second << std::endl;
    else {
        std::cout << "Error: 2 not found" << std::endl;
        return 2;
    }

    // Access via operator[]
    if (m[3] == "three") std::cout << "m[3] is correct" << std::endl;
    else {
        std::cout << "Error: m[3] is " << m[3] << std::endl;
        return 3;
    }

    // Erase
    m.erase(2);
    if (m.size() != 2) {
        std::cout << "Error: Size after erase is " << m.size() << ", expected 2" << std::endl;
        return 4;
    }

    // Clear
    m.clear();
    if (!m.empty()) {
        std::cout << "Error: Map not empty after clear" << std::endl;
        return 5;
    }

    // Range constructor and swap
    msl::pair<const int, std::string> arr[] = {
        msl::pair<const int, std::string>(10, "ten"),
        msl::pair<const int, std::string>(20, "twenty")
    };
    msl::map<int, std::string> m2(arr, arr + 2);
    
    m[100] = "hundred";
    m.swap(m2);
    
    if (m.size() != 2 || m2.size() != 1) {
        std::cout << "Swap failed" << std::endl;
        return 6;
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include "set.h"
#include <iostream>
#include <string>

void print(){
    std::cout << "==========================================" << std::endl;
}
// Simple test for set
int main() {
    print();
    msl::set<int> s;
    s.insert(10);
    s.insert(5);
    s.insert(15);
    s.insert(10); // Duplicate

    std::cout << "Size: " << s.size() << std::endl; // Should be 3
    if (s.size() != 3) {
        std::cout << "Error: Size is " << s.size() << ", expected 3" << std::endl;
        return 1;
    }

    // Traversal
    std::cout << "Elements: ";
    for (msl::set<int>::iterator it = s.begin(); it != s.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // Find
    if (s.find(5) != s.end()) std::cout << "Found 5" << std::endl;
    else {
        std::cout << "Error: 5 not found" << std::endl;
        return 2;
    }
    if (s.find(20) == s.end()) std::cout << "Did not find 20" << std::endl;
    else {
        std::cout << "Error: 20 found" << std::endl;
        return 3;
    }

    // Erase
    s.erase(5);
    if (s.size() != 2) {
        std::cout << "Error: Size after erase is " << s.size() << ", expected 2" << std::endl;
        return 4;
    }
    if (s.find(5) != s.end()) {
        std::cout << "Error: 5 still found after erase" << std::endl;
        return 5;
    }

    // Clear
    s.clear();
    if (!s.empty()) {
        std::cout << "Error: Set not empty after clear" << std::endl;
        return 6;
    }

    // Test range constructor
    int arr[] = {1, 2, 3};
    msl::set<int> s2(arr, arr + 3);
    if (s2.size() != 3) {
         std::cout << "Range constructor failed" << std::endl;
         return 7;
    }

    // Test swap
    s.insert(100);
    s.swap(s2);
    if (s.size() != 3 || s2.size() != 1) {
        std::cout << "Swap failed. s size: " << s.size() << ", s2 size: " << s2.size() << std::endl;
        return 8;
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

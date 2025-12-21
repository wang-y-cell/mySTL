#include "stl_tree.h"
#include <functional>
#include <iostream>

// Define identity functor if not available or for compatibility
template <class T>
struct identity {
    const T& operator()(const T& x) const { return x; }
};

int main(){
    // Use msl::rb_tree and msl::alloc
    msl::rb_tree<int, int, identity<int>, std::less<int>, msl::alloc> tree;
    
    std::cout << "Tree initialized. Empty: " << (tree.empty() ? "yes" : "no") << std::endl;
    
    // Test insert_unique
    tree.insert_unique(10);
    tree.insert_unique(5);
    tree.insert_unique(15);
    tree.insert_unique(10); // Duplicate, should not be inserted
    
    std::cout << "Size after unique inserts (10, 5, 15, 10): " << tree.size() << std::endl;
    
    // Test insert_equal
    tree.insert_equal(10); // Duplicate, should be inserted
    
    std::cout << "Size after equal insert (10): " << tree.size() << std::endl;
    
    // Iterate and print
    std::cout << "Elements: ";
    for(auto it = tree.begin(); it != tree.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}

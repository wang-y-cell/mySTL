#include "memory.h"
#include "stl_vector.h"
#include "stl_hashtable.h"
#include <iostream>
#include <cassert>

using namespace msl;

struct IntHash {
    size_t operator()(int x) const { return (size_t)x; }
};

struct IntIdentity {
    const int& operator()(const int& x) const { return x; }
};

struct IntEqual {
    bool operator()(int a, int b) const { return a == b; }
};

void test_hashtable() {
    std::cout << "Testing hashtable construction..." << std::endl;
    // Initial size 50 -> next prime is 53
    hashtable<int, int, IntHash, IntIdentity, IntEqual> ht(50, IntHash(), IntEqual());
    
    assert(ht.bucket_count() == 53);
    std::cout << "bucket_count: " << ht.bucket_count() << " (Expected 53)" << std::endl;

    std::cout << "Testing insert_unique..." << std::endl;
    // Insert 1
    auto res1 = ht.insert_unique(1);
    assert(res1.second == true);
    assert(*res1.first == 1);
    std::cout << "Insert 1 successful." << std::endl;
    
    // Insert 1 again (fail)
    auto res2 = ht.insert_unique(1);
    assert(res2.second == false);
    assert(*res2.first == 1); 
    std::cout << "Insert 1 duplicate handled." << std::endl;
    
    // Insert 54 (collision with 1: 54%53 = 1)
    auto res3 = ht.insert_unique(54);
    assert(res3.second == true);
    assert(*res3.first == 54);
    std::cout << "Insert 54 (collision) successful." << std::endl;
    
    std::cout << "Testing insert_equal..." << std::endl;
    // Insert 1 again with equal
    auto it4 = ht.insert_equal(1);
    assert(*it4 == 1);
    std::cout << "Insert 1 (equal) successful." << std::endl;
    
    // Check if we can find the items by iterating? 
    // We don't have begin(), but we can check if inserting more triggers resize.
    // max_bucket_count is large.
    
    // Create new node test
    auto node = ht.new_node(999);
    assert(node->val == 999);
    ht.delete_node(node);
    std::cout << "new_node/delete_node successful." << std::endl;

    std::cout << "Testing clear()..." << std::endl;
    // Current size should be > 0 (1, 54, 1(equal) -> 3 elements)
    assert(!ht.empty());
    assert(ht.size() == 3);
    
    ht.clear();
    
    assert(ht.empty());
    assert(ht.size() == 0);
    assert(ht.bucket_count() == 53); // Bucket count should remain same
    std::cout << "clear() successful. Size is 0." << std::endl;

    // Insert again after clear
    ht.insert_unique(100);
    assert(ht.size() == 1);
    assert(*ht.insert_unique(100).first == 100);
    std::cout << "Insert after clear successful." << std::endl;

    std::cout << "All basic tests passed!" << std::endl;
}

int main() {
    test_hashtable();
    return 0;
}

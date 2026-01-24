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

    std::cout << "Testing count..." << std::endl;
    ht.clear();
    ht.insert_unique(10);
    ht.insert_unique(20);
    ht.insert_unique(10); // duplicate, count should still be 1 for unique
    
    assert(ht.count(10) == 1);
    assert(ht.count(20) == 1);
    assert(ht.count(30) == 0);

    ht.insert_equal(10);
    assert(ht.count(10) == 2);
    ht.insert_equal(10);
    assert(ht.count(10) == 3);
    
    std::cout << "count() successful." << std::endl;

    std::cout << "Testing erase..." << std::endl;
    // Erase by key
    size_t erased_count = ht.erase(10);
    assert(erased_count == 3);
    assert(ht.count(10) == 0);
    std::cout << "erase(key) successful." << std::endl;

    // Erase by iterator
    ht.insert_unique(99);
    auto it = ht.find(99);
    assert(it != ht.end());
    ht.erase(it);
    assert(ht.find(99) == ht.end());
    std::cout << "erase(iterator) successful." << std::endl;

    std::cout << "Testing swap..." << std::endl;
    ht.clear(); // Clear ht first to make it empty
    hashtable<int, int, IntHash, IntIdentity, IntEqual> ht2(50, IntHash(), IntEqual());
    ht2.insert_unique(777);
    ht.swap(ht2);
    assert(ht.count(777) == 1);
    assert(ht2.count(777) == 0);
    assert(ht2.empty()); // Now ht2 should be empty because ht was cleared before swap
    std::cout << "swap() successful." << std::endl;

    std::cout << "All basic tests passed!" << std::endl;
}



int main() {
    test_hashtable();
    return 0;
}

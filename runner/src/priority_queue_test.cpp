#include <iostream>
#include "queue.h"
#include <vector>
#include <cassert>
void print(){
    std::cout << "==========================================" << std::endl;
}
// Simple test for priority_queue
void test_priority_queue() {
    msl::priority_queue<int> pq;
    
    assert(pq.empty());
    assert(pq.size() == 0);

    pq.push(3);
    pq.push(1);
    pq.push(4);
    pq.push(1);
    pq.push(5);

    assert(!pq.empty());
    assert(pq.size() == 5);
    assert(pq.top() == 5);

    pq.pop();
    assert(pq.top() == 4);
    pq.pop();
    assert(pq.top() == 3);
    
    // Test iterator constructor
    int arr[] = {10, 20, 5};
    
    msl::priority_queue<int> pq2(arr, arr + 3);
    assert(pq2.size() == 3);
    assert(pq2.top() == 20);

    // Test copy constructor
    msl::priority_queue<int> pq3(pq2);
    assert(pq3.size() == 3);
    assert(pq3.top() == 20);
    pq2.pop();
    assert(pq3.size() == 3); // pq3 should be independent
    assert(pq2.size() == 2);

    // Test copy assignment
    msl::priority_queue<int> pq4;
    pq4 = pq3;
    assert(pq4.size() == 3);
    assert(pq4.top() == 20);

    // Test move constructor
    msl::priority_queue<int> pq5(msl::move(pq3));
    assert(pq5.size() == 3);
    assert(pq5.top() == 20);
    // pq3 state is unspecified but usually empty for vector
    // assert(pq3.empty()); // valid for vector implementation

    // Test move assignment
    msl::priority_queue<int> pq6;
    pq6 = msl::move(pq5);
    assert(pq6.size() == 3);
    assert(pq6.top() == 20);

    std::cout << "priority_queue test passed" << std::endl;
}

int main() {
    print();
    test_priority_queue();
    return 0;
}

#include <iostream>
#include <cassert>
#include <sstream>
#include "vector.h"
#include "deque.h"
#include "iterator.h"
#include "stl_algo.h" 

void text_back_insert_iterator(){
    std::cout << "Testing back_insert_iterator..." << std::endl;

    // Test 1: Manual usage
    msl::vector<int> v;
    msl::back_insert_iterator<msl::vector<int> > it(v);
    
    *it = 1;
    *it = 2;
    *it = 3;

    assert(v.size() == 3);
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);
    std::cout << "Manual insertion passed." << std::endl;

    // Test 2: Using back_inserter helper
    msl::vector<int> v2;
    auto it2 = msl::back_inserter(v2);
    *it2 = 10;
    ++it2; // check operator++
    *it2 = 20;
    it2++; // check operator++(int)
    *it2 = 30;

    assert(v2.size() == 3);
    assert(v2[0] == 10);
    assert(v2[1] == 20);
    assert(v2[2] == 30);
    std::cout << "back_inserter helper passed." << std::endl;
}
void text_front_insert_iterator(){
    std::cout << "Testing front_insert_iterator..." << std::endl;
    // Test 3: Manual usage with deque
    msl::deque<int> d;
    msl::front_insert_iterator<msl::deque<int> > fit(d);
    
    *fit = 1;
    *fit = 2;
    *fit = 3;

    assert(d.size() == 3);
    assert(d[0] == 3);
    assert(d[1] == 2);
    assert(d[2] == 1);
    std::cout << "Manual front insertion passed." << std::endl;

    // Test 4: Using front_inserter helper
    msl::deque<int> d2;
    auto fit2 = msl::front_inserter(d2);
    *fit2 = 10;
    ++fit2;
    *fit2 = 20;
    fit2++;
    *fit2 = 30;

    assert(d2.size() == 3);
    assert(d2[0] == 30);
    assert(d2[1] == 20);
    assert(d2[2] == 10);
    std::cout << "front_inserter helper passed." << std::endl;
}
void text_reverse_iterator(){
    std::cout << "Testing reverse_iterator..." << std::endl;
    msl::vector<int> v;
    for(int i=1; i<=5; ++i) v.push_back(i);

    int expected[] = {5, 4, 3, 2, 1};
    int i = 0;
    for(msl::vector<int>::reverse_iterator rit = v.rbegin(); rit != v.rend(); ++rit, ++i){
        assert(*rit == expected[i]);
    }
    std::cout << "Reverse traversal passed." << std::endl;

    auto rit = v.rbegin();
    assert(*rit == 5);
    assert(*(rit + 1) == 4);
    assert(rit[2] == 3);
    
    rit += 2;
    assert(*rit == 3);
    
    rit -= 1;
    assert(*rit == 4);
    
    std::cout << "Reverse iterator operators passed." << std::endl;
}
void text_insert_iterator(){
    std::cout << "Testing insert_iterator..." << std::endl;
    msl::vector<int> v;
    auto it = msl::inserter(v, v.begin());
    *it = 1;
    *it = 2;
    *it = 3;

    assert(v.size() == 3);
    assert(v[0] == 1);
    assert(v[1] == 2);
    assert(v[2] == 3);
    std::cout << "insert_iterator passed." << std::endl;
}
void text_istream_iterator(){
    std::cout << "Testing istream_iterator..." << std::endl;
    // Simulate input using stringstream since std::cin blocks
    std::stringstream ss("10 20 30");
    
    msl::vector<int> id;
    msl::istream_iterator<int> it(ss), eos;
    msl::copy(it, eos, msl::inserter(id, id.begin()));
    
    assert(id.size() == 3);
    assert(id[0] == 10);
    assert(id[1] == 20);
    assert(id[2] == 30);
    
    for(int i=0; i<id.size(); ++i){
        std::cout << id[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "istream_iterator passed." << std::endl;
}
void text_ostream_iterator() {
    std::cout << "Testing ostream_iterator..." << std::endl;
    msl::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    std::cout << "Values in vector: ";
    msl::copy(v.begin(), v.end(), msl::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
    std::cout << "ostream_iterator passed." << std::endl;
}

int main() {
    text_back_insert_iterator();
    text_front_insert_iterator();
    text_reverse_iterator();
    text_insert_iterator();
    text_istream_iterator();
    text_ostream_iterator();
    std::cout << "All tests passed!" << std::endl;

    return 0;
}

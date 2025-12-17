#include <iostream>
#include <stack>
#include <cassert>
#include "stack.h"

int main() {
    msl::stack<int> s;
    std::stack<int> std_s;

    assert(s.empty());
    assert(s.size() == 0);

    for (int i = 0; i < 10; ++i) {
        s.push(i);
        std_s.push(i);
    }

    assert(!s.empty());
    assert(s.size() == 10);
    assert(s.top() == 9);

    while (!s.empty()) {
        assert(s.top() == std_s.top());
        s.pop();
        std_s.pop();
    }

    assert(s.size() == 0);
    assert(s.empty());

    // Comparison tests
    msl::stack<int> s1, s2;
    s1.push(1);
    s2.push(1);
    assert(s1 == s2);
    
    s2.push(2);
    assert(s1 < s2);
    assert(s1 != s2);

    std::cout << "stack test passed" << std::endl;
    return 0;
}

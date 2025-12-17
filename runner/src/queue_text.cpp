#include <iostream>
#include <queue>
#include "list.h"
#include <cassert>
#include "queue.h"

int main() {
    msl::queue<int> q;
    std::queue<int> std_q;
    msl::queue<int, msl::list<int>> qt; // 可以使用list作为底层容器

    assert(q.empty());
    assert(q.size() == 0);

    for (int i = 0; i < 10; ++i) {
        q.push(i);
        std_q.push(i);
    }

    assert(!q.empty());
    assert(q.size() == 10);
    assert(q.front() == 0);
    assert(q.back() == 9);

    while (!q.empty()) {
        assert(q.front() == std_q.front());
        q.pop();
        std_q.pop();
    }

    assert(q.size() == 0);
    assert(q.empty());

    // Comparison tests
    msl::queue<int> q1, q2;
    q1.push(1);
    q2.push(1);
    assert(q1 == q2);
    
    q2.push(2);
    assert(q1 < q2);
    assert(q1 != q2);

    std::cout << "queue test passed" << std::endl;
    return 0;
}

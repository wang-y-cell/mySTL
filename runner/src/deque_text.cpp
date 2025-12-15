#include<cassert>
#include<cstdlib>
#include<ctime>
#include<deque>
#include<iostream>
#include "deque.h"

void deque_equal(const std::deque<int>& dq, const msl::deque<int>& dq2) {
    assert(dq.size() == dq2.size());
    for(int i = 0; i < dq.size(); ++i) {
        assert(dq[i] == dq2[i]);
    }
}



int main(){
    srand(time(0));


    std::cout << "text deque" << std::endl;
    //  msl::deque<int> dq(10, 1);
    // assert(dq.size() == 10);
    // for(int i = 0; i < 10; ++i) {
    //     assert(dq[i] == 1);
    // }
    // for(int i = 0; i < 10; ++i)dq[i] = i;
    // for(int i = 0; i < 10; ++i) {
    //     assert(dq[i] == i);
    // }
    // std::cout << "deque size: " << dq.size() << std::endl;
    // std::cout << "deque front: " << dq.front() << std::endl;
    // std::cout << "deque back: " << dq.back() << std::endl;
    // std::cout << "deque begin: " << *dq.begin() << std::endl;
    // std::cout << "deque end: " << *dq.end() << std::endl;
    // std::cout << "deque --end(): " << *(--dq.end()) << std::endl;

    // 测试随机插入和删除
    msl::deque<int> dq;
    std::deque<int> dq2;
    int size = 2000;
    for(int i = 0; i < size; i++){
        int node = rand() % 100 + 1;
        // std::cout << "iter: " << i << " push_back: " << node << std::endl;
        dq.push_back(node);
        // std::cout << "dq pushed. size: " << dq.size() << std::endl;
        dq2.push_back(node);
        // std::cout << "dq2 pushed." << std::endl;
        deque_equal(dq2, dq);
        // std::cout << "equal check passed." << std::endl;
    }

    for(int i = 0; i < size; i++){
        int node = rand() % 100 + 1;
        // std::cout << "push_front: " << node << std::endl;
        dq.push_front(node);
        dq2.push_front(node);
        deque_equal(dq2, dq);
    }










    std::cout << "deque test pass" << std::endl;
    return 0;
}
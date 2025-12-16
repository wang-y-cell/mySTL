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
     msl::deque<int> dq(10, 1);
    assert(dq.size() == 10);
    for(int i = 0; i < 10; ++i) {
        assert(dq[i] == 1);
    }
    for(int i = 0; i < 10; ++i)dq[i] = i;
    for(int i = 0; i < 10; ++i) {
        assert(dq[i] == i);
    }
    std::cout << "deque size: " << dq.size() << std::endl;
    std::cout << "deque front: " << dq.front() << std::endl;
    std::cout << "deque back: " << dq.back() << std::endl;
    std::cout << "deque begin: " << *dq.begin() << std::endl;
    // std::cout << "deque end: " << *dq.end() << std::endl; // end() points to past-the-last element, dereferencing is UB
    std::cout << "deque --end(): " << *(--dq.end()) << std::endl;
    
    std::cout << "deque clear" << std::endl;   
    dq.clear();
    assert(dq.size() == 0);

    // 测试随机插入和删除
    std::cout << std::endl;
    std::cout << "deque push_back and push_front" << std::endl;
    std::deque<int> dq2;
    int size = 2000;
    for(int i = 0; i < size; i++){
        int node = rand() % 100 + 1;
        dq.push_back(node);
        dq2.push_back(node);
        deque_equal(dq2, dq);
    }

    for(int i = 0; i < size; i++){
        int node = rand() % 100 + 1;
        dq.push_front(node);
        dq2.push_front(node);
        deque_equal(dq2, dq);
    }
    for(int i = 0; i < size; i++){
        dq.pop_back();
        dq2.pop_back();
        deque_equal(dq2, dq);
    }
    std::cout << "deque size after pop_back: " << dq.size() << std::endl;

    for(int i = 0; i < size; i++){
        dq.pop_front();
        dq2.pop_front();
        deque_equal(dq2, dq);
    }
    std::cout << "deque size after pop_front: " << dq.size() << std::endl;

    // 测试随机插入
    std::cout << std::endl;
    std::cout << "deque insert" << std::endl;
    std::cout << dq.size() << std::endl;
    dq.push_back(100);
    dq2.push_back(100);
    
    for(int i = 0; i < size; i++){
        int node = rand() % 100 + 1;
        int pos = rand() % dq.size();
        dq.insert(dq.begin() + pos, node);
        dq2.insert(dq2.begin() + pos, node);
        deque_equal(dq2, dq);
    }
    std::cout << "deque size after insert: " << dq.size() << std::endl;

    std::cout << std::endl;
    std::cout << "deque erase" << std::endl;
    for(int i = 0; i < size; i++){
        int pos = rand() % dq.size();
        dq.erase(dq.begin() + pos);
        dq2.erase(dq2.begin() + pos);
        deque_equal(dq2, dq);
    }
    std::cout << "deque size after erase: " << dq.size() << std::endl;



    std::cout << "deque test pass" << std::endl;
    return 0;
}
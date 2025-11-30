#include"list.h"
#include<iostream>
#include<algorithm>

int main(){
    msl::list<int> l;
    msl::list<int>::iterator it = l.begin();
    std::cout << l.size() << std::endl;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.push_back(3);
    l.push_back(3);
    l.push_back(4);
    l.push_back(5);
    l.unique();
    for(it = l.begin(); it != l.end(); it++){
        std::cout << *it << std::endl;
    }
}

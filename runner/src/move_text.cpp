#include "utility.h"
#include <iostream>
#include <string>

void print(){
    std::cout << "==========================================" << std::endl;
}



void process(int& i) {
    std::cout << "process(int&): " << i << std::endl;
}

void process(int&& i) {
    std::cout << "process(int&&): " << i << std::endl;
}

template<typename T>
void wrapper(T&& param) {
    process(msl::forward<T>(param));
}

class MoveTest {
public:
    std::string s;
    MoveTest(std::string str) : s(msl::move(str)) {
        std::cout << "MoveTest constructor" << std::endl;
    }
};

int main() {
    print();
    int a = 10;
    std::cout << "Test forward lvalue:" << std::endl;
    wrapper(a); // Should call process(int&)

    std::cout << "Test forward rvalue:" << std::endl;
    wrapper(20); // Should call process(int&&)

    std::cout << "Test move:" << std::endl;
    std::string str = "Hello";
    MoveTest mt(msl::move(str));
    if (str.empty()) { 
         std::cout << "String moved successfully" << std::endl;
    } else {
         std::cout << "String NOT moved (or implementation dependent)" << std::endl;
    }

    return 0;
}

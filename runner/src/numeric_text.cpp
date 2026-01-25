#include "stl_numeric.h"
#include "stl_functional.h"
#include <iostream>
void print(){
    std::cout << "==========================================" << std::endl;
}
void text_adjacent_difference(){
    int a[] = {1, 2, 3, 4, 5};
    int b[5];
    std::cout << "adjacent_difference: ";
    msl::adjacent_difference(a, a + 5, b);
    for(int i = 0; i < 5; ++i) {
        std::cout << b[i] << " ";
    }
    std::cout << std::endl;
}

void text_adjacent_difference_v2(){
    int a[] = {1, 2, 3, 4, 5};
    int b[5];
    std::cout << "adjacent_difference v2 (multiplies): ";
    msl::adjacent_difference(a, a + 5, b, msl::multiplies<int>());
    for(int i = 0; i < 5; ++i) {
        std::cout << b[i] << " ";
    }
    std::cout << std::endl;
}

void text_accumulate(){
    int a[] = {1, 2, 3, 4, 5};
    int sum = msl::accumulate(a, a + 5, 0);
    std::cout << "accumulate: " << sum << std::endl;
}

void text_partial_sum(){
    int a[] = {1, 2, 3, 4, 5};
    int b[5];
    std::cout << "partial_sum: ";
    msl::partial_sum(a, a + 5, b);
    for(int i = 0; i < 5; ++i) {
        std::cout << b[i] << " ";
    }
    std::cout << std::endl;
}

void text_partial_sum_v2(){
    int a[] = {1, 2, 3, 4, 5};
    int b[5];
    std::cout << "partial_sum v2 (multiplies): ";
    msl::partial_sum(a, a + 5, b, msl::multiplies<int>());
    for(int i = 0; i < 5; ++i) {
        std::cout << b[i] << " ";
    }
    std::cout << std::endl;
}

void text_power(){
    int a = 2;
    int b = 3;
    int res = msl::power(a, b, msl::multiplies<int>());
    std::cout << "power: " << res << std::endl;
}

void text_iota(){
    int a[5];
    msl::iota(a, a + 5, 1);
    std::cout << "iota: ";
    for(int i = 0; i < 5; ++i) {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
}

int main(){
    print();
    text_accumulate();
    text_adjacent_difference();
    text_adjacent_difference_v2();
    text_partial_sum();
    text_partial_sum_v2();
    text_power();
    text_iota();
    return 0;
}

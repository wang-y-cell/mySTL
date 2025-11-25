#include<iostream>
#include"vector.h"
#include<string>


int main(){
    msl::vector<int> a;
    for(int i=1;i<=5;++i) a.push_back(i);
    for (int i = 0; i < a.size(); ++i) std::cout << a[i] << " "; std::cout << "\n";

    a.insert(a.begin(), 3, 99);
    //std::cout << a.size() << std::endl;
    for (int i = 0; i < a.size(); ++i) std::cout << a[i] << " "; std::cout << "\n";

    msl::vector<int> b;
    b.push_back(7); b.push_back(8); b.push_back(9);
    a.insert(a.begin()+2, b.begin(), b.end());
    for (int i = 0; i < a.size(); ++i) std::cout << a[i] << " "; std::cout << "\n";

    a.erase(a.begin()+1);
    for (int i = 0; i < a.size(); ++i) std::cout << a[i] << " "; std::cout << "\n";

    if (a.size() > 4) a.erase(a.begin()+1, a.begin()+4);
    for (int i = 0; i < a.size(); ++i) std::cout << a[i] << " "; std::cout << "\n";

    a.resize(10, -1);
    for (int i = 0; i < a.size(); ++i) std::cout << a[i] << " "; std::cout << "\n";

    a.resize(4);
    for (int i = 0; i < a.size(); ++i) std::cout << a[i] << " "; std::cout << "\n";

    a.reserve(20);
    a.push_back(100);
    for (int i = 0; i < a.size(); ++i) std::cout << a[i] << " "; std::cout << "\n";

    msl::vector<int> c(a);
    for (int i = 0; i < c.size(); ++i) std::cout << c[i] << " "; std::cout << "\n";

    msl::vector<int> d; d = a;
    for (int i = 0; i < d.size(); ++i) std::cout << d[i] << " "; std::cout << "\n";

    msl::vector<std::string> s;
    s.push_back("hello");
    s.push_back("world");
    s.insert(s.begin()+1, 2, std::string("x"));
    msl::vector<std::string> t; t.push_back("A"); t.push_back("B");
    s.insert(s.begin()+2, t.begin(), t.end());
    for (int i = 0; i < s.size(); ++i) std::cout << s[i] << " "; std::cout << "\n";

    try { (void)a.at(100); std::cout << "no" << "\n"; }
    catch (const std::out_of_range&) { std::cout << "out_of_range" << "\n"; }

    a.clear();
    std::cout << a.size() << "\n";

    return 0;
}

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <list>
#include <vector>
#include <iostream>
#include "list.h"

static std::vector<int> collect(const msl::list<int>& a) {
    std::vector<int> out;
    for (msl::list<int>::const_iterator it = a.begin(); it != a.end(); ++it) out.push_back(*it);
    return out;
}

static std::vector<int> collect(const std::list<int>& b) {
    std::vector<int> out;
    for (std::list<int>::const_iterator it = b.begin(); it != b.end(); ++it) out.push_back(*it);
    return out;
}

static void assert_equal(const msl::list<int>& a, const std::list<int>& b) {
    auto va = collect(a);
    auto vb = collect(b);
    std::cout << "collect sizes: msl=" << va.size() << ", std=" << vb.size() << std::endl;
    assert(va.size() == vb.size());
    for (std::size_t i = 0; i < va.size(); ++i) assert(va[i] == vb[i]);
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));

    msl::list<int> l;
    std::list<int> ref;

    for (int i = 0; i < 50; ++i) {
        int x = std::rand() % 1000;
        l.push_back(x);
        ref.push_back(x);
    }
    std::cout << "sizes after push_back: msl=" << l.size() << ", std=" << ref.size() << std::endl;
    assert_equal(l, ref);

    for (int i = 0; i < 10; ++i) {
        int y = std::rand() % 1000;
        l.push_front(y);
        ref.push_front(y);
    }
    assert_equal(l, ref);

    if (!l.empty()) {
        msl::list<int>::iterator p = l.begin();
        l.erase(p);
        ref.erase(ref.begin());
        assert_equal(l, ref);
    }

    for (int i = 0; i < 5; ++i) { l.push_back(7); ref.push_back(7); }
    l.remove(7); ref.remove(7);
    assert_equal(l, ref);

    for (int i = 0; i < 3; ++i) { l.push_back(42); ref.push_back(42); }
    l.unique(); ref.unique();
    assert_equal(l, ref);

    msl::list<int> s1, s2; std::list<int> rs1, rs2;
    for (int i = 0; i < 10; ++i) { int v = std::rand() % 100; s1.push_back(v); rs1.push_back(v); }
    for (int i = 0; i < 8; ++i) { int v = std::rand() % 100; s2.push_back(v); rs2.push_back(v); }
    s1.splice(s1.begin(), s2); rs1.splice(rs1.begin(), rs2);
    assert_equal(s1, rs1);
    assert_equal(s2, rs2);
    if (!s1.empty()) {
        msl::list<int>::iterator it = s1.begin(); ++it;
        std::list<int>::iterator rit = rs1.begin(); ++rit;
        s2.splice(s2.end(), s1, it); rs2.splice(rs2.end(), rs1, rit);
        assert_equal(s1, rs1);
        assert_equal(s2, rs2);
    }
    s1.swap(s2); rs1.swap(rs2);
    assert_equal(s1, rs1);
    assert_equal(s2, rs2);

    msl::list<int> a, b;
    std::list<int> ra, rb;
    for (int i = 0; i < 20; ++i) a.push_back(std::rand() % 200);
    for (int i = 0; i < 20; ++i) b.push_back(std::rand() % 200);
    for (int i = 0; i < 20; ++i) ra.push_back(a.front()), a.erase(a.begin());
    for (int i = 0; i < 20; ++i) rb.push_back(b.front()), b.erase(b.begin());
    a.clear(); b.clear();
    for (int v : ra) a.push_back(v);
    for (int v : rb) b.push_back(v);
    ra.sort(); rb.sort();
    a.sort(); b.sort();
    a.merge(b); ra.merge(rb);
    assert_equal(a, ra);

    l.reverse(); ref.reverse();
    assert_equal(l, ref);

    l.sort(); ref.sort();
    assert_equal(l, ref);

    std::cout << "list tests passed" << std::endl;
    return 0;
}

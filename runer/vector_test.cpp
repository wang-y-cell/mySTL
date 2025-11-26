#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include "vector.h"

static void assert_equal(const msl::vector<int>& a, const std::vector<int>& b) {
    assert(a.size() == b.size());
    for (msl::size_t i = 0; i < a.size(); ++i) {
        assert(a[i] == b[i]);
    }
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));

    msl::vector<int> v;
    std::vector<int> ref;

    for (int i = 0; i < 200; ++i) {
        int x = std::rand() % 1000;
        v.push_back(x);
        ref.push_back(x);
    }
    assert_equal(v, ref);

    for (int i = 0; i < 50; ++i) {
        if (!ref.empty()) { v.pop_back(); ref.pop_back(); }
    }
    assert_equal(v, ref);

    msl::size_t cap_before = v.capacity();
    v.reserve(static_cast<msl::size_t>(ref.size() + 100));
    assert(v.capacity() >= ref.size() + 100);
    assert(v.size() == ref.size());

    int val = 7;
    v.insert(v.begin() + (v.size() ? v.size() / 2 : 0), 5, val);
    ref.insert(ref.begin() + (ref.size() ? ref.size() / 2 : 0), 5, val);
    assert_equal(v, ref);

    msl::vector<int> tmp;
    for (int i = 0; i < 10; ++i) tmp.push_back(i * i);
    v.insert(v.begin() + (v.size() ? v.size() / 3 : 0), tmp.begin(), tmp.end());
    ref.insert(ref.begin() + (ref.size() ? ref.size() / 3 : 0), tmp.begin(), tmp.end());
    assert_equal(v, ref);

    if (v.size() > 5) {
        msl::size_t i = v.size() / 4;
        msl::size_t j = i + 3;
        v.erase(v.begin() + i, v.begin() + j);
        ref.erase(ref.begin() + static_cast<std::vector<int>::size_type>(i),
                  ref.begin() + static_cast<std::vector<int>::size_type>(j));
    }
    assert_equal(v, ref);

    v.assign(12, 42);
    ref.assign(12, 42);
    assert_equal(v, ref);

    msl::vector<int> src_msl;
    for (int i = 0; i < 25; ++i) src_msl.push_back(std::rand() % 500);
    v.assign(src_msl.begin(), src_msl.end());
    ref.assign(src_msl.begin(), src_msl.end());
    assert_equal(v, ref);

    msl::vector<int> src_input_like;
    for (int i = 0; i < 15; ++i) src_input_like.push_back(std::rand() % 300);
    v.assign(src_input_like.data(), src_input_like.data() + src_input_like.size());
    ref.assign(src_input_like.data(), src_input_like.data() + src_input_like.size());
    assert_equal(v, ref);

    msl::vector<int> v2(v);
    assert(v == v2);
    if (!v2.empty()) { v2[0] = v2[0] + 1; assert(v2 > v); }

    try { (void)v.at(static_cast<msl::size_t>(v.size() + 10)); assert(false); }
    catch (const std::out_of_range&) {}

    msl::vector<int>::allocator_type a1 = v.get_allocator(); (void)a1;

    msl::vector<std::string> vs;
    vs.push_back("alpha");
    vs.push_back("beta");
    vs.insert(vs.begin() + 1, 2, std::string("x"));
    msl::vector<std::string> ts; ts.push_back("p"); ts.push_back("q");
    vs.insert(vs.begin() + 2, ts.begin(), ts.end());
    assert(vs.size() >= 4);

    v.clear();
    ref.clear();
    assert(v.size() == 0);
    assert_equal(v, ref);

#if __cplusplus >= 201103L
    msl::vector<int> v_init{1, 2, 3, 4, 5};
    std::vector<int> ref_init{1, 2, 3, 4, 5};
    assert_equal(v_init, ref_init);
    v_init.insert(v_init.begin() + 2, {7, 8});
    ref_init.insert(ref_init.begin() + 2, {7, 8});
    assert_equal(v_init, ref_init);

    struct P { int x; int y; P(int a, int b) : x(a), y(b) {} };
    msl::vector<P> pv;
    pv.emplace_back(1, 2);
    pv.emplace_back(3, 4);
    pv.emplace(pv.begin() + 1, 9, 8);
    assert(pv.size() == 3);
    assert(pv[0].x == 1 && pv[0].y == 2);
    assert(pv[1].x == 9 && pv[1].y == 8);
    assert(pv[2].x == 3 && pv[2].y == 4);

    msl::vector<std::string> vs2;
    vs2.emplace_back(3, 'a');
    vs2.emplace(vs2.begin(), 2, 'b');
    assert(vs2.size() == 2);
    assert(vs2[0] == std::string("bb"));
    assert(vs2[1] == std::string("aaa"));

    msl::vector<int> mv_src;
    std::vector<int> ref_mv;
    for (int i = 0; i < 10; ++i) { mv_src.push_back(i); ref_mv.push_back(i); }
    msl::vector<int> mv_move(std::move(mv_src));
    assert(mv_src.size() == 0);
    assert_equal(mv_move, ref_mv);
    msl::vector<int> mv_assign;
    mv_assign = std::move(mv_move);
    assert(mv_move.size() == 0);
    assert_equal(mv_assign, ref_mv);
#endif

    return 0;
}

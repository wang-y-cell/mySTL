#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <chrono>
#include<vector>
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

    const int N = 100000;
    std::vector<int> data; data.reserve(N);
    for (int i = 0; i < N; ++i) data.push_back(std::rand());

    msl::vector<int> m_push;
    std::vector<int> s_push;
    msl::size_t m_cap = m_push.capacity();
    msl::size_t s_cap = s_push.capacity();
    msl::size_t m_expansions = 0;
    msl::size_t s_expansions = 0;
    auto t0 = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) {
        m_push.push_back(data[i]);
        msl::size_t c = m_push.capacity();
        if (c != m_cap) { ++m_expansions; m_cap = c; }
    }
    auto t1 = std::chrono::steady_clock::now();
    for (int i = 0; i < N; ++i) {
        s_push.push_back(data[i]);
        msl::size_t c = static_cast<msl::size_t>(s_push.capacity());
        if (c != s_cap) { ++s_expansions; s_cap = c; }
    }
    auto t2 = std::chrono::steady_clock::now();
    auto ms_m_push = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto ms_s_push = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    auto mem_m_push = static_cast<unsigned long long>(m_push.capacity()) * sizeof(int);
    auto mem_s_push = static_cast<unsigned long long>(s_push.capacity()) * sizeof(int);
    std::cout << "push_back ms: msl=" << ms_m_push << ", std=" << ms_s_push
              << "; expansions: msl=" << m_expansions << ", std=" << s_expansions
              << "; mem(bytes): msl=" << mem_m_push << ", std=" << mem_s_push << "\n";

    int K = 10000;
    msl::size_t m_exp_ins = 0, s_exp_ins = 0;
    msl::size_t m_cap_ins = m_push.capacity();
    msl::size_t s_cap_ins = static_cast<msl::size_t>(s_push.capacity());
    auto t3 = std::chrono::steady_clock::now();
    for (int i = 0; i < K; ++i) {
        msl::vector<int>::iterator pos = m_push.begin() + (m_push.size() / 2);
        m_push.insert(pos, 1, i);
        msl::size_t c = m_push.capacity();
        if (c != m_cap_ins) { ++m_exp_ins; m_cap_ins = c; }
    }
    auto t4 = std::chrono::steady_clock::now();
    for (int i = 0; i < K; ++i) {
        std::vector<int>::iterator pos = s_push.begin() + (s_push.size() / 2);
        s_push.insert(pos, 1, i);
        msl::size_t c = static_cast<msl::size_t>(s_push.capacity());
        if (c != s_cap_ins) { ++s_exp_ins; s_cap_ins = c; }
    }
    auto t5 = std::chrono::steady_clock::now();
    auto ms_m_ins = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count();
    auto ms_s_ins = std::chrono::duration_cast<std::chrono::milliseconds>(t5 - t4).count();
    auto mem_m_ins = static_cast<unsigned long long>(m_push.capacity()) * sizeof(int);
    auto mem_s_ins = static_cast<unsigned long long>(s_push.capacity()) * sizeof(int);
    std::cout << "insert(mid,1) ms: msl=" << ms_m_ins << ", std=" << ms_s_ins
              << "; expansions: msl=" << m_exp_ins << ", std=" << s_exp_ins
              << "; mem(bytes): msl=" << mem_m_ins << ", std=" << mem_s_ins << "\n";

    msl::size_t m_size = m_push.size();
    msl::size_t s_size = static_cast<msl::size_t>(s_push.size());
    msl::size_t rm = m_size / 10;
    auto t6 = std::chrono::steady_clock::now();
    m_push.erase(m_push.begin() + (m_size / 3), m_push.begin() + (m_size / 3 + rm));
    auto t7 = std::chrono::steady_clock::now();
    s_push.erase(s_push.begin() + static_cast<std::vector<int>::size_type>(s_size / 3),
                 s_push.begin() + static_cast<std::vector<int>::size_type>(s_size / 3 + rm));
    auto t8 = std::chrono::steady_clock::now();
    auto ms_m_erase = std::chrono::duration_cast<std::chrono::milliseconds>(t7 - t6).count();
    auto ms_s_erase = std::chrono::duration_cast<std::chrono::milliseconds>(t8 - t7).count();
    std::cout << "erase(range) ms: msl=" << ms_m_erase << ", std=" << ms_s_erase << "\n";

    long long sum_m = 0, sum_s = 0;
    auto t9 = std::chrono::steady_clock::now();
    for (msl::size_t i = 0; i < m_push.size(); ++i) sum_m += m_push[i];
    auto t10 = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < s_push.size(); ++i) sum_s += s_push[i];
    auto t11 = std::chrono::steady_clock::now();
    auto ms_m_read = std::chrono::duration_cast<std::chrono::milliseconds>(t10 - t9).count();
    auto ms_s_read = std::chrono::duration_cast<std::chrono::milliseconds>(t11 - t10).count();
    std::cout << "read sum ms: msl=" << ms_m_read << ", std=" << ms_s_read << "; sum diff=" << (sum_m - sum_s) << "\n";
#endif

    return 0;
}

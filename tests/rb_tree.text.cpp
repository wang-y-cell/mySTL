#include "stl_tree.h"
#include <functional>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <cassert>
void print(){
    std::cout << "==========================================" << std::endl;
}

// Define identity functor
template <class T>
struct identity {
    const T& operator()(const T& x) const { return x; }
};

void test_random_operations() {
    std::cout << "开始随机操作测试..." << std::endl;
    
    // 配置
    const int NUM_ELEMENTS = 2000;
    const int RANGE_MIN = 0;
    const int RANGE_MAX = 5000;

    // 设置随机数生成器
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(RANGE_MIN, RANGE_MAX);

    // 使用 msl::rb_tree
    typedef msl::rb_tree<int, int, identity<int>, std::less<int>, msl::alloc> TreeType;
    TreeType tree;
    
    std::vector<int> expected_data;

    std::cout << "1. 插入 " << NUM_ELEMENTS << " 个随机元素 (insert_equal)..." << std::endl;
    for (int i = 0; i < NUM_ELEMENTS; ++i) {
        int val = dist(rng);
        tree.insert_equal(val);
        expected_data.push_back(val);
    }

    // 验证大小
    if (tree.size() != NUM_ELEMENTS) {
        std::cerr << "错误: 树的大小不匹配。预期 " << NUM_ELEMENTS << ", 实际 " << tree.size() << std::endl;
        return;
    }

    // 验证顺序
    std::sort(expected_data.begin(), expected_data.end());
    int mismatch_count = 0;
    int idx = 0;
    for (auto it = tree.begin(); it != tree.end(); ++it, ++idx) {
        if (*it != expected_data[idx]) {
            if (mismatch_count < 5) {
                std::cerr << "错误: 索引 " << idx << " 处内容不匹配。树: " << *it << ", 预期: " << expected_data[idx] << std::endl;
            }
            mismatch_count++;
        }
    }
    
    if (mismatch_count == 0) {
        std::cout << "通过: 树元素已正确排序。" << std::endl;
    } else {
        std::cerr << "失败: 发现 " << mismatch_count << " 处不匹配。" << std::endl;
    }

    std::cout << "2. 测试随机删除..." << std::endl;
    
    // 打乱预期数据的顺序以进行随机删除
    std::vector<int> delete_order = expected_data;
    std::shuffle(delete_order.begin(), delete_order.end(), rng);

    // 删除一半的元素
    int delete_count = NUM_ELEMENTS / 2;
    for (int i = 0; i < delete_count; ++i) {
        int val_to_delete = delete_order[i];
        
        // 在树中查找元素
        // 注意：由于我们有重复元素 (insert_equal)，我们需要小心。
        // 我们只需找到 一个 实例并删除它。
        auto it = tree.begin();
        while(it != tree.end() && *it != val_to_delete) {
            ++it; 
            // 理想情况下我们应该使用 find()，但当前接口暴露了迭代器操作。
            // 由于元素是排序的，如果我们超过了该值，可以提前退出。
             if (*it > val_to_delete) { it = tree.end(); break; }
        }

        if (it != tree.end() && *it == val_to_delete) {
            tree.erase(it);
        } else {
            std::cerr << "错误: 无法找到要删除的元素 " << val_to_delete << "。" << std::endl;
        }
    }

    std::cout << "通过: 已删除 " << delete_count << " 个元素。" << std::endl;
    std::cout << "删除后的树大小: " << tree.size() << std::endl;

    if (tree.size() != NUM_ELEMENTS - delete_count) {
         std::cerr << "错误: 删除后树的大小不正确。预期 " << (NUM_ELEMENTS - delete_count) << ", 实际 " << tree.size() << std::endl;
    }

    std::cout << "3. 清空剩余元素..." << std::endl;
    while (!tree.empty()) {
        tree.erase(tree.begin());
    }

    if (tree.size() == 0 && tree.empty()) {
        std::cout << "通过: 清空后树为空。" << std::endl;
    } else {
        std::cerr << "失败: 树不为空。大小: " << tree.size() << std::endl;
    }
}

void test_hint_insert() {
    std::cout << "Testing insert_unique with hint..." << std::endl;
    typedef msl::rb_tree<int, int, identity<int>, std::less<int>, msl::alloc> TreeType;
    TreeType tree;

    // 1. Insert into empty tree
    tree.insert_unique(tree.begin(), 10);
    assert(tree.size() == 1);
    assert(*tree.begin() == 10);

    // 2. Insert at beginning
    tree.insert_unique(tree.begin(), 5);
    assert(tree.size() == 2);
    assert(*tree.begin() == 5);

    // 3. Insert at end
    tree.insert_unique(tree.end(), 15);
    assert(tree.size() == 3);
    assert(*tree.rbegin() == 15);

    // 4. Insert in middle
    auto it = tree.find(15);
    tree.insert_unique(it, 12);
    assert(tree.size() == 4);

    // Verify order
    auto iter = tree.begin();
    assert(*iter++ == 5);
    assert(*iter++ == 10);
    assert(*iter++ == 12);
    assert(*iter++ == 15);

    std::cout << "insert_unique hint tests passed!" << std::endl;
}

int main() {
    print();
    test_hint_insert();
    test_random_operations();
    return 0;
}

#ifndef MYSTL_HEAP_H
#define MYSTL_HEAP_H

#include "iterator.h"

namespace msl {

/**
 * @brief 将新元素插入到堆中（上浮操作）
 * 
 * 该函数用于将位于 holeIndex 处的元素（通常是新添加的元素）向上调整到正确的位置，
 * 以维持堆的性质（默认为最大堆）。
 * 
 * @param first 堆的起始迭代器
 * @param holeIndex 新元素初始所在的索引（通常是堆的最后一个位置）
 * @param topIndex 堆顶的索引（通常是 0）
 * @param value 新元素的值
 */
template <typename RandomAccessIterator, typename Distance, typename T>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value) {
    Distance parent = (holeIndex - 1) / 2; // 计算父节点索引
    // 当尚未到达堆顶，且父节点的值小于新元素的值时，执行上浮
    while (holeIndex > topIndex && *(first + parent) < value) {
        *(first + holeIndex) = *(first + parent); // 将父节点下移
        holeIndex = parent;                       // 更新空洞位置为父节点位置
        parent = (holeIndex - 1) / 2;             // 继续计算新的父节点
    }
    *(first + holeIndex) = value; // 将新元素填入最终找到的位置
}

/**
 * @brief 将新元素插入到堆中（上浮操作）- 自定义比较器版本
 */
template <typename RandomAccessIterator, typename Distance, typename T, typename Compare>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value, Compare comp) {
    Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

// 辅助函数：提取距离类型和值类型，调用 __push_heap
template <typename RandomAccessIterator, typename Distance, typename T>
inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
    // 容器的最后一个元素（即新插入的元素）的位置是 (last - first) - 1
    msl::__push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
}

template <typename RandomAccessIterator, typename Distance, typename T, typename Compare>
inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare comp) {
    msl::__push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)), comp);
}

/**
 * @brief 将范围 [first, last) 中的最后一个元素插入到已有的堆 [first, last-1) 中
 * 
 * 前置条件：[first, last-1) 已经是一个有效的堆。
 * 操作后：[first, last) 成为一个有效的堆。
 */
template <typename RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
    __push_heap_aux(first, last, distance_type(first), value_type(first));
}

template <typename RandomAccessIterator, typename Compare>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    __push_heap_aux(first, last, distance_type(first), value_type(first), comp);
}

/**
 * @brief 调整堆（下沉操作）
 * 
 * 从 holeIndex 开始，将较大的子节点提升，填补空洞，直到到达叶子节点或合适的位置。
 * 最后调用 __push_heap 将原始的 value 值插入到调整后的位置。
 * 
 * @param first 堆的起始迭代器
 * @param holeIndex 需要调整的空洞位置
 * @param len 堆的长度
 * @param value 原始需要重新放置的值（通常是原堆尾元素）
 */
template <typename RandomAccessIterator, typename Distance, typename T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value) {
    Distance topIndex = holeIndex; // 记录初始空洞位置
    Distance secondChild = 2 * holeIndex + 2; // 右子节点索引
    
    while (secondChild < len) {
        // 比较左子节点和右子节点，取较大的那个（最大堆）
        if (*(first + secondChild) < *(first + (secondChild - 1)))
            secondChild--; // 如果右子节点小，则 secondChild 指向左子节点
        
        // 将较大的子节点上移填补空洞
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild; // 空洞下移
        secondChild = 2 * (secondChild + 1); // 继续计算下一层右子节点
    }
    
    // 如果没有右子节点，但有左子节点（堆的最后一个节点是左子节点）
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    
    // 此时 holeIndex 位于叶子节点层，或者接近叶子节点
    // 此时 holeIndex 是空位，但 value 可能比 holeIndex 的父节点大
    // 所以需要执行一次上浮操作 (__push_heap) 将 value 放到正确位置
    msl::__push_heap(first, holeIndex, topIndex, value);
}

/**
 * @brief 调整堆（下沉操作）
 * 
 * 从 holeIndex 开始，将较大的子节点提升，填补空洞，直到到达叶子节点或合适的位置。
 * 最后调用 __push_heap 将原始的 value 值插入到调整后的位置。
 * 
 * @param first 堆的起始迭代器
 * @param holeIndex 需要调整的空洞位置
 * @param len 堆的长度
 * @param value 原始需要重新放置的值（通常是原堆尾元素）
 * @param comp 比较函数对象，用于判断元素大小
 */

template <typename RandomAccessIterator, typename Distance, typename T, typename Compare>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value, Compare comp) {
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    msl::__push_heap(first, holeIndex, topIndex, value, comp);
}

/**
 * @brief 堆的弹出操作底层实现
 * 
 * 将堆顶元素（first）放到结果位置（result，通常是堆尾），
 * 然后将原堆尾的值（value）调整到堆顶并执行下沉操作。
 */
template <typename RandomAccessIterator, typename T, typename Distance>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*) {
    *result = *first; // 将堆顶元素（最大值）保存到尾部
    msl::__adjust_heap(first, Distance(0), Distance(last - first), value); // 调整堆，将原尾部值 value 放入堆中合适位置
}

template <typename RandomAccessIterator, typename T, typename Distance, typename Compare>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*, Compare comp) {
    *result = *first;
    msl::__adjust_heap(first, Distance(0), Distance(last - first), value, comp);
}

template <typename RandomAccessIterator, typename T>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*) {
    // 调用底层实现，将堆顶移到 last-1，并用 last-1 的值重新调整堆 [first, last-1)
    msl::__pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
}

template <typename RandomAccessIterator, typename T, typename Compare>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*, Compare comp) {
    msl::__pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first), comp);
}

/**
 * @brief 将堆顶元素移动到区间末尾，并调整剩余区间保持堆性质
 * 
 * 操作后：[first, last-1) 是一个有效的堆，*(last-1) 是原堆中的最大值。
 */
template <typename RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) { //容器范围
    __pop_heap_aux(first, last, value_type(first));
}

// first表示删除的位置, last表示最后一个元素的下一个位置
template <typename RandomAccessIterator, typename Compare>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    __pop_heap_aux(first, last, value_type(first), comp);
}

/**
 * @brief 堆排序
 * 
 * 不断执行 pop_heap 操作，将最大值依次交换到序列末尾，最终得到递增序列。
 */
template <typename RandomAccessIterator>
inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
    while (last - first > 1) {
        msl::pop_heap(first, last--); // 每次将最大值移到 last-1，然后缩小堆范围
    }
}

template <typename RandomAccessIterator, typename Compare>
inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    while (last - first > 1) {
        msl::pop_heap(first, last--, comp);
    }
}

/**
 * @brief 建堆操作底层实现
 * 
 * 从最后一个非叶子节点开始，依次向前调用 __adjust_heap 进行下沉调整。
 */
template <typename RandomAccessIterator, typename T, typename Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*) {
    if (last - first < 2) return; // 长度小于 2 无需建堆
    Distance len = last - first;
    // 计算最后一个非叶子节点的索引：(len - 2) / 2
    Distance parent = (len - 2) / 2;

    while (true) {
        // 对当前父节点进行下沉调整
        msl::__adjust_heap(first, parent, len, T(*(first + parent)));
        // 对每个非叶子节点都进行调整
        if (parent == 0) return; // 如果调整到根节点已经调整结束,则调整完成
        parent--; // 向前移动到下一个非叶子节点
    }
}

template <typename RandomAccessIterator, typename T, typename Distance, typename Compare>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*, Compare comp) {
    if (last - first < 2) return;
    Distance len = last - first;
    Distance parent = (len - 2) / 2;

    while (true) {
        msl::__adjust_heap(first, parent, len, T(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}

/**
 * @brief 将范围 [first, last) 重排为一个堆
 */
template <typename RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
    __make_heap(first, last, value_type(first), distance_type(first));
}

template <typename RandomAccessIterator, typename Compare>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    __make_heap(first, last, value_type(first), distance_type(first), comp);
}

} // namespace msl

#endif // MYSTL_HEAP_H

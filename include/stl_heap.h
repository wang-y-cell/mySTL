#ifndef MYSTL_HEAP_H
#define MYSTL_HEAP_H

#include "iterator.h"

namespace msl {

template <typename RandomAccessIterator, typename Distance, typename T>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value) {
    Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && *(first + parent) < value) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

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

template <typename RandomAccessIterator, typename Distance, typename T>
inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
    msl::__push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
}

template <typename RandomAccessIterator, typename Distance, typename T, typename Compare>
inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare comp) {
    msl::__push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)), comp);
}

template <typename RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
    __push_heap_aux(first, last, distance_type(first), value_type(first));
}

template <typename RandomAccessIterator, typename Compare>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    __push_heap_aux(first, last, distance_type(first), value_type(first), comp);
}

template <typename RandomAccessIterator, typename Distance, typename T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value) {
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (*(first + secondChild) < *(first + (secondChild - 1)))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    //如果只有一个左子节点
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    //此时holeIndex是空洞,需要将value插入到holeIndex
    //但是holeIndex的父节点可能大于value,所以需要继续调整,调用__push_heap
    //__push_heap会将value插入到正确的位置
    msl::__push_heap(first, holeIndex, topIndex, value);
}

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

template <typename RandomAccessIterator, typename T, typename Distance>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*) {
    *result = *first;
    msl::__adjust_heap(first, Distance(0), Distance(last - first), value);
}

template <typename RandomAccessIterator, typename T, typename Distance, typename Compare>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*, Compare comp) {
    *result = *first;
    msl::__adjust_heap(first, Distance(0), Distance(last - first), value, comp);
}

template <typename RandomAccessIterator, typename T>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*) {
    msl::__pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
}

template <typename RandomAccessIterator, typename T, typename Compare>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*, Compare comp) {
    msl::__pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first), comp);
}

template <typename RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
    __pop_heap_aux(first, last, value_type(first));
}

//first表示删除的位置,last表示最后一个元素的下一个位置
template <typename RandomAccessIterator, typename Compare>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    __pop_heap_aux(first, last, value_type(first), comp);
}

//堆排序
template <typename RandomAccessIterator>
inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
    while (last - first > 1) {
        msl::pop_heap(first, last--);
    }
}

template <typename RandomAccessIterator, typename Compare>
inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    while (last - first > 1) {
        msl::pop_heap(first, last--, comp);
    }
}

template <typename RandomAccessIterator, typename T, typename Distance>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*) {
    if (last - first < 2) return;
    Distance len = last - first;
    //从最后一个非叶子节点开始调整
    Distance parent = (len - 2) / 2;

    while (true) {
        msl::__adjust_heap(first, parent, len, T(*(first + parent)));
        //对每个非叶子节点都进行调整
        if (parent == 0) return;//如果调整到根节点已经调整结束,则调整完成
        parent--;
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

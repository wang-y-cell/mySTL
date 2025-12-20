#ifndef MYSTL_TREE_H
#define MYSTL_TREE_H

#include "iterator.h" 

namespace msl {

typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;  // 红色
const __rb_tree_color_type __rb_tree_black = true; // 黑色

struct __rb_tree_node_base {
    typedef __rb_tree_color_type color_type;
    typedef __rb_tree_node_base* base_ptr;

    color_type color; // 节点颜色
    base_ptr parent;  // 父节点指针
    base_ptr left;    // 左子节点指针
    base_ptr right;   // 右子节点指针

    // 查找子树中的最小节点
    static base_ptr minimum(base_ptr x) {
        while (x->left != 0) x = x->left;
        return x;
    }

    // 查找子树中的最大节点
    static base_ptr maximum(base_ptr x) {
        while (x->right != 0) x = x->right;
        return x;
    }
};

template <typename Value>
struct __rb_tree_node : public __rb_tree_node_base {
    typedef __rb_tree_node<Value>* link_type;
    Value value_field;
};

struct __rb_tree_base_iterator {
    typedef __rb_tree_node_base::base_ptr base_ptr;
    typedef bidirectional_iterator_tag iterator_category; 
    typedef ptrdiff_t difference_type;

    base_ptr node;

    // 前向迭代器的增量操作
    void increment() {
        if (node->right != 0) {
            node = node->right;
            while (node->left != 0)
                node = node->left;
        } else {
            base_ptr y = node->parent;
            while (node == y->right) {
                node = y;
                y = y->parent;
            }
            if (node->right != y)
                node = y;
        }
    }

    // 后向迭代器的增量操作
    void decrement() {
        if (node->color == __rb_tree_red && node->parent->parent == node) {
            node = node->right;
        } else if (node->left != 0) {
            base_ptr y = node->left;
            while (y->right != 0)
                y = y->right;
            node = y;
        } else {
            base_ptr y = node->parent;
            while (node == y->left) {
                node = y;
                y = y->parent;
            }
            node = y;
        }
    }
};

template <typename Value, typename Ref, typename Ptr>
struct __rb_tree_iterator : public __rb_tree_base_iterator {
    typedef Value value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
    typedef __rb_tree_iterator<Value, const Value&, const Value*> const_iterator;
    typedef __rb_tree_iterator<Value, Ref, Ptr> self;
    typedef __rb_tree_node<Value>* link_type;

    __rb_tree_iterator() {}
    __rb_tree_iterator(link_type x) { node = x; }
    __rb_tree_iterator(base_ptr x) { node = x; }
    __rb_tree_iterator(const iterator& it) { node = it.node; }

    reference operator*() const { return static_cast<link_type>(node)->value_field; }
    pointer operator->() const { return &(operator*()); }

    self& operator++() {
        increment();
        return *this;
    }
    self operator++(int) {
        self tmp = *this;
        increment();
        return tmp;
    }

    self& operator--() {
        decrement();
        return *this;
    }
    self operator--(int) {
        self tmp = *this;
        decrement();
        return tmp;
    }
    
    bool operator==(const __rb_tree_iterator& y) const { return node == y.node; }
    bool operator!=(const __rb_tree_iterator& y) const { return node != y.node; }
};

} // namespace msl

#endif // MYSTL_TREE_H

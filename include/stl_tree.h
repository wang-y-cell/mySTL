#ifndef MYSTL_TREE_H
#define MYSTL_TREE_H

#include "iterator.h" 
#include "stl_alloc.h"
#include "stl_construct.h"
#include "stl_pair.h"

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

inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
    __rb_tree_node_base* y = x->right;
    x->right = y->left;
    if (y->left != 0)
        y->left->parent = x;
    y->parent = x->parent;

    if (x == root)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
    __rb_tree_node_base* y = x->left;
    x->left = y->right;
    if (y->right != 0)
        y->right->parent = x;
    y->parent = x->parent;

    if (x == root)
        root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
    x->color = __rb_tree_red;
    while (x != root && x->parent->color == __rb_tree_red) {
        if (x->parent == x->parent->parent->left) {
            __rb_tree_node_base* y = x->parent->parent->right;
            if (y && y->color == __rb_tree_red) {
                x->parent->color = __rb_tree_black;
                y->color = __rb_tree_black;
                x->parent->parent->color = __rb_tree_red;
                x = x->parent->parent;
            } else {
                if (x == x->parent->right) {
                    x = x->parent;
                    __rb_tree_rotate_left(x, root);
                }
                x->parent->color = __rb_tree_black;
                x->parent->parent->color = __rb_tree_red;
                __rb_tree_rotate_right(x->parent->parent, root);
            }
        } else {
            __rb_tree_node_base* y = x->parent->parent->left;
            if (y && y->color == __rb_tree_red) {
                x->parent->color = __rb_tree_black;
                y->color = __rb_tree_black;
                x->parent->parent->color = __rb_tree_red;
                x = x->parent->parent;
            } else {
                if (x == x->parent->left) {
                    x = x->parent;
                    __rb_tree_rotate_right(x, root);
                }
                x->parent->color = __rb_tree_black;
                x->parent->parent->color = __rb_tree_red;
                __rb_tree_rotate_left(x->parent->parent, root);
            }
        }
    }
    root->color = __rb_tree_black;
}

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

template <typename T, typename Alloc>
class __rb_tree_base {
public:
    typedef Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }

    __rb_tree_base(const allocator_type& a)
    : header(0) {header = get_node();}
    ~__rb_tree_base() { put_node(header); }

protected:
    __rb_tree_node<T>* header;
    typedef simple_alloc<__rb_tree_node<T>, Alloc> node_allocator;

    __rb_tree_node<T>* get_node() { return node_allocator::allocate(); }
    void put_node(__rb_tree_node<T>* p) { node_allocator::deallocate(p); }
};





template<typename Key,typename Value, typename KeyOfValue,typename Compare,typename Alloc = alloc>
class rb_tree : public __rb_tree_base<Value, Alloc> {
    typedef __rb_tree_base<Value, Alloc> base;
protected:
    typedef __rb_tree_node_base* base_ptr;
    typedef __rb_tree_node<Value> rb_tree_node;
    typedef __rb_tree_color_type color_type;
public:
    typedef Key key_type;
    typedef Value value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef __rb_tree_node<Value>* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
protected:
    using base::header;
    using base::get_node;
    using base::put_node;

protected:
    link_type create_node(const value_type& x) {
        link_type tmp = get_node();
        MYSTL_TRY {
            construct(&tmp->value_field, x);
        } MYSTL_CATCH_ALL {
            put_node(tmp);
            throw;
        }
        return tmp;
    }

    link_type clone_node(link_type x) {
        link_type tmp = create_node(x->value_field);
        tmp->color = x->color;
        tmp->left = 0;
        tmp->right = 0;
        return tmp;
    }

    void destroy_node(link_type p) {
        destroy(&p->value_field);
        put_node(p);
    }

protected:
    link_type& root() const { return (link_type&)header->parent; }
    link_type& leftmost() const { return (link_type&)header->left; }
    link_type& rightmost() const { return (link_type&)header->right; }

    static link_type& left(link_type x) { return (link_type&)x->left; }
    static link_type& right(link_type x) { return (link_type&)x->right; }
    static link_type& parent(link_type x) { return (link_type&)x->parent; }
    static reference value(link_type x) { return x->value_field; }
    static const Key& key(link_type x) { return KeyOfValue()(value(x)); }
    static color_type& color(link_type x) { return (color_type&)x->color; }

    static link_type& left(base_ptr x) { return (link_type&)x->left; }
    static link_type& right(base_ptr x) { return (link_type&)x->right; }
    static link_type& parent(base_ptr x) { return (link_type&)x->parent; }
    static reference value(base_ptr x) { return ((link_type)x)->value_field; }
    static const Key& key(base_ptr x) { return KeyOfValue()(value(link_type(x))); }
    static color_type& color(base_ptr x) { return (color_type&)x->color; }

    static link_type minimum(link_type x) { return (link_type)__rb_tree_node_base::minimum(x); }
    static link_type maximum(link_type x) { return (link_type)__rb_tree_node_base::maximum(x); }

protected:
    size_type node_count; // 节点数量
    Compare key_compare; // 键值比较函数

public:
    typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
    typedef __rb_tree_iterator<Value, const Value&, const Value*> const_iterator;

private:
    void empty_initialize() {
        header = get_node(); // 请求一个节点作为header
        color(header) = __rb_tree_red; // header为红色，与root区分(root为黑色)
        
        root() = 0;
        leftmost() = header;
        rightmost() = header;
    }

public:
    rb_tree(const Compare& comp = Compare()) 
    : base(Alloc()), node_count(0), key_compare(comp) { 
        empty_initialize(); 
    }

    ~rb_tree() {
        // clear(); 
        put_node(header);
    }

    Compare key_comp() const { return key_compare; }
    iterator begin() { return leftmost(); }
    const_iterator begin() const { return leftmost(); }
    iterator end() { return header; }
    const_iterator end() const { return header; }
    bool empty() const { return node_count == 0; }
    size_type size() const { return node_count; }
    size_type max_size() const { return size_type(-1); }

    pair<iterator, bool> insert_unique(const value_type& v) {
        link_type y = header;
        link_type x = root();
        bool comp = true;
        while (x != 0) {
            y = x;
            comp = key_compare(KeyOfValue()(v), key(x));
            x = comp ? left(x) : right(x);
        }
        iterator j = iterator(y);
        if (comp) {
            if (j == begin())
                return pair<iterator, bool>(__insert(x, y, v), true);
            else
                --j;
        }
        if (key_compare(key(j.node), KeyOfValue()(v)))
            return pair<iterator, bool>(__insert(x, y, v), true);
        return pair<iterator, bool>(j, false);
    }

    iterator insert_equal(const value_type& v) {
        link_type y = header;
        link_type x = root();
        while (x != 0) {
            y = x;
            x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
        }
        return __insert(x, y, v);
    }

private:
    iterator __insert(base_ptr x_, base_ptr y_, const value_type& v) {
        link_type x = (link_type)x_;
        link_type y = (link_type)y_;
        link_type z;

        if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y))) {
            z = create_node(v);  // 创建新节点
            left(y) = z;         // 挂在父节点的左边
        if (y == header) {   // 情况 A: 树为空，这是第一个节点
            root() = z;      // header->parent 指向根节点
            rightmost() = z; // header->right 指向最大值（也就是目前唯一的节点）
        } else if (y == leftmost()) { // 情况 B: 父节点是当前的最小值
            leftmost() = z;  // 新节点比最小值还小，更新 header->left 指向新节点
        }
        } else {
            z = create_node(v);// 创建新节点
            right(y) = z;       // 挂在父节点的右边
            if (y == rightmost())
                rightmost() = z;
        }
        parent(z) = y;
        left(z) = 0;
        right(z) = 0;
        __rb_tree_rebalance(z, header->parent);
        ++node_count;
        return iterator(z);
    }







};









} // namespace msl

#endif // MYSTL_TREE_H

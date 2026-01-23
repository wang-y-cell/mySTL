#ifndef MYSTL_TREE_H
#define MYSTL_TREE_H

#include "stl_iterator.h" 
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

//删除
inline __rb_tree_node_base*
__rb_tree_rebalance_for_erase(__rb_tree_node_base* __z,
                             __rb_tree_node_base*& __root,
                             __rb_tree_node_base*& __leftmost,
                             __rb_tree_node_base*& __rightmost)
{
  __rb_tree_node_base* __y = __z;
  __rb_tree_node_base* __x = 0;
  __rb_tree_node_base* __x_parent = 0;
  if (__y->left == 0)     
    __x = __y->right;   
  else
    if (__y->right == 0) 
      __x = __y->left;    
    else {                   
      __y = __y->right;   
      while (__y->left != 0)
        __y = __y->left;
      __x = __y->right;
    }
    // 如果只有1个或者0个子节点,__y不变还是等于__z,否则y等于z的后继节点(右子树的最左节点)
    //如果z有两个节点,那y指向z的后继节点(右子树的最左节点),这个节点是实际要删除的节点
    //如果1个子节点,x指向右子节点或左子节点,如果没有子节点,x指向0
    //如果两个子节点都有,x指向右子树的最左节点

  if (__y != __z) {          
    __z->left->parent = __y; 
    __y->left = __z->left;
    if (__y != __z->right) {
      __x_parent = __y->parent;
      if (__x) __x->parent = __y->parent;
      __y->parent->left = __x;      
      __y->right = __z->right;
      __z->right->parent = __y;
      //y父亲的左孩子指向x
    }
    else
      __x_parent = __y;  
    if (__root == __z)
      __root = __y;
    else if (__z->parent->left == __z)
      __z->parent->left = __y;
    else 
      __z->parent->right = __y;
    __y->parent = __z->parent;
    __rb_tree_color_type __tmp_color = __y->color;
    __y->color = __z->color;
    __z->color = __tmp_color;
    __y = __z;
    // __y 指向z,要删除z
  }
  else {                        
    __x_parent = __y->parent;
    if (__x) __x->parent = __y->parent;   
    if (__root == __z)
      __root = __x;
    else 
      if (__z->parent->left == __z)
        __z->parent->left = __x;
      else
        __z->parent->right = __x;
    if (__leftmost == __z) 
      if (__z->right == 0)       
        __leftmost = __z->parent;
    
      else
        __leftmost = __rb_tree_node_base::minimum(__x);
    if (__rightmost == __z)  
      if (__z->left == 0)        
        __rightmost = __z->parent;  
    
      else                     
        __rightmost = __rb_tree_node_base::maximum(__x);
  }


  if (__y->color != __rb_tree_red) { 
    while (__x != __root && (__x == 0 || __x->color == __rb_tree_black))
      if (__x == __x_parent->left) {
        __rb_tree_node_base* __w = __x_parent->right;
        if (__w->color == __rb_tree_red) {
          __w->color = __rb_tree_black;
          __x_parent->color = __rb_tree_red;
          __rb_tree_rotate_left(__x_parent, __root);
          __w = __x_parent->right;
        }
        if ((__w->left == 0 || 
             __w->left->color == __rb_tree_black) &&
            (__w->right == 0 || 
             __w->right->color == __rb_tree_black)) {
          __w->color = __rb_tree_red;
          __x = __x_parent;
          __x_parent = __x_parent->parent;
        } else {
          if (__w->right == 0 || 
              __w->right->color == __rb_tree_black) {
            if (__w->left) __w->left->color = __rb_tree_black;
            __w->color = __rb_tree_red;
            __rb_tree_rotate_right(__w, __root);
            __w = __x_parent->right;
          }
          __w->color = __x_parent->color;
          __x_parent->color = __rb_tree_black;
          if (__w->right) __w->right->color = __rb_tree_black;
          __rb_tree_rotate_left(__x_parent, __root);
          break;
        }
      } else {                  
        __rb_tree_node_base* __w = __x_parent->left;
        if (__w->color == __rb_tree_red) {
          __w->color = __rb_tree_black;
          __x_parent->color = __rb_tree_red;
          __rb_tree_rotate_right(__x_parent, __root);
          __w = __x_parent->left;
        }
        if ((__w->right == 0 || 
             __w->right->color == __rb_tree_black) &&
            (__w->left == 0 || 
             __w->left->color == __rb_tree_black)) {
          __w->color = __rb_tree_red;
          __x = __x_parent;
          __x_parent = __x_parent->parent;
        } else {
          if (__w->left == 0 || 
              __w->left->color == __rb_tree_black) {
            if (__w->right) __w->right->color = __rb_tree_black;
            __w->color = __rb_tree_red;
            __rb_tree_rotate_left(__w, __root);
            __w = __x_parent->left;
          }
          __w->color = __x_parent->color;
          __x_parent->color = __rb_tree_black;
          if (__w->left) __w->left->color = __rb_tree_black;
          __rb_tree_rotate_right(__x_parent, __root);
          break;
        }
      }
    if (__x) __x->color = __rb_tree_black;
  }
  return __y;
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
    typedef msl::reverse_iterator<iterator> reverse_iterator;
    typedef msl::reverse_iterator<const_iterator> const_reverse_iterator;

private:
    void empty_initialize() {
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
        clear();
    }

    rb_tree(const rb_tree& x) : base(Alloc()), node_count(0), key_compare(x.key_compare) {
        empty_initialize();
        if (x.root() != 0) {
            root() = __copy(x.root(), header);
            leftmost() = minimum(root());
            rightmost() = maximum(root());
            node_count = x.node_count;
        }
    }

    rb_tree& operator=(const rb_tree& x) {
        if (this != &x) {
            clear();
            node_count = 0;
            key_compare = x.key_compare;
            if (x.root() != 0) {
                root() = __copy(x.root(), header);
                leftmost() = minimum(root());
                rightmost() = maximum(root());
                node_count = x.node_count;
            }
        }
        return *this;
    }

    Compare key_comp() const { return key_compare; }
    iterator begin() { return leftmost(); }
    const_iterator begin() const { return leftmost(); }
    iterator end() { return header; }
    const_iterator end() const { return header; }

    #if MYSTL_CPP_VERSION >= 11
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }
    #endif

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
    #if MYSTL_CPP_VERSION >= 11
    const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }
    #endif

    bool empty() const { return node_count == 0; }
    size_type size() const { return node_count; }
    size_type max_size() const { return size_type(-1); }
public:
    void erase(iterator position);
    size_type erase(const Key& x);
    void erase(iterator first, iterator last);

    iterator find(const Key& k);
    const_iterator find(const Key& k) const;
    size_type count(const Key& k) const;
    iterator lower_bound(const Key& k);
    const_iterator lower_bound(const Key& k) const;
    iterator upper_bound(const Key& k);
    const_iterator upper_bound(const Key& k) const;
    pair<iterator, iterator> equal_range(const Key& k);
    pair<const_iterator, const_iterator> equal_range(const Key& k) const;

    void clear(){
        __erase(root());
        root() = 0;
        leftmost() = header;
        rightmost() = header;
        node_count = 0;
    }



    void swap(rb_tree& t) {
        link_type tmp_header = header;
        header = t.header;
        t.header = tmp_header;
        
        size_type tmp_count = node_count;
        node_count = t.node_count;
        t.node_count = tmp_count;
        
        Compare tmp_comp = key_compare;
        key_compare = t.key_compare;
        t.key_compare = tmp_comp;
    }

    template <class InputIterator>
    void insert_unique(InputIterator first, InputIterator last) {
        for (; first != last; ++first)
            insert_unique(*first);
    }
    
    /**
     * @brief 插入不相等元素
     * 
     * @param position 插入位置的迭代器
     * @param v 要插入的值
     * @return iterator 指向插入位置的迭代器
     */
    iterator insert_unique(iterator position, const value_type& v) {
        if (position.node == header->left) { // begin()
            if (size() > 0 && key_compare(KeyOfValue()(v), key(position.node)))
                return __insert(position.node, position.node, v);
            else
                return insert_unique(v).first;
        } else if (size() > 0 && position.node == header) { // end()
            if (key_compare(key(rightmost()), KeyOfValue()(v)))
                return __insert(0, rightmost(), v);
            else
                return insert_unique(v).first;
        } else {
            iterator before = position;
            --before;
            if (key_compare(key(before.node), KeyOfValue()(v)) &&
                key_compare(KeyOfValue()(v), key(position.node))) {
                if (right(before.node) == 0)// 如果前驱节点没有右孩子，就挂在它的右边
                    return __insert(0, before.node, v);
                else
                    return __insert(position.node, position.node, v);
            } else {
                return insert_unique(v).first;
            }
        }
    }

    /**
     * @brief 插入不相等元素
     * 
     * @param v 要插入的值
     * @return pair<iterator, bool> 指向插入位置的迭代器和是否成功插入的标志
     */
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
                --j;//回到前驱节点
        }
        if (key_compare(key(j.node), KeyOfValue()(v)))
            return pair<iterator, bool>(__insert(x, y, v), true);
        return pair<iterator, bool>(j, false);
        //例子：树中有 [10, 20] ，我们要插入 10 。
        //走到 20 时，发现 10 < 20 ，准备插在 20 的左边。
        //此时 y = 20 。我们知道 10 != 20 。
        //但我们需要检查 20 的前一个数（也就是 10 ）是不是和新来的 10 相等。
    }

    /**
     * @brief 可以插入相等元素
     * 
     * @param v 要插入的值
     * @return iterator 指向插入位置的迭代器
     */
    iterator insert_equal(const value_type& v) {
        link_type y = header;
        link_type x = root();
        while (x != 0) {
            y = x;
            x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
        }
        return __insert(x, y, v);
    }

    /**
     * @brief 可以插入相等元素
     * 
     * @param first 输入迭代器的起始位置
     * @param last 输入迭代器的结束位置
     */
    template<typename InputIterator>
    void insert_equal(InputIterator first, InputIterator last) {
        for (; first != last; ++first)
            insert_equal(*first);
    }

    iterator insert_equal(iterator position, const value_type& v) {
        if(position.node == header->left) { // begin()
            if(size() > 0 && key_compare(KeyOfValue()(v), key(position.node)))
                return __insert(position.node, position.node, v);
            else
                return insert_equal(v);
        } else if (size() > 0 && position.node == header) { // end()
            if (!key_compare(KeyOfValue()(v), key(rightmost())))
                return __insert(0, rightmost(), v);
            else
                return insert_equal(v);
        } else {
            iterator before = position;
            --before;
            if (!key_compare(KeyOfValue()(v),key(before.node)) &&
                !key_compare(key(position.node),KeyOfValue()(v))) {
                if (right(before.node) == 0)
                    return __insert(0, before.node, v);
                else
                    return __insert(position.node, position.node, v);
            } else {
                return insert_equal(v);
            }
        }
    }


private:
    link_type __copy(link_type x, link_type p) {
        link_type top = clone_node(x);
        top->parent = p;
        
        MYSTL_TRY {
            if (x->right)
                top->right = __copy(right(x), top);
            p = top;
            x = left(x);
            while (x != 0) {
                link_type y = clone_node(x);
                p->left = y;
                y->parent = p;
                if (x->right)
                    y->right = __copy(right(x), y);
                p = y;
                x = left(x);
            }
        } MYSTL_CATCH_ALL {
            __erase(top);
            throw;
        }
        return top;
    }


    /**
     * @brief 递归删除所有子节点
     * 
     * @param __x 要删除的节点
     */
    void __erase(link_type __x);  //递归删除所有子节点

    /**
     * @brief 递归插入节点
     * 
     * @param x_ 要插入的位置的节点
     * @param y_ 要插入的位置的父节点
     * @param v 要插入的值
     * @return iterator 指向插入位置的迭代器
     */
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

//删除节点iterator position
template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc >
inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator position) {
    link_type __y = 
    (link_type) __rb_tree_rebalance_for_erase(position.node,
                                              header->parent,
                                              header->left,
                                              header->right
                                            );
    destroy_node(__y);
    --node_count;
}

template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc >
void rb_tree<Key, Value,KeyOfValue,Compare,Alloc>
::__erase(link_type __x){
    while(__x != 0){
        __erase(right(__x));
        link_type __y = left(__x);
        destroy_node(__x);
        __x = __y;
    }
}

template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) {
    link_type y = header; 
    link_type x = root(); 

    while (x != 0) {
        if (!key_compare(key(x), k)) {
            y = x, x = left(x); 
        } else {
            x = right(x); 
        }
    }
    iterator j = iterator(y);
    return (j == end() || key_compare(k, key(j.node))) ? end() : j;
}

template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const Key& k) const {
    link_type y = header; 
    link_type x = root(); 

    while (x != 0) {
        if (!key_compare(key(x), k)) {
            y = x, x = left(x); 
        } else {
            x = right(x); 
        }
    }
    const_iterator j = const_iterator(y);
    return (j == end() || key_compare(k, key(j.node))) ? end() : j;
}

template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const Key& k) const {
    pair<const_iterator, const_iterator> p = equal_range(k);
    size_type n = 0;
    distance(p.first, p.second, n);
    return n;
}

template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const Key& k) {
    link_type y = header; 
    link_type x = root(); 

    while (x != 0) {
        if (!key_compare(key(x), k)) {
            y = x, x = left(x); 
        } else {
            x = right(x); 
        }
    }
    return iterator(y);
}

template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const Key& k) const {
    link_type y = header; 
    link_type x = root(); 

    while (x != 0) {
        if (!key_compare(key(x), k)) {
            y = x, x = left(x); 
        } else {
            x = right(x); 
        }
    }
    return const_iterator(y);
}

template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const Key& k) {
    link_type y = header; 
    link_type x = root(); 

    while (x != 0) {
        if (key_compare(k, key(x))) {
            y = x, x = left(x); 
        } else {
            x = right(x); 
        }
    }
    return iterator(y);
}

template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const Key& k) const {
    link_type y = header; 
    link_type x = root(); 

    while (x != 0) {
        if (key_compare(k, key(x))) {
            y = x, x = left(x); 
        } else {
            x = right(x); 
        }
    }
    return const_iterator(y);
}

template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator> 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const Key& k) {
    return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
}

template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator,
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator> 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const Key& k) const {
    return pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
}

template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const Key& x) {
    pair<iterator, iterator> p = equal_range(x);
    size_type n = 0;
    distance(p.first, p.second, n);
    erase(p.first, p.second);
    return n;
}

template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first, iterator last) {
    if (first == begin() && last == end())
        clear();
    else
        while (first != last) erase(first++);
}

} // namespace msl

#endif // MYSTL_TREE_H

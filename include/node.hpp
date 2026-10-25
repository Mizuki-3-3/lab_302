#pragma once
#include "shared_ptr.hpp"

template <typename comparator, typename T>
concept compare = std::strict_weak_order<comparator, T, T>;

template<typename T, typename K, compare<K> compare_ = std::less<K>>
class rb_tree;

template <typename T, typename K>
class node{//короче идея: будем делать дерево прошитым в обе стороны, чтобы можно было вызывать обход ps идея плохая
    shared_ptr<node> left;
    shared_ptr<node> right;
    weak_ptr<node> parent;//дали добро на оставить родителя (ура, мы не сироты)
    T value;
    K key;
    bool RED;
    template <typename U, typename N, typename C> friend class rb_tree;
public:
    node(const T& v, const K& k, bool red=true): key(k),value(v), RED(red){}
    node(const node& other) = delete;
    node operator=(const node& other) = delete;
    shared_ptr<node>& copy() const{
        shared_ptr<node> new_node = shared_ptr<node>::make_shared(value, key, RED);
        return new_node;
    }
    bool operator==(node& other) const{
        if (RED!=other.RED||key!=other.key||value!=other.value){return false;}
        return true;
    }
};
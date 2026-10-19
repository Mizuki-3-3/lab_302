#pragma once

#include "shared_ptr.hpp"
#include "weak_ptr.hpp"
#include <concepts>
#include <cstddef>
#include <iostream>
#include <functional>
#include <stack>

enum order{//обходы, попробуем прошить хоть как-то
    LRV,
    LVR,
    VLR,
    VRL,
    RVL,
    RLV
};
template <typename comparator, typename T>
concept compare = std::strict_weak_order<comparator, T, T>;

template<typename T, typename K, order Order = LVR, compare<K> compare_ = std::less<K>>//если true то влево, а если false то вправо
//у каждого красного дети только чёрные, 
// чёрная высота одинакова на всех путях, корень чёрни
//
class rb_tree{
private:
    struct node{
        shared_ptr<node> left;
        shared_ptr<node> right;
        weak_ptr<node> parent;
        T value;
        K key;
        bool RED = true;
        node(const T& v, const K& k): key(k),value(v){}
        node(const node& other): key(other.key), value(other.value), RED(other.RED){
            if (other.left){left = shared_ptr<node>::make_shared(*other.left);}
            if (other.right){right = shared_ptr<node>::make_shared(*other.right);}
            if (other.parent.lock()){parent = weak_ptr<node>(other.parent.lock());}
        }
    };
    shared_ptr<node> root;
    compare_ comp;
    size_t size;
    bool red(shared_ptr<node> n) const { return n&&n->RED; }
    bool black(shared_ptr<node> n) const { return n&&!n->RED||!n; }
    bool equal(const T& a, const T& b) const{
        return !comp(a, b) && !comp(b, a);
    }
    //тут передаем узел который спустится ниже, а его сын встанет на его место
    void rotate_left(shared_ptr<node> x){
        shared_ptr<node> y = x->right;
        auto parent_x = x->parent.lock();
        x->right = y->left;
        if (y->left){y->left->parent = x;}
        y->parent = x->parent;
        if (!parent_x){root = y;}
        else if (x == parent_x->left){parent_x->left = y;}
        else {parent_x->right = y;}
        y->left = x;
        x->parent = y;
    }
    void rotate_right(shared_ptr<node> x){
        shared_ptr<node> y = x->left;
        auto parent_x = x->parent.lock();
        x->left = y->right;
        if (y->right){y->right->parent = x;}
        y->parent = x->parent;
        if (!parent_x){root = y;}
        else if (x == parent_x->left){parent_x->left = y;}
        else {parent_x->right = y;}
        y->right = x;
        x->parent = y;
    }
    
    void balance_after_remove(shared_ptr<node> parent, shared_ptr<node> sibling){
        while(parent){
        if (!sibling)return;
        bool sib_is_left = (sibling == parent->left);
        auto far = sib_is_left ? sibling->left : sibling->right;
        auto near = sib_is_left ? sibling->right : sibling->left;
        if (sibling->RED){
            sibling->RED = false;
            parent->RED = true;
            sib_is_left?rotate_right(parent): rotate_left(parent);
            sibling = near;
            continue;
        }
        if (red(far)){
            sibling->RED = parent->RED;
            parent->RED = false;
            far->RED = false;
            sib_is_left? rotate_left(parent): rotate_right(parent);
            return;
        }
        if (red(near)){
            near->RED = false;
            sibling->RED = true;
            sib_is_left? rotate_right(parent): rotate_left(parent);
            sibling = near;
            continue;
        }
        sibling->RED = true;
        if (parent->RED){
            parent->RED = false;
            return;
        }else{
            auto grandparent = parent->parent.lock();
            if (grandparent){
                sibling = (parent == grandparent->left) ? grandparent->right : grandparent->left;
                parent = grandparent;
                continue;
            }else{return;}
        }
    }
    }

//аааааа... вот эта вставка не родная нифига
    void balance_insert(shared_ptr<node> noda){
        shared_ptr<node> parent_ = noda->parent.lock();
        if (parent_ && parent_->RED) {
            shared_ptr<node> grandparent = parent_->parent.lock();
            shared_ptr<node> uncle = grandparent->left.get() == parent_.get()? grandparent->right: grandparent->left;
            if (uncle && uncle->RED) {
                parent_->RED = false;
                uncle->RED = false;
                grandparent->RED = true;
                balance_insert(grandparent);
            }else{
                if (parent_.get() == grandparent->left.get()){
                    if (noda.get() == parent_->right.get()){
                        rotate_left(parent_);//теперь у нас нода выше чем ее бывший родитель
                        parent_.swap(noda);
                    }
                    rotate_right(grandparent);
                    parent_->RED = false; grandparent->RED = true;
                }else{
                    if (noda.get() == parent_->left.get()){
                        rotate_right(parent_);//теперь у нас нода выше чем ее бывший родитель
                        parent_.swap(noda);
                    }
                    rotate_left(grandparent);// а чем плох вариант noda = black, grandparent=black? чтобы листики черные а общая вершинка красная
                    parent_->RED = false; grandparent->RED = true;
                }
            }

        }
        root->RED = false;
    }
    
    shared_ptr<node> search_(const K& KEY) const{//если найден возвращает указатель на узел, иначе nullptr
        auto current = root;
        while (current){
            if (equal(KEY, current->key)) {return current;}
            else if (comp(KEY, current->key)){current=current->left;}
            else {current=current->right;}
        }
        return shared_ptr<node>();
    }
    rb_tree(const node& other){
        root = shared_ptr<node>::make_shared(other);
        comp = other.comp;
        size = 0;//TODO подумать как искать размер
    }
public:
    rb_tree(): root(nullptr), size(0){}
    rb_tree(const rb_tree& other){
        root = shared_ptr<node>::make_shared(*(other.root));
        comp = other.comp;
        size = other.size;
    }
//вставка родная
    shared_ptr<node> insert(const K& KEY, const T& value){//при вставке возвращает указатель на то куда вставили 
        auto new_node = shared_ptr<node>::make_shared(value, KEY);
        shared_ptr<node> current = root;
        shared_ptr<node> parent;
        while (current){
            parent = current;
            if (equal(KEY, current->key)){break;}
            else if(comp(KEY, current->key)){
                current = current->left;
            } else {
                current = current->right;
            }
        }//теперь у нас либо лист либо узел в середине с уже имеющимися значениями
        //если лист
        if (!current){
            new_node->parent = parent;
            if (!parent){root=new_node;}
            else if (comp(KEY, parent->key)){parent->left = new_node;}
            else{parent->right = new_node;}
            balance_insert(new_node);
            ++size;
        }else{
            current->value=value;
        }
        return new_node;
    }

//удаление ппц
    void remove(const K& KEY){
        shared_ptr<node> node_to_del = search_(KEY);
        if (!node_to_del) {return;}
        //подтягиваем самого правого из левого поддерева
        if (node_to_del->right){
            shared_ptr<node> pred = node_to_del->right;
            while (pred->left){
                pred = pred->left;
            }
            node_to_del->value = pred->value;
            node_to_del->key=pred->key;
            node_to_del = pred;//всегда лист удаляем, поэтому балансируем по минимуму
        }else if (node_to_del->left){
            shared_ptr<node> succ = node_to_del->left;
            while (succ->right){
                succ = succ->right;
            }
            node_to_del->value = succ->value;
            node_to_del->key=succ->key;
            node_to_del = succ;//всегда лист удаляем, поэтому балансируем по минимуму
        }
        shared_ptr<node> p = node_to_del->parent.lock();
        shared_ptr<node> sibling;

        if (!p){
            root.reset();
            return;
        }else if (p->left == node_to_del){
            p->left.reset();
            sibling = p->right;
        }else{
            p->right.reset();
            sibling = p->left;
        }
        if (!node_to_del->RED){balance_after_remove(p, sibling);}
        root->RED = false;
        size--;
        return ;
    }
//займемся прошивкой

//обходы
//

    void order(){
        std::stack<shared_ptr<node>> s;
        s.push(root);
        while(!s.empty()){
            auto noda = s.top();
            s.pop();
            std::cout<<noda->value<<" ";
            //ну что-то сделать тут надо
            if (noda->right){s.push(noda->right);}
            if (noda->left){s.push(noda->left);}
        }
    }
    rb_tree threading(){

    }
bool check_all_red_h_b(shared_ptr<node> curr, bool flag){//lvr
    if (curr->RED&&(curr->left&&curr->left->RED || curr->right&&curr->right->RED)){flag = false;}
    if(curr->left) {flag = check_all_red_h_b(curr->left, flag);}
    if(curr->right) {flag = check_all_red_h_b(curr->right, flag);}
    return flag;
}
bool ch(){
    return check_all_red_h_b(root, true);
}
bool ch2(){
    return check_black_height(root) != -1;
}

int check_black_height(shared_ptr<node> curr){
    if (!curr){return 1;}
    int l = check_black_height(curr->left);
    int r = check_black_height(curr->right);
    if (l==-1||r==-1||l!=r){return -1;}

    return l + (curr->RED ? 0 : 1);
}

T search(const K& KEY){
    auto finded = search_(KEY);
    if (finded){return search_(KEY)->value;}
    else{return T();}
}

rb_tree sub_tree(const K& KEY){
    auto finded = search_(KEY);
    if (finded){return rb_tree(*finded);}
    else throw not_found("sub_tree: key not found");
}
void print_t(shared_ptr<node> curr){
    if (curr){
        if (curr->left){print_t(curr->left);}
        if (curr->right){print_t(curr->right);}
        std::cout<<curr->value<<" ";
        curr->RED? std::cout<<"R ":std::cout<<"B ";
    }
    return;
}

void print_tree(){
    print_t(root);
    std::cout<<"\n";
}
};
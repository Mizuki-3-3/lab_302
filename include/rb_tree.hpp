#pragma once

#include "shared_ptr.hpp"
#include <cstddef>
#include <iostream>
#include "errors.hpp"
#include "node.hpp"
#include "traversers.hpp"

template<typename T, typename K, compare<K> compare_>//если true то влево, а если false то вправо
class rb_tree{//пусть у дефолтного красночерного будет только вставка удаление поиск, тогда обходы и итераторы реализуем в надстройке
private:
    shared_ptr<node<T, K>> root;
    compare_ comp;
    size_t size;
//да начнутся итераторы
    class iterator{
        shared_ptr<node<T, K>> current;
        traverser<T, K> traverser_;
    public:
    explicit iterator(shared_ptr<node<T, K>> ptr, traverser<T, K> trav = lvr_traverser<T,K>()) noexcept: current(ptr), traverser_(trav){}//инордер по умолч
    ~iterator() = default;
    T& operator*(){return current->value;}
    T* operator->(){return &(current->value);}
    bool operator==(const iterator& other) const {
      return current == other.current;
    }
    bool operator!=(const iterator& other) const {
      return !(*this == other);
    }
    iterator& operator++(){
        current = traverser_.next(current);
        return *this;
    }
    iterator operator++(int){
        iterator tmp = *this;
        current = traverser_.next(current);
        return tmp;
    }
    iterator operator--(){
        current = traverser_.prev(current);
        return *this;
    }
    iterator operator--(int){
        iterator tmp = *this;
        current = traverser_.prev(current);
        return tmp;
    }
    shared_ptr<node<T, K>> get_last_(shared_ptr<node<T, K>> root_){return traverser_.get_last(root_);}//сюда можно только корень передавать, иначе атата
    };


    
    
    bool red(shared_ptr<node<T, K>> n) const { return n&&n->RED; }
    bool equal(const T& a, const T& b) const{
        return !comp(a, b) && !comp(b, a);
    }

    //тут передаем узел который спустится ниже, а его сын встанет на его место
    void rotate_left(shared_ptr<node<T, K>> x){
        shared_ptr<node<T, K>> y = x->right;
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
    void rotate_right(shared_ptr<node<T, K>> x){
        shared_ptr<node<T, K>> y = x->left;
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
    
    void balance_after_remove(shared_ptr<node<T, K>> parent, shared_ptr<node<T, K>> sibling){
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
                sib_is_left? rotate_right(parent): rotate_left(parent);
                return;
            }
            if (red(near)){
                near->RED = false;
                sibling->RED = true;
                sib_is_left? rotate_left(parent): rotate_right(parent);
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
    void balance_insert(shared_ptr<node<T, K>> noda){
        shared_ptr<node<T, K>> parent_ = noda->parent.lock();
        if (parent_ && parent_->RED) {
            shared_ptr<node<T, K>> grandparent = parent_->parent.lock();
            shared_ptr<node<T, K>> uncle = grandparent->left.get() == parent_.get()? grandparent->right: grandparent->left;
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
    
    shared_ptr<node<T, K>> search_(const K& KEY) const{//если найден возвращает указатель на узел, иначе nullptr
        if (!root){throw empty_container("this tree is empty");}
        auto current = root;
        while (current){
            if (equal(KEY, current->key)) {return current;}
            else if (comp(KEY, current->key)){current=current->left;}
            else {current=current->right;}
        }
        return shared_ptr<node<T, K>>();
    }
    rb_tree(const node<T, K>& new_root){
        root = shared_ptr<node<T, K>>::make_shared(new_root);
        root.parent.reset();
        root->RED = false;
        size = 0;//TODO подумать как искать размер
    }//оставлять в этом дереве или нет оч хороший вопрос на подумать

public:
    rb_tree(): root(), size(0){}
    ~rb_tree()=default;
    rb_tree(const rb_tree& other){
        root = shared_ptr<node<T, K>>::make_shared(*(other.root));
        comp = other.comp;
        size = other.size;

    }
    //////////////////////////////////сначала итераторы чтоль бахнуть или стеком или обходом?
    bool operator==(rb_tree& other){
        if (size!=other.size) return false;
        for (auto& x: *this){

        }

    }

    shared_ptr<node<T, K>> insert(const K& KEY, const T& value){//при вставке возвращает указатель на то куда вставили 
        auto new_node = shared_ptr<node<T, K>>::make_shared(value, KEY);
        shared_ptr<node<T, K>> current = root;
        shared_ptr<node<T, K>> parent;
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
        shared_ptr<node<T, K>> node_to_del = search_(KEY);
        if (!node_to_del) {throw not_found("there isn't such key");}
        //подтягиваем самого правого из левого поддерева
        if (node_to_del->right){
            shared_ptr<node<T, K>> pred = node_to_del->right;
            while (pred->left){
                pred = pred->left;
            }
            node_to_del->value = pred->value;
            node_to_del->key=pred->key;
            node_to_del = pred;//всегда лист удаляем, поэтому балансируем по минимуму
        }else if (node_to_del->left){
            shared_ptr<node<T, K>> succ = node_to_del->left;
            while (succ->right){
                succ = succ->right;
            }
            node_to_del->value = succ->value;
            node_to_del->key=succ->key;
            node_to_del = succ;//всегда лист удаляем, поэтому балансируем по минимуму
        }
        shared_ptr<node<T, K>> p = node_to_del->parent.lock();
shared_ptr<node<T, K>> child = node_to_del->left ? node_to_del->left : node_to_del->right;

        if (!p){
            root=child;
            if (root){ root->parent.reset(); root->RED = false; }
            size--;
            return;
        }
        shared_ptr<node<T, K>> sibling= (p->left == node_to_del) ? p->right : p->left;
        if (child) child->parent = p;
        if (p->left == node_to_del) p->left = child;
        else p->right = child;
        if (!node_to_del->RED){
            if (red(child)) child->RED = false;      // красный ребёнок «возмещает» чёрную высоту
            else balance_after_remove(p, sibling);
        }
        if (root) root->RED = false;
        size--;
        return ;
    }

bool check_all_red_h_b(shared_ptr<node<T, K>> curr, bool flag){//lvr
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

int check_black_height(shared_ptr<node<T, K>> curr){
    if (!curr){return 1;}
    int l = check_black_height(curr->left);
    int r = check_black_height(curr->right);
    if (l==-1||r==-1||l!=r){return -1;}

    return l + (curr->RED ? 0 : 1);
}

T search(const K& KEY){
    auto finded = search_(KEY);
    if (finded){return search_(KEY)->value;}
    else{throw not_found("no such value in tree");}
}
//////////////////////////////////////////
rb_tree sub_tree(const K& KEY){
    auto finded = search_(KEY);
    if (finded){return rb_tree(*finded);}
    else throw not_found("sub_tree: key not found");
}//подумать убирать ли в надстройку, если убираем, то и извлечение поддерева туда же кидаем
void print_t(shared_ptr<node<T, K>> curr){
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

//ээээ крч надо извлечение поддерева свести к копирующему коструктору по заданному корню
//поиск на вхождение поддерева сводим к извлечению поддерева (sub_tree) и сравения поддерева с исходным заданным (operator==)
//сохранение в строку сделаем так: сброс прошивки, новая прошивка, затев обход дерева с функцией пуш в строку, возвращаем строку
bool contains_sub_tree(const rb_tree& other) const{//TODO руализовать ==
    rb_tree maybe = sub_tree(other.root->key);
    return maybe==other;
}
};
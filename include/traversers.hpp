#pragma once
#include "node.hpp"
#include "shared_ptr.hpp"

template<typename T, typename K>
struct traverser{
    virtual shared_ptr<node<T, K>> next(shared_ptr<node<T, K>> noda) = 0;
    virtual shared_ptr<node<T, K>> prev(shared_ptr<node<T, K>> noda) = 0;
    virtual shared_ptr<node<T, K>> get_last(shared_ptr<node<T, K>> root);
    virtual shared_ptr<node<T, K>> get_first(shared_ptr<node<T, K>> root);
};

//придумаю как избавиться от дублирования попозже, ща вообще не до этого

template<typename T, typename K>
struct lrv_traverser: public traverser<T, K>{//postorder
    shared_ptr<node<T, K>> next(shared_ptr<node<T, K>> noda) override final {
    if (!noda || !noda->parent) return shared_ptr<node<T, K>>();

    if (noda->parent->right == noda || !noda->parent->right) {
      return noda->parent;
    }
    noda = noda->parent_->right;
    while (noda->left) {
      noda = noda->left;
    }
    return noda;
  }
  shared_ptr<node<T, K>> prev(shared_ptr<node<T, K>> noda) override final {
    if (!noda) return shared_ptr<node<T, K>>();
    if (noda->right) return noda->right;
    if (noda->left) return noda->left;
    if (noda->parent) {
      if (noda->parent->right == noda && noda->parent->left) return noda->parent->left;
      noda = noda->parent;
      while (noda->parent) {
        if (noda->parent->right == noda && noda->parent->left) return noda->parent->left;
        noda = noda->parent;
      }
    }
    return shared_ptr<node<T, K>>();
  }
  shared_ptr<node<T,K>> get_first(shared_ptr<node<T, K>> root)override{
        if(!root) return shared_ptr<node<T, K>>();
        shared_ptr<node<T, K>> noda = root;
        while (noda->right||noda->left){
            while (noda->left) noda=noda->left;
            if (noda->right) noda = noda->right;
        }
        return noda;
    }
  shared_ptr<node<T,K>> get_last(shared_ptr<node<T, K>> root)override{return root;}
};
//обход можно реализвоать так: функция выбора первого, функция выбора следующего (это как раз итераторы и делают....)

template<typename T, typename K>
struct lvr_traverser: public traverser<T, K>{//inorder
    shared_ptr<node<T, K>> next(shared_ptr<node<T, K>> noda) override {
        if (!noda) return shared_ptr<node<T, K>>();
        if (noda->right){
            noda = noda->right;
            while (noda->left){
                noda = noda->left;
            }
            return noda;
        }
        while (noda->parent){
            if (noda->parent->left == noda) return noda->parent;
            noda = noda->parent;
        }
        return shared_ptr<node<T, K>>();
    }
    shared_ptr<node<T, K>> prev(shared_ptr<node<T, K>> noda) override{
        if (!noda) return shared_ptr<node<T, K>>();
        if (noda->left){
            noda = noda->left;
            while (noda->right){
                noda = noda->right;
            }
            return noda;
        }
        
        while (noda->parent) {
            shared_ptr<node<T, K>> old_node = noda;
            noda = noda->parent;
            if (noda->right == old_node) return noda;
        }
        return shared_ptr<node<T, K>>();
    }
    shared_ptr<node<T,K>> get_last(shared_ptr<node<T, K>> root) override{
        if(!root) return shared_ptr<node<T, K>>();
        shared_ptr<node<T, K>> noda = root;
        while (noda->right){
            noda = noda->right;
        }
        return noda;
    }

    shared_ptr<node<T,K>> get_first(shared_ptr<node<T, K>> root)override{
        if(!root) return shared_ptr<node<T, K>>();
        shared_ptr<node<T, K>> noda = root;
        while (noda->left){
            noda = noda->left;
        }
        return noda;
    }

};

template<typename T, typename K>
struct vlr_traverser: public traverser<T, K>{//preorder
    shared_ptr<node<T, K>> next(shared_ptr<node<T, K>> noda) override final {
    // std::cout << "Preorder next\n";
    if (!noda) return nullptr;
    if (noda->left) return noda->left;
    if (noda->right) return noda->right;

    while (noda->parent ) {
      shared_ptr<node<T, K>> old_node = noda;
      noda = noda->parent;
      if (noda->left == old_node && noda->right) return noda->right;
    }
    return nullptr;
  }

  shared_ptr<node<T, K>> prev(shared_ptr<node<T, K>> noda) override final {
    // std::cout << "Preorder prev\n";
    if (!noda || !noda->parent) return nullptr;
    
    if (noda->parent->left && noda->parent->right == noda) {
      noda = noda->parent->left;
      while (noda->right||noda->left) {
        noda = noda->right? noda->right: noda->left;
      }
      return noda;
    };
    return noda->parent;
  }
    shared_ptr<node<T,K>> get_last(shared_ptr<node<T, K>> root)override{
        if(!root) return shared_ptr<node<T, K>>();
        shared_ptr<node<T, K>> noda = root;
        while (noda->right||noda->left){
            while (noda->right) noda=noda->right;
            if (noda->left) noda = noda->left;
        }
        return noda;
    }
    shared_ptr<node<T, K>> get_first(shared_ptr<node<T, K>> root)override{return root;}

};


template<typename T, typename K>
struct vrl_traverser: public traverser<T, K>{

    shared_ptr<node<T, K>> next(shared_ptr<node<T, K>> noda) override final {

        if (!noda) return nullptr;
        if (noda->right) return noda->right;
        if (noda->left) return noda->left;
        
        while (noda->parent ) {
        shared_ptr<node<T, K>> old_node = noda;
        noda = noda->parent;
        if (noda->right == old_node && noda->left) return noda->left;
        }
        return nullptr;
    }

    shared_ptr<node<T, K>> prev(shared_ptr<node<T, K>> noda) override final {
        if (!noda || !noda->parent) return nullptr;
        
        if (noda->parent->right && noda->parent->left == noda) {
            noda = noda->parent->right;
            while (noda->right||noda->left) {
            noda = noda->left? noda->left: noda->right;
        }
        return noda;
        };
        return noda->parent;
  }
  shared_ptr<node<T,K>> get_last(shared_ptr<node<T, K>> root)override{
        if(!root) return shared_ptr<node<T, K>>();
        shared_ptr<node<T, K>> noda = root;
        while (noda->right||noda->left){
            while (noda->left) noda=noda->left;
            if (noda->right) noda = noda->right;
        }
        return noda;
    }
    shared_ptr<node<T, K>> get_first(shared_ptr<node<T, K>> root) override{return root;}

};

template<typename T, typename K>
struct rvl_traverser: public traverser<T, K>{
    shared_ptr<node<T, K>> next(shared_ptr<node<T, K>> noda) override {
        if (!noda) return shared_ptr<node<T, K>>();
        if (noda->left){
            noda = noda->left;
            while (noda->right){
                noda = noda->right;
            }
            return noda;
        }
        while (noda->parent){
            if (noda->parent->right == noda) return noda->parent;
            noda = noda->parent;
        }
        return shared_ptr<node<T, K>>();
    }
    shared_ptr<node<T, K>> prev(shared_ptr<node<T, K>> noda) override{
        if (!noda) return shared_ptr<node<T, K>>();
        if (noda->right){
            noda = noda->right;
            while (noda->left){
                noda = noda->left;
            }
            return noda;
        }
        
        while (noda->parent) {
            shared_ptr<node<T, K>> old_node = noda;
            noda = noda->parent;
            if (noda->left == old_node) return noda;
        }
        return shared_ptr<node<T, K>>();
    }
    shared_ptr<node<T,K>> get_last(shared_ptr<node<T, K>> root)override{
        if(!root) return shared_ptr<node<T, K>>();
        shared_ptr<node<T, K>> noda = root;
        while (noda->left){
            noda = noda->left;
        }
        return noda;
    }
    shared_ptr<node<T, K>> get_first(shared_ptr<node<T, K>> root) override{
        if(!root) return shared_ptr<node<T, K>>();
        shared_ptr<node<T, K>> noda = root;
        while (noda->right){
            noda = noda->right;
        }
        return noda;
    }

};

template<typename T, typename K>
struct rlv_traverser: public traverser<T, K>{
    shared_ptr<node<T, K>> next(shared_ptr<node<T, K>> noda) override final {

    if (!noda || !noda->parent) return shared_ptr<node<T, K>>();

    if (noda->parent->left == noda || !noda->parent->left) {
      return noda->parent;
    }
    noda = noda->parent->left;
    while (noda->right) {
      noda = noda->right;
    }
    return noda;
  }
  shared_ptr<node<T, K>> prev(shared_ptr<node<T, K>> noda) override final {
    if (!noda) return shared_ptr<node<T, K>>();
    if (noda->left) return noda->left;
    if (noda->right) return noda->right;
    if (noda->parent) {
      if (noda->parent->left == noda && noda->parent->right) return noda->parent->right;
      noda = noda->parent;
      while (noda->parent) {
        if (noda->parent->left == noda && noda->parent->right) return noda->parent->right;
        noda = noda->parent;
      }
    }
    return shared_ptr<node<T, K>>();
  }
  shared_ptr<node<T,K>> get_first(shared_ptr<node<T, K>> root)override{
        if(!root) return shared_ptr<node<T, K>>();
        shared_ptr<node<T, K>> noda = root;
        while (noda->right||noda->left){
            while (noda->right) noda=noda->right;
            if (noda->left) noda = noda->left;
        }
        return noda;
    }
    shared_ptr<node<T,K>> get_last(shared_ptr<node<T, K>> root)override{return root;}

};

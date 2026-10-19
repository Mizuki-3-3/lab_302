#include "bst.hpp"
#include <iostream>

void test_add(){
    rb_tree<int, int> bleb;
    for (int i =0; i<11; i++){
        bleb.trivial_insert(i,i);
    bleb.print_tree();
    }
    if (bleb.ch()){std::cout<<"eeeeeeeeeeeeeeeeeeeeeeeeee\n";}//вставка норм, удаление как у дибила ебаного
    if (bleb.ch2()){std::cout<<"wowowowowow\n";}
    bleb.remove(3);
    
    bleb.print_tree();
    if (bleb.ch()){std::cout<<"eee\n";}//вреоятно проблемы с красными уйдут если поправить вставку, если нет, то чекнем удаление
    if (bleb.ch2()){std::cout<<"wowowowowow\n";}//уже высоты после удаления съехали
    std::cout<<bleb.search_(47)<<std::endl;//единственная исправно работающая функция
    std::cout<<bleb.search_(7)<<std::endl;

    bleb.remove(10);
    if (bleb.ch()){std::cout<<"ee\n";}
    if (bleb.ch2()){std::cout<<"wowowowowow\n";}
    
    bleb.remove(7);
    std::cout<<bleb.search_(47)<<std::endl;
    if (bleb.ch()){std::cout<<"e\n";}
    if (bleb.ch2()){std::cout<<"wowowowowow\n";}//надо написать обход и осначтить его отладочным выводом
    bleb.remove(1);
    std::cout<<bleb.search_(47)<<std::endl;
    if (bleb.ch()){std::cout<<"e\n";}
    if (bleb.ch2()){std::cout<<"wowowowowow\n";}

}


int main(){
    test_add();
}
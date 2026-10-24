#include "rb_tree.hpp"
#include "test.hpp"

void test_add(){
    rb_tree<int, int> bleb;
    for (int i =0; i<11; i++){
        bleb.insert(i,i);
    bleb.print_tree();
    }
    if (bleb.ch()){std::cout<<"eeeeeeeeeeeeeeeeeeeeeeeeee\n";}//вставка норм, удаление как у дибила ебаного
    if (bleb.ch2()){std::cout<<"wowowowowow\n";}
    bleb.remove(3);
    
    bleb.print_tree();
    if (bleb.ch()){std::cout<<"eee\n";}//вреоятно проблемы с красными уйдут если поправить вставку, если нет, то чекнем удаление
    if (bleb.ch2()){std::cout<<"wowowowowow\n";}//уже высоты после удаления съехали
    std::cout<<bleb.search(47)<<std::endl;//единственная исправно работающая функция
    std::cout<<bleb.search(7)<<std::endl;

    bleb.remove(10);
    if (bleb.ch()){std::cout<<"ee\n";}
    if (bleb.ch2()){std::cout<<"wowowowowow\n";}
    
    bleb.remove(7);
    std::cout<<bleb.search(47)<<std::endl;
    if (bleb.ch()){std::cout<<"e\n";}
    if (bleb.ch2()){std::cout<<"wowowowowow\n";}//надо написать обход и осначтить его отладочным выводом
    bleb.remove(1);
    std::cout<<bleb.search(47)<<std::endl;
    if (bleb.ch()){std::cout<<"e\n";}
    if (bleb.ch2()){std::cout<<"wowowowowow\n";}

}

TEST(search_simple){
    rb_tree<int, int> tree;
    for (int i =0; i<11; i++) tree.insert(i, i);
    CHECK(tree.search(9));
}

TEST(search_in_empty_tree){
    rb_tree<int, int> tree;
    bool thrown = false;
    try {
        tree.search(47);
    }catch(empty_container& e){
        thrown = true;
    }
    CHECK(thrown);
}

TEST(search_no_such_key){
    rb_tree<int, int> tree;
    bool thrown = false;
    tree.insert(5,5);
    try {
        tree.search(47);
    }catch(not_found& e){
        thrown = true;
    }
    CHECK(thrown);
}

TEST(insert_simple){
    rb_tree<int, int> tree;
    for (int i =0; i<11; i++) tree.insert(i,i);
    CHECK(tree.ch());
    CHECK(tree.ch2());
}
TEST(insert_red_uncle){
    rb_tree<int, int> tree;
    for (int i =0; i<4; i++) tree.insert(i,i);
    tree.insert(6, 6);
    tree.insert(5, 5);
    CHECK(tree.ch());
    CHECK(tree.ch2());
}

TEST(insert_such_key_alredy_in_tree){
    rb_tree<int, int> tree;
    for (int i =0; i<5; i++) tree.insert(i,i);
    tree.insert(3, 123456); // вставляем ключ, который уже есть в дереве
    CHECK(tree.ch());
    CHECK(tree.ch2());
    CHECK(tree.search(3) == 123456);
}
TEST(insert_when_black_uncle_p_g_in_dif_sides){
    rb_tree<int, int> tree;
    for (int i =0; i<5; i++) tree.insert(i,i);
    tree.insert(6, 6);
    tree.insert(5, 5);
    CHECK(tree.ch());
    CHECK(tree.ch2());
}
TEST(remove_red_middle_node){
    rb_tree<int, int> tree;
    for (int i =0; i<11; i++) tree.insert(i,i);
    tree.remove(7); // удаляем красный средний узел
    CHECK(tree.ch());
    CHECK(tree.ch2());
}

TEST(remove_red_leaf_node){
    rb_tree<int, int> tree;
    for (int i =0; i<11; i++) tree.insert(i,i);
    tree.remove(8);
    CHECK(tree.ch());
    CHECK(tree.ch2());
}

TEST(remove_black_node_without_children){
    rb_tree<int, int> tree;
    for (int i =0;i<11;i++) tree.insert(i,i);
    tree.remove(4);
    CHECK(tree.ch());
    CHECK(tree.ch2());
}

TEST(remove_black_node_with_one_child){
    rb_tree<int, int> tree;
    for (int i =0;i<10;i++) tree.insert(i,i);
    tree.remove(8);
    CHECK(tree.ch());
    CHECK(tree.ch2());
}

TEST(remove_when_uncle_is_black_and_1_or_2_of_his_children_are_red){
    rb_tree<int, int> tree;
    for (int i =0;i<11;i++) tree.insert(i,i);
    tree.remove(7);
    tree.print_tree();
    CHECK(tree.ch());
    CHECK(tree.ch2());
}

TEST(remove_when_uncle_is_black_and_his_children_are_black){
    rb_tree<int, int> tree;
    for (int i =0;i<11;i++) tree.insert(i,i);
    tree.remove(4);
    CHECK(tree.ch());
    CHECK(tree.ch2());
}

TEST(remove_when_uncle_is_red){
    rb_tree<int, int> tree;
    for (int i =0;i<9;i++) tree.insert(i,i);
    tree.remove(2);
    CHECK(tree.ch());
    CHECK(tree.ch2());
}
TEST(remove_keeps_right_child) {
    rb_tree<int,int> t;
    t.insert(10,10); t.insert(5,5); t.insert(15,15); t.insert(20,20);
    t.remove(10);
    CHECK(t.search(20));
    CHECK(t.search(15));
    CHECK(t.search(5));
}
TEST(remove_red_leaf){
    rb_tree<int, int> tree;
    for (int i =0;i<9;i++) tree.insert(i,i);
    tree.remove(8);
    CHECK(tree.ch());
    CHECK(tree.ch2());
}
TEST(remove_black_leaf){
    rb_tree<int, int> tree;
    for (int i =0;i<9;i++) tree.insert(i,i);
    tree.remove(0);
    CHECK(tree.ch());
    CHECK(tree.ch2());
}
/////////////////////////////////////////////////////

#include <vector>

using tree = rb_tree<int, int>;   // значение, ключ

std::vector<int> seq(tree& t) {
    std::vector<int> v;
    t.order([&](const int& x) {
        if (v.size() > 1000) throw std::runtime_error("цепочка зациклилась");
        v.push_back(x);
    });
    return v;
}

// сравнивает с ожидаемым, а если не совпало - печатает, что получилось
bool same(const std::vector<int>& got, const std::vector<int>& expected) {
    if (got == expected) return true;
    std::cout << "    получили:";
    for (int x : got) std::cout << ' ' << x;
    std::cout << "\n    ждали:   ";
    for (int x : expected) std::cout << ' ' << x;
    std::cout << '\n';
    return false;
}

void add_keys(tree& t, const std::vector<int>& keys) {
    for (int k : keys) t.insert(k, k);      // значение = ключ
}

void add_range(tree& t, int from, int to) {
    for (int k = from; k <= to; k++) t.insert(k, k);
}

const order ALL[6] = {LRV, LVR, VLR, VRL, RVL, RLV};   // как в enum order

const std::vector<int> B_LRV = {0, 2, 1, 4, 6, 8, 10, 9, 7, 5, 3};
const std::vector<int> B_LVR = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const std::vector<int> B_VLR = {3, 1, 0, 2, 5, 4, 7, 6, 9, 8, 10};
const std::vector<int> B_VRL = {3, 5, 7, 9, 10, 8, 6, 4, 1, 2, 0};
const std::vector<int> B_RVL = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
const std::vector<int> B_RLV = {10, 8, 9, 6, 7, 4, 5, 2, 0, 1, 3};

TEST(thread_empty_tree) {
    tree t;
    for (order o : ALL) {
        t.threading(o);
        CHECK(seq(t).empty());
    }
}

TEST(thread_one_node) {
    tree t;
    t.insert(42, 42);
    for (order o : ALL) {
        t.threading(o);
        CHECK(same(seq(t), {42}));
    }
}

TEST(thread_two_nodes) {
    tree t;
    add_keys(t, {1, 2});            // 1 - корень, 2 - справа
    t.threading(LRV); CHECK(same(seq(t), {2, 1}));
    t.threading(LVR); CHECK(same(seq(t), {1, 2}));
    t.threading(VLR); CHECK(same(seq(t), {1, 2}));
    t.threading(VRL); CHECK(same(seq(t), {1, 2}));
    t.threading(RVL); CHECK(same(seq(t), {2, 1}));
    t.threading(RLV); CHECK(same(seq(t), {2, 1}));
}

TEST(thread_three_nodes) {
    tree t;
    add_keys(t, {2, 1, 3});         // 2 - корень, 1 слева, 3 справа
    t.threading(LRV); CHECK(same(seq(t), {1, 3, 2}));
    t.threading(LVR); CHECK(same(seq(t), {1, 2, 3}));
    t.threading(VLR); CHECK(same(seq(t), {2, 1, 3}));
    t.threading(VRL); CHECK(same(seq(t), {2, 3, 1}));
    t.threading(RVL); CHECK(same(seq(t), {3, 2, 1}));
    t.threading(RLV); CHECK(same(seq(t), {3, 1, 2}));
}


TEST(thread_LRV) { tree t; add_range(t, 0, 10); t.threading(LRV); CHECK(same(seq(t), B_LRV)); }
TEST(thread_LVR) { tree t; add_range(t, 0, 10); t.threading(LVR); CHECK(same(seq(t), B_LVR)); }
TEST(thread_VLR) { tree t; add_range(t, 0, 10); t.threading(VLR); CHECK(same(seq(t), B_VLR)); }
TEST(thread_VRL) { tree t; add_range(t, 0, 10); t.threading(VRL); CHECK(same(seq(t), B_VRL)); }
TEST(thread_RVL) { tree t; add_range(t, 0, 10); t.threading(RVL); CHECK(same(seq(t), B_RVL)); }
TEST(thread_RLV) { tree t; add_range(t, 0, 10); t.threading(RLV); CHECK(same(seq(t), B_RLV)); }

TEST(thread_every_order_visits_all_11_nodes) {
    tree t;
    add_range(t, 0, 10);
    for (order o : ALL) {
        t.threading(o);
        CHECK(seq(t).size() == 11);
    }
}

TEST(rethread_forward) {
    tree t;
    add_range(t, 0, 10);
    t.threading(LRV); CHECK(same(seq(t), B_LRV));
    t.threading(LVR); CHECK(same(seq(t), B_LVR));
    t.threading(VLR); CHECK(same(seq(t), B_VLR));
    t.threading(VRL); CHECK(same(seq(t), B_VRL));
    t.threading(RVL); CHECK(same(seq(t), B_RVL));
    t.threading(RLV); CHECK(same(seq(t), B_RLV));
}

TEST(rethread_backward) {
    tree t;
    add_range(t, 0, 10);
    t.threading(RLV); CHECK(same(seq(t), B_RLV));
    t.threading(RVL); CHECK(same(seq(t), B_RVL));
    t.threading(VRL); CHECK(same(seq(t), B_VRL));
    t.threading(VLR); CHECK(same(seq(t), B_VLR));
    t.threading(LVR); CHECK(same(seq(t), B_LVR));
    t.threading(LRV); CHECK(same(seq(t), B_LRV));
}

TEST(rethread_same_order_twice) {
    tree t;
    add_range(t, 0, 10);
    t.threading(VLR);
    t.threading(VLR);
    CHECK(same(seq(t), B_VLR));
}

TEST(rethread_after_insert) {
    tree t;
    add_range(t, 0, 10);
    t.threading(LVR);
    t.insert(11, 11);
    t.threading(LVR);
    CHECK(seq(t).size() == 12);
    CHECK(seq(t).back() == 11);
}

TEST(rethread_after_remove) {
    tree t;
    add_range(t, 0, 10);
    t.threading(LVR);
    t.remove(5);
    t.threading(LVR);
    CHECK(same(seq(t), {0, 1, 2, 3, 4, 6, 7, 8, 9, 10}));
}

TEST(threading_returns_the_same_tree) {
    tree t;
    add_range(t, 0, 3);
    CHECK(&t.threading(LVR) == &t);
}

TEST(order_on_empty_tree_does_nothing) {
    tree t;
    int calls = 0;
    t.order([&](const int&) { calls++; });
    CHECK(calls == 0);
}

TEST(order_before_threading_does_nothing) {
    tree t;
    add_range(t, 0, 10);
    int calls = 0;
    t.order([&](const int&) { calls++; });
    CHECK(calls == 0);
}

TEST(order_gives_values_not_keys) {
    tree t;
    for (int k = 0; k <= 4; k++) t.insert(k, k + 100);   // ключ k, значение k + 100
    t.threading(LVR);
    CHECK(same(seq(t), {100, 101, 102, 103, 104}));
}

TEST(order_twice_gives_the_same_result) {
    tree t;
    add_range(t, 0, 10);
    t.threading(VRL);
    CHECK(same(seq(t), seq(t)));
}

TEST(order_works_on_const_tree) {
    tree t;
    add_range(t, 0, 10);
    t.threading(LVR);
    const tree& ct = t;
    int sum = 0;
    ct.order([&](const int& x) { sum += x; });
    CHECK(sum == 55);                                    // 0 + 1 + ... + 10
}

// ------------------------------------------------- map

TEST(map_on_empty_tree_does_nothing) {
    tree t;
    int calls = 0;
    t.map([&](int&) { calls++; });
    CHECK(calls == 0);
}

TEST(map_before_threading_does_nothing) {
    tree t;
    add_range(t, 0, 10);
    int calls = 0;
    t.map([&](int&) { calls++; });
    CHECK(calls == 0);
}

TEST(map_one_node) {
    tree t;
    t.insert(7, 70);
    t.threading(LVR);
    t.map([](int& v) { v += 1; });
    CHECK(same(seq(t), {71}));
}

TEST(map_changes_values_in_the_tree) {
    tree t;
    add_range(t, 0, 10);
    t.threading(LVR);
    t.map([](int& v) { v *= 2; });
    CHECK(same(seq(t), {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20}));
    t.map([](int& v) { v += 1; });
    CHECK(same(seq(t), {1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21}));
}

TEST(map_visits_every_node_once) {
    for (order o : ALL) {
        tree t;
        add_range(t, 0, 10);
        t.threading(o);
        int calls = 0;
        t.map([&](int&) { calls++; });
        CHECK(calls == 11);
    }
}

TEST(map_goes_in_threading_order) {
    std::vector<int> visited;

    tree a;
    add_range(a, 0, 10);
    a.threading(LRV);
    a.map([&](int& v) { visited.push_back(v); });
    CHECK(same(visited, B_LRV));

    visited.clear();
    tree b;
    add_range(b, 0, 10);
    b.threading(VLR);
    b.map([&](int& v) { visited.push_back(v); });
    CHECK(same(visited, B_VLR));

    visited.clear();
    tree c;
    add_range(c, 0, 10);
    c.threading(RVL);
    c.map([&](int& v) { visited.push_back(v); });
    CHECK(same(visited, B_RVL));
}

TEST(map_does_not_break_the_tree_structure) {
    tree t;
    add_range(t, 0, 10);
    t.threading(LVR);
    t.map([](int& v) { v += 1; });
    t.threading(VLR);                                    // порядок тот же, значения на 1 больше
    CHECK(same(seq(t), {4, 2, 1, 3, 6, 5, 8, 7, 10, 9, 11}));
    t.threading(RLV);
    CHECK(same(seq(t), {11, 9, 10, 7, 8, 5, 6, 3, 1, 2, 4}));
    CHECK(t.ch());                                       // красных детей у красных нет
    CHECK(t.ch2());                                      // чёрная высота одинакова
}

TEST(map_returns_the_same_tree) {
    tree t;
    add_range(t, 0, 3);
    t.threading(LVR);
    CHECK(&t.map([](int&) {}) == &t);
}

TEST(map_can_be_chained) {
    tree t;
    add_range(t, 0, 4);
    t.threading(LVR);
    t.map([](int& v) { v += 1; }).map([](int& v) { v *= 10; });
    CHECK(same(seq(t), {10, 20, 30, 40, 50}));
}

TEST_ENTRY_POINT
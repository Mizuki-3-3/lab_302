#pragma once
#include <iostream>
#include <vector>


using test_func = void (*)();

static unsigned int passed = 0;
static unsigned int failed = 0;

struct test{
    const char* name;
    test_func test_;
    test(const char* n, test_func t): name(n), test_(t){}
};

static std::vector<test> tests;
void run_tests(){
    for (auto& t: tests){
        try{t.test_();}
        catch(...){
            failed++;
            std::cout << "  FAIL: unknown exception\n" << " (" << __FILE__ << ":" << __LINE__ << ")\n";
        }
        
    }
}

void register_test(const char *name, test_func t){
    tests.push_back({name, t});
}

int print_stats(){
    if (!(passed+failed)) return 0;
    std::cout<<"Passed: "<<passed<<" Failed: "<<failed<<"  succes percentage: " << 100*static_cast<float>(passed)/static_cast<float>(passed+failed);
    return 1;
}

void assert_fail(const char* condition, const char* f,int l){
    ++failed;
    std::cout << "  FAIL: " << condition << " (" << f << ":" << l << ")\n";
}


void assert_success(const char* condition){
    ++passed;
    std::cout << "  PASS: " << condition << '\n';
}
#define TEST(test_name) \
    void test_name##_test(); \
    void __attribute__((constructor)) test_name##_init() { \
        register_test(#test_name, test_name##_test); \
    } \
    void test_name##_test()


#define TEST_ENTRY_POINT \
    int main(void) { \
        run_tests(); \
        return print_stats(); \
    }
#define CHECK(expr) \
    ((expr) ? assert_success(#expr) : assert_fail(#expr, __FILE__, __LINE__))
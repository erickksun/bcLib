#include "../includes/bcFunction.hpp"
#include <iostream>
struct Test
{
    int foo(int,char)
    {
        std::cout << "[" << __FUNCTION__<< "," << __LINE__ << "]"<< std::endl;
        return __LINE__;
    }
} test;

int foo(int,char)
{
    std::cout << "[" << __FUNCTION__<< "," << __LINE__ << "]"<< std::endl;
    return __LINE__;
}

int main()
{
    bc::Function<int (int,char)> foo1;
    if (foo1.IsValide())
        foo1(0,'0');
    foo1 = bc::Function<int (int,char)>::Bind(&foo);

    bc::Function<int (int,char)> foo2;
    foo2 = bc::Function<int (int,char)>::Bind(&test, &Test::foo);
    std::cout << foo1(0,'0') << std::endl;
    std::cout << foo2(0,'0') << std::endl;

    return 0;
}
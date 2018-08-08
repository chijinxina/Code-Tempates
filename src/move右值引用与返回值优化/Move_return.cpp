//
// Created by chijinxin on 18-8-5.
//
#include <iostream>
using namespace std;

/*
 * 返回值优化
 * add_definitions(-fno-elide-constructors)
 */
class A {
public:
    A(){}
    //拷贝构造函数
    A(const A &AA)
    {
        cout<<"Copy"<<endl;
    }
    A(A &&AA)
    {
        cout<<"move"<<endl;
    }
    //赋值构造函数
    A& operator=(const A &AA)
    {
        cout<<"operator="<<endl;
        return *this;
    }
    A& operator=(A &&AA)
    {
        cout<<"move operator="<<endl;
        return *this;
    }
};

A getA()
{
    A a;
    return a;
}

int main()
{
    A a = getA();
    return 0;
}

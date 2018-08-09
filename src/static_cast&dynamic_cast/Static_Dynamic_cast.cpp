//
// Created by chijinxin on 18-8-8.
//
#include <iostream>
using namespace std;

/*
 * C语言的const其实只是只读变量 可以用指针间接改变其值
 * const 修饰的局部变量在栈上分配空间
 * const 修饰的全局变量在只读存储区分配空间
 */
const int aaa1 = 10;

void IFNULL(int* z)
{
    if(z==NULL) cout<<"Is NULL"<<endl;
    else cout<<"Is not NULL"<<endl;
}

int main()
{
    /*
     * const_cast
     * const_cast是一种C++运算符，主要是用来去除复合类型中const和volatile属性（没有真正去除）。
     * 变量本身的const属性是不能去除的，要想修改变量的值，一般是去除指针（或引用）的const属性，再进行间接修改。
     */
    volatile const int aaa2 = 11;
    cout<<"previous: aaa1="<<aaa1<<endl;
    cout<<"previous: aaa2="<<aaa2<<endl;
   // int *aa1 = (int*)&aaa1;
    int* aa1 = const_cast<int*>(&aaa1);
    int* aa2 = const_cast<int*>(&aaa2);
    int* ppp = (int*)&aaa2;

    cout<<"aaa1:"<<&aaa1<<endl;
    cout<<" aa1:"<<aa1<<endl;

    IFNULL(aa1);
    IFNULL(aa2);

    cout<<"after: aaa1="<<aaa1<<endl;
    cout<<"after: aaa2="<<aaa2<<endl;




    return 0;
}

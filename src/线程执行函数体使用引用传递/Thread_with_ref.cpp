//
// Created by chijinxin on 17-12-6.
//
#include <iostream>
#include <thread>
#include <functional>
#include <string>
using namespace std;
/*
 * 《确保线程执行函数体正确使用引用传递》
 *    线程执行函数采用了引用传递方式，但是编译器仍然通过拷贝来将我们的对象的副本传入执行函数，
 *  因此线程执行体采用引用传递时并不能达到我们所期望的结果。
 *
 *  1）直接使用引用&传递不能达到预期目的，需要使用std::ref来封装需要引用传递的对象；
 *  2）使用指针可以达到我们期望的目的；
 *  3）线程使用lambda表达式同样可以达到我们期望的目的。
 *
 *  http://blog.csdn.net/acaiwlj/article/details/49472871
 */


void threadFunc(std::string& oldName, const std::string& newName)   //  线程执行函数体
{
    std::cout<<"the name is \""<<oldName<<"\" change to ";
    oldName = newName;
    std::cout<<"\""<<oldName<<"\""<<std::endl;
}

void threadFunc2(std::string* oldName, const std::string& newName)   //  使用指针操作实现引用类型的传递
{
    std::cout<<"the name is \""<<*oldName<<"\" change to ";
    *oldName = newName;
    std::cout<<"\""<<*oldName<<"\""<<std::endl;
}




int main(){
    string oldName = "chijinxin";
    std::thread t1 = std::thread(std::bind(&threadFunc,std::ref(oldName),"qq")); //（1）使用std::ref来封装需要引用传递的对象，避免对象的拷贝操作
    //auto t2 = std::thread(std::bind(&threadFunc2,&oldName,"aa"));//(2)使用指针可以达到我们期望的目的；
//    auto t3 = std::thread(std::bind([&](string& oldName, const string& newName)mutable
//                                    {
//                                        std::cout<<"the name is \""<<oldName<<"\" change to ";
//                                        oldName = newName;
//                                        std::cout<<"\""<<oldName<<"\""<<std::endl;
//                                    },std::ref(oldName),"zzz"));
    t1.join();
    //threadFunc(oldName,newName);
    cout<<"Name is \""<<oldName<<"\""<<endl;
    return 0;
}
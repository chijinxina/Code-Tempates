//
// Created by chijinxin on 17-12-6.
//
#include <iostream>
#include <vector>
#include <algorithm> //for_each
#include <functional>//std::bind std::ref

#include <boost/bind.hpp>//boost::bind()
#include <boost/ref.hpp>//boost::ref()

using namespace std;


/*
 * for_each的使用
 * boost::bind() boost::ref() boost::_1占位符
 */
void print(std::ostream &out,int x)
{
    out<<x<<std::endl;
}


int main()
{
    vector<int> v;
    for(int i=0;i<10;i++) v.push_back(i);

    /*
     * for_each的使用
     * STL lambda表达式
     */
    cout<<"STL lambda表达式"<<endl;
    for_each(v.begin(),v.end(),[](int a){std::cout<<a<<std::endl;});

    /*
     * for_each的使用
     * boost::bind() boost::ref() boost::_1占位符
     */
    cout<<"for_each的使用(boost::bind() boost::ref() boost::_1占位符)"<<endl;
    for_each(v.begin(),v.end(),boost::bind(&print,boost::ref(std::cout),_1));
    return 0;
}
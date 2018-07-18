//
// Created by chijinxin on 17-12-8.
//
#include <iostream>
#include <iomanip>  //格式控制符


int main()
{
    double a = 0.33333333333333;
    std::cout<<std::setprecision(2)<<a<<std::endl;
    std::cout<<std::setprecision(3)<<a<<std::endl;
    std::cout<<a<<std::endl;
    std::cout<<std::fixed<<a<<std::endl;
    return 0;
}


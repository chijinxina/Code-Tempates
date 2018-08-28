//
// Created by chijinxin on 18-8-28.
//
#include <iostream>
using namespace std;
/*
 * 二分法实现pow()
 */
double power(double x, int n)
{
    if(n==0) return 1;   //x^0 = 1

    double v = power(x, n/2);  //二分法递归下去
    //n是偶数
    if(n%2 == 0)
    {
        return v*v;
    }
    //n是奇数
    else
    {
        return v*v*x;
    }
}

double pow1(double x, int n)
{
    if(n > 0) return power(x,n);
    else return 1.0/power(x,-n);
}


/*
 * 二分法实现sqrt()
 */
int sqrt1(int x)
{
    int left = 0;
    int right= x;
    double diff = 0.01;   //精度

    while(left <= right)
    {
        double mid = (right + left)/2;
        if ((x - mid * mid) > diff)
        {
            left = (int)mid+1;
        }
        else if ((x - mid * mid) < -diff)
        {
            right = (int)mid-1;
        }
        else return (int)mid;
    }
    return right;
}


int main()
{
    cout<<"pow(2,10)="<<pow1(2,10)<<endl;
    cout<<"pow(2,-2)="<<pow1(2,-2)<<endl;
    cout<<"sqrt(4)="<<sqrt1(4)<<endl;
    cout<<"sqrt(10)="<<sqrt1(10)<<endl;
    return 0;
}

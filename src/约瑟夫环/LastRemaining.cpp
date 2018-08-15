//
// Created by chijinxin on 18-8-15.
//
#include <iostream>
using namespace std;
/*
 * 约瑟夫环
 *
 * 令f[i]表示i个人玩游戏报m退出最后胜利者的编号，最后的结果自然是f[n]。
 * 递推公式:
 *      f[1]=0;
 *      f[i]=(f[i-1]+m )%i;  (i>1)
 */
int LastRemaining(int n, int m)
{
    if(n==0) return -1;
    if(n==1) return 0;
    return (LastRemaining(n-1, m) + m) % n;
}

int main()
{
    return 0;
}

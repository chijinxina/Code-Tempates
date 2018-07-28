#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

int k,n=10;

int a[10]={1,1,1,3,3,5,5,5,5,6};


int main()
{
    for(int i=0;i<n;i++)cout<<a[i]<<" ";
    cout<<endl;
    while(scanf("%d",&k))
    {
        cout<<k<<"的第一个大于等于它的位置在"<<((lower_bound(a,a+n,k))-a)<<endl;
        cout<<k<<"的第一个大于它的位置在"<<((upper_bound(a,a+n,k))-a)<<endl;
    }
}
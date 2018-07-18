//
// Created by chijinxin on 18-3-24.
//
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

void BubbleSort(vector<int>::iterator begin, vector<int>::iterator end)
{
    int tmp=0;
    for(int i=0;(begin+i)!=end;i++)
    {
        for(int j=0;(begin+j)<end-i-1;j++)
        {
            if(*(begin+j)>*(begin+j+1))
            {
                tmp = *(begin+j);
                *(begin+j) = *(begin+j+1);
                *(begin+j+1) = tmp;
            }
        }
    }
}


int main()
{
    vector<int> data={6,1,2,7,9,3,4,5,10,8};
    cout<<"begin:"<<endl;
    for_each(data.begin(),data.end(),[](int a){cout<<a<<" ";});
    cout<<endl;

    BubbleSort(data.begin(),data.end());

    cout<<"after:"<<endl;
    for_each(data.begin(),data.end(),[](int a){cout<<a<<" ";});
    cout<<endl;
    return 0;
}

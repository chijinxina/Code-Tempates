//
// Created by chijinxin on 18-3-24.
//
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void SelectSort(vector<int>::iterator begin, vector<int>::iterator end)
{
    for(auto iter=begin;iter!=end;++iter)
    {
        int min = *iter;
        auto index = iter;
        for(auto jter=iter+1;jter!=end;++jter)
        {
            if(*jter < min)
            {
                min = *jter;
                index = jter;
            }
        }
        if(min < *iter)
        {
            int tmp = *iter;
            *iter = *index;
            *index = tmp;
        }
    }
}


int main()
{
    vector<int> data={6,1,2,7,9,3,4,5,10,8};
    cout<<"begin:"<<endl;
    for_each(data.begin(),data.end(),[](int a){cout<<a<<" ";});
    cout<<endl;

    SelectSort(data.begin(),data.end());

    cout<<"after:"<<endl;
    for_each(data.begin(),data.end(),[](int a){cout<<a<<" ";});
    cout<<endl;
    return 0;
}

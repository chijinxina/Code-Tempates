//
// Created by chijinxin on 17-12-6.
//
#include <iostream>
#include <vector>
#include <algorithm> //min_element
using namespace std;


int main(){
    vector<int> v;
    for(int i=10;i<100;i++) v.push_back(i);

    cout<<"min demo:"<<endl;
    /*
     * min demo
     */
    cout<<min(v[0],v[1])<<endl;

    cout<<"min_element demo:(min)"<<endl;
    /*
     * min_element min
     */
    vector<int>::iterator iter;
    iter = min_element(v.begin(),v.end());
    cout<<"min in vector v:"<<*iter<<endl;

    cout<<"min_element demo:(max)"<<endl;
    /*
     * min_element max
     * with STL-lambda表达式
     */
    vector<int>::iterator iter2;
    iter2 = min_element(v.begin(),v.end(),[](int x1,int x2){return x1>=x2?true:false;});
    cout<<"min in vector v:"<<*iter2<<endl;

    return 0;
}

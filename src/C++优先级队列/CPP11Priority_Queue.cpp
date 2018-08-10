//
// Created by chijinxin on 18-8-10.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

/*
 * priority_queue 对于基本类型的使用方法相对简单。他的模板声明带有三个参数:
 *      priority_queue<Type, Container, Functional> 其中Type 为数据类型， Container 为保存数据的容器，Functional 为元素比较方式。
 *  Container 必须是用数组实现的容器，比如 vector, deque 但不能用 list.
 *  STL里面默认用的是 vector. 比较方式默认用 operator< , 所以如果你把后面俩个参数缺省的话，
 *  优先队列就是大顶堆，队头元素最大。
 */


int main()
{
    priority_queue<int,vector<int>,greater<int>> q;  //默认是大顶堆
    for (int i=0;i<10;++i)
        q.push(rand()%10);

    while(!q.empty())
    {
        cout<<q.top()<<" ";
        q.pop();
    }
    return 0;
}


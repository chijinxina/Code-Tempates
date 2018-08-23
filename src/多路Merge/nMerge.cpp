//
// Created by chijinxin on 18-8-23.
//
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Node{
    int lu;
    int index;
    int val;
    Node(int v, int l, int i):val(v),lu(l),index(i){}
    bool operator > (const Node &n) const
    {
        if(val > n.val) return true;
        else return false;
    }
};

vector<int> nMerger(vector<vector<int>> &vec)
{
    vector<Node> heap;
    vector<int> result;
    for(int i=0; i<vec.size(); i++)
    {
        if(!vec[i].empty())
        {
            heap.emplace_back(vec[i][0], i, 0);
        }
    }
   // int n = heap.size();
    //建堆
    make_heap(heap.begin(), heap.end(), greater<Node>());  //建立小顶堆

    while(!heap.empty())
    {
        //取出堆顶元素
        pop_heap(heap.begin(), heap.end(), greater<Node>());
        Node temp = heap.back();
        result.push_back(temp.val);
        heap.pop_back();  //删除堆顶元素
        if(temp.index+1 < vec[temp.lu].size())
        {
            heap.emplace_back(vec[temp.lu][temp.index+1], temp.lu, temp.index+1);
        }
        push_heap(heap.begin(), heap.end(), greater<Node>());
    }
    return result;
}


int main()
{
    vector<int> k1 = {1,2,3,4,5,6,7,8,9};
    vector<int> k2 = {1,1,1,3,3,5,6,7,8,10};
    vector<int> k3 = {5};
    vector<int> k4 = {3,5,7};
    vector<int> k5 = {8,9,10,10,11};
    vector<vector<int>> vec;
    vec.push_back(k1);
    vec.push_back(k2);
    vec.push_back(k3);
    vec.push_back(k4);
    vec.push_back(k5);

    vector<int> result = nMerger(vec);
    for(int x : result)
    {
        cout<<x<<" ";
    }

    return 0;
}

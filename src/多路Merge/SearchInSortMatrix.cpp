//
// Created by chijinxin on 18-9-17.
//
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct node{
    int value;
    int i;
    int j;
    node(int v, int ii, int jj):value(v),i(ii),j(jj){}
    bool operator > (const node &n) const
    {
        if(value > n.value) return true;
        else return false;
    }
};

int SearchInSortMatrix(const vector<vector<int>> &matrix, int k)
{
    int n = matrix.size();    //行
    int m = matrix[0].size(); //列
    priority_queue<node, vector<node>, greater<node> > pq;
    for(int i=0;i<n;i++)
    {
        pq.emplace(matrix[i][0],i,0);
    }
    node result(0,0,0);
    while(k--)
    {
        result = pq.top();
        pq.pop();
        if(result.j+1 < m)
        {
            pq.emplace(matrix[result.i][result.j+1],result.i,result.j+1);
        }
    }
    return result.value;
}


int main()
{
    vector<vector<int>> matrix{{1,5,9},{10,11,13},{12,13,15}};
    cout<<SearchInSortMatrix(matrix,8)<<endl;
    return 0;
}

//
// Created by chijinxin on 18-3-23.
// 快速排序
//
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;


/*
 * 链表快排
 */
struct Node{
    int value;
    Node *next;
    Node(int v, Node* n):value(v),next(n){}
};

Node* GetPartition(Node* begin, Node* end)
{
    int value = begin->value;
    Node* p = begin;
    Node* q = p->next;

    while(q != end)
    {
        if(q->value < value)
        {
            p = p->next;
            swap(p->value, q->value);
        }
        q = q->next;
    }
    swap(p->value, begin->value);
    return p;
}

void QSort_list(Node* begin, Node* end)
{
    if(begin != end)
    {
        Node* mid = GetPartition(begin, end);
        QSort_list(begin, mid);
        QSort_list(mid->next, end);
    }
}


vector<int> data={6,1,2,7,9,3,4,5,10,8};
int a[] = {6,1,2,7,9,3,4,5,10,8};


void quicksort(vector<int>::iterator begin,vector<int>::iterator end,int left,int right)
{
    if(left>right) return;

    int i=left;  //左哨兵
    int j=right; //右哨兵
    int benchmark = *(begin+left);//基准数
    int tmp;
    while(i!=j)
    {
        while(*(begin+j) >= benchmark && i<j) //先从右往左找小于基准数的数字
        {
            j--;
        }
        while(*(begin+i) <= benchmark && i<j)//然后从左往右找大于基准数的数字
        {
            i++;
        }
        //交换两个哨兵
        if(i<j)
        {
            tmp = *(begin+i);
            *(begin+i) = *(begin+j);;
            *(begin+j) =tmp;
        }
    }
    //最后将基准数归位
    *(begin+left) = *(begin+i);
    *(begin+i) = benchmark;

    quicksort(begin,end,left,i-1);//递归地排左边
    quicksort(begin,end,i+1,right);//递归地排右边
}


void quicksort_2(int a[],int n,int left,int right)
{
    if(left>right) return; //递归出口

    int i = left;  //左哨兵
    int j = right; //右哨兵
    int benchmark = a[left];

    while(i!=j)
    {
        while(a[j]>=benchmark && i<j)
        {
            j--;
        }
        a[i] = a[j];
        a[j] = benchmark;
        while(a[i]<=benchmark && i<j)
        {
            i++;
        }
        a[j] = a[i];
        a[i] = benchmark;
    }
    quicksort_2(a,n,left,i-1);
    quicksort_2(a,n,i+1,right);
}



int main()
{
    int n=10;
//    cout<<"begin:"<<endl;
//    for_each(data.begin(),data.end(),[](int a){cout<<a<<" ";});
//    cout<<endl;
//    //test(data.begin(),data.end());
//    quicksort(data.begin(),data.end(),0,data.size()-1);
//    cout<<"after:"<<endl;
//    for_each(data.begin(),data.end(),[](int a){cout<<a<<" ";});

    cout<<"begin:"<<endl;
    for(int i=0;i<10;i++) cout<<a[i]<<" ";
    cout<<endl;
    //test(data.begin(),data.end());
    //quicksort(data.begin(),data.end(),0,data.size()-1);
    quicksort_2(a,10,0,9);
    cout<<"after:"<<endl;
    for(int i=0;i<10;i++) cout<<a[i]<<" ";
    return 0;
}

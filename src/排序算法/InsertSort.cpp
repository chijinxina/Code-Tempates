//
// Created by chijinxin on 18-3-28.
// 直接插入排序
//

#include <iostream>
using namespace std;

int a[] = {6,1,2,7,9,3,4,5,10,8};
//int a[] = {1,2,3,5,6,7,8,4,9,10};
/*
 * 数组直接插入排序
 */
void InsertSort(int a[], int n)
{
    int tmp=0;
    int j=0;
    for(int i=1;i<n;i++)
    {
        if(a[i]<a[i-1])
        {
            tmp = a[i];
            for(j=i;j>0;j--)
            {
                if(a[j-1]<tmp)
                {
                    break;
                }
                else
                {
                    a[j] = a[j-1];
                }
            }
            a[j] = tmp;
        }
    }

}


/*
 * 链表直接插入排序
 */
//链表节点类
class list_node{
public:
    int data; //节点数据域
    list_node* next; //节点前驱指针
    list_node* prev; //节点后继指针
    list_node():next(NULL),prev(NULL){}
    list_node(int d, list_node* p, list_node* n):data(d),prev(p),next(n){}
};

class List {
public:
    list_node* HeadNode;
    List()
    {
        HeadNode = new list_node(0,NULL,NULL);
        HeadNode->next = HeadNode;
        HeadNode->prev = HeadNode; //尾节点
    }
    void push_back(int d)
    {
        list_node* newNode = new list_node(d,NULL,NULL);
        //HeadNode->prev为上一次的尾节点
        HeadNode->prev->next = newNode;
        newNode->next = HeadNode;
        newNode->prev = HeadNode->prev;
        HeadNode->prev = newNode;
    }
};
//链表插入排序排序
void InsertSort4List(List &list)
{
    list_node* iii = list.HeadNode;
    list_node* tmp = list.HeadNode;
    list_node* iter = list.HeadNode->next;
    list_node* iter_begin = list.HeadNode->next;
    list_node* iter_end = list.HeadNode;
   // for(iter=iter_begin->next;iter!=iter_end; iter=iter->next)
    iter=iter_begin->next;
    while(iter!=iter_end)
    {
        if(iter->data < iter->prev->data)
        {
            //保护现场
            tmp = iter->next;
            //
            (iter->prev)->next = (iter->next);
            (iter->next)->prev = (iter->prev);//拆除链接 重建链接

            iii = iter->prev;
            while(iii->data > iter->data && iii!=list.HeadNode)
            {
                iii=iii->prev;
            }
            iter->next = iii->next;
            iter->next->prev = iter;
            iii->next = iter;
            iter->prev = iii;
            //恢复现场
            iter = tmp;
            //
        }
        else
        {
            iter=iter->next;
        }
    }

}



int main()
{
//    cout<<"befor:"<<endl;
//    for(int i=0;i<10;i++)
//        cout<<a[i]<<" ";
//    cout<<endl;
//    int n=10;
//    InsertSort(a,10);
//    cout<<"befor:"<<endl;
//    for(int i=0;i<10;i++)
//        cout<<a[i]<<" ";
//    cout<<endl;
    List cjx;
    for(int i=0;i<10;i++)
    {
        cjx.push_back(a[i]);
    }
    list_node* iter = cjx.HeadNode->next;
    cout<<"befor:"<<endl;
    for(;iter!=cjx.HeadNode;iter=iter->next)
    {
        cout<<iter->data<<" ";
    }
    cout<<endl;
    cout<<"after:"<<endl;
    InsertSort4List(cjx);
    for(iter = cjx.HeadNode->next;iter!=cjx.HeadNode;iter=iter->next)
    {
        cout<<iter->data<<" ";
    }
    cout<<endl;
    return 0;
}
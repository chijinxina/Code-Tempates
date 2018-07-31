//
// Created by chijinxin on 18-7-31.
//
#include <iostream>
using namespace std;

//循环单链表节点定义
class ListNode{
public:
    int val;
    ListNode* next;
    ListNode(int v):val(v),next(NULL){}
};
//循环单链表定义
class List{
public:
    ListNode* head;
    ListNode* end;

    List(int n):head(NULL),end(NULL)
    {
        for(int i=0;i<n;i++)
        {
            push_back(0);
        }
    }

    void push_back(int back)
    {
        if(head==NULL)
        {
            head = new ListNode(back);
            end = head;
            end->next = head;
        }
        else
        {
            ListNode* temp = new ListNode(back);
            end->next = temp;
            end = end->next;
            end->next = head;
        }
    }
};

//打印循环单链表
//void PrintList(List &l)
//{
//    if(l.head==NULL) return;
//    ListNode* current = l.head;
//    do
//    {
//        cout<<current->val<<" ";
//        current = current->next;
//    }while(current!=l.end);
//    cout<<l.end->val<<" ";
//}

/*
 * 循环队列 循环单链表实现
 */
class CircularQueue{
public:
    CircularQueue(int size):l(size)
    {
        read = l.head;
        write = l.head;
    }

    int front()
    {
        if(!isEmpty()) return read->val;
        else return 99999;
    }

    void pop()
    {
        if(!isEmpty()) read = read->next;
    }

    bool isEmpty()
    {
        if(read == write) return true;
        else return false;
    }

    void push_back(int v)
    {
        if(!isFull())
        {
            write->val = v;
            write = write->next;
        }
    }

    bool isFull()
    {
        if(write->next == read) return true;
        else return false;
    }

    ListNode* read;  //读指针
    ListNode* write; //写指针
    List l;
};


void TRUE(bool b)
{
    if(b) cout<<"True"<<endl;
    else cout<<"False"<<endl;
}

int main()
{
    CircularQueue q(10);
    TRUE(q.isEmpty());
    TRUE(q.isFull());
    q.push_back(1);
    q.push_back(2);
    q.push_back(3);
    TRUE(q.isEmpty());
    cout<<q.front()<<endl;
    q.pop();
    cout<<q.front()<<endl;
    q.pop();
    cout<<q.front()<<endl;
    //多余pop()
    q.pop();
    TRUE(q.isEmpty());
    cout<<q.front()<<endl;

    for(int i=0;i<11;i++)
    {
        q.push_back(11-i);
    }
    TRUE(q.isFull());
    cout<<q.front()<<endl;
    return 0;
}

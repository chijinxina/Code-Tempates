//
// Created by chijinxin on 18-7-31.
//
#include <iostream>
using namespace std;

/*
 * 循环队列通用元素长度计算公式
 *
 * len = (end - front + QueueSize) % QueueSize
 */

class CircleQueue{
public:
    CircleQueue(int size):size_(size),read_(0),write_(0)//,queue_(new int[size])
    {

    }

    bool isEmpty()
    {
        //判空 front == end
       if( write_ == read_ ) return true;
       else return false;
    }

    bool isFull()
    {
        //判满 (write_+1)%size == read_
        if( (write_ + 1)%size_ == read_ ) return true;
        else return false;
    }

    int length()
    {
        return (write_ - read_ + size_ ) % size_;
    }

    void push_back(int v)
    {
        if(!isFull())
        {
            queue_[write_] = v;
            write_ = (++write_)%size_;
        }
        else cout<<"isFull, can not push data!"<<endl;
    }

    int front()
    {
        int result = 99999;
        if(!isEmpty())
        {
            result = queue_[read_];
        }
        else cout<<"isEmpty, can not read top data!"<<endl;
        return result;
    }

    void pop()
    {
        if(!isEmpty())
        {
            read_ = (++read_)%size_;
        }
        else cout<<"isEmpty, can not pop data!"<<endl;
    }

private:
    int queue_[10];
    int size_;
    int read_;
    int write_;
};

void TRUE(bool b)
{
    if(b) cout<<"True"<<endl;
    else cout<<"False"<<endl;
}

int main()
{
    CircleQueue q(10);
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
    cout<<"length: "<<q.length()<<endl;
    for(int i=0;i<14;i++)
    {
        q.push_back(14-i);
        cout<<"length: "<<q.length()<<endl;
    }
    TRUE(q.isFull());
    cout<<q.front()<<endl;
    return 0;
}

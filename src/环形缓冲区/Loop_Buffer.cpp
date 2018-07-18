/*
 * 环形缓冲区
 * 主要应用于生产者-消费者场景中，生产者往缓冲区中添加数据，消费者从缓冲区拿数据
 * 缓冲区有一个读指针 和 一个写指针
 */

#include <iostream>
#include <mutex>
#include <memory>

using namespace std;

/*
 * 循环链表节点定义
 */
template <typename T>
class list_node{
public:
    T data;  //节点数据域
    std::shared_ptr<list_node> next;  //指向下一个节点的智能指针
};

/*
 * 循环链表类定义
 */
template <typename T>
class LoopList{
public:
    LoopList(int s):size(s),head(new list_node<T>())
    {
        current = head;
        for(int i=1;i<s;i++)
        {
            std::shared_ptr<list_node<T>> newNode = std::make_shared<list_node<T>>();
            current->next = newNode;
            current = current->next;
        }
        current->next = head;

    }
//private:
    std::shared_ptr< list_node<T> > head; //循环链表头结点
    std::shared_ptr< list_node<T> > current; //循环链表头结点
    int size;   //循环链表容量
};



/*
 * 环形缓冲区类定义
 */
template <typename T>
class LoopBuffer{
/*成员函数*/
public:
    LoopBuffer():buffer(100),size(100){pR = buffer.head; pW = buffer.head; free=size; }     //默认构造函数
    LoopBuffer(int s):buffer(s),size(s){pR = buffer.head; pW = buffer.head; free=size; }  //带参构造函数
    int readData(T* buf, int len); //读取数据
    int writeData(T* buf, int len); //写入数据
/*公有类成员*/
public:
    int size;   //缓冲区总长度
    int free;   //缓冲区空闲长度
/*私有类成员*/
protected:
    LoopList<T> buffer;  //循环链表作为缓冲区
    std::shared_ptr< list_node<T> > pR;  //读指针
    std::shared_ptr< list_node<T> > pW;  //写指针
    std::mutex mu;   //互斥锁
};

template <typename T>
int LoopBuffer<T>::readData(T* buf, int len)
{
    int index = 0;
    mu.lock();   //进入临界区加锁
    if(free==size || len==0) return 0;
    else
    {
        while(free < size && index<=len)
        {
            *(buf+index) = pR->data;
            pR = pR->next;
            free++ ;
            index ++;
        }
        return index;
    }
    mu.unlock(); //退出临界区解锁
}

template <typename T>
int LoopBuffer<T>::writeData(T* buf, int len)
{
    int index = 0;
    mu.lock(); //进入临界区加锁
    if(free==0) return 0;
    else
    {
        while(free>0 && index<=len)
        {
            pW->data = *(buf+index);
            pW = pW->next;
            index++;
            free--;
        }
        return index;
    }
    mu.unlock(); //退出临界区解锁
}

int main()
{
    LoopBuffer<int> lb(10);
    int a[100],b[100];
    for(int i=0;i<100;i++)
    {
        a[i] = i;
        b[i] = 0;
    }


    cout<<"写数据："<<lb.writeData(a,100)<<endl;
    cout<<"空间剩余长度："<<lb.free<<endl;
    cout<<"读数据："<<lb.readData(b,100)<<endl;
    cout<<"读取的数据为："<<endl;
    for(int i=0;i<10;i++)
        cout<<b[i]<<" ";

    return 0;
}
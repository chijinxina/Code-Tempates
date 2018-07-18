//
// Created by chijinxin on 18-7-18.
//
#include <iostream>
#include <atomic>
using namespace std;
/*
 * 实现基于引用计数的智能指针
 */
template <class T>
class Smart_ptr{
public:
    //构造函数
    Smart_ptr(T* p) : ptr(p)
    {
        try
        {
            use_count = new std::atomic_int(1);
            cout<<"Memory is Allocated!"<<endl;
        }
        catch (...)
        {
            delete ptr;
            ptr = nullptr;
            use_count = nullptr;
            cerr<<"Allocate Memory for use_count failed！"<<endl;
            exit(1);
        }
    }
    //析构函数
    ~Smart_ptr()
    {
        //只有在最后一个对象引用析构时，ptr才会释放内存
        if(--(*use_count) == 0)
        {
            delete ptr;         //释放内存
            delete use_count;   //释放引用计数器
            ptr = nullptr;
            use_count = nullptr;
            cout<<"Free Memory!"<<endl;
        }
    }
    //拷贝构造函数
    Smart_ptr(const Smart_ptr<T> &origin) {               //浅拷贝
        ptr = origin.ptr;
        use_count = origin.use_count;
        ++(*use_count);  //引用计数加1
    }

    /*
     * 赋值构造函数
     * 赋值构造函数不同于拷贝构造函数，如果=左边对象已经指向了某块内存，这样我们得先判断左边对象指向内存已经被引用的次数。
     * 如果左边对象指向内存的引用次数为1，那么我们可以释放这块内存；反之不释放，由其他对象来释放这块内存
     */
    Smart_ptr<T>& operator= (const Smart_ptr<T> &rhs) {    //深拷贝

        //赋值运算符在减少左边对象的引用计数之前，应先使rhs的引用计数加1，从而防止自身赋值导致内存提前释放
        ++(*rhs.use_count);
        //将左边对象的引用计数减1，如果引用计数小于1则释放该对象托管的内存
        if(--(*this->use_count) < 1)
        {
            delete ptr;
            delete use_count;
            cout<<"Left Object Memory is deleted!"<<endl;
        }
        this->ptr = rhs.ptr;
        this->use_count = rhs.use_count;
        return *this;
    }

public:
    T* ptr;
    std::atomic_int* use_count;   //引用计数器（线程安全）声明为指针，为了方便对其进行递增和递减操作
};


/*
 * demo
 */

int main()
{
    Smart_ptr<int> p1(new int(0));
    cout<<"use_count="<<*p1.use_count<<endl;
    p1 = p1;
    cout<<"use_count="<<*p1.use_count<<endl;
    Smart_ptr<int> p2(p1);
    cout<<"use_count="<<*p1.use_count<<endl;
    Smart_ptr<int> p3(new int(1));
    cout<<"use_count="<<*p1.use_count<<endl;
    p3=p1;
    cout<<"use_count="<<*p1.use_count<<endl;
    return 0;
}

//
// Created by chijinxin on 18-8-5.
//
#include <iostream>
#include <algorithm>
using namespace std;
/*
 * 实现unique_ptr
 *
 */
template <class T>
class Unique_ptr {
public:
    //构造函数
    explicit Unique_ptr(T *pp = nullptr): un_ptr(pp){}
    ~Unique_ptr()
    {
        delete (un_ptr);
        un_ptr = nullptr; //避免空悬指针
    }
private:
    //托管的内存指针
    T* un_ptr;

public:
    //不允许拷贝和赋值操作
    Unique_ptr(const Unique_ptr&) = delete;
    Unique_ptr& operator=(const Unique_ptr&) = delete;

    /*可以进行 移动拷贝 和 移动赋值*/
    //移动构造函数
    Unique_ptr(Unique_ptr &&rhs) : un_ptr(rhs.un_ptr)
    {
        rhs.un_ptr = nullptr;
    }
    //移动赋值函数
    Unique_ptr& operator=(Unique_ptr &&rhs)
    {
        if(this != &rhs)
        {
            un_ptr = rhs.un_ptr;
            rhs.un_ptr = nullptr;
        }
        return *this;
    }

    //realease() 放弃对内存区域的控制权，返回指针 并将un_ptr置空
    T* release()
    {
        T* temp = un_ptr;
        un_ptr = nullptr;
        return temp;
    }
    //reset() 释放u指向的对象
    //reset(q) 将当前unique_ptr对象的控制指针重置为q
    //reset(nullptr) 将un_ptr置空
    void reset() {delete(un_ptr);}
    void reset(T* q)
    {
        if(un_ptr)
        {
            delete(q);
            un_ptr = q;
        }
        else
        {
            un_ptr = q;
        }
    }
    //swap()
    void swap(Unique_ptr &other)
    {
        std::swap(un_ptr, other.un_ptr);
    }
    //get()
    T* get() {return un_ptr;}
    //*()
    T& operator*(){return *un_ptr;}
    //->()
    T* operator->(){ return un_ptr;}
};


int main()
{
    Unique_ptr<std::string> p1(new std::string("chijinxin"));
    std::cout << *p1 << std::endl;
    {
        Unique_ptr<int> fp1;
        Unique_ptr<int> fp2(new int(4));
        std::cout << "before move " << fp2.get() << std::endl;
        fp1 = std::move(fp2);
        std::cout << "after move " << fp1.get() << std::endl;
        Unique_ptr<int> fp3(std::move(fp1));
        std::cout << "after move " << fp3.get() << std::endl;
    }
    return 0;
}


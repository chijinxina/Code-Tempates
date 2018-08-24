//
// Created by chijinxin on 18-8-23.
//
#include <iostream>
#include <vector>

using namespace std;
/*
 * 在 C++ 中，traits 习惯上总是被实现为 struct ，但它们往往被称为 traits classes。Traits classes 的作用主要是用来为使用者提供类型信息。
 *  traits 使用的关键技术 —— 模板的特化与偏特化。
 */


//函数模板的通用定义：
template <typename T>
struct my_is_void{
    static const bool value = false;
};
//针对 void 类型，有以以下的特化版本：
template <>
struct my_is_void<void>
{
    static const bool value = true;
};

/*
 * 偏特化（Patial Spcialization）
 * 模板特化时，可以只指定一部分而非所有模板参数，或者是参数的一部分而非全部特性，这叫做模板的偏特化。
 * 一个类模板的偏特化本身是一个模板，使用它时用户还必须为那些在特例化版本中未指定的模板参数提供实参。
 */
//通用的原始模板：
template <typename T>
struct my_is_pointer{
    static const bool value = false;
};
//对模板参数T进行限制，要求其为一个指针的类型：
template <typename T>
struct my_is_pointer<T*>
{
    static const bool value = true;
};


/*
 * C++ 并不总是把 class 和 typename 视为等价。有时候我们一定得使用 typename。
 * 默认情况下，C++ 语言假定通过作用域运算符访问的名字不是类型。
 * 因此，如果我们希望使用一个模板类型参数的类型成员，就必须显式告诉编译器该名字是一个类型。
 * 我们通过使用关键字 typename 来实现这一点：
 * 当我们希望通知编译器一个名字表示类型时，必须使用关键字 typename，而不能使用 class。
 */
template <typename T>
typename T::value_type top(const T &c)
{
    if(!c.empty())
    {
        return c.back();
    }
    else
    {
        return typename T::value_type();
    }
}


/*
 * 实现 Traits Classes
 * 算法如何从迭代器类中萃取出容器元素的类型 -> 使用traits classes实现
 * 迭代器所指对象的类型，称为该迭代器的value_type
 * template <class T, ...>
    class vector {
    public:
        class iterator {
        public:
            typedef T value_type;
            ...
        };
    ...
    };
 */
template <class IterT>
struct my_iterator_traits{
    typedef typename IterT::value_type value_type;
};
//对于my_iterator_traits，我们再声明一个偏特化版本：
//如果 my_iterator_traits 的实参为指针类型时，直接使用指针所指元素类型作为 value_type
template <class IterT>
struct my_iterator_traits<IterT*> {
    typedef IterT value_type;
};
void fun(int a)
{
    cout << "fun(int) is called" << endl;
}
void fun(double a)
{
    cout << "fun(double) is called" << endl;
}
void fun(char a)
{
    cout << "fun(char) is called" << endl;
}

int main()
{
    cout<<"test1:"<<endl;
    my_is_void<bool> t1;
    cout << t1.value << endl;  // 输出0
    my_is_void<void> t2;
    cout << t2.value << endl;  // 输出1
    cout<<"test2:"<<endl;
    my_is_pointer<int> p1;
    cout << p1.value << endl;  // 输出 0，使用原始模板
    my_is_pointer<int*> p2;
    cout << p2.value << endl;  // 输出 1，使偏特化模板，因为指定了 int * 类型的参数
    cout<<"test3:"<<endl;
    vector<int> vec;
//    vec.push_back(1);
//    vec.push_back(2);
//    vec.push_back(3);
    cout << top< vector<int> >(vec) << endl;  // 输出3

    cout<<"test my_iterator_traits:"<<endl;
    my_iterator_traits<vector<int>::iterator>::value_type a;
    fun(a);  // 输出 fun(int) is called
    my_iterator_traits<vector<double>::iterator>::value_type b;
    fun(b);  // 输出 fun(double) is called
    my_iterator_traits<char*>::value_type c;
    fun(c);  // 输出 fun(char) is called

    return 0;
}

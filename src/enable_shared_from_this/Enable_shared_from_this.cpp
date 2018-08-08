//
// Created by chijinxin on 18-8-8.
//
#include <iostream>
#include <memory>

using namespace std;

/*
 * 错误的示例 将this托管给shared_ptr
 */
class Bad {
public:
    std::shared_ptr<Bad> getPtr()
    {
        return std::shared_ptr<Bad>(this);   //错误！不要把this指针给shared_ptr托管，类似二龙治水，导致this二次析构
    }
    ~Bad()
    {
        cout<<"~Bad()"<<endl;
    }
};

/*
 * 正确的示例 利用enable_shared_from_this
 */
class Good : public std::enable_shared_from_this<Good> {
public:
    std::shared_ptr<Good> getPtr()
    {
        return shared_from_this();   //正确！ 共享this 让一个对象安全地生成其它额外shared_ptr的实例
    }

    ~Good()
    {
        cout<<"~Good()"<<endl;
    }
};


int main()
{
    /* 错误的示例 将this托管给shared_ptr*/
    //每个shared_ptr都认为自己是对象的仅有持有者，引用计数器私有 造成多次析构
    {
        std::shared_ptr<Bad> bp1(new Bad);
        std::shared_ptr<Bad> bp2 = bp1->getPtr();
        cout<<"bp1 use_count="<<bp1.use_count()<<endl;
        cout<<"bp2 use_count="<<bp2.use_count()<<endl;
    }

    /* 正确的示例 enable_shared_from_this*/
    //多个shared_ptr共享this 不会造成多次析构
    {
        std::shared_ptr<Good> bp1(new Good);
        std::shared_ptr<Good> bp2 = bp1->getPtr();
        cout<<"bp1 use_count="<<bp1.use_count()<<endl;
        cout<<"bp2 use_count="<<bp2.use_count()<<endl;
    }

    return 0;
}

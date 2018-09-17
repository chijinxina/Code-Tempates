//
// Created by chijinxin on 18-8-28.
//
#include <iostream>
#include <vector>
#include <memory>
#include <mutex>
#include <assert.h>
using namespace std;
/*
 * C++11 借助多参数模板 实现一对多回调（信号槽机制）
 */

template <typename Callback>
class SlotImpl;


template <typename Callback>
class SignalImpl {
public:
    //不允许拷贝和赋值
    SignalImpl(const SignalImpl& rhs) = delete;
    SignalImpl& operator=(const SignalImpl& rhs) = delete;

    typedef std::vector< std::weak_ptr<SlotImpl<Callback>> > SlotList;
    //构造函数
    SignalImpl():slots_(new SlotList){}

    void copyOnWrite()
    {
        std::lock_guard<std::mutex> lc(mu_);
        if(!slots_.unique())
        {
            slots_.reset(new SlotList(*slots_));
        }
        assert(slots_.unique());
    }

    void clean()
    {
        std::lock_guard<std::mutex> lc(mu_);
        copyOnWrite();
        SlotList& list(*slots_);
        typename SlotList::iterator iter(list.begin());
        while(iter != list.end())
        {
            if(iter->expired())
            {
                iter = list.erase(iter);
            }
            else ++iter;
        }
    }
private:
    std::shared_ptr<SlotList> slots_; //槽列表
    std::mutex mu_;                   //互斥量
};


template<typename Callback>
class SlotImpl {
public:
    //不允许拷贝和赋值
    SlotImpl(const SlotImpl& rhs) = delete;
    SlotImpl& operator=(const SlotImpl& rhs) = delete;

    typedef SignalImpl<Callback> Data;
    //构造函数
    SlotImpl(const std::shared_ptr<Data> &data, Callback&& cb, const std::shared_ptr<void> &tie)
            :data_(data), cb_(cb), tie_(tie), tied_(true)
    { }

    //析构函数
    ~SlotImpl()
    {
        std::shared_ptr<Data> data(data_.lock());   //弱指针提升为强指针shared_ptr
        if(data)  //提升成功，说明对象存在
        {
            data->clean();
        }
    }

private:
    std::weak_ptr<Data> data_;
    Callback cb_;
    std::weak_ptr<void> tie_;
    bool tied_;
};
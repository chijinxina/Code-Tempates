//
// Created by chijinxin on 18-8-30.
//
#include <iostream>
#include <functional>
#include <atomic>
#include <thread>
#include <assert.h>
#include <sys/poll.h>
#include <sstream>
#include "unistd.h"

using namespace std;

class EventLoop;
__thread EventLoop* t_loopInThisThread = 0;  //线程局部存储变量（Thread Local Storage， TLS)

class EventLoop{
public:
    EventLoop():looping_(false)
    {
        stringstream ss;
        ss<<this_thread::get_id();
        ss>>threadId_;
        cout<<"EventLoop created "<<this<<" in thread id="<<threadId_<<endl;
        if(t_loopInThisThread) cout<<"Another EventLoop "<<t_loopInThisThread<<" exists in this thread id="<<threadId_<<endl;
        else t_loopInThisThread = this;
    }

    ~EventLoop()
    {
        t_loopInThisThread = NULL;
    }

    bool isInLoopThread() const
    {
        stringstream ss;
        int temp;
        ss<<this_thread::get_id();
        ss>>temp;
        return threadId_ == temp;
    }

    void loop()
    {
        assert(!looping_);
        assert(isInLoopThread());
        looping_ = true;
        ::poll(NULL, 0, 5*1000);
        cout<<"EventLoop "<<this<<" stop looping"<<endl;
        looping_ = false;
    }

    void updateChannel()

    static EventLoop* getEventLoopOfCurrentThread()
    {
        return t_loopInThisThread;
    }

private:
    std::atomic_bool looping_;   //运行中标志
    int threadId_;               //线程id
};

/*
 * 每个channel自始至终只属于一个Eventloop，即每个channel对象只属于某一个IO线程
 * 每个channel对象自始至终只负责一个文件描述符的IO事件分发
 */
class Channel{
public:
    typedef std::function<void()> EventCallback;
    //构造函数
    Channel(EventLoop* loop, int fd)
            :loop_(loop),
             fd_(fd),
             events_(0),
             revents_(0),
             index_(-1)
    {
    }

    void handleEvent()
    {

    }

    void setReadCallback(const EventCallback &cb)
    {

    }

    void setWriteCallback(const EventCallback &cb)
    {

    }

    void setErrorCallback(const EventCallback &cb)
    {

    }

private:
    void update()
    {

    }

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop* loop_;
    const int fd_;
    int events_;
    int revents_;
    int index_;   //used by Poller

    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;

};
const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;


void threadFunc()
{
    cout<<"threadFunc run in thread id="<<this_thread::get_id()<<endl;
    EventLoop loop;
    loop.loop();
}

int main()
{
    cout<<"main run in thread id="<<this_thread::get_id()<<endl;
    EventLoop loop;
    std::thread thread1(threadFunc);
    loop.loop();
    thread1.join();
    return 0;
}
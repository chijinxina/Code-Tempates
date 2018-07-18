//
// Created by chijinxin on 18-4-25.
//
#include <iostream>
#include <list>
#include <vector>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <thread>

using namespace std;

/*
 * C++11 信号量实现
 */
template <typename Mutex, typename CondVar>
class basic_semaphore {
public:
    using native_handle_type = typename CondVar::native_handle_type;

    explicit basic_semaphore(size_t count = 0);  //信号量构造函数（资源个数）
    basic_semaphore(const basic_semaphore& ) = delete;
    basic_semaphore(basic_semaphore&& ) = delete;
    basic_semaphore& operator = (const basic_semaphore&) = delete;
    basic_semaphore& operator = (basic_semaphore&&) = delete;

    void notify();  //执行通知操作 V操作
    void wait();    //等待资源操作 P操作
    bool try_wait();
    template<class Rep, class Period>
    bool wait_for(const std::chrono::duration<Rep, Period>& d);
    template<class Clock, class Duration>
    bool wait_until(const std::chrono::time_point<Clock, Duration>& t);

    native_handle_type native_handle();
    size_t  mCount;  //信号量计数
private:
    Mutex   mMutex;  //互斥锁
    CondVar mCv;     //条件变量

};

template <typename Mutex, typename CondVar>
basic_semaphore<Mutex, CondVar>::basic_semaphore(size_t count)
        : mCount{count}
{}

template <typename Mutex, typename CondVar>
void basic_semaphore<Mutex, CondVar>::notify() {
    std::lock_guard<Mutex> lock{mMutex};
    ++mCount;
    mCv.notify_one();
}

template <typename Mutex, typename CondVar>
void basic_semaphore<Mutex, CondVar>::wait() {
    std::unique_lock<Mutex> lock{mMutex};
    mCv.wait(lock, [&]{ return mCount > 0; });
    --mCount;
}

template <typename Mutex, typename CondVar>
bool basic_semaphore<Mutex, CondVar>::try_wait() {
    std::lock_guard<Mutex> lock{mMutex};

    if (mCount > 0) {
        --mCount;
        return true;
    }

    return false;
}

template <typename Mutex, typename CondVar>
template<class Rep, class Period>
bool basic_semaphore<Mutex, CondVar>::wait_for(const std::chrono::duration<Rep, Period>& d) {
    std::unique_lock<Mutex> lock{mMutex};
    auto finished = mCv.wait_for(lock, d, [&]{ return mCount > 0; });

    if (finished)
        --mCount;

    return finished;
}

template <typename Mutex, typename CondVar>
template<class Clock, class Duration>
bool basic_semaphore<Mutex, CondVar>::wait_until(const std::chrono::time_point<Clock, Duration>& t) {
    std::unique_lock<Mutex> lock{mMutex};
    auto finished = mCv.wait_until(lock, t, [&]{ return mCount > 0; });

    if (finished)
        --mCount;

    return finished;
}

template <typename Mutex, typename CondVar>
typename basic_semaphore<Mutex, CondVar>::native_handle_type basic_semaphore<Mutex, CondVar>::native_handle() {
    return mCv.native_handle();
}



using semaphore = basic_semaphore<std::mutex, std::condition_variable>;


/*
 * 环形缓冲区
 */
template <class T>
class list_node{
public:
    T data;
    shared_ptr<list_node> next;
};

template <class T>
class LoopBuffer{
public:
    LoopBuffer(int c):buffer(new list_node<T>),total(c),free(c)
    {
        std::shared_ptr<list_node<T>> current = buffer;
        for(int i=1;i<c;i++)
        {
            std::shared_ptr<list_node<T>> tmp(new list_node<T>);
            current->next = tmp;
            current = tmp;
        }
        current->next = buffer;
        pRead = buffer;
        pWrite= buffer;
    }

    int total; //全部空间
    int free;  //空闲空间
    std::shared_ptr< list_node<T> > pRead;  //读指针
    std::shared_ptr< list_node<T> > pWrite; //写指针
    std::shared_ptr< list_node<T> > buffer; //缓冲区
};

/*
 * 生产者-消费者模型
 */
class ProductorConsumer {
public:
    ProductorConsumer(int p, int c, int b)   //p:生产者个数 c:消费者个数 b:环形缓冲区长度
        :pLoopBuffer( new LoopBuffer<int>(b) ), //创建b个元素长度的环形缓冲区
        sem_empty(b),sem_full(0)
    {
        /*
         * 产生p个生产者线程
         */
        for(int i=1;i<=p;i++)
        {
            vProductorThread.push_back(std::make_shared<std::thread>(std::bind(&ProductorConsumer::ProductorFunc,this,i)));
        }
        /*
         * 产生c个消费者线程
         */
        for(int j=1;j<=c;j++)
        {
            vConsumerThread.push_back(std::make_shared<std::thread>(std::bind(&ProductorConsumer::ConsumerFunc,this,j)));
        }
    }

    void ProductorFunc(int x); //生产者功能函数
    void ConsumerFunc(int y);  //消费者功能函数


private:
    std::vector< std::shared_ptr<std::thread> > vProductorThread;   //生产者线程集合
    std::vector< std::shared_ptr<std::thread> > vConsumerThread;    //消费者线程集合
    std::shared_ptr< LoopBuffer<int> > pLoopBuffer;   //环形缓冲区指针
    /*
     * 一组生产者和一组消费者之间公用n个元素长度的环形缓冲区
     * 4个信号量，执行PV操作
     * 先执行同步信号量的P操作，然后执行互斥信号量的P操作
     */
    semaphore sem_empty;    //表示缓冲区是否为空，初值为缓冲区长度
    semaphore sem_full;     //表示缓冲区是否为满，初值为0
    std::mutex mP;          //生产者线程之间的互斥锁
    std::mutex mC;          //消费者线程之间的互斥锁
};

/*
 * 生产者线程函数
 */
void ProductorConsumer::ProductorFunc(int x)
{
    while(1)
    {
        sem_empty.wait();           //1. P(sem_empty)

        std::unique_lock<std::mutex> lock(mP);  //2. P(mP)
        /*
         * 生产数据到缓冲区
         */
        pLoopBuffer->pWrite->data = x;
        pLoopBuffer->pWrite = pLoopBuffer->pWrite->next; //写指针后移
       // pLoopBuffer->free --;
        cerr<<"生产者["<<x<<"] 生产数据"<<x;

        lock.unlock();             //3. V(mP)
        sem_full.notify();         //4. V(sem_full)
        cerr<<"   缓冲区空闲空间="<<10-sem_full.mCount<<endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}
/*
 * 消费者线程函数
 */
void ProductorConsumer::ConsumerFunc(int y)
{
    while(1)
    {
        sem_full.wait();           //1. P(sem_full)

        std::unique_lock<std::mutex> lock(mC); //2. P(mC)
        /*
         * 从缓冲区拿数据
         */
        cout<<"消费者["<<y<<"] 取出数据"<<pLoopBuffer->pRead->data;
        pLoopBuffer->pRead = pLoopBuffer->pRead->next; //读指针右移
        pLoopBuffer->free ++;
        lock.unlock();             //3. V(mP)
        sem_empty.notify();        //4. V(sem_empty)
        cout<<"   缓冲区空闲空间="<<sem_empty.mCount<<endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}


int main()
{
    ProductorConsumer pc(2,1,10);
    while(1)
    {
        cout<<"main process!"<<endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000000));
    }
    return 0;
}
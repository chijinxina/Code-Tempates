//
// Created by chijinxin on 18-4-25.
//
#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std;

/*
 * 用互斥锁和条件变量实现 信号量 Semaphore
 */

class Semaphore
{
public:
    Semaphore(int c) //构造函数
            :count(c){}

    inline void notify()
    {
        std::unique_lock<std::mutex> lock(mtx);
        count ++ ;        //信号量 + 1
        cv.notify_one();  //唤醒一个等待线程
    }

    inline void wait()
    {
        std::unique_lock<std::mutex> lock(mtx);
        while(count == 0)
        {
            cv.wait(lock);
        }
        count --;  //信号量 - 1
    }
private:
    std::mutex mtx; //互斥锁
    std::condition_variable cv; //条件变量
    int count;  //信号量资源计数
};




int main()
{
    return 0;
}

//
// Created by chijinxin on 18-7-31.
//
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class OddAndEven{
public:
    OddAndEven(long n):N_(n),num_(0),
                       oddThread_(std::bind(&OddAndEven::OddFunc,this,N_)),
                       evenThread_(std::bind(&OddAndEven::EvenFunc,this,N_))
    { }

    void OddFunc(long N)
    {
        std::unique_lock<std::mutex> lock(mu_);
        while(num_ <= N_)
        {
            cv_.wait(lock,
                     [this]()
                     {
                         if(this->num_%2!=0) return true;
                         else return false;
                     });
            cout<<"Odd  Thread Print: "<<num_<<endl;
            ++num_;
            cv_.notify_one();
        }
    }

    void EvenFunc(long N)
    {
        std::unique_lock<std::mutex> lock(mu_);
        while(num_ <= N_)
        {
            cv_.wait(lock,
                     [this]()
                     {
                         if(this->num_%2==0) return true;
                         else return false;
                     });
            cout<<"Even Thread Print: "<<num_<<endl;
            ++num_;
            cv_.notify_one();
        }
    }

    void join()
    {
        if(oddThread_.joinable())
        {
            oddThread_.join();
        }
        if(evenThread_.joinable())
        {
            evenThread_.join();
        }
    }

private:
    long N_;
    long num_;
    std::mutex mu_;
    std::condition_variable cv_;
    std::thread oddThread_;
    std::thread evenThread_;
};
int main()
{
    OddAndEven chijinxin(1000);
    chijinxin.join();
    return 0;
}

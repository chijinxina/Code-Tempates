//
// Created by chijinxin on 18-8-16.
//
#include <iostream>
#include <new>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <thread>

using namespace std;

#define TEXT_SIZE 1024

/*
 * 生产者任务和消费者任务之间通过共享内存机制实现跨进程的共享缓冲池；
 * 在信号量集合中支持一个信号量（或利用一个POSIX信号量），实现对共享缓冲池的互斥访问；
 * 缓冲区的分配计数通过修改缓冲池结构中的计数变量来实现。　
 */


/*跨进程缓冲区 -  共享内存*/
class Shared_Buffer{
public:
    Shared_Buffer():size_(10),read_(0),write_(0)
    {
        //初始化信号量
        sem_init(&sem_empty_, 1, 10);    //空信号量
        sem_init(&sem_full_, 1, 0);      //满信号量
        cout<<"Construct the Shared_Buffer"<<endl;
    }
    //判空
    bool isEmpty()
    {
        //判空 read == write
        if( read_ == write_ ) return true;
        else return false;
    }
    //判满
    bool isFull()
    {
        //判满 (read+1)%size == write
        if( (write_ + 1)%size_ ==  read_) return true;
        else return false;
    }
    //写数据
    bool put(string str)
    {
        //P(empty)操作  sem_wait()
        if(sem_wait(&sem_empty_) == -1)
        {
            cerr<<"sem_wait error!"<<endl;
            return false;
        }
        if(isFull())
        {
            cerr<<"put error! Buffer is full"<<endl;
            return false;
        }
        strcpy(Buffer_[write_], str.c_str());
        write_ = (++write_)%size_;
        //V(full)操作 sem_post()
        sem_post( &sem_full_ );
        return true;
    }
    //读数据
    string get()
    {
        //P(full)操作  sem_wait()
        if(sem_wait(&sem_full_) == -1)
        {
            cerr<<"sem_wait error!"<<endl;
            return false;
        }
        if(isEmpty())
        {
            cerr<<"get error! Buffer is empty"<<endl;
            return false;
        }
        string result(Buffer_[read_]);
        read_ = (++read_)%size_;
        //V(empty)操作 sem_post()
        sem_post( &sem_empty_ );
        return result;
    }
private:
    sem_t sem_empty_;           //空信号量 进程间同步
    sem_t sem_full_;            //满信号量 进程间同步
    char Buffer_[10][TEXT_SIZE];//缓冲区
    int size_;                  //缓冲区大小
    int read_;                  //读指针
    int write_;                 //写指针
};

/*生产者类*/
class Producer{
public:
    Producer():running(true),count(0)
    {
        key_t ipc_key = ftok("/home/chijinxin/CLionProjects/Code-Tempates/src/共享内存_生产者消费者模型",0x21);
        shmid = shmget(ipc_key, sizeof(Shared_Buffer), 0666 | IPC_CREAT);
        cout<<"Shared_Buffer size= "<< sizeof(Shared_Buffer)<<endl;
        if(shmid == -1)
        {
            cerr<<"create shm failed! shmget error!"<<endl;
        }
        shm = shmat(shmid, NULL, 0);    //将共享内存连接到一个进程的虚拟地址空间
        if(shm == NULL)
        {
            cerr<<"shmat error! shm is NULL!"<<endl;
        }
        cout<<"Memory attached at "<<(intptr_t)shm<<endl;

        pshared = new(shm)Shared_Buffer; //在已存在的内存（共享内存）之上构造一个对象
        cout<<"pshared addr: "<<(intptr_t)pshared<<endl;
    }

    void run()
    {
        while(running)
        {
            count++;
            string str = "string of buffer: "+to_string(count);
            if(pshared->put(str))
            {
                cout<<"put "<<str<<" success!!!"<<endl;
            }
            else
            {
                cerr<<str<<" failed!!!"<<endl;
            }
           // this_thread::sleep_for(chrono::milliseconds(10));
        }
        //流程结束将共享内存从进程地址空间剥离
        if( shmdt(shm) == -1 )
        {
            cerr<<"shmdt error！"<<endl;
        }

        /*查看及修改共享内存段的shmid_ds结构，删除该结构以及相连的共享存储段标识
        struct shmid_ds
        {
            uid_t shm_perm.uid;
            uid_t shm_perm.gid;
            mode_t shm_perm.mode;
        };*/
        if(shmctl(shmid, IPC_RMID, 0) == -1)        //失败
        {
            cerr<<"shmctl(shmid, IPC_RMID, 0) error!!!"<<endl;
        }
    }

    void stop()
    {
        running = false;
    }

private:
    bool running;             //运行标志
    int shmid;                //共享内存标识符
    void* shm;                //共享存储段连接的实际地址
    Shared_Buffer* pshared;   //共享存储段连接的实际地址
    int count;                //计数器
};


int main()
{
    Producer p;
    p.run();
    return 0;
}
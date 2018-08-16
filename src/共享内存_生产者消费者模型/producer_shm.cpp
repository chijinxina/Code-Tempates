//
// Created by chijinxin on 18-8-16.
//
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
using namespace std;

#define TEXT_SIZE 1024

/*
 * 生产者任务和消费者任务之间通过共享内存机制实现跨进程的共享缓冲池；
 * 在信号量集合中支持一个信号量（或利用一个POSIX信号量），实现对共享缓冲池的互斥访问；
 * 缓冲区的分配计数通过修改缓冲池结构中的计数变量来实现。　
 */


/*跨进程缓冲区*/
struct shared_cache{
    int bitmap[5];          //为0表示对应的缓冲区未被生产者使用，可分配但不可消费；为1表示对应的缓冲区可以被生产者使用，不可分配但可消费；
    char Buffer[5][TEXT_SIZE];  //5个字符串缓冲区
    sem_t sem_;             //信号量 进程间同步
};


int main()
{
    bool running = true;
    void* shm = NULL;   //共享存储段连接的实际地址
    shared_cache *pshared = NULL;

    int shmid;          //共享内存标识符
    key_t ipc_key = ftok("/home/chijinxin/CLionProjects/Code-Tempates/src/共享内存_生产者消费者模型",0x21);
    shmid = shmget(ipc_key, sizeof(shared_cache), 0666 | IPC_CREAT);

    if(shmid == -1)
    {
        cerr<<"create shm failed! shmget error!"<<endl;
        return -1;
    }

    shm = shmat(shmid, NULL, 0); //将共享内存连接到一个进程的虚拟地址空间

    if(shm == NULL)
    {
        cerr<<"shmat error! shm is NULL!"<<endl;
        return -1;
    }
    cout<<"Memory attached at "<<(intptr_t)shm<<endl;

    pshared = static_cast<shared_cache*>(shm);  //将缓冲区作为共享内存存储段连接地址

    for(int i=0; i<5; ++i)
    {
        pshared->bitmap[i] = 0;   //对缓冲区进行初始化， 0表示可以生产
    }

    sem_init(&(pshared->sem_), 1, 1); //信号量初始化，初始值为1，表示可以生产

    int i = 0;
    while(running)
    {
        //P()操作  sem_wait()
        if(sem_wait( &(pshared->sem_) ) == -1)
        {
            cerr<<"sem_wait error!"<<endl;
            return -1;
        }
        //寻找空闲的缓冲区
        for(i=0; i<5 && pshared->bitmap[i]==1; i++);

        if(i==5)        //表示所有共享内存缓冲区全被消费者占用
        {
            //V()操作 sem_post()
            sem_post( &(pshared->sem_) );
            sleep(1);
            cout<<"the buffer is full, waiting for some time"<<endl;
        }
        else
        {
            //V()操作 sem_post()
            sem_post( &(pshared->sem_) );
            pshared->bitmap[i%5] = 1;   //表示该缓冲区被生产者使用了
            cout<<"Input some text to buffer:"<<endl;
            cin>>pshared->Buffer[i%5];  //输入字符串
            if( strcmp(pshared->Buffer[i%5],"end") == 0 )
            {
                //输入end 结束流程
                running = false;
            }
        }
    }

    //流程结束将共享内存从进程地址空间剥离
    if( shmdt(shm) == -1 )
    {
        cerr<<"shmdt error！"<<endl;
        return -1;
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
        exit(EXIT_FAILURE);
    }

    return 0;
}
//
// Created by chijinxin on 18-8-16.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#define TEXT_SZ 1024

//缓冲池结构
struct shared_use_st
{
    int Index[5];    //5个缓冲池,为0表示对应的缓冲区未被生产者使用，可分配但不可消费；为1表示对应的缓冲区被生产者使用，不可分配但可消费
    char Buffer[5][TEXT_SZ];    //5个字符串缓冲区
    sem_t sem;        //信号量，同步功能
};

int main()
{
    int running = 1;
    int i = 0;
    void *shm = NULL;        //共享存储段连接的实际地址
    shared_use_st *shared = NULL;    //缓冲池
    int shmid;        //声明共享内存标识符
    key_t ipc_key = ftok("/home/chijinxin/CLionProjects/Code-Tempates/src/共享内存_生产者消费者模型",0x21);
    shmid = shmget(ipc_key, sizeof(struct shared_use_st), 0666|IPC_CREAT); //获得或创建一个共享内存标识符
    if(shmid == -1)        //获取或创建一个共享内存标识符失败
    {
        exit(EXIT_FAILURE);
    }

    //将共享内存段连接到一个进程的地址空间中，返回void *指针
    shm = shmat(shmid, 0, 0);    //返回共享存储段连接的实际地址
    if(shm == (void*)-1)        //失败
    {
        exit(EXIT_FAILURE);
    }
    printf("Memory attached at %ld\n", (intptr_t)shm);
    shared = (struct shared_use_st*)shm;        //缓冲池为共享存储段连接地址
    while(running)
    {
        if(sem_wait(&(shared->sem)) == -1)        //sem_wait为P操作，减少信号量的值
        {
            printf("P操作 ERROR!\n");
            exit(EXIT_FAILURE);
        }
        for(i = 0; i < 5 && shared->Index[i] == 0; i++)
            ;
        //五个缓冲区没有都被生产者占用
        if(i != 5)
        {
            printf("You wrote: %s\n", shared->Buffer[i%5]);        //打印出生产者写入的字符
            shared->Index[i%5] = 0;        //为0时，表示已被消费者使用
            sem_post(&shared->sem);        //sem_post为V操作
            sleep(1);
            if( strncmp(shared->Buffer[i%5], "end", 3) == 0 )        //缓冲区的字符为end时，结束循环
            {
                running= 0;
            }
        }
            //五个空间都被占用，输出waiting...
        else
        {
            sem_post(&shared->sem);        //V操作
            sleep(1);
            printf("Waiting for some time...\n");
        }
    }
    //将共享内存从当前进程中分离
    if(shmdt(shm) == -1)        //分离失败
    {
        exit(EXIT_FAILURE);
    }
    if(shmctl(shmid, IPC_RMID, 0) == -1)    //失败
    {
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

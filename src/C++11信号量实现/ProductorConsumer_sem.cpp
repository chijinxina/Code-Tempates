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

/*
 * linux信号量
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/*
 * 1. semget函数
 *功能： 用来创建和访问一个信号量集
 *原型： int semget(key_t key, int nsems, int semflg);
 *参数：
 *      key:信号量集的名字
 *      nsems：信号集中信号量的个数
 *      semflg:9个权限标志
 *返回值：返回信号集的标识码，失败返回-1
 */
/*
 * 2. semctl函数
 *功能： 用于控制信号量集
 *原型： int semctl(int semid, int semnum, int cmd,...);
 *参数：
 *      semid:由semget返回的信号集标识码
 *      semnum:信号集中信号量的序号（从0开始）
 *      cmd:将要采取的动作（有3个可取值）
 *              SETVAL   -> 设置信号量集中信号量的计数值
 *              GETVAL   -> 获取信号集中信号量的计数值
 *              IPC_STAT -> 把semid_ds结构中的数据设置为信号集的当前关联值
 *              IPC_SET  -> 在进程有足够权限的前提下，把信号集的当前关联值设置为semid_ds数据结构中给出的值
 *              IPC_RMID -> 删除信号集
 *返回值：失败返回-1
 */

/*
 * 3. semop函数
 *功能： 用来创建和访问一个信号量集
 *原型： int semop(int semid, struct sembuf *sops, unsigned nsops);
 *参数：
 *      semid:当前信号量的标识码，也是semget的返回值
 *      sops:一个指向结构数值的指针，通过结构体参数来告诉操作系统如何进行信号量的操作
 *      nsops：信号量的个数
 *             sembuf结构体
 *             struct sembuf{
 *                  short sem_num; //信号量编号
 *                  short sem_op;  //信号量进行一次PV操作加减的数值，一般是 -1->P操作  +1->V操作
 *                  short sem_flag;//两个取值 IPC_NOWAIT SEM_UNDO
 *             };
 *返回值：成功返回0，失败返回-1
 */

using namespace std;


/*
 * 信号量操作
 */
union semu
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

/*
 * 信号量 P操作
 */
int semP(int semid)
{
    sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;  //P操作 申请资源 信号量减1
    sb.sem_flg = SEM_UNDO;
    semop(semid,&sb,1);
    return 0;
}
/*
 * 信号量 V操作
 */
int semV(int semid)
{
    sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = +1;  //V操作 释放资源 信号量加1
    sb.sem_flg = SEM_UNDO;
    semop(semid,&sb,1);
    return 0;
}

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
            :pLoopBuffer( new LoopBuffer<int>(b) ) //创建b个元素长度的环形缓冲区
    {
        /*
         * 初始化4个信号量
         */
        key_t key1=0x1234; key_t key2=0x1235; key_t key3=0x1236; key_t key4=0x1237;
        sem_empty = semget(key1,1,0666|IPC_CREAT);
        sem_full  = semget(key2,1,0666|IPC_CREAT);
        mutex_P   = semget(key3,1,0666|IPC_CREAT);
        mutex_C   = semget(key4,1,0666|IPC_CREAT);
        if(sem_empty==-1 || sem_full==-1 || mutex_P==-1 || mutex_C==-1) perror("semget error!");
        cout<<"sem_empty id="<<sem_empty<<endl;
        cout<<"sem_full  id="<<sem_full<<endl;
        cout<<"mutex_P   id="<<mutex_P<<endl;
        cout<<"mutex_C   id="<<mutex_C<<endl;

        /*
         * 设置信号量初始值
         */
        union semu un;
        un.val = b;
        if(-1 == semctl(sem_empty,0,SETVAL,un))  perror("sem1 semctl error!");
        un.val = 0;
        if(-1 == semctl(sem_full ,0,SETVAL,un))  perror("sem2 semctl error!");
        un.val = 1;
        if(-1 == semctl(mutex_P  ,0,SETVAL,un))  perror("sem3 semctl error!");
        un.val = 1;
        if(-1 == semctl(mutex_C  ,0,SETVAL,un))  perror("sem4 semctl error!");

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
    int sem_empty; //表示缓冲区是否为空，初值为缓冲区长度
    int sem_full;  //表示缓冲区是否为满，初值为0
    int mutex_P;   //生产者线程之间的互斥锁 初值为1
    int mutex_C;   //消费者线程之间的互斥锁 初值为1

    std::mutex mCOUT;
};

/*
 * 生产者线程函数
 */
void ProductorConsumer::ProductorFunc(int x)
{
    while(1)
    {
        semP(sem_empty);   //1. P(empty)
        semP(mutex_P);     //2. P(muP)
        cerr<<"生产者["<<x<<"] 生产数据"<<x<<"到缓冲区      ";
        pLoopBuffer->pWrite->data = x;
        pLoopBuffer->pWrite = pLoopBuffer->pWrite->next;
        semV(mutex_P);     //3. V(muP)
        semV(sem_full);    //4. V(full)

        std::unique_lock<std::mutex> lk(mCOUT);
        cerr<<"缓冲区剩余空间 "<<semctl(sem_empty,0,GETVAL,0)<<endl;
        lk.unlock();


        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
/*
 * 消费者线程函数
 */
void ProductorConsumer::ConsumerFunc(int y)
{
    while(1)
    {
        semP(sem_full);    //1. P(full)
        semP(mutex_C);     //2. P(muC)
        cout<<"消费者["<<y<<"] 拿到数据"<<pLoopBuffer->pRead->data;
        pLoopBuffer->pRead = pLoopBuffer->pRead->next;
        semV(mutex_C);     //3. V(muC)
        semV(sem_empty);   //4. V(empty)

        std::unique_lock<std::mutex> lk(mCOUT);
        cout<<"             缓冲区剩余空间 "<<semctl(sem_empty,0,GETVAL,0)<<endl;
        lk.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}


int main()
{
    ProductorConsumer pc(1,2,10);
    while(1)
    {
        //cout<<"main process!"<<endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10000000));
    }
    return 0;
}
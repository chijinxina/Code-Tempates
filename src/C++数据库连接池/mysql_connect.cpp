//
// Created by chijinxin on 18-4-26.
//
#include <iostream>
#include "mysql_connect.h"


ConnectionPool* ConnectionPool::pool = nullptr;

//私有构造函数 实现单例
ConnectionPool::ConnectionPool(std::string name, std::string pwd, std::string nurl, int maxSize)
        : username(name), password(pwd), url(nurl), poolSize(maxSize)
{
    //得到mysql驱动
    driver = get_driver_instance();
    //开始初始化大小一半
    initConnectPool(poolSize/2);
}

//析构函数
ConnectionPool::~ConnectionPool()
{
    destoryPool();
}


//得到连接池大小
int ConnectionPool::getPoolSize()
{
    return conList.size();
}


//增加连接
void ConnectionPool::addConn(int size)
{
    for(int i = 0; i < size; ++i)
    {
        //创建连接
        Connection *conn = driver->connect(url, username, password);
        std::shared_ptr<Connection> sp(conn, [](Connection *conn){ delete conn; });
        conList.push_back(std::move(sp));
//        conList.push_back(std::shared_ptr<Connection>(driver->connect(url, username, password)));
    }
}


//初始化连接池
void ConnectionPool::initConnectPool(int initialSize)
{
    //加锁，增添一个连接
    std::lock_guard<std::mutex> locker(mu);
    addConn(initialSize);
}


//销毁一个连接
void ConnectionPool::destoryOneConn()
{
    //智能指针加std::move转移一个连接的“所有权”，当出作用域时，自动调用关闭connect
    std::shared_ptr<Connection> &&sp = std::move(conList.front());

    sp->close();
    --poolSize;
}


//销毁整个连接池
void ConnectionPool::destoryPool()
{
    for(auto &conn : conList)
    {
        //依次转移所有权，出作用域时，关闭连接，出作用域时智能指针自动释放
        std::shared_ptr<Connection> &&sp = std::move(conList.front());
        sp->close();
    }
}


//扩大连接池
void ConnectionPool::expandPool(int size)
{
    std::lock_guard<std::mutex> locker(mu);
    addConn(size);
    poolSize += size;
}



//缩小连接池
void ConnectionPool::reducePool(int size)
{
    std::lock_guard<std::mutex> locker(mu);
    //减小的大小不能超过存储的大小
    if(size > poolSize) { return; }

    for(int i = 0; i < size; i++)
    {
        //sp point new object, old object release
        destoryOneConn();
    }
    poolSize -= size;
}




//public //得到连接池实例
ConnectionPool* ConnectionPool::getInstance()
{
    if(pool == nullptr)
    {
        //3306是mysql占用的端口，其实创建40个连接
        pool = new ConnectionPool("root", "123", "tcp://127.0.0.1:3306", 40);
    }
    return pool;
}



//得到一个连接
std::shared_ptr<Connection> ConnectionPool::getConnect()
{
    std::lock_guard<std::mutex> locker(mu);
    //std::shared_ptr<Connection> sp = conList.front();
    //std::shared_ptr< Connection > sp = conList.front();
    conList.pop_front();

    return conList.front();
}


//归还一个连接
void ConnectionPool::retConnect(std::shared_ptr<Connection> &ret)
{
    std::lock_guard<std::mutex>locker(mu);
    conList.push_back(ret);
}





/*
 * demo
 */

ConnectionPool *pool = ConnectionPool::getInstance();




int main()
{
    std::shared_ptr<Connection> con; //连接对象
    Statement *state;
    ResultSet *result;
    //获得一个连接
    con = pool->getConnect();
    //获得一个数据库连接对象
    state = con->createStatement();
    //使用chi1这个数据库
    state->execute("use chi1");
    //查询语句
    result = state->executeQuery("select * from student;");
    while(result->next())
    {
        int id = result->getInt("id");
        std::string name = result->getString("name");
        std::string gender = result->getString("gender");
        std::cout << "id:" << id << " name:" << name <<" gender:"<<gender<< std::endl;
    }
    sleep(10);

    pool->retConnect(con);

    std::cout << pool->getPoolSize() << std::endl;

    sleep(10);

    return 0;
}
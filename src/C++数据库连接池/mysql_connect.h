//
// Created by chijinxin on 18-4-26.
//

#ifndef CODE_TEMPATES_MYSQL_CONNECT_H
#define CODE_TEMPATES_MYSQL_CONNECT_H

//C++
#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <functional>

//mysql driver
#include "jdbc/mysql_driver.h"
#include "jdbc/mysql_connection.h"

//mysql execute
#include "jdbc/cppconn/driver.h"
#include "jdbc/cppconn/statement.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "jdbc/cppconn/resultset.h"
#include "jdbc/cppconn/exception.h"

//thread mutex
#include <mutex>

using namespace std;
using namespace sql;

using delFunc = std::function< void(sql::Connection*) >;

/*
 * 数据库连接池对象
 */
class ConnectionPool
{
public:
    //获取数据库连接池对象（static单例模式）
    static ConnectionPool* getInstance();
    //获取一条数据库连接
    std::shared_ptr<Connection> getConnect();
    //归还一条数据库连接
    void retConnect(std::shared_ptr<Connection> &ret);
    //数据库连接池对象析构
    ~ConnectionPool();

private:
    //构造函数私有 单例模型
    ConnectionPool(std::string name, std::string pwd, std::string nurl, int maxsize);
    //初始化数据库连接池对象
    void initConnectPool(int initalSize);
    //释放连接池对象
    void destoryPool();
    //释放一条数据库连接
    void destoryOneConn();
    //扩容数据库连接池
    void expandPool(int size);
    //缩小数据库连接池
    void reducePool(int size);
    //添加连接
    void addConn(int size);

public:
    //读取数据库连接池大小
    int getPoolSize();

private:
    std::string username;   //用户名
    std::string password;   //密码
    std::string url;        //连接url
    int poolSize;           //连接池大小

    //存放所有的连接集合
    std::list< std::shared_ptr<Connection> > conList;
    //连接池对象 私有
    static ConnectionPool *pool;
    //互斥锁
    std::mutex mu;
    //mysql driver
    Driver *driver;
};

#endif //CODE_TEMPATES_MYSQL_CONNECT_H


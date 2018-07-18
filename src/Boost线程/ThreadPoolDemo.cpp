//
// Created by chijinxin on 17-12-5.
//
#include <iostream>
#include <boost/ref.hpp>
#include <boost/threadpool.hpp>

using namespace std;


class Server{   //利用线程池模拟服务器
public:
    typedef boost::threadpool::pool ThreadPool; //线程池类

    class Client{   //内部类 模拟客户端
    public:
        int id;
        bool shutdown;
        int data;
        Client()
        {
            std::cout<<"调用client默认构造函数！"<<std::endl;
            this->id = id;
            shutdown = false;
            data = 0;
        }
        Client(int id)
        {
            //std::cout<<"调用client带参构造函数！"<<std::endl;
            this->id = id;
            shutdown = false;
            data = 0;
        }
        ~Client(){}
    };

    Server(int Capacity):tp(Capacity)
    {
        std::cout<<"调用Server构造函数！ ";
        std::cout<<"初始化服务器线程池，容量="<<tp.size()<<std::endl;
    }
    ~Server()
    {
        tp.clear();
        std::cout<<"Server析构！"<<std::endl;
    }

    void insertClient(Client c);    //接入客户端
    void eraseClient(int id);       //客户端退出
    void taskClient(int &id, bool &shutdown, int &data);    //客户端函数执行体 客户端服务线程
    void printClientInfo(); //打印接入的所有客户端信息
protected:
    ThreadPool tp;  //线程池
    boost::mutex clientUpdata;  //接入客户端信息更新互斥锁
    std::map<int,Client> ID_Client; //客户端管理信息索引
};

void Server::insertClient(Client c)
{
    boost::mutex::scoped_lock lock(clientUpdata);
    std::map<int,Client>::iterator iter = ID_Client.find(c.id);
    if(iter!=ID_Client.end()){
        std::cout<<"Client ID 已存在，Client接入失败！"<<std::endl;
        return;
    }

    ID_Client.insert(pair<int,Client>(c.id,c));

    tp.schedule(boost::bind(&Server::taskClient,this,
                            boost::ref(ID_Client[c.id].id),boost::ref(ID_Client[c.id].shutdown),boost::ref(ID_Client[c.id].data)));
}

//客户端任务执行线程函数
void Server::taskClient(int &id, bool &shutdown, int &data) {
    std::cout << "Client ID=" << id << " 已接入"<< std::endl;
    while(1)
    {
        {
            boost::mutex::scoped_lock lock(clientUpdata);
            if (shutdown == true) {
                break;
            }
        }
        data++;
        //std::cout << "Client ID=" << id << " 计算data=" << data << std::endl;
        usleep(1000000);
    }
    /*
     * 删除客户端缓存map
     */
    {
        boost::mutex::scoped_lock lock(clientUpdata);
        std::map<int,Client>::iterator iter_erase = ID_Client.find(id);
        ID_Client.erase(iter_erase);
        std::cout<<"Client ID="<<id<<" 已退出！"<<std::endl;
    }
    return;
}

//客户端退出函数
void Server::eraseClient(int id) {
    std::cout<<"删除Client ID="<<id<<endl;
    boost::mutex::scoped_lock lock(clientUpdata);
    std::map<int,Client>::iterator iter_erase = ID_Client.find(id);
    if(iter_erase==ID_Client.end())
    {
        std::cout<<"Client ID="<<id<<"不存在！"<<std::endl;
        return;
    }
    else
    {
        ID_Client[id].shutdown = true;
    }
}


//打印接入客户端信息
void Server::printClientInfo() {
    boost::mutex::scoped_lock lock(clientUpdata);
    std::map<int,Client>::iterator iter;
    for(iter=ID_Client.begin();iter!=ID_Client.end();++iter)
    {
        std::cout<<"ID="<<iter->first<<" shutdown="<<iter->second.shutdown<<" data="<<iter->second.data<<std::endl;
    }
}



/*
 * Demo
 */
int main(){
    Server server(200);   //构造服务器对象
    int event=0;    //shell交互

    while(cin>>event)
    {
        if(event>0 && event<10) //接入客户端1-9
        {
            Server::Client c(event);
            cout<<"insert client id="<<c.id<<endl;
            server.insertClient(c);
            event=0;
        }
        else if(event>10 && event<100)   //删除客户端 模拟客户端退出， 删除的客户端ID=event-10
        {
            server.eraseClient(event-10);
            event=0;
        }
        server.printClientInfo();   //打印客户端信息
        usleep(100);
    }
    return 0;
}
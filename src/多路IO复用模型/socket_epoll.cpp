//
// Created by chijinxin on 18-4-2.
//
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>  //linux socket
#include <netinet/in.h>
#include <sys/epoll.h>  //linux I/O epoll
#include <string.h>
#include <arpa/inet.h>  //IP Addrs and Port process
#include <unistd.h>  //read write
#include <unordered_map>

class ClientINFO{
public:
    std::string ipaddr;
    int port;
};

std::unordered_map<int,ClientINFO> vclient;


int main()
{
    /*
     * 初始化服务器socket
     */
    int socket_fd, connect_fd;
    char buff[4096];
    struct sockaddr_in serveraddr;
    //1.初始化socket
    if((socket_fd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        std::cerr<<"create socket error!"<<std::endl;
        exit(0);
    }

    //初始化
    memset(&serveraddr,0,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);//将服务器IP地址设置成INADDR_ANY,让系统自己获取本机的IP地址
    serveraddr.sin_port = htons(8000); //设置端口为8000

    //2. bind()绑定本地地址到所创建的套接字上
    if((bind(socket_fd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)))==-1)
    {
        std::cerr<<"bind socket error!"<<std::endl;
        exit(0);
    }

    //3. listen()开始监听是否有客户端连接
    if((listen(socket_fd,10))==-1)
    {
        std::cerr<<"listen socket error!"<<std::endl;
        exit(0);
    }
    std::cout<<"-----------waiting for client's request-----------"<<std::endl;

    /*
     * 利用epoll实现socket多路I/O复用功能
     *
     * epoll_event结构体:
     * struct epoll_event
     * {
     *      uint32_t     events;      //Epoll events
     *      epoll_data_t data;        //User data variable
     * };
     * typedef union epoll_data
     * {
     *      void *ptr;
     *      int fd;
     *      uint32_t u32;
     *      uint64_t u64;
     * }epoll_data_t;
    */

    //1. epoll_create() 创建epoll
    int epfd = epoll_create(256); //可处理的最大句柄数
    if(epfd<0)
    {
        std::cerr<<"epoll create error!"<<std::endl;
        exit(0);
    }

    //epoll 初始化
    struct epoll_event _ev; //epoll事件结构体填充
    _ev.events = EPOLLIN;   //初始关心的事件为读事件
    _ev.data.fd = socket_fd;  //主socket事件

    //2. epoll_ctl 添加监视IO事件到epfd中 （托管）
    epoll_ctl(epfd,EPOLL_CTL_ADD,socket_fd,&_ev); //将主socket(socket_fd)句柄添加到epfd中,监视socket_fd上产生的读事件

    struct epoll_event revs[64]; //产生的事件集合

    //3. epoll_wait() 检测事件
    int num=0;
    int done=0;
    while(!done)
    {
        num = epoll_wait(epfd,revs,64,-1); //返回需要处理的事件数目 最大64个待处理事件
        switch(num)
        {
            case  0: std::cout<<"epoll timeout!"<<std::endl;
                break;
            case -1: std::cerr<<"epoll wait error!"<<std::endl;
                break;
            default: //正常情况
            {
                struct sockaddr_in peer;
                socklen_t len = sizeof(peer);
                for(int i=0;i<num;i++) //处理num个待处理的事件
                {
                    //准确的获取就绪链表中的文件描述符
                    int readfd = revs[i].data.fd;
                    //主socket(socket_fd)句柄上发生了读事件
                    if( readfd==socket_fd && (revs[i].events)&&EPOLLIN )
                    {
                        //调用accept() 与客户端建立连接
                        int newfd = accept(socket_fd,(struct sockaddr*)&peer,&len);
                        if(newfd>0)
                        {
                            std::cout<<"get a new client: "<<inet_ntoa(peer.sin_addr)<<":"<<ntohs(peer.sin_port)<<std::endl;
                            //加入到客户端集合中去
                            ClientINFO tmp;
                            tmp.ipaddr = inet_ntoa(peer.sin_addr);
                            tmp.port = ntohs(peer.sin_port);
                            vclient.insert(std::pair<int,ClientINFO>(newfd,tmp));
                        }
                        _ev.events = EPOLLIN | EPOLLET; //设置为读事件 和 ET边沿触发
                        _ev.data.fd = newfd;
                        //二次托管 将新接入的newfd加入epoll事件监视
                        epoll_ctl(epfd,EPOLL_CTL_ADD,newfd,&_ev);
                    }
                    //对接下来的num-1个读取客户端消息的事件进行处理
                    else
                    {
                        if(revs[i].events & EPOLLIN) //检查是否为读事件
                        {
                            //从fd中读取socket数据
                            ssize_t _s = read(readfd,buff,sizeof(buff)-1);
                            if(_s>0)  //成功读取revs[i].data.fd接收到的数据
                            {
                                buff[_s] = '\0';
                                std::cout<<"Client :"<<vclient[readfd].ipaddr<<":"<<vclient[readfd].port;
                                std::cout<<" Recved message: "<<buff<<std::endl;
                            }
                            else if(_s == 0) //客户端关闭连接
                            {
                                std::cout<<"Client :"<<vclient[readfd].ipaddr<<":"<<vclient[readfd].port;
                                std::cout<<" closed connection"<<std::endl;

                                epoll_ctl(epfd,EPOLL_CTL_DEL,readfd,NULL); //删除readfd的事件监控
                                vclient.erase( vclient.find(readfd) ); //删除客户端信息库中数据

                                //_ev.events = EPOLLOUT | EPOLLET;
                                //_ev.data.fd = readfd;
                                //epoll_ctl(epfd,EPOLL_CTL_DEL,readfd,&_ev);    //二次托管
                            }
                            else
                            {
                                std::cerr<<"read error!"<<std::endl;
                            }
                        }
                    }
                }
            }
        }

    }
    return 0;
}

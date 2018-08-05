//
// Created by chijinxin on 18-8-4.
//
#include <iostream>
#include <unordered_map>
#include <list>
using namespace std;
/*
 * 实现LRU  双向链表 + HashMap
 * 使用HashMap保存节点的key-value实现O(1)时间复杂度的元素查找 对应get工作
 */
struct CacheNode{
    int key;
    string value;
    CacheNode(int k, string v):key(k),value(v){}
};

/*
 * LRU实现类
 * get(key) 得到key对应的value,并将其移动到队列首部
 * put() 不存在：队列首部加入，根据容量淘汰尾部元素   存在：将器移动到队首
 */
class LRUCache{
public:
    typedef list< pair<int, string> >::iterator LIST_ITER;
    //构造函数
    LRUCache(int capacity):capacity_(capacity){ }

private:
    int capacity_;  //容量
    unordered_map<int,LIST_ITER> mp_;   //hashmap
    list< pair<int, string> > list_;        //双向队列

public:
    string get(int key)
    {
        //在LRU中找到了该数据对
        if(mp_.find(key) != mp_.end())
        {
            //通过把put把响应的元素移动到链表首部
            put(key, mp_[key]->second);
            return mp_[key]->second;
        }
        //在LRU中找不到数据
        return "";
    }

    void put(int key, string value)
    {
        //数据已存在于LRU中
        if(mp_.find(key) != mp_.end())
        {
            //1. 先把原来链表中的数据删掉
            list_.erase(mp_[key]);
        }
        else if( list_.size() >= capacity_ )
        {
            mp_.erase(list_.back().first);  //删除链表尾部对应的hashmap中的元素
            list_.pop_back();               //删除链表尾部元素
        }
        //2. 把元素放在链表首部
        list_.push_front(make_pair(key,value));
        //mp_.insert(make_pair(key,list_.begin()));
        mp_[key] = list_.begin();
    }

    int getSize()
    {
        return list_.size();
    }
};

int main()
{
    LRUCache lru(3);
    lru.put(1,"chijinxin");
    lru.put(2,"qwe");
    lru.put(3,"adadsd");
    cout<<lru.getSize()<<endl;

    cout<<lru.get(3)<<endl;
    lru.put(4,"zxczxczxc");
    if(lru.get(1)=="") cout<<"TRUE"<<endl;
    return 0;
}

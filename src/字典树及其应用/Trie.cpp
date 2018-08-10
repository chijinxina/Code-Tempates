//
// Created by chijinxin on 18-8-10.
//
#include <iostream>
using namespace std;

struct TrieNode{
    string value;
    TrieNode* next[26];  //26个字母
    TrieNode():value("") {}
};

class Trie{
public:
    Trie():root(NULL){}

    void insert(string keyStr, string valueStr)
    {
        if(!root) root = new TrieNode;
        TrieNode* location = root;

        for(int i=0;i<keyStr.length();i++)
        {
            int num = keyStr[i] - 'a';
            if(location->next[num] == NULL) location->next[num] = new TrieNode;
            location = location->next[num];
        }
        location->value = valueStr;  //叶子节点域为 value
    }

    string search(string keyStr)
    {
        TrieNode* location = root;
        for(int i=0; i<keyStr.length(); i++)
        {
            int num = keyStr[i] - 'a';
            if(location->next[num] == NULL)
            {
                return "";  //下一个节点不存在 提前结束 不存在要查找的字符串
            }
            location = location->next[num];
        }
        return location->value;
    }

private:
    TrieNode* root;
};

int main()
{
    Trie t;
    t.insert("chijinxin","adx");
    t.insert("zzz","123");
    t.insert("cccc","321");
    cout<<t.search("chijinxin")<<endl;
    return 0;
}

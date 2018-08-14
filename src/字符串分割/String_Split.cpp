//
// Created by chijinxin on 17-12-4.
//
#include <iostream>
#include <string>
#include <vector>
#include <thread>

using namespace std;
/*
 * 字符串分割函数
 * 参数：  str--待分割字符串  pattern--分隔符
 * 返回值： 子字符串数组
 */
std::vector<std::string> split(std::string str, std::string pattern){
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern; //拓展字符串以方便操作
    int size = str.size();
    for(int i=0;i<size;i++)
    {
        pos = str.find(pattern,i);
        if(pos<size)
        {
            std::string s = str.substr(i,pos-i);
            result.push_back(s);
            i = pos+ pattern.size()-1;
        }
    }
    return result;
}


/*
 * 测试demo
 */
int main(){

    vector<string> a = split("1,10;32,45",";");

    return 0;
}
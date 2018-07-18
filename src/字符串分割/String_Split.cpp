//
// Created by chijinxin on 17-12-4.
//
#include <iostream>
#include <string>
#include <vector>
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
    string str ="10 11 123123\na b c\n";
    cout<<str<<endl;
    cout<<endl<<endl;
    vector<string> res = split(str," ");
    for(int i=0;i<res.size();i++)
    {
        cout<<res[i]<<endl;
    }
    return 0;
}
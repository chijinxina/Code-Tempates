//
// Created by chijinxin on 17-12-4.
//
#include <iostream>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>

using namespace std;
/*
 * 遍历某一文件夹下所有文件
 * 依赖： Boost（filesystem） #include <boost/filesystem.hpp>
 * 参数： path--文件夹路径 flag=true -- 返回文件名
 *                      flag=false -- 返回文件绝对路径
 * 返回值：文件数组
 */
std::vector<std::string> findAllFile(std::string path,bool flag){
    boost::filesystem::directory_iterator end;
    boost::filesystem::directory_iterator pos(path);
    if(pos==end) {std::cerr<<"文件夹为空 or 文件夹不存在"<<std::endl;}
    std::vector<std::string> result;
    for(;pos!=end;++pos)
    {
        if(flag){
            result.push_back(pos->path().filename().string());
        } else{
            result.push_back(pos->path().string());
        }
    }
    return result;
}


/*
 * 测试demo
 */
int main(){
    string path = "../src/遍历目录下的所有文件/test";
    vector<string> a = findAllFile(path,false);
    for(int i=0;i<a.size();i++){
        cout<<a[i]<<endl;
    }
    return 0;
}


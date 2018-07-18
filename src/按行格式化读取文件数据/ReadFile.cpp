//
// Created by chijinxin on 17-12-4.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

using namespace std;
/*
 * 按行读取数据并存入vector数组中
 * 参数： path--文件路径 cols--文件数据列数
 * 返回值： 文件数据二维数组
 */
template <typename T>
std::vector<std::vector<T>> getFileRow(std::string path,int cols){
    ifstream file_in(path);
    if(!file_in) { std::cerr<<"文件路径不存在！"<<std::endl;}
    string line;
    stringstream ss;
    std::vector<std::vector<T>> result;
    std::vector<T> res;
    T temp;
    while(getline(file_in,line))
    {
        ss.clear();
        ss<<line;
        for(int i=0;i<cols;i++){
            ss>>temp;
            res.push_back(temp);
        }
        result.push_back(res);
        res.clear();
    }
    file_in.close();//关闭文件
    return result;
}

/*
 * 测试demo
 */
int main(){
    string path = "../src/按行格式化读取文件数据/test.txt";
    vector<vector<double>> a = getFileRow<double>(path,7);
    for(int i=0;i<a.size();i++){
        for(int j=0;j<a[i].size();j++){
            cout<<a[i][j]<<" ";
        }
        cout<<endl;
    }
    return 0;
}

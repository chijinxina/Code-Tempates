//
// Created by chijinxin on 17-12-8.
//
#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/*
 * 利用OpenCV读写YAML配置文件
 */
int main()
{
    /*
     * OpenCV写YAML文件操作
     */
    const std::string settingFilePath = "../src/OpenCV操作yaml参数文件/test.yaml";
    cv::FileStorage settingFile(settingFilePath.c_str(), cv::FileStorage::WRITE);   //打开文件进行写操作

    if(!settingFile.isOpened())     //判断文件是否打开
    {
        std::cerr << "Failed to open settings file at: " << settingFilePath <<std::endl;
        exit(-1);
    }


    settingFile.write("Camera-name", "chijinxin");//写参数配置到yaml文件中
    settingFile.write("Camera-x", 88.888);
    settingFile.write("Camera-y", 58.588);

    /*
     * 写 CvMat对象到YAML文件中
     * @brief Writes the registered C structure (CvMat, CvMatND, CvSeq).
     * CvMat，Mat和IplImage之间的转化和拷贝 http://blog.csdn.net/holybin/article/details/17711013
     */
    //cv::Mat srcImage = cv::imread("../src/OpenCV操作yaml参数文件/test.png");
    cv::Mat testMat1(3,3,CV_64FC1,cv::Scalar(8)); //cv::Scalar()
    cv::Mat testMat2 = cv::Mat::eye(3,3,CV_64FC1);//单位矩阵
    CvMat obj = testMat2;
    settingFile.writeObj("testMat-data",&obj);


    /*********关闭文件句柄***********/
    settingFile.release();
    /******************************/

    /*
     * OpenCV读YAML文件操作
     */
    const std::string readFileName = "../src/OpenCV操作yaml参数文件/test.yaml";

    cv::FileStorage readFile(readFileName,cv::FileStorage::READ);//打开参数文件，获取文件句柄

    std::string name = readFile["Camera-name"];
    std::cout<<"Camera-name:"<<name<<std::endl;

    double x = readFile["Camera-x"];
    double y = readFile["Camera-y"];
    std::cout<<"Camera-x="<<x<<std::endl<<"Camera-y="<<y<<std::endl;

    CvMat* a = (CvMat*)readFile["testMat-data"].readObj();
    cv::Mat chijinxin =  cv::cvarrToMat(a); //  CvMat* 转 cv::Mat 的方法
    std::cout<<chijinxin<<std::endl;

    readFile.release();//关闭文件句柄

    return 0;
}



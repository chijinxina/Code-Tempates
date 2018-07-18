//
// Created by chijinxin on 18-3-17.
//

#include <iostream>

using namespace std;


int main()
{
    FILE* gp;
    gp = popen("gnuplot -persist","w"); //popen()函数用创建管道的方式启动一个进程,并调用shell.因为管道是被定义成单向的,
                                        //所以type参数只能定义成只读或者只写, 不能是两者同时, 结果流也相应的是只读或者只写.
    if(gp==NULL)
    {
        std::cerr<<"管道创建失败！"<<std::endl;
    }
    fprintf(gp,"set isosample 100/n");
    fprintf(gp,"min=-1/n");
    fprintf(gp,"max=1/n");
    fprintf(gp,"pi=3.1415926/n");
    fprintf(gp,"set hidden3d/n");
    fprintf(gp,"set pm3d/n");
    fprintf(gp,"set contour/n");
    fprintf(gp,"splot [min:max] [min:max] x*x+2*y*y-0.3*cos(3*pi*x)-0.4*cos(4*pi*y)+0.7/n");
    fprintf(gp,"pause -1/n");

    pclose(gp);//关闭管道
    return 0;
}
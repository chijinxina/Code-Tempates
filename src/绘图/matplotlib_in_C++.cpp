/*
 * g++ -DWITHOUT_NUMPY minimal.cpp -I/usr/include/python2.7 -lpython2.7 -o minimal -std=c++11
 * include_directories(/usr/include/python2.7)
 * target_link_libraries(${target} python2.7)
 */
#include <iostream>
#include <vector>
#include <math.h>
#include "matplotlibcpp.h"

using namespace std;

namespace plt = matplotlibcpp;

int main() {
    vector<double> x;
    vector<double> y;
    vector<double> z;
    double xx=0;
    for(int i=0;xx<10;i++)
    {
        xx+=0.01;
        x.push_back(xx);
        y.push_back(sin(xx));
        z.push_back(log(xx));
    }
    plt::plot(x,y,"b--");
    plt::plot(x,z,"r");
   // plt::named_plot("sin(x)",x,y);
   // plt::named_plot("log(x)",x,z);
   // plt::title("Sample figure");
    plt::xlim(0,4);
   // plt::legend();
    plt::show();
}
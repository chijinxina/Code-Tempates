//
// Created by chijinxin on 18-8-15.
//
#include <iostream>
using namespace std;

bool isNumeric(string str)
{
    //标记符号、小数点、e是否出现过
    bool sign = false, decimal = false, hasE = false;

    for(int i=0;i<str.length();i++)
    {
        if(str[i]=='e' || str[i]=='E')
        {
            if(i==str.length()-1) return false; //e后面一定要跟数字
            if(hasE) return false;            //不允许存在两个e
            hasE = true;
        }
        else if(str[i] == '+' || str[i] == '-')
        {
            //第二次出现+-符号,必须是紧跟e之后
            if(sign && str[i-1]!='e' && str[i-1]!='E') return false;
            //第一次出现+-符号，且不是在字符串的开头，则也必须紧跟e之后
            if(!sign && i>0 && str[i-1]!='e' && str[i-1]!='E') return false;
            sign = true;
        }
        else if(str[i]=='.')
        {
            //e后面不能跟小数点，小数点不能出现两次
            if(hasE || decimal) return false;
            decimal = true;
        }
        else if(str[i]<'0' || str[i]>'9') return false;
    }
    return true;
}


int main()
{
    return 0;
}

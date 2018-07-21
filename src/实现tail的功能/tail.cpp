//
// Created by chijinxin on 18-7-21.
//
#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
using namespace std;
/*
 * C++实现tail -f -----实时读取变化文件中的增量内容
 */

long offset = 0;
char text[4096];

int tailf(const char* file_name)
{
    FILE* fp = fopen(file_name,"r");
    if(!fp)
    {
        cout<<"Can not open file "<<file_name<<endl;
        return -1;
    }
    fseek(fp, offset, SEEK_SET);
    int len = 0;

    while(!feof(fp))
    {
        fgets(text, 4096, fp);
        len = strlen(text);
        if(len ==0 || text[len-1] != '\n')
            continue;
        text[len-1] = '\0';
        offset += len;
        cout<<text<<endl;
    }
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        cout<<"Usage: exe file_name\n"<<endl;
        return -1;
    }
    while (true)
    {
        tailf(argv[1]);
        usleep(1000);
    }
    return 0;
}

//
// Created by chijinxin on 18-8-9.
//
#include <iostream>
using namespace std;
/*
 * memcpy
 */
void* my_memcpy(void* dst, const void* src, int len)
{
    if(src == NULL || dst == NULL) return NULL;

    char* pdst = static_cast<char*>(dst);
    char* psrc = static_cast<char*>(src);
    char* temp = pdst;
    while( (len--) > 0)
    {
        *(pdst++) = *(psrc++);
    }
    return static_cast<void*>(temp);
}

/*
 * memmove
 */
void* my_memmove(void* dst, const void *src, int len)
{
    if(dst == NULL || src == NULL) return NULL;

    char* pdst = static_cast<char*>(dst);
    char* psrc = static_cast<char*>(src);
    char* temp = pdst;
    /*
     * 两种情况，正向拷贝 和 反向拷贝
     */
    //反向拷贝
    if( (psrc < pdst) && (psrc + len > pdst))
    {
        for(int i=0;i<len;i++)
        {
            *(pdst+len-i) = *(psrc+len-i);
        }
    }
    //正向拷贝
    else
    {
        while((len--)>0)
        {
            *(pdst++) = *(psrc++);
        }
    }
    return static_cast<void*>(temp);
}


int main()
{
    char test[100]={0};

    return 0;
}

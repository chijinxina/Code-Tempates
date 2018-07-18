//
// Created by chijinxin on 18-3-28.
//堆排序  O(nlogn)
//

#include <iostream>
using namespace std;

int a[] = {6,1,2,7,9,3,4,5,10,8};


/*
 * 堆排序基本思想： 将待排序的序列构造成一个大顶堆，此时，整个序列的最大值就是堆顶的根节点，将根节点与末尾元素进行交换，此时序列末尾就是最大值。
 * 然后将剩余的n-1个元素重新构造成一个堆，这样会得到n个元素的次小值。如此反复执行，便可以得到一个有序序列。
 */

/*
 * 元素交换
 */
void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

/*
 * 调整当前堆 使其满足大顶堆定义
 */
void adjustHeap(int a[], int i, int len)
{
    int tmp = a[i]; //先取出当前第i个元素（非叶子节点）
    for(int k=i*2+1;k<len;k=k*2+1) //从i节点的左子节点开始，也就是2*i+1处开始 (左子节点：2K+1  右子节点:2K+2)
    {
        /*
         * 比较左右子节点 找到左右节点大的那个节点 去与父节点比较
         */
        if(k+1<len && a[k]<a[k+1]) //如果左子节点小于右子节点 k指向右子节点
        {
            k++;
        }

        /*
         * 父节点小于子节点
         */
        if(a[k] > tmp) //如果子节点大于父节点，将子节点值赋给父节点（不用进行交换）
        {
            a[i] = a[k];
            i = k;
        }
        else
        {
            break;
        }
    }
    a[i] = tmp; //将tmp值放在最终位置
}


void HeapSort(int a[], int n)
{
    //1. 构建大顶堆
    for(int i=n/2-1;i>=0;i--)
    {
        //从第一个非叶子节点（arr.length/2-1）从下至上，从右至左调整结构
        adjustHeap(a,i,n);
    }
    //2. 交换堆顶元素与末尾元素 + 调整堆结构
    for(int j=n-1;j>0;j--)
    {
        swap(a[0],a[j]);
        adjustHeap(a,0,j);
    }
}


int main()
{
    cout<<"befor:"<<endl;
    for(int i=0;i<10;i++)
        cout<<a[i]<<" ";
    cout<<endl;
    int n=10;
    HeapSort(a,10);
    cout<<"befor:"<<endl;
    for(int i=0;i<10;i++)
        cout<<a[i]<<" ";
    cout<<endl;
}
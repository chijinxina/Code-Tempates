//
// Created by chijinxin on 18-8-28.
//
#include <iostream>
#include <vector>

using namespace std;

/*
 * 递归法二分查找
 */
int binSearch(vector<int> &vec, int left, int right, int key)
{
    if(left > right) return -1;

    int mid = left + (right - left)/2;
    if(vec[mid] < key)
    {
        return binSearch(vec, mid + 1, right, key);
    }
    else if(vec[mid] > key)
    {
        return binSearch(vec, left, mid - 1, key);
    }
    else return mid;
}

/*
 * 非递归法二分查找
 */
int binSearch2(vector<int> &vec, int key)
{
    int left = 0;
    int right = vec.size()-1;

    while(left <= right)
    {
        int mid = left + (right - left)/2;
        if(vec[mid] < key)
        {
            left = mid + 1;
        }
        else if(vec[mid] > key)
        {
            right = mid - 1;
        }
        else return mid;
    }
}



int main()
{
    vector<int> data = {1,2,3,4,5,6,7,8,9};

    cout<<binSearch(data,0,8,4)<<endl;
    cout<<binSearch2(data,4)<<endl;

    return 0;
}

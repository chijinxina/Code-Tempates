#!/usr/bin/bash

a="hello world!"
echo "A is:$a"

num=2
echo "this is $numnd"
echo "this is ${num}nd"

#1. 整数的计算
#在*和/之前要冠以反斜线，整数的运算一般通过let和expr这两个指令来实现
#例：对变量x加1可以写作：
x=1
let "x=$x+1"
echo "=$x"
x=`expr $x + 1`
echo "=$x"

#2. 命令行参数
echo $#     #传入脚本的命令行参数个数

echo $*     #所有命令行参数值，在各参数值之间留有空格

echo $0     #命令本身（shell文件名）
echo $1     #第1个命令行参数
echo $2     #第2个命令行参数

#3. FOR结构
for((i=0;i<3;i++)); do
    touch test_$i.txt
done

#4. IF ELSE结构
# -a : and
# -o : or
#  ! : 非
# -eq: 相等
if [[ $1 -eq $1 ]]   #[]两端必须有空格
then echo "True"
else echo "False"
fi
#include <stdio.h>
#include <string.h>

typedef struct memblock{
    int head;                //地址
    int length;               //长度
    int state;                //状态，0表示空闲，1表示占用
    int jobid;                //已分配，记录作业ID，否则为0
    memblock * pre;          //所在空闲分区队列的前一个分区
    memblock * next;         //所在空闲分区队列的后一个分区
}memblock;


int main(int argc, char const *argv[])
{
    
    return 0;
}

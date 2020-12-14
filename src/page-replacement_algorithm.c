#include <stdio.h>

// 最大页数
#define N 20
// 内存页个数
#define Msize 3
// 缺页率
float rate;
// 缺页数
int lack;
// 页面走向序列
int Pages[N] = {7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1};
// 内存页
int memory[Msize];


// 输出当前内存页面情况
void print_m(int cur)
{
    for (int i = 0; i < Msize; ++i)
        if (-1 != memory[i])printf("%d\t",memory[i]);
    printf("\n");
}
// 初始化缺页率,缺页数和内存分配信息
void init()
{
    rate = 0;
    lack = 0;
    for (int i = 0; i < Msize; ++i)
    {
        memory[i] = -1;
    }   
}

// 判断内存中是否还有空余,无空余返回-1,有空余返回空闲内存序号
int is_full()
{
    for (int i = 0; i < Msize; i++)
    {
        if (-1 == memory[i])
        {
            return i;
        }
    }
    return -1;
}
// 判断当前页是否已在内存中,不在返回-1, 若在返回所在内存序号
int is_in(int page)
{
    for (int i = 0; i < Msize; ++i)
    {
        if(page == memory[i]) return i;
    }
    return -1;
}
// 返回最长时间后使用到的内存分页序号,即OPT应该淘汰的内存分页,传入当前页面走向序列号开始搜索
int will_longest_used(int cur)
{
    int i, j;
    int next_time[Msize]  = {0};   //记录下次出现时间
    int longest_index = 0;
    for (i = 0; i < Msize; i++) {
        for (j = cur + 1; j < N; j++)
        {
            if(Pages[j] == memory[i]) {
                next_time[i] = j - cur;
                if(next_time[longest_index] < next_time[i]) longest_index = i;
                break;
            }
        }
        if(j == N) return i;
    }
    return longest_index;
}
// 最佳置换算法(OPT)
void OPT()
{
    init();
    int cur = 0;    // 当前请求序号
    lack = 0;       // 缺页数置为0
    for (cur = 0; cur < N; ++cur)
    {
        if (-1 != is_full())
        {   // 内存有空位执行循环体
            memory[is_full()] = Pages[cur];
            lack++;
            printf("%d\t",Pages[cur]);
            print_m(cur);
        }
        else if (-1 == is_in(Pages[cur]))
        {   // 是否在内存中, 不在执行循环体
            memory[will_longest_used(cur)] = Pages[cur];
            lack++;
            printf("%d\t",Pages[cur]);
            print_m(cur);
        }
        else
        {
            printf("%d\n",Pages[cur]);
        }
    }
    
}

// 先进先出(FIFO)页面置换算法
void FIFO()
{
    init();
    int oldest = 0;
    for (int i = 0; i < N; i++)
    {
        if (-1 != is_full())
        {   // 内存有空位执行循环体
            memory[is_full()] = Pages[i];
            lack++;
            printf("%d\t",Pages[i]);
            print_m(i);
        }
        else if(-1 == is_in(Pages[i]))// 无空位直接淘汰最先来的
        {
            memory[oldest] = Pages[i];
            oldest = (++oldest)%Msize;
            printf("%d\t",Pages[i]);
            print_m(i);
        }
        else
        {
            printf("%d\n",Pages[i]);
        }
        
    }
    
}

int main()
{

    FIFO(); 
    
    return 0;
}

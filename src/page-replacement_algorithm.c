#include <stdio.h>

// 最大页数
#define N 12
// 内存页个数
#define Msize  5//4 3
// 缺页率
float rate[4];
// 缺页数
int lack[4];
// 页面走向序列
int Pages[N] = {1,2,3,4,1,2,5,1,2,3,4,5};//{4,3,2,1,4,3,5,4,3,2,1,5};
// 内存页
int memory[Msize];

// 返回数组中的最大值下标
int max(int a[], int size)
{
    int m = 0;
    for (int i = 0; i < size; ++i)
    {
        if (a[i] > a[m])
        {
            m = i;
        } 
    }
    return m;
}
// 输出当前内存页面情况
void print_m(int cur)
{
    for (int i = 0; i < Msize; ++i)
        if (-1 != memory[i]) printf("%d\t",memory[i]);
    printf("\n");
}
// 初始化缺页率,缺页数和内存分配信息
void init(int x)
{
    rate[x] = 0;
    lack[x] = 0;
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
// LRU支持函数, 给dwell驻留时间数组提供时间步进
void dwell_p(int * a, int x)
{
    for (int i = 0; i < Msize; ++i)
    {
        if (-1 != memory[i])
            a[i]++;
        if(x == memory[i]) a[i] = 0;
    }
    
}
// 最佳置换算法(OPT)
void OPT()
{
    init(0);
    int cur = 0;    // 当前请求序号
    for (cur = 0; cur < N; ++cur)
    {
        if(-1 != is_in(Pages[cur]))
        {   // 页面在内存中
            printf("%d\t|\n",Pages[cur]);
        }
        else if (-1 != is_full())
        {   // 内存有空位执行代码
            memory[is_full()] = Pages[cur];
            lack[0]++;
            printf("%d\t|",Pages[cur]);
            print_m(cur);
        }
        else if (-1 == is_in(Pages[cur]))
        {   // 是否在内存中, 不在执行代码
            memory[will_longest_used(cur)] = Pages[cur];
            lack[0]++;
            printf("%d\t|",Pages[cur]);
            print_m(cur);
        }
    }
}
// 先进先出(FIFO)页面置换算法
void FIFO()
{
    init(1);
    int oldest = 0;
    for (int i = 0; i < N; i++)
    {
        if(-1 != is_in(Pages[i]))
        {   // 页面在内存中
            printf("%d\t|\n",Pages[i]);
        }
        else if (-1 != is_full())
        {   // 内存有空位执行代码
            memory[is_full()] = Pages[i];
            lack[1]++;
            printf("%d\t|",Pages[i]);
            print_m(i);
        }
        else if(-1 == is_in(Pages[i]))// 无空位直接淘汰最先来的
        {
            memory[oldest] = Pages[i];
            oldest = (++oldest)%Msize;
            lack[1]++;
            printf("%d\t|",Pages[i]);
            print_m(i);
        }

    }
}
// 最近最久未使用(Last Recently Used)置换算法
void LRU()
{
    int dwell[Msize] = {0};   //存储每个内存页面中未使用时间长短
    init(2);
    for (int i = 0; i < N; i++)
    {
        if(-1 != is_in(Pages[i]))
        {   // 页面在内存中
            printf("%d\t|\n",Pages[i]);
        }
        else if (-1 != is_full())
        {   // 内存有空位执行代码
            memory[is_full()] = Pages[i];
            lack[2]++;
            printf("%d\t|",Pages[i]);
            print_m(i);
        }
        else if(-1 == is_in(Pages[i])) // 无空位淘汰驻留时间最长的
        {
            memory[max(dwell, Msize)] = Pages[i];
            lack[2]++;
            printf("%d\t|",Pages[i]);
            print_m(i);
        }

        dwell_p(dwell,Pages[i]);
    }
}
// 为Clock置换算法找到下个需要淘汰的内存页面并修改访问位,传入为当前时钟指针位置,返回值为淘汰页面序号
int next_clock_page(int R[],int *p)
{
    int i,j;
    for ( i = *p,j = 0; j < Msize; i = (++i) % Msize,++j)
    {
        if(R[i] == 0) {
            R[i] = 1;
            *p = (*p + 1) % Msize;
            return i;
        }
    }
    for ( i = 0; i < Msize; i++)
    {
        R[i] = 0;
    }
    i = *p;
    *p = (*p + 1) % Msize;
    return i;
}
// Clock置换算法
void CLOCK()
{
    int R[Msize];
    int p = 0;
    init(3);
    for (int i = 0; i < N; i++)
    {
        if(-1 != is_in(Pages[i]))
        {   // 页面在内存中
            R[is_in(Pages[i])] = 1;
            printf("%d\t|\n",Pages[i]);
        }
        else if (-1 != is_full())
        {   // 内存有空位执行代码
            R[is_full()] = 1;           // 置访问位为 1
            memory[is_full()] = Pages[i];
            // p = (++p) % Msize;
            lack[3]++;
            printf("%d\t|",Pages[i]);
            print_m(i);

        }
        else if(-1 == is_in(Pages[i]))  // 无空位且不在内存中则进入淘汰流程
        {
            memory[next_clock_page(R, &p)] = Pages[i];           
            lack[3]++;
            printf("%d\t|",Pages[i]);
            print_m(i);
        }
    }    
}

// 打印菜单
void print_menu()
{
    printf("\n=============菜单================\n");
    printf("0. 退出\n");
    printf("1. OPT算法实现\n");
    printf("2. FIFO算法实现\n");
    printf("3. LRU算法实现\n");
    printf("4. CLOCK算法\n");
    printf("5. 显示缺页率对比表\n");
    printf("=================================\n");
    printf("请输入数字以选择:");
}
// 打印缺页率对比表
void print_table()
{
    printf("\n**************************************\n");
    printf("算法\t缺页数\t缺页率\n");
    printf("OPT\t");printf("%d\t%.2f\n",lack[0],lack[0]*1.0/N);
    printf("FIFO\t");printf("%d\t%.2f\n",lack[1],lack[1]*1.0/N);
    printf("LRU\t");printf("%d\t%.2f\n",lack[2],lack[2]*1.0/N);
    printf("CLOCK\t");printf("%d\t%.2f\n",lack[3],lack[3]*1.0/N);
    printf("\n**************************************\n");
}
// 程序入口
int main()
{
    printf("***********页面淘汰/置换算法***********\n");
    print_menu();
    int choice;
    scanf("%d", &choice);
    while (choice)
    {
        switch (choice)
        {
        case 1:
            printf("\n**************************************\n");
            printf("OPT算法内存页面引用序列如下:\n");
            OPT();
            printf("\n**************************************\n");
            break;
        case 2:
            printf("\n**************************************\n");
            printf("FIFO算法内存页面引用序列如下:\n");
            FIFO();
            printf("\n**************************************\n");
            break;
        case 3:
            printf("\n**************************************\n");
            printf("LRU算法内存页面引用序列如下:\n");
            LRU();
            printf("\n**************************************\n");
            break;
        case 4:
            printf("\n**************************************\n");
            printf("CLOCK算法内存页面引用序列如下:\n");
            CLOCK();
            printf("\n**************************************\n");
            break;
        case 5:
            print_table();
        }
        print_menu();
        scanf("%d",&choice);
    }
    printf("*******************退出程序!*******************\n");
    return 0;    
}

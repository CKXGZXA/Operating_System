#include <stdio.h>
#include <malloc.h>
#define MAX 10

typedef struct memblock{
    int head;                //地址
    int length;               //长度
    // int state;                //状态，0表示空闲，1表示占用
    // int jobid;                //已分配，记录作业ID，否则为0
}memblock;

typedef struct 
{
    int jobid;
    int memory;
    int mem_no;
}Job;



int N;                          // 空闲内存数目
memblock free_mem[MAX];         // 空闲分区数组
// memblock backup[MAX];           // 备份数组,用于还原初始空闲内存状态
struct{                 // 作业简易队列
    Job jobs[MAX];
    int front;
    int rear;
    int M;                          // 请求分配内存作业数目
}job_queue;                         

// 初始化空闲分区
void init_memory();

// 输出空闲内存分区表信息
void print_Mem_info();

// 初始化请求作业
void init_jobs();

// 首次适应算法
void First_Fit()
{
    for (int i = 0; i < job_queue.M; ++i)
    {
        for ( int j = 0; j < N; ++j)
        {
            if (job_queue.jobs[i].memory <= free_mem[j].length)
            {
                free_mem[j].length -= job_queue.jobs[i].memory;
                free_mem[j].head += job_queue.jobs[i].memory;
                job_queue.jobs[i].mem_no = j;
                break;
            }            
        }
    }
}

// 循环首次适应算法
void Next_Fit()
{
    int p = 0;  // 记录上一次分配内存的位置
    for (int i = 0; i < job_queue.M; ++i)
    {
        for ( int j = 0; j < N; ++j)
        {
            if (job_queue.jobs[i].memory <= free_mem[p].length)
            {
                free_mem[p].length -= job_queue.jobs[i].memory;
                free_mem[p].head += job_queue.jobs[i].memory;
                job_queue.jobs[i].mem_no = p;
                p = (++p)%N;
                break;
            }            
        }        
    }
    
}

// 最佳适应算法
void Best_Fit()
{  
    Sort_Freemem(0); // 将内存按大小从小到达进行排序
    for (int i = 0; i < job_queue.M; ++i)
    {
        for ( int j = 0; j < N; ++j)
        {
            if (job_queue.jobs[i].memory <= free_mem[j].length)
            {
                free_mem[j].length -= job_queue.jobs[i].memory;
                free_mem[j].head += job_queue.jobs[i].memory;
                job_queue.jobs[i].mem_no = j;
                break;
            }            
        }
        Sort_Freemem(0); // 分配完进行再排序 
    }
}

void Worst_Fit()
{
    Sort_Freemem(1); // 将内存按大小从大较小进行排序
    for (int i = 0; i < job_queue.M; ++i)
    {
        for ( int j = 0; j < N; ++j)
        {
            if (job_queue.jobs[i].memory <= free_mem[j].length)
            {
                free_mem[j].length -= job_queue.jobs[i].memory;
                free_mem[j].head += job_queue.jobs[i].memory;
                job_queue.jobs[i].mem_no = j;
                break;
            }            
        }
        Sort_Freemem(1); // 分配完进行再排序 
    }
}


// 回收指定作业的内存空间
int Recycling_Mem();

// 恢复初始状态
void Recover();

// 将空闲分区表按空间进行指定顺序排序
void Sort_Freemem(int op);

// 打印菜单
void print_menu();

int main()
{
    printf("*************内存分配和回收***********\n");
    init_memory();
    init_jobs();
    print_menu();
    int choice;
    scanf("%d", &choice);
    while (choice)
    {
        switch (choice)
        {
        case 1:
            First_Fit();
            break;
        case 2:
            Next_Fit();
            break;
        case 3:
            Best_Fit();
            break;
        case 4:
            Worst_Fit();
            break;
        case 5:
            Recycling_Mem();
            break;
        case 6:
            print_Mem_info();
            break;
        case 7:
            Recover();
            break;
        default:
            break;
        }
        print_menu();
        scanf("%d",&choice);
    }    
    return 0;
}

// 打印菜单
void print_menu()
{
    printf("\n=================================\n");
    printf("0. 退出\n");
    printf("1. 首次适应算法为队首作业分配内存空间\n");
    printf("2. 循环首次适应算法为队首作业分配内存空间\n");
    printf("3. 最佳适应算法为队首作业分配内存空间\n");
    printf("4. 最坏适应算法为队首作业分配内存空间\n");
    printf("5. 回收指定作业的内存空间\n");
    printf("6. 显示当前内存分配情况\n");
    printf("7. 还原初始状态\n");
    printf("=================================\n");
    printf("请输入数字以选择:");
}

// 初始化空闲分区
void init_memory()
{
    int n;
    printf("请输入内存分区数量:");
    scanf("%d", &N);
    printf("请输入各空闲分区大小和内存起址:\n");    
    for(int i = 0;i < N; ++i)
    {
        // 初始化空闲分区的各项信息
        printf("分区号%d:",i);
        scanf("%d %d",&free_mem[i].length, &free_mem[i].head);
        // backup[i].length = free_mem[i].length;
        // backup[i].head = free_mem[i].head;
    }
} 

// 初始化请求作业
void init_jobs()
{
    job_queue.front = job_queue.rear = 0;
    printf("\n=================================\n");   
    printf("========初始化请求作业============\n"); 
    printf("请输入待请求作业个数:");
    scanf("%d", &job_queue.M);
    printf("请输入作业ID以及请求的内存大小:\n");
    for (int i = 0; i < job_queue.M; ++i)
    {
        printf("作业%d:",i+1);
        scanf("%d%d",&job_queue.jobs[job_queue.rear].jobid,&job_queue.jobs[job_queue.rear].memory);
        job_queue.jobs[job_queue.rear].mem_no = -1;     //初始化为-1, 表示未分配
        ++job_queue.rear;
    }  
}

// 输出空闲内存分区表信息
void print_Mem_info()
{
    int seq;
    printf("\n=========空闲内存分区表信息===============\n");
    printf("\n分区号\t分区大小(KB)\t分区始址\n");
    for (int i = 0,seq = 0; i < N; i++)
    {
        if(0 != free_mem[i].length){
            printf("%4d\t%5d\t\t%5d\n",i,free_mem[i].length,free_mem[i].head);
            ++seq;
        }
    }
    printf("\n========================================\n");
}

void Recover()
{
    for (int i = 0; i < job_queue.M; ++i)
    {
        if(job_queue.jobs[i].mem_no != -1){
            free_mem[job_queue.jobs[i].mem_no].head -= job_queue.jobs[i].memory;
            free_mem[job_queue.jobs[i].mem_no].length += job_queue.jobs[i].memory;
            job_queue.jobs[i].mem_no = -1;
        }
    }
}

int Recycling_Mem()
{
    int no;
    printf("请输入需要释放内存的进程ID:");
    scanf("%d", &no);
    for (int i = 0; i < job_queue.M; ++i)
    {
        if (job_queue.jobs[i].jobid == no && job_queue.jobs[i].mem_no != -1)
        {
            free_mem[job_queue.jobs[i].mem_no].head -= job_queue.jobs[i].memory;
            free_mem[job_queue.jobs[i].mem_no].length += job_queue.jobs[i].memory;
            job_queue.jobs[i].mem_no = -1;
            return 1;
        }
        else if (job_queue.jobs[i].jobid == no && job_queue.jobs[i].mem_no == -1)
        {
            printf("该进程未占用内存或请求内存失败!");
            return 1;
        }
    }
    printf("该进程ID不存在!");
    return 0;
}

// 按输入指令比较大小, 如果op为0则返回x>y的值, 如果op为1则返回x<y的值
int cmp(int x, int y, int op)
{
    switch (op)
    {
    case 0:
        return x > y;
    
    case 1:
        return x < y;
    }
}

void Sort_Freemem(int op)
{
    for (int i = 0; i < N - 1; i++) {       
        for (int j = i; j < N; j++) {
            if (cmp(free_mem[i].length,free_mem[j].length,op)) {
                for (int k = 0; k < job_queue.M; ++k)
                {
                    if(job_queue.jobs[k].mem_no == i)
                    {
                        job_queue.jobs[k].mem_no = j;
                    }
                    else if (job_queue.jobs[k].mem_no == j)
                    {
                        job_queue.jobs[k].mem_no = i;
                    }
                    
                }
                
                memblock temp = free_mem[i];
                free_mem[i] = free_mem[j];
                free_mem[j] = temp;
            }
        }
    }
}
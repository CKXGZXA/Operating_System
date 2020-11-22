// 代码地址:https://gitee.com/ZXA200009/operating_system/blob/master/Process_Scheduling.c
// 模拟实现进程调度的经典算法，包括FCFS、SJF（SPF）、HRRN和RR(时间片大小分别为1和4)。
// 输出调度过程，并计算不同调度算法的周转时间、平均周转时间、带权周转时间、平均带权周转时间、
// 等待时间、平均等待时间等信息。

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#define TRUE 1
#define FALSE 0
#define LEN 5           // 作业数组大小


typedef  struct  Job { 
    char job_pid; 		// 任务号
    int arrive_time;	// 到达时间
    int require_time;	// 要求服务时间
    int used_time;      // 已用时间
    int ended_time;		// 完成时间
    int cycle_time;	    // 周转时间
    int waited_time;	// 等待时间
    float w_cycle_time;	// 带权周转时间
    int has_serviced;   // 用于RR算法中标记开始服务系统时间
    int servied_time;   // 记录RR算法中开始服务的时间
    struct Job  *next;
} Job; 

/*作业队列的结构体*/
typedef struct linked_queue
{
    Job * front ;        //队头指向结点的指针
    Job * rear;          //队尾指向结点的指针
    int count;           //队列当前长度
}linked_queue;

// 作业数组全局变量
struct Job job_array[LEN];

// 后备队列
linked_queue * created_queue;
// 就绪队列
linked_queue * ready_queue;
// 完成队列
linked_queue * ended_queue;

// 初始化作业数组的每个作业信息
void init_jobs();
// 初始化各个队列
void init_queues();
// 打印菜单
void print_menu();
// 先来先服务调度算法 (FCFS)
void fcfs_jobs();
// 短作业优先调度算法 (SJF)
void sjf_jobs();
// 轮转法(RR)
void rr_jobs(int q);
// 最高响应比调度算法 (HRRN)
void hrrn_jobs();

// 打印输出所有作业的各种时间平均值
void print_average_value();
int is_queue_empty(linked_queue * queue);
// 作业出队列函数
Job * de_queue(linked_queue * queue);
// 返回队头作业(不出队)
Job * peek_queue(linked_queue * queue);
// 计算该作业的各种时间
void record_job_time(Job * record_job);
// 作业节点入队
void en_queue_node(linked_queue * queue, Job * en_queue_pcb_node);
// 得到需求时间最短的作业
Job * get_shortest_job(linked_queue *queue);
// 得到最高响应比作业
Job * get_high_response_job(linked_queue *queue, int system_time);

/* 程序入口 */
int main()
{
    // 记录用户键盘输入的选择键
    char user_opt;
    // 打印菜单
    print_menu();
    // 初始化作业数组中每个作业
    init_jobs();
    // 初始化各个队列
    init_queues();

    scanf("%c", &user_opt);
    getchar();
    switch (user_opt)
    {
    case '1':
        fcfs_jobs();
        break;
    case '2':
        sjf_jobs();
        break;
    case '3':
        rr_jobs(1);
        break;
    case '4':
        rr_jobs(4);
        break;
    case '5':
        hrrn_jobs();
        break;
    case 'q':
        exit(0);
        break;
    }
    
    return 0;
}

void print_menu()
{
    printf("\n======================\n");
    printf("按1键FCFS\n");
    printf("按2键SJF \n");
    printf("按3键RR时间片轮转(q=1)\n");
    printf("按4键RR时间片轮转(q=4)\n");
    printf("按5键HRRN\n");
    printf("按q键退出 \n");
    printf("您的选择: ");
    printf("\n======================\n");
}

void init_jobs() 
{
    // 根据题目要求初始化每个进程的信息
    job_array[0].job_pid = 'A'; job_array[0].arrive_time = 0; job_array[0].require_time = 3;job_array[0].has_serviced = FALSE;
    job_array[1].job_pid = 'B'; job_array[1].arrive_time = 2; job_array[1].require_time = 6;job_array[1].has_serviced = FALSE;
    job_array[2].job_pid = 'C'; job_array[2].arrive_time = 4; job_array[2].require_time = 4;job_array[2].has_serviced = FALSE;
    job_array[3].job_pid = 'D'; job_array[3].arrive_time = 6; job_array[3].require_time = 5;job_array[3].has_serviced = FALSE;
    job_array[4].job_pid = 'E'; job_array[4].arrive_time = 8; job_array[4].require_time = 2;job_array[4].has_serviced = FALSE;
}

void init_queues()
{
    // 释放原队列
    free(created_queue);
    free(ready_queue);
    free(ended_queue);
    // 后备队列 并初始化
    created_queue = (linked_queue *) malloc(sizeof(linked_queue));
    created_queue->front = created_queue->rear = NULL;
    // 就绪队列 并初始化
    ready_queue = (linked_queue *) malloc(sizeof(linked_queue));
    ready_queue->front = ready_queue->rear = NULL;
    // 完成队列 并初始化
    ended_queue = (linked_queue *) malloc(sizeof(linked_queue));
    ended_queue->front = ended_queue->rear = NULL;

    // 将作业数组中所有作业放入后备队列
    int i;
    for ( i = 0; i < LEN; i++)
        en_queue_node(created_queue, &job_array[i]);
}

void record_job_time(Job * record_job)
{
    // 计算该作业的各种时间
    record_job->cycle_time = record_job->ended_time - record_job->arrive_time;
    record_job->waited_time = record_job->cycle_time - record_job->require_time;
    record_job->w_cycle_time = (float) record_job->cycle_time / (float) record_job->require_time;
}

void print_average_value()
{
    // 平均周转时间
    float avg_cycle_time = 0;
    // 平均等待时间
    float avg_waited_time = 0;
    // 平均带权周转时间
    float avg_w_cycle_time = 0;
    // 遍历作业数组, 求和
    int i;
    for ( i = 0; i < LEN; i++)
    {
        avg_cycle_time      += (float) job_array[i].cycle_time;
        avg_waited_time     += (float) job_array[i].waited_time;
        avg_w_cycle_time    += job_array[i].w_cycle_time;
    }
    // 计算均值
    avg_cycle_time      /= (float) LEN;
    avg_waited_time     /= (float) LEN;
    avg_w_cycle_time    /= (float) LEN;

    printf("平均周转时间:%5.2f 平均等待时间:%5.2f 平均带权周转时间%5.2f\n",avg_cycle_time, avg_waited_time, avg_w_cycle_time);

}

void add_new_job(linked_queue * created_queue, int system_time)
{
    // 判断当前系统时间, 将后备队列中到达系统的作业链入就绪队列
    while (!is_queue_empty(created_queue))
    {
        Job * front_job = peek_queue(created_queue);
        if (front_job->arrive_time > system_time)
        {
            // 如果队头的到达系统时间比当前系统时间晚则退出循环
            break;
        }
        else
        {
            // 把后备队列的队头出队, 然后将该作业入就绪队列
            en_queue_node(ready_queue, de_queue(created_queue));
        }
    }    
}

void fcfs_jobs() 
{
    // 先来先服务算法
    int system_time = 0;        // 系统当前时间
    Job * running_job = NULL;   // 当前正在使用处理机的作业
    // 当后备作业队列不为空时
    // 或就绪队列不为空时
    // 或当前有作业正在使用处理机时
    // 进行作业调度
    while (!is_queue_empty(created_queue) || !is_queue_empty(ready_queue) || running_job != NULL)
    {
        // 判断当前系统时间, 将后备队列中到达系统的作业链入就绪队列
        add_new_job(created_queue, system_time);

        // 判断当前是否有作业正在使用处理机
        if (running_job == NULL)
        {
            // 无作业正在使用处理机
            // 当前就绪队列不为空时, 则从就绪队列出队一个作业去使用处理机
            if (!is_queue_empty(ready_queue))
                running_job = de_queue(ready_queue);
            else
            {
                // 就绪队列为空时, 系统时间步进一个时间单位, 退出该层调度的循环
                printf("系统%d时刻, 就绪队列为空, 处理机空闲\n", system_time);
                // 系统向前推进一个时间单位
                ++system_time;
                continue;
            }
        }
        // 当前有一个作业正在使用处理机
        else
        {
            // 如果当前有作业正在使用处理机
            // 判断其使用处理机的时间已经满足其要求服务时间
            if (running_job->used_time == running_job->require_time)
            {
                // 该作业要求服务时间已满足
                // 记录该作业完成时间
                running_job->ended_time = system_time;
                // 计算其各种时间(周转时间、等待时间、带权周转时间)
                record_job_time(running_job);
                // 将该作业链入完成队列
                en_queue_node(ended_queue, running_job);
                printf("作业号:%3c 到达时间:%3d 响应时间:%3d 完成时间:%3d 周转时间:%3d 带权周转时间:%.2f\n", 
                                running_job->job_pid, running_job->arrive_time,
                                running_job->ended_time - running_job->require_time, running_job->ended_time,
                                running_job->cycle_time, running_job->w_cycle_time);
                // 调度新的作业使用处理机
                if (!is_queue_empty(ready_queue))
                    running_job = de_queue(ready_queue);
                else
                    running_job = NULL;
            }
        }
        
        // 系统时间步进
        system_time++;
        // 正在运行的作业的使用处理机时间 +1
        if (running_job != NULL)
            running_job->used_time++;
    }
    printf("FCFS算法的调度信息:\n");
    printf("--------------------\n");
    print_average_value();
    printf("--------------------\n");
    
}

void sjf_jobs()
{   // 短作业优先算法
    int system_time = 0;        // 系统当前时间
    Job * running_job = NULL;   // 当前正在使用处理机的作业
    /* 当后备作业队列不为空时 */
    /* 或就绪队列不为空时 */
    /* 或当前有作业正在使用处理机时 */
    /* 进行作业调度 */
    while (!is_queue_empty(created_queue) || !is_queue_empty(ready_queue) || running_job != NULL)
    {
        // 判断当前系统时间, 将后备队列中到达系统的作业链入就绪队列
        add_new_job(created_queue, system_time);

        // 判断当前是否有作业正在使用处理机
        if (running_job == NULL)
        {
            // 无作业正在使用处理机
            // 当前就绪队列不为空时, 则将就绪队列出队一个所需时间最短的作业去使用处理机
            if (!is_queue_empty(ready_queue))
                running_job = get_shortest_job(ready_queue);
            else
            {
                // 就绪队列为空时, 系统时间步进一个时间单位, 退出该层调度的循环
                printf("系统%d时刻, 就绪队列为空, 处理机空闲\n", system_time);
                // 系统向前推进一个时间单位
                ++system_time;
                continue;
            }
        }
        else
        {   // 当前有一个作业正在使用处理机
            // 则判断其使用处理机的时间已经满足其要求服务时间
            if (running_job->used_time == running_job->require_time)
            {
                // 该作业要求服务时间已满足
                // 记录该作业完成时间
                running_job->ended_time = system_time;
                // 计算其各种时间(周转时间、等待时间、带权周转时间)
                record_job_time(running_job);
                // 将该作业链入完成队列
                en_queue_node(ended_queue, running_job);
                printf("作业号:%3c 到达时间:%3d 响应时间:%3d 完成时间:%3d 周转时间:%3d 带权周转时间:%.2f\n", 
                                running_job->job_pid, running_job->arrive_time, 
                                running_job->ended_time - running_job->require_time, running_job->ended_time,
                                running_job->cycle_time, running_job->w_cycle_time);
                // 调度新的作业使用处理机
                if (!is_queue_empty(ready_queue))
                    running_job = get_shortest_job(ready_queue);
                else
                    running_job = NULL;
            }
        }
        
        // 系统时间步进
        system_time++;
        // 正在运行的作业的使用处理机时间 +1
        if (running_job != NULL)
            running_job->used_time++;
    }
    printf("SJF算法的调度信息:\n");
    printf("--------------------\n");
    print_average_value();
    printf("--------------------\n");
    

}

void rr_jobs(int q)
{   // 时间片轮转RR算法, 传入时间片参数q
    int system_time = 0;        // 系统当前时间
    Job * running_job = NULL;          // 当前正在使用处理机的作业
    while (!is_queue_empty(created_queue) || !is_queue_empty(ready_queue) || running_job != NULL)
    {
        // 判断当前系统时间, 将后备队列中到达系统的作业链入就绪队列
        add_new_job(created_queue,system_time);
 	    if(running_job != NULL)
    	{   // 当前有作业正在使用处理机
            // 则将此作业重新入就绪队列, 并将当前作业置空
    		en_queue_node(ready_queue, running_job);
    		running_job = NULL;	
    	}
    	int used_time_slice = 0, flag = TRUE;
    	while(flag) 
    	{
    		if(running_job == NULL)
    		{   // 当前无作业正在使用处理机 
    			if(!is_queue_empty(ready_queue)){ // 如果就绪队列非空, 则从就绪队列中出队一个作业使用处理机
    				running_job = de_queue(ready_queue);
                    if (!running_job->has_serviced){ // 如果该作业未被服务过, 则将当前系统时间记录下来作为开始服务时间
                        running_job->has_serviced = TRUE;
                        running_job->servied_time = system_time;
                    }
                }
    			else
    			{
    				printf("系统%d时刻, 就绪队列为空, 处理机空闲\n", system_time);
    				++system_time;
    				break;	
    			}	
    		}
    		else
    		{
                // 当前有作业正在使用处理机
                // 时间片步进一个单位
    			used_time_slice++;
                // 系统时间步进一个单位
    			system_time++;
                // 当前作业使用时间步进一个单位
    			running_job->used_time++;
    			if(running_job->used_time == running_job->require_time)
    			{   // 如果当前作业的需求时间已经得到满足
                    // 记录结束时间
    				running_job->ended_time = system_time;
                    // 计算该进程被调度过程中的各种时间
    				record_job_time(running_job);
                    // 将此队列链入完成队列
    				en_queue_node(ended_queue, running_job);
                    // 输出进程调度信息
                printf("作业号:%3c 到达时间:%3d 响应时间:%3d 完成时间:%3d 周转时间:%3d 带权周转时间:%.2f\n", 
                                running_job->job_pid, running_job->arrive_time,
                                running_job->servied_time, running_job->ended_time,
                                running_job->cycle_time, running_job->w_cycle_time);
                    // 将正在使用处理机的作业置为空, 并将flag置为0, 即分配新的时间片
    				running_job = NULL;
                    flag = 0;
    			} else if(used_time_slice == q)
    			{
                    // 当前时间片完, 分配新的时间片
    				flag = 0;
    			}	
    		}	
    	}
    }

    printf("RR算法(时间片为%d)的调度信息:\n",q);
    printf("--------------------\n");
    print_average_value();
    printf("--------------------\n");
}

void hrrn_jobs() 
{   // 最高响应比优先算法 HRRN(非抢占)
    int system_time = 0;        // 系统当前时间
    Job * running_job = NULL;   // 当前正在使用处理机的作业
    /* 当后备作业队列不为空时 */
    /* 或就绪队列不为空时 */
    /* 或当前有作业正在使用处理机时 */
    /* 进行作业调度 */
    while (!is_queue_empty(created_queue) || !is_queue_empty(ready_queue) || running_job != NULL)
    {
        // 判断当前系统时间, 将后备队列中到达系统的作业链入就绪队列
        add_new_job(created_queue, system_time);

        // 判断当前是否有作业正在使用处理机
        if (running_job == NULL)
        {
            // 无作业正在使用处理机
            // 当前就绪队列不为空时, 则将就绪队列出队一个所需时间最短的作业去使用处理机
            if (!is_queue_empty(ready_queue))
                running_job = get_high_response_job(ready_queue, system_time);
            else
            {
                // 就绪队列为空时, 系统时间步进一个时间单位, 退出该层调度的循环
                printf("系统%d时刻, 就绪队列为空, 处理机空闲\n", system_time);
                // 系统向前推进一个时间单位
                ++system_time;
                continue;
            }
        }
        else
        {   // 当前有一个作业正在使用处理机
            // 则判断其使用处理机的时间已经满足其要求服务时间
            if (running_job->used_time == running_job->require_time)
            {
                // 该作业要求服务时间已满足
                // 记录该作业完成时间
                running_job->ended_time = system_time;
                // 计算其各种时间(周转时间、等待时间、带权周转时间)
                record_job_time(running_job);
                // 将该作业链入完成队列
                en_queue_node(ended_queue, running_job);
                printf("作业号:%3c 到达时间:%3d 响应时间:%3d 完成时间:%3d 周转时间:%3d 带权周转时间:%.2f\n", 
                                running_job->job_pid, running_job->arrive_time,
                                running_job->ended_time - running_job->require_time, running_job->ended_time,
                                running_job->cycle_time, running_job->w_cycle_time);
                // 调度新的作业使用处理机
                if (!is_queue_empty(ready_queue))
                    running_job = get_high_response_job(ready_queue, system_time);
                else
                    running_job = NULL;
            }
        }
        
        // 系统时间步进
        system_time++;
        // 正在运行的作业的使用处理机时间 +1
        if (running_job != NULL)
            running_job->used_time++;
    }
    printf("HRRN算法的调度信息:\n");
    printf("--------------------\n");
    print_average_value();
    printf("--------------------\n");    
}

// 作业节点入队
void en_queue_node(linked_queue * queue, Job * en_queue_pcb_node)
{
    // 将传入的Job作业节点入队
    if (is_queue_empty(queue))
    {
        queue->front = en_queue_pcb_node;
        queue->rear = en_queue_pcb_node;
    } else {// 队列非空
        queue->rear->next = en_queue_pcb_node;
        queue->rear = en_queue_pcb_node;
    }
}

// 判断队列是否非空
int is_queue_empty(linked_queue * queue)
{
    if ((queue->front == NULL) && (queue->rear == NULL)) return TRUE;
    else return FALSE;
}

// 队列出队队头元素
Job * de_queue(linked_queue * queue)
{
    Job * return_job;
    if (is_queue_empty(queue))
    {
        printf("队列为空, 无法出队\n");
        return NULL;
    }
    return_job = queue->front;
    if (queue->front == queue->rear)
    {// 只有一个节点
        queue->front = queue->rear =NULL;
    } else {
        // 多于一个节点时
        queue->front = queue->front->next;
    }
    return_job->next = NULL;
    return return_job;
}

// 返回队头作业, 但不出队
Job * peek_queue(linked_queue * queue) 
{
    return queue->front;
}

// 得到队列中需求时间最短的作业
Job * get_shortest_job(linked_queue *queue)
{
    Job *p, *q, *shortest_job, *pre;
    shortest_job = queue->front;
    pre = queue->front;
    for (p = queue->front; p != NULL; p = p->next)
    {
        if (shortest_job->require_time > p->require_time)
        {
            shortest_job = p;
            pre = q;
        }
        q = p;
    }
    if (shortest_job == queue->front)
    {
        if (queue->front == queue->rear)
        {
            queue->rear = NULL;
            queue->front = NULL;
        } else {
            queue->front = queue->front->next;
            shortest_job->next = NULL;            
        }
    } else if (shortest_job == queue->rear)
    {
        queue->rear = pre;
        pre->next = NULL;
    } else
    {
        pre->next = shortest_job->next;
        shortest_job->next = NULL;
    }
    return shortest_job;  
}

// 得到队列中最高响应比的作业
Job * get_high_response_job(linked_queue * queue, int system_time)
{
    Job *p, *q, *shortest_job, *pre;
    shortest_job = queue->front;
    pre = queue->front;
    for (p = queue->front; p != NULL; p = p->next)
    {
        float res1 = (float)(system_time - shortest_job->arrive_time + shortest_job->require_time)/(float)(shortest_job->require_time);
        float res2 = (float)(system_time - p->arrive_time + p->require_time)/(float)(p->require_time);
        if (res1 < res2)
        {
            shortest_job = p;
            pre = q;
        }
        q = p;
    }
    if (shortest_job == queue->front)
    {
        if (queue->front == queue->rear)
        {
            queue->rear = NULL;
            queue->front = NULL;
        } else {
            queue->front = queue->front->next;
            shortest_job->next = NULL;            
        }
    } else if (shortest_job == queue->rear)
    {
        queue->rear = pre;
        pre->next = NULL;
    } else
    {
        pre->next = shortest_job->next;
        shortest_job->next = NULL;
    }
    return shortest_job;  
}
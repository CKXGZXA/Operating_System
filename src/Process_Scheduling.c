// �����ַ:https://gitee.com/ZXA200009/operating_system/blob/master/Process_Scheduling.c
// ģ��ʵ�ֽ��̵��ȵľ����㷨������FCFS��SJF��SPF����HRRN��RR(ʱ��Ƭ��С�ֱ�Ϊ1��4)��
// ������ȹ��̣������㲻ͬ�����㷨����תʱ�䡢ƽ����תʱ�䡢��Ȩ��תʱ�䡢ƽ����Ȩ��תʱ�䡢
// �ȴ�ʱ�䡢ƽ���ȴ�ʱ�����Ϣ��

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#define TRUE 1
#define FALSE 0
#define LEN 5           // ��ҵ�����С


typedef  struct  Job { 
    char job_pid; 		// �����
    int arrive_time;	// ����ʱ��
    int require_time;	// Ҫ�����ʱ��
    int used_time;      // ����ʱ��
    int ended_time;		// ���ʱ��
    int cycle_time;	    // ��תʱ��
    int waited_time;	// �ȴ�ʱ��
    float w_cycle_time;	// ��Ȩ��תʱ��
    int has_serviced;   // ����RR�㷨�б�ǿ�ʼ����ϵͳʱ��
    int servied_time;   // ��¼RR�㷨�п�ʼ�����ʱ��
    struct Job  *next;
} Job; 

/*��ҵ���еĽṹ��*/
typedef struct linked_queue
{
    Job * front ;        //��ͷָ�����ָ��
    Job * rear;          //��βָ�����ָ��
    int count;           //���е�ǰ����
}linked_queue;

// ��ҵ����ȫ�ֱ���
struct Job job_array[LEN];

// �󱸶���
linked_queue * created_queue;
// ��������
linked_queue * ready_queue;
// ��ɶ���
linked_queue * ended_queue;

// ��ʼ����ҵ�����ÿ����ҵ��Ϣ
void init_jobs();
// ��ʼ����������
void init_queues();
// ��ӡ�˵�
void print_menu();
// �����ȷ�������㷨 (FCFS)
void fcfs_jobs();
// ����ҵ���ȵ����㷨 (SJF)
void sjf_jobs();
// ��ת��(RR)
void rr_jobs(int q);
// �����Ӧ�ȵ����㷨 (HRRN)
void hrrn_jobs();

// ��ӡ���������ҵ�ĸ���ʱ��ƽ��ֵ
void print_average_value();
int is_queue_empty(linked_queue * queue);
// ��ҵ�����к���
Job * de_queue(linked_queue * queue);
// ���ض�ͷ��ҵ(������)
Job * peek_queue(linked_queue * queue);
// �������ҵ�ĸ���ʱ��
void record_job_time(Job * record_job);
// ��ҵ�ڵ����
void en_queue_node(linked_queue * queue, Job * en_queue_pcb_node);
// �õ�����ʱ����̵���ҵ
Job * get_shortest_job(linked_queue *queue);
// �õ������Ӧ����ҵ
Job * get_high_response_job(linked_queue *queue, int system_time);

/* ������� */
int main()
{
    // ��¼�û����������ѡ���
    char user_opt;
    // ��ӡ�˵�
    print_menu();
    // ��ʼ����ҵ������ÿ����ҵ
    init_jobs();
    // ��ʼ����������
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
    printf("��1��FCFS\n");
    printf("��2��SJF \n");
    printf("��3��RRʱ��Ƭ��ת(q=1)\n");
    printf("��4��RRʱ��Ƭ��ת(q=4)\n");
    printf("��5��HRRN\n");
    printf("��q���˳� \n");
    printf("����ѡ��: ");
    printf("\n======================\n");
}

void init_jobs() 
{
    // ������ĿҪ���ʼ��ÿ�����̵���Ϣ
    job_array[0].job_pid = 'A'; job_array[0].arrive_time = 0; job_array[0].require_time = 3;job_array[0].has_serviced = FALSE;
    job_array[1].job_pid = 'B'; job_array[1].arrive_time = 2; job_array[1].require_time = 6;job_array[1].has_serviced = FALSE;
    job_array[2].job_pid = 'C'; job_array[2].arrive_time = 4; job_array[2].require_time = 4;job_array[2].has_serviced = FALSE;
    job_array[3].job_pid = 'D'; job_array[3].arrive_time = 6; job_array[3].require_time = 5;job_array[3].has_serviced = FALSE;
    job_array[4].job_pid = 'E'; job_array[4].arrive_time = 8; job_array[4].require_time = 2;job_array[4].has_serviced = FALSE;
}

void init_queues()
{
    // �ͷ�ԭ����
    free(created_queue);
    free(ready_queue);
    free(ended_queue);
    // �󱸶��� ����ʼ��
    created_queue = (linked_queue *) malloc(sizeof(linked_queue));
    created_queue->front = created_queue->rear = NULL;
    // �������� ����ʼ��
    ready_queue = (linked_queue *) malloc(sizeof(linked_queue));
    ready_queue->front = ready_queue->rear = NULL;
    // ��ɶ��� ����ʼ��
    ended_queue = (linked_queue *) malloc(sizeof(linked_queue));
    ended_queue->front = ended_queue->rear = NULL;

    // ����ҵ������������ҵ����󱸶���
    int i;
    for ( i = 0; i < LEN; i++)
        en_queue_node(created_queue, &job_array[i]);
}

void record_job_time(Job * record_job)
{
    // �������ҵ�ĸ���ʱ��
    record_job->cycle_time = record_job->ended_time - record_job->arrive_time;
    record_job->waited_time = record_job->cycle_time - record_job->require_time;
    record_job->w_cycle_time = (float) record_job->cycle_time / (float) record_job->require_time;
}

void print_average_value()
{
    // ƽ����תʱ��
    float avg_cycle_time = 0;
    // ƽ���ȴ�ʱ��
    float avg_waited_time = 0;
    // ƽ����Ȩ��תʱ��
    float avg_w_cycle_time = 0;
    // ������ҵ����, ���
    int i;
    for ( i = 0; i < LEN; i++)
    {
        avg_cycle_time      += (float) job_array[i].cycle_time;
        avg_waited_time     += (float) job_array[i].waited_time;
        avg_w_cycle_time    += job_array[i].w_cycle_time;
    }
    // �����ֵ
    avg_cycle_time      /= (float) LEN;
    avg_waited_time     /= (float) LEN;
    avg_w_cycle_time    /= (float) LEN;

    printf("ƽ����תʱ��:%5.2f ƽ���ȴ�ʱ��:%5.2f ƽ����Ȩ��תʱ��%5.2f\n",avg_cycle_time, avg_waited_time, avg_w_cycle_time);

}

void add_new_job(linked_queue * created_queue, int system_time)
{
    // �жϵ�ǰϵͳʱ��, ���󱸶����е���ϵͳ����ҵ�����������
    while (!is_queue_empty(created_queue))
    {
        Job * front_job = peek_queue(created_queue);
        if (front_job->arrive_time > system_time)
        {
            // �����ͷ�ĵ���ϵͳʱ��ȵ�ǰϵͳʱ�������˳�ѭ��
            break;
        }
        else
        {
            // �Ѻ󱸶��еĶ�ͷ����, Ȼ�󽫸���ҵ���������
            en_queue_node(ready_queue, de_queue(created_queue));
        }
    }    
}

void fcfs_jobs() 
{
    // �����ȷ����㷨
    int system_time = 0;        // ϵͳ��ǰʱ��
    Job * running_job = NULL;   // ��ǰ����ʹ�ô��������ҵ
    // ������ҵ���в�Ϊ��ʱ
    // ��������в�Ϊ��ʱ
    // ��ǰ����ҵ����ʹ�ô����ʱ
    // ������ҵ����
    while (!is_queue_empty(created_queue) || !is_queue_empty(ready_queue) || running_job != NULL)
    {
        // �жϵ�ǰϵͳʱ��, ���󱸶����е���ϵͳ����ҵ�����������
        add_new_job(created_queue, system_time);

        // �жϵ�ǰ�Ƿ�����ҵ����ʹ�ô����
        if (running_job == NULL)
        {
            // ����ҵ����ʹ�ô����
            // ��ǰ�������в�Ϊ��ʱ, ��Ӿ������г���һ����ҵȥʹ�ô����
            if (!is_queue_empty(ready_queue))
                running_job = de_queue(ready_queue);
            else
            {
                // ��������Ϊ��ʱ, ϵͳʱ�䲽��һ��ʱ�䵥λ, �˳��ò���ȵ�ѭ��
                printf("ϵͳ%dʱ��, ��������Ϊ��, ���������\n", system_time);
                // ϵͳ��ǰ�ƽ�һ��ʱ�䵥λ
                ++system_time;
                continue;
            }
        }
        // ��ǰ��һ����ҵ����ʹ�ô����
        else
        {
            // �����ǰ����ҵ����ʹ�ô����
            // �ж���ʹ�ô������ʱ���Ѿ�������Ҫ�����ʱ��
            if (running_job->used_time == running_job->require_time)
            {
                // ����ҵҪ�����ʱ��������
                // ��¼����ҵ���ʱ��
                running_job->ended_time = system_time;
                // ���������ʱ��(��תʱ�䡢�ȴ�ʱ�䡢��Ȩ��תʱ��)
                record_job_time(running_job);
                // ������ҵ������ɶ���
                en_queue_node(ended_queue, running_job);
                printf("��ҵ��:%3c ����ʱ��:%3d ��Ӧʱ��:%3d ���ʱ��:%3d ��תʱ��:%3d ��Ȩ��תʱ��:%.2f\n", 
                                running_job->job_pid, running_job->arrive_time,
                                running_job->ended_time - running_job->require_time, running_job->ended_time,
                                running_job->cycle_time, running_job->w_cycle_time);
                // �����µ���ҵʹ�ô����
                if (!is_queue_empty(ready_queue))
                    running_job = de_queue(ready_queue);
                else
                    running_job = NULL;
            }
        }
        
        // ϵͳʱ�䲽��
        system_time++;
        // �������е���ҵ��ʹ�ô����ʱ�� +1
        if (running_job != NULL)
            running_job->used_time++;
    }
    printf("FCFS�㷨�ĵ�����Ϣ:\n");
    printf("--------------------\n");
    print_average_value();
    printf("--------------------\n");
    
}

void sjf_jobs()
{   // ����ҵ�����㷨
    int system_time = 0;        // ϵͳ��ǰʱ��
    Job * running_job = NULL;   // ��ǰ����ʹ�ô��������ҵ
    /* ������ҵ���в�Ϊ��ʱ */
    /* ��������в�Ϊ��ʱ */
    /* ��ǰ����ҵ����ʹ�ô����ʱ */
    /* ������ҵ���� */
    while (!is_queue_empty(created_queue) || !is_queue_empty(ready_queue) || running_job != NULL)
    {
        // �жϵ�ǰϵͳʱ��, ���󱸶����е���ϵͳ����ҵ�����������
        add_new_job(created_queue, system_time);

        // �жϵ�ǰ�Ƿ�����ҵ����ʹ�ô����
        if (running_job == NULL)
        {
            // ����ҵ����ʹ�ô����
            // ��ǰ�������в�Ϊ��ʱ, �򽫾������г���һ������ʱ����̵���ҵȥʹ�ô����
            if (!is_queue_empty(ready_queue))
                running_job = get_shortest_job(ready_queue);
            else
            {
                // ��������Ϊ��ʱ, ϵͳʱ�䲽��һ��ʱ�䵥λ, �˳��ò���ȵ�ѭ��
                printf("ϵͳ%dʱ��, ��������Ϊ��, ���������\n", system_time);
                // ϵͳ��ǰ�ƽ�һ��ʱ�䵥λ
                ++system_time;
                continue;
            }
        }
        else
        {   // ��ǰ��һ����ҵ����ʹ�ô����
            // ���ж���ʹ�ô������ʱ���Ѿ�������Ҫ�����ʱ��
            if (running_job->used_time == running_job->require_time)
            {
                // ����ҵҪ�����ʱ��������
                // ��¼����ҵ���ʱ��
                running_job->ended_time = system_time;
                // ���������ʱ��(��תʱ�䡢�ȴ�ʱ�䡢��Ȩ��תʱ��)
                record_job_time(running_job);
                // ������ҵ������ɶ���
                en_queue_node(ended_queue, running_job);
                printf("��ҵ��:%3c ����ʱ��:%3d ��Ӧʱ��:%3d ���ʱ��:%3d ��תʱ��:%3d ��Ȩ��תʱ��:%.2f\n", 
                                running_job->job_pid, running_job->arrive_time, 
                                running_job->ended_time - running_job->require_time, running_job->ended_time,
                                running_job->cycle_time, running_job->w_cycle_time);
                // �����µ���ҵʹ�ô����
                if (!is_queue_empty(ready_queue))
                    running_job = get_shortest_job(ready_queue);
                else
                    running_job = NULL;
            }
        }
        
        // ϵͳʱ�䲽��
        system_time++;
        // �������е���ҵ��ʹ�ô����ʱ�� +1
        if (running_job != NULL)
            running_job->used_time++;
    }
    printf("SJF�㷨�ĵ�����Ϣ:\n");
    printf("--------------------\n");
    print_average_value();
    printf("--------------------\n");
    

}

void rr_jobs(int q)
{   // ʱ��Ƭ��תRR�㷨, ����ʱ��Ƭ����q
    int system_time = 0;        // ϵͳ��ǰʱ��
    Job * running_job = NULL;          // ��ǰ����ʹ�ô��������ҵ
    while (!is_queue_empty(created_queue) || !is_queue_empty(ready_queue) || running_job != NULL)
    {
        // �жϵ�ǰϵͳʱ��, ���󱸶����е���ϵͳ����ҵ�����������
        add_new_job(created_queue,system_time);
 	    if(running_job != NULL)
    	{   // ��ǰ����ҵ����ʹ�ô����
            // �򽫴���ҵ�������������, ������ǰ��ҵ�ÿ�
    		en_queue_node(ready_queue, running_job);
    		running_job = NULL;	
    	}
    	int used_time_slice = 0, flag = TRUE;
    	while(flag) 
    	{
    		if(running_job == NULL)
    		{   // ��ǰ����ҵ����ʹ�ô���� 
    			if(!is_queue_empty(ready_queue)){ // ����������зǿ�, ��Ӿ��������г���һ����ҵʹ�ô����
    				running_job = de_queue(ready_queue);
                    if (!running_job->has_serviced){ // �������ҵδ�������, �򽫵�ǰϵͳʱ���¼������Ϊ��ʼ����ʱ��
                        running_job->has_serviced = TRUE;
                        running_job->servied_time = system_time;
                    }
                }
    			else
    			{
    				printf("ϵͳ%dʱ��, ��������Ϊ��, ���������\n", system_time);
    				++system_time;
    				break;	
    			}	
    		}
    		else
    		{
                // ��ǰ����ҵ����ʹ�ô����
                // ʱ��Ƭ����һ����λ
    			used_time_slice++;
                // ϵͳʱ�䲽��һ����λ
    			system_time++;
                // ��ǰ��ҵʹ��ʱ�䲽��һ����λ
    			running_job->used_time++;
    			if(running_job->used_time == running_job->require_time)
    			{   // �����ǰ��ҵ������ʱ���Ѿ��õ�����
                    // ��¼����ʱ��
    				running_job->ended_time = system_time;
                    // ����ý��̱����ȹ����еĸ���ʱ��
    				record_job_time(running_job);
                    // ���˶���������ɶ���
    				en_queue_node(ended_queue, running_job);
                    // ������̵�����Ϣ
                printf("��ҵ��:%3c ����ʱ��:%3d ��Ӧʱ��:%3d ���ʱ��:%3d ��תʱ��:%3d ��Ȩ��תʱ��:%.2f\n", 
                                running_job->job_pid, running_job->arrive_time,
                                running_job->servied_time, running_job->ended_time,
                                running_job->cycle_time, running_job->w_cycle_time);
                    // ������ʹ�ô��������ҵ��Ϊ��, ����flag��Ϊ0, �������µ�ʱ��Ƭ
    				running_job = NULL;
                    flag = 0;
    			} else if(used_time_slice == q)
    			{
                    // ��ǰʱ��Ƭ��, �����µ�ʱ��Ƭ
    				flag = 0;
    			}	
    		}	
    	}
    }

    printf("RR�㷨(ʱ��ƬΪ%d)�ĵ�����Ϣ:\n",q);
    printf("--------------------\n");
    print_average_value();
    printf("--------------------\n");
}

void hrrn_jobs() 
{   // �����Ӧ�������㷨 HRRN(����ռ)
    int system_time = 0;        // ϵͳ��ǰʱ��
    Job * running_job = NULL;   // ��ǰ����ʹ�ô��������ҵ
    /* ������ҵ���в�Ϊ��ʱ */
    /* ��������в�Ϊ��ʱ */
    /* ��ǰ����ҵ����ʹ�ô����ʱ */
    /* ������ҵ���� */
    while (!is_queue_empty(created_queue) || !is_queue_empty(ready_queue) || running_job != NULL)
    {
        // �жϵ�ǰϵͳʱ��, ���󱸶����е���ϵͳ����ҵ�����������
        add_new_job(created_queue, system_time);

        // �жϵ�ǰ�Ƿ�����ҵ����ʹ�ô����
        if (running_job == NULL)
        {
            // ����ҵ����ʹ�ô����
            // ��ǰ�������в�Ϊ��ʱ, �򽫾������г���һ������ʱ����̵���ҵȥʹ�ô����
            if (!is_queue_empty(ready_queue))
                running_job = get_high_response_job(ready_queue, system_time);
            else
            {
                // ��������Ϊ��ʱ, ϵͳʱ�䲽��һ��ʱ�䵥λ, �˳��ò���ȵ�ѭ��
                printf("ϵͳ%dʱ��, ��������Ϊ��, ���������\n", system_time);
                // ϵͳ��ǰ�ƽ�һ��ʱ�䵥λ
                ++system_time;
                continue;
            }
        }
        else
        {   // ��ǰ��һ����ҵ����ʹ�ô����
            // ���ж���ʹ�ô������ʱ���Ѿ�������Ҫ�����ʱ��
            if (running_job->used_time == running_job->require_time)
            {
                // ����ҵҪ�����ʱ��������
                // ��¼����ҵ���ʱ��
                running_job->ended_time = system_time;
                // ���������ʱ��(��תʱ�䡢�ȴ�ʱ�䡢��Ȩ��תʱ��)
                record_job_time(running_job);
                // ������ҵ������ɶ���
                en_queue_node(ended_queue, running_job);
                printf("��ҵ��:%3c ����ʱ��:%3d ��Ӧʱ��:%3d ���ʱ��:%3d ��תʱ��:%3d ��Ȩ��תʱ��:%.2f\n", 
                                running_job->job_pid, running_job->arrive_time,
                                running_job->ended_time - running_job->require_time, running_job->ended_time,
                                running_job->cycle_time, running_job->w_cycle_time);
                // �����µ���ҵʹ�ô����
                if (!is_queue_empty(ready_queue))
                    running_job = get_high_response_job(ready_queue, system_time);
                else
                    running_job = NULL;
            }
        }
        
        // ϵͳʱ�䲽��
        system_time++;
        // �������е���ҵ��ʹ�ô����ʱ�� +1
        if (running_job != NULL)
            running_job->used_time++;
    }
    printf("HRRN�㷨�ĵ�����Ϣ:\n");
    printf("--------------------\n");
    print_average_value();
    printf("--------------------\n");    
}

// ��ҵ�ڵ����
void en_queue_node(linked_queue * queue, Job * en_queue_pcb_node)
{
    // �������Job��ҵ�ڵ����
    if (is_queue_empty(queue))
    {
        queue->front = en_queue_pcb_node;
        queue->rear = en_queue_pcb_node;
    } else {// ���зǿ�
        queue->rear->next = en_queue_pcb_node;
        queue->rear = en_queue_pcb_node;
    }
}

// �ж϶����Ƿ�ǿ�
int is_queue_empty(linked_queue * queue)
{
    if ((queue->front == NULL) && (queue->rear == NULL)) return TRUE;
    else return FALSE;
}

// ���г��Ӷ�ͷԪ��
Job * de_queue(linked_queue * queue)
{
    Job * return_job;
    if (is_queue_empty(queue))
    {
        printf("����Ϊ��, �޷�����\n");
        return NULL;
    }
    return_job = queue->front;
    if (queue->front == queue->rear)
    {// ֻ��һ���ڵ�
        queue->front = queue->rear =NULL;
    } else {
        // ����һ���ڵ�ʱ
        queue->front = queue->front->next;
    }
    return_job->next = NULL;
    return return_job;
}

// ���ض�ͷ��ҵ, ��������
Job * peek_queue(linked_queue * queue) 
{
    return queue->front;
}

// �õ�����������ʱ����̵���ҵ
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

// �õ������������Ӧ�ȵ���ҵ
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
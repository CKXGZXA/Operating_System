#include <stdio.h>
#include <malloc.h>
#define MAX 10

typedef struct memblock{
    int head;                //��ַ
    int length;               //����
    // int state;                //״̬��0��ʾ���У�1��ʾռ��
    // int jobid;                //�ѷ��䣬��¼��ҵID������Ϊ0
}memblock;

typedef struct 
{
    int jobid;
    int memory;
    int mem_no;
}Job;



int N;                          // �����ڴ���Ŀ
memblock free_mem[MAX];         // ���з�������
// memblock backup[MAX];           // ��������,���ڻ�ԭ��ʼ�����ڴ�״̬
struct{                 // ��ҵ���׶���
    Job jobs[MAX];
    int front;
    int rear;
    int M;                          // ��������ڴ���ҵ��Ŀ
}job_queue;                         

// ��ʼ�����з���
void init_memory();

// ��������ڴ��������Ϣ
void print_Mem_info();

// ��ʼ��������ҵ
void init_jobs();

// �״���Ӧ�㷨
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

// ѭ���״���Ӧ�㷨
void Next_Fit()
{
    int p = 0;  // ��¼��һ�η����ڴ��λ��
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

// �����Ӧ�㷨
void Best_Fit()
{  
    Sort_Freemem(0); // ���ڴ水��С��С�����������
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
        Sort_Freemem(0); // ��������������� 
    }
}

void Worst_Fit()
{
    Sort_Freemem(1); // ���ڴ水��С�Ӵ��С��������
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
        Sort_Freemem(1); // ��������������� 
    }
}


// ����ָ����ҵ���ڴ�ռ�
int Recycling_Mem();

// �ָ���ʼ״̬
void Recover();

// �����з������ռ����ָ��˳������
void Sort_Freemem(int op);

// ��ӡ�˵�
void print_menu();

int main()
{
    printf("*************�ڴ����ͻ���***********\n");
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

// ��ӡ�˵�
void print_menu()
{
    printf("\n=================================\n");
    printf("0. �˳�\n");
    printf("1. �״���Ӧ�㷨Ϊ������ҵ�����ڴ�ռ�\n");
    printf("2. ѭ���״���Ӧ�㷨Ϊ������ҵ�����ڴ�ռ�\n");
    printf("3. �����Ӧ�㷨Ϊ������ҵ�����ڴ�ռ�\n");
    printf("4. ���Ӧ�㷨Ϊ������ҵ�����ڴ�ռ�\n");
    printf("5. ����ָ����ҵ���ڴ�ռ�\n");
    printf("6. ��ʾ��ǰ�ڴ�������\n");
    printf("7. ��ԭ��ʼ״̬\n");
    printf("=================================\n");
    printf("������������ѡ��:");
}

// ��ʼ�����з���
void init_memory()
{
    int n;
    printf("�������ڴ��������:");
    scanf("%d", &N);
    printf("����������з�����С���ڴ���ַ:\n");    
    for(int i = 0;i < N; ++i)
    {
        // ��ʼ�����з����ĸ�����Ϣ
        printf("������%d:",i);
        scanf("%d %d",&free_mem[i].length, &free_mem[i].head);
        // backup[i].length = free_mem[i].length;
        // backup[i].head = free_mem[i].head;
    }
} 

// ��ʼ��������ҵ
void init_jobs()
{
    job_queue.front = job_queue.rear = 0;
    printf("\n=================================\n");   
    printf("========��ʼ��������ҵ============\n"); 
    printf("�������������ҵ����:");
    scanf("%d", &job_queue.M);
    printf("��������ҵID�Լ�������ڴ��С:\n");
    for (int i = 0; i < job_queue.M; ++i)
    {
        printf("��ҵ%d:",i+1);
        scanf("%d%d",&job_queue.jobs[job_queue.rear].jobid,&job_queue.jobs[job_queue.rear].memory);
        job_queue.jobs[job_queue.rear].mem_no = -1;     //��ʼ��Ϊ-1, ��ʾδ����
        ++job_queue.rear;
    }  
}

// ��������ڴ��������Ϣ
void print_Mem_info()
{
    int seq;
    printf("\n=========�����ڴ��������Ϣ===============\n");
    printf("\n������\t������С(KB)\t����ʼַ\n");
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
    printf("��������Ҫ�ͷ��ڴ�Ľ���ID:");
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
            printf("�ý���δռ���ڴ�������ڴ�ʧ��!");
            return 1;
        }
    }
    printf("�ý���ID������!");
    return 0;
}

// ������ָ��Ƚϴ�С, ���opΪ0�򷵻�x>y��ֵ, ���opΪ1�򷵻�x<y��ֵ
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
#include <stdio.h>
#define TRUE 1
#define FALSE 0
#define M 10            // Ԥ�������Դ����
#define N 10            // Ԥ����������Ŀ

int n;                  // ������Ŀ
int m;                  // ������Դ����
int Available[M];       // ��������Դ����
int Max[N][M];          // ����������
int Allocation[N][M];   // �������
int Need[N][M];         // �������
int Request[M];         // ���̵���������

// ��ʼ����Դ����
void init_status()
{
    int i, j;
    printf("��������������Ŀ�Ϳ�����Դ������:");
    scanf("%d%d",&n,&m);
    printf("�����������Դ��ϵͳ�п�����Ŀ:");
    for(i = 0;i<m;++i)
        scanf("%d",&Available[i]);
    printf("��������̶Ը���Դ���������:\n");
    for ( i = 0; i < n; ++i)
    {
        printf("����P%d:",i);
        for ( j = 0; j < m; j++)
            scanf("%d", &Max[i][j]);       
    }
    printf("������������:\n");
    for (i = 0; i < n; ++i)
    {
        printf("P%d:",i);
        for (j = 0; j < m; ++j)
            scanf("%d",&Allocation[i][j]);
    }
    // ��ʼ���������
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
            Need[i][j] = Max[i][j] - Allocation[i][j];
    }
    
}

// �ж�a�����Ƿ���ȫС��b����, ���Ƿ���true, ���򷵻�false
int cmp_vector(int * a, int * b);
// ����������Դ�����ļ�����, �������a��
void cal_vector(int * a, int * b, char op);

// ��ȫ���㷨,�ж�ϵͳ�Ƿ��ڰ�ȫ״̬,��ȫ����TRUE
int is_safe();
// ���м��㷨
void banker_algorithm();

int main(int argc, char const *argv[])
{
    printf("--------------����ģ��ʵ�����м��㷨--------------\n");
    // ��ʼ������
    init_status();
    printf("����ѡ��:\n1.�жϴ�����Դ����״̬�Ƿ�ȫ\n2.�ж�ĳ����������Ӧ��Դ��ϵͳ�ܷ���������Դ����\n���������ѡ���ܻ��߼���0�˳�����:");
    int choice;
    scanf("%d",&choice);
    while(choice)
    {   
         switch (choice)
        {
            case 1:
                is_safe();
                break;
            case 2:
                banker_algorithm();
        }
        printf("����ѡ��:\n1.�жϴ�����Դ����״̬�Ƿ�ȫ\n2.�ж�ĳ����������Ӧ��Դ��ϵͳ�ܷ���������Դ����\n���������ѡ���ܻ��߼���0�˳�����:");
        scanf("%d",&choice);
    }
    printf("\n--------------�˳�ϵͳ�ɹ�--------------\n");
    return 0;
}


// ��ȫ���㷨,�ж�ϵͳ��ʱ�Ƿ��ڰ�ȫ״̬
int is_safe()
{
    int i;
    int Work_and_Allocation[N][M];
    int Seq[N],s = 0;   // ������ڰ�ȫ�������¼
    int Work[N][M],w = 0;    // ��������Work
    int Finish[N];  // ָʾϵͳ�Ƿ����㹻����Դ���������ʹ֮���
    // ��ʼ��Work����ʹ֮����Available����
    for ( i = 0; i < m; ++i)
        Work[w][i] = Available[i];
    // ��ʼ��Finish����ȫΪFALSE
    for ( i = 0; i < n; ++i)
        Finish[i] = FALSE;
    int flag1 = TRUE;   // ָʾ�Ƿ��ҵ��ܹ�������Դ�Ľ���
    while(flag1)
    {
        int flag2 = TRUE;
        for (i = 0; i < n; ++i)
        {
            if (Finish[i] == FALSE)
            {
                if(cmp_vector(Need[i], Work[w])){
                    {   // ��ϵͳ���㹻����Դ���Է��������
                        // ���ڽ������н������ջ���Դ
                        int j;
                        for ( j = 0; j < m; ++j)
                        {
                            Work_and_Allocation[w][j] = Work[w][j] + Allocation[i][j];
                            Work[w+1][j] = Work_and_Allocation[w][j];
                        }
                        ++w;
                        Finish[i] = TRUE;
                        Seq[s++] = i;
                        flag2 = FALSE;
                        break;  
                    }
                }
            }
        }
        if (flag2)
        {
            for ( i = 0; i < n; ++i)
            {
                if(Finish[i] == FALSE) {
                    break;
                }
            }
            flag1 = FALSE;
        }    
    }
    if (i >= n) {
        printf("ϵͳ�ǰ�ȫ��, ��ȫ��������:\n");
        printf("����\tWork\tNeed\tAllocation\tWork+Allocation\n");
        for(i = 0;i <n;++i)
        {
            printf("P%d\t",Seq[i]);
            for (int j = 0; j < m; ++j)
                printf("%d ",Work[i][j]);
            printf("\t");
            for (int j = 0; j < m; ++j)      
                printf("%d ",Need[Seq[i]][j]);
            printf("\t");
            for (int j = 0; j < m; ++j)      
                printf("%d ",Allocation[Seq[i]][j]);
            printf("\t");
            for (int j = 0; j < m; ++j)      
                printf("%d ",Work_and_Allocation[i][j]);
            printf("\n");
        }
        return TRUE;
    }
    else return FALSE;    
}

// ���м��㷨
void banker_algorithm()
{
    int No;
    printf("��������̺�:");
    scanf("%d",&No);
    printf("���������P%d����������:", No);
    for (int i = 0; i < m; ++i)
        scanf("%d", &Request[i]);
    
    if (cmp_vector(Request, Need[No]))
    {
        if (cmp_vector(Request,Available))
        {   // ��ϵͳ���㹻��Դ���������
            // ����̽�Եؽ���Դ���������
            // ���޸����������ֵ
            cal_vector(Available, Request, '-');
            cal_vector(Allocation[No], Request, '+');
            cal_vector(Need[No], Request, '-');  

            // ���ִ�а�ȫ���㷨, �ж�ϵͳ�Ƿ��ڰ�ȫ״̬
            is_safe();

            // �ָ��������Դ����״̬
            cal_vector(Available, Request, '+');
            cal_vector(Allocation[No], Request, '-');
            cal_vector(Need[No], Request, '+');                
        } else {
            printf("��ʱϵͳ�����㹻��Դ,P%d��ȴ�\n",No);
        }
        
    } else { //����Ҫ����Դ�Ѿ������������������ֵ,����
        printf("ERROR!������Դ����,���������������,�޷�����!");
    }    
}


// �ж�a�����Ƿ���ȫС��b����, ���Ƿ���true, ���򷵻�false
int cmp_vector(int * a, int * b)
{
    int i;
    for (i = 0; i < m; ++i)
        if(a[i] > b[i]) break;
    if(i >= m) return TRUE;
    else return FALSE;
}

// ����������Դ�����ļ�����, �������a��
void cal_vector(int * a, int *b, char op)
{
    switch (op)
    {
    case '+':
        for (int i = 0; i < m; ++i)
            a[i] += b[i];
        break;
    case '-':
        for (int i = 0; i < m; ++i)
            a[i] -= b[i];
    default:
        break;
    }
}
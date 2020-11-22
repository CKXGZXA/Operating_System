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
int cmp_vector(int * a, int * b)
{
    int i;
    for (i = 0; i < m; ++i)
        if(a[i] > b[i]) break;
    if(i >= m) return TRUE;
    else return FALSE;
}

// ��ȫ���㷨,�ж�ϵͳ�Ƿ��ڰ�ȫ״̬
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

void banker_algorithm(int n)
{
    printf("���������P%d����������", n);
    if (cmp_vector(Request[n], Need[n]))
    {
        if (cmp_vector(Request,Available))
        {
            
        }
        
    } else { //����Ҫ����Դ�Ѿ������������������ֵ,����
        printf("ERROR!������Դ����,�޷�����!");
    }    
}

int main(int argc, char const *argv[])
{
    // freopen("in.txt","r",stdin);
    // ��ʼ������
    init_status();
    // �жϰ�ȫ��
    printf("%d",is_safe());
    return 0;
}

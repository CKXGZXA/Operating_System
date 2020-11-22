#include <stdio.h>
#define TRUE 1
#define FALSE 0
#define M 10            // 预设最大资源种类
#define N 10            // 预设最大进程数目

int n;                  // 进程数目
int m;                  // 可用资源种类
int Available[M];       // 可利用资源向量
int Max[N][M];          // 最大需求矩阵
int Allocation[N][M];   // 分配矩阵
int Need[N][M];         // 需求矩阵
int Request[M];         // 进程的请求向量

// 初始化资源分配
void init_status()
{
    int i, j;
    printf("请输入最大进程数目和可用资源种类数:");
    scanf("%d%d",&n,&m);
    printf("请输入各类资源在系统中可用数目:");
    for(i = 0;i<m;++i)
        scanf("%d",&Available[i]);
    printf("输入各进程对各资源的最大需求:\n");
    for ( i = 0; i < n; ++i)
    {
        printf("进程P%d:",i);
        for ( j = 0; j < m; j++)
            scanf("%d", &Max[i][j]);       
    }
    printf("请输入分配矩阵:\n");
    for (i = 0; i < n; ++i)
    {
        printf("P%d:",i);
        for (j = 0; j < m; ++j)
            scanf("%d",&Allocation[i][j]);
    }
    // 初始化需求矩阵
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
            Need[i][j] = Max[i][j] - Allocation[i][j];
    }
    
}

// 判断a向量是否完全小于b向量, 若是返回true, 否则返回false
int cmp_vector(int * a, int * b)
{
    int i;
    for (i = 0; i < m; ++i)
        if(a[i] > b[i]) break;
    if(i >= m) return TRUE;
    else return FALSE;
}

// 安全性算法,判断系统是否处于安全状态
int is_safe()
{
    int i;
    int Work_and_Allocation[N][M];
    int Seq[N],s = 0;   // 如果存在安全序列则记录
    int Work[N][M],w = 0;    // 工作向量Work
    int Finish[N];  // 指示系统是否有足够的资源分配给进程使之完成
    // 初始化Work向量使之等于Available向量
    for ( i = 0; i < m; ++i)
        Work[w][i] = Available[i];
    // 初始化Finish向量全为FALSE
    for ( i = 0; i < n; ++i)
        Finish[i] = FALSE;
    int flag1 = TRUE;   // 指示是否找到能够分配资源的进程
    while(flag1)
    {
        int flag2 = TRUE;
        for (i = 0; i < n; ++i)
        {
            if (Finish[i] == FALSE)
            {
                if(cmp_vector(Need[i], Work[w])){
                    {   // 若系统有足够的资源可以分配给进程
                        // 则在进程运行结束后收回资源
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
        printf("系统是安全的, 安全序列如下:\n");
        printf("进程\tWork\tNeed\tAllocation\tWork+Allocation\n");
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
    printf("请输入进程P%d的请求向量", n);
    if (cmp_vector(Request[n], Need[n]))
    {
        if (cmp_vector(Request,Available))
        {
            
        }
        
    } else { //所需要的资源已经超过它所宣布的最大值,报错
        printf("ERROR!请求资源过多,无法分配!");
    }    
}

int main(int argc, char const *argv[])
{
    // freopen("in.txt","r",stdin);
    // 初始化分配
    init_status();
    // 判断安全性
    printf("%d",is_safe());
    return 0;
}

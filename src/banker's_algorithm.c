#include <stdio.h>
#define M 10            // 预设最大资源种类
#define N 10            // 预设最大进程数目

int n;                  // 进程数目
int m;                  // 可用资源种类
int Available[M];       // 可利用资源向量
int Max[N][M];          // 最大需求矩阵
int Allocation[N][M];   // 分配矩阵
int Need[N][M];         // 需求矩阵
int Request[N];         // 每个进程的请求向量

void init_status()
{// 初始化资源分配
    int i, j;
    printf("请输入最大进程数目和可用资源种类数:");
    scanf("%d%d",&n,&m);
    printf("请输入各类资源在系统中可用数目:");
    for(i = 0;i<m;++i)
        scanf("%d",&Available[i]);
    printf("输入各进程对各资源的最大需求:");
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
            Need[n][m] = Max[n][m] - Allocation[n][m];
    }
    
}

int main(int argc, char const *argv[])
{
    
    return 0;
}

/*简易模拟实现银行家算法*/
/* 代码地址:https://gitee.com/ZXA200009/operating_system*/
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
int Work_and_Allocation[N][M];
int Seq[N],s = 0;   // 记录安全序列
int Work[N][M],w = 0;    // 工作向量Work
int Finish[N];  // 指示系统是否有足够的资源分配给进程使之完成

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
int cmp_vector(int * a, int * b);
// 进行两个资源向量的简单运算, 结果存于a中
void cal_vector(int * a, int * b, char op);
// 输出分配表信息
void print_info();

// 安全性算法,判断系统是否处于安全状态,安全返回TRUE, 否则返回FALSE
int is_safe();
// 银行家算法
void banker_algorithm();

int main(int argc, char const *argv[])
{
    printf("--------------简易模拟实现银行家算法--------------\n");
    // 初始化分配
    init_status();
    // printf("\n功能选择:\n1.判断此种资源分配状态是否安全\n2.判断某进程请求相应资源后系统能否满足其资源请求\n请输入序号选择功能或者键入0退出程序:");
    int choice = 1; //记录用户选择项
    while(choice)
    {
        printf("\n功能选择:\n1.判断此种资源分配状态是否安全\n2.判断某进程请求相应资源后系统能否满足其资源请求\n请输入序号选择功能:"); 
        scanf("%d",&choice);
        switch (choice)
        {
            case 1:
                if(is_safe()) print_info();
                else{
                    printf("此时系统处于不安全状态!");
                }
                break;
            case 2:
                banker_algorithm();
        }
        printf("是否继续(1/0)?");
        scanf("%d",&choice);
    }
    printf("\n--------------退出系统成功--------------\n");
    return 0;
}


// 安全性算法,判断系统此时是否处于安全状态
int is_safe()
{
    int i;

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
            for (i = 0; i < n; ++i)
            {
                if(Finish[i] == FALSE) {
                    Seq[s++] = i;
                    break;
                }
            }
            flag1 = FALSE;
        }    
    }
    if (i >= n) {
        return TRUE;
    }
    else 
        return FALSE;    
}

// 银行家算法
void banker_algorithm()
{
    int No;
    printf("\n请输入进程号:");
    scanf("%d",&No);
    printf("请输入进程P%d的请求向量:", No);
    for (int i = 0; i < m; ++i)
        scanf("%d", &Request[i]);
    
    if (cmp_vector(Request, Need[No]))
    {
        if (cmp_vector(Request,Available))
        {   // 若系统有足够资源分配给进程
            // 则试探性地将资源分配给进程
            // 并修改相关向量的值
            cal_vector(Available, Request, '-');
            cal_vector(Allocation[No], Request, '+');
            cal_vector(Need[No], Request, '-');  

            // 随后执行安全性算法, 判断系统是否处于安全状态
            if(is_safe()) {
                print_info();
                printf("系统可以满足该进程的资源请求!");
                printf("\n****************************************\n");                
            }
            else{
                printf("\n****************************************\n");
                printf("未找到可以避免死锁的安全序列,\n系统无法满足该进程的资源请求!");
                printf("\n****************************************\n");
            }

            // 恢复最初的资源分配状态
            cal_vector(Available, Request, '+');
            cal_vector(Allocation[No], Request, '-');
            cal_vector(Need[No], Request, '+');                
        } else {
            printf("此时系统尚无足够资源,P%d需等待\n",No);
        }
        
    } else { //所需要的资源已经超过它所宣布的最大值,报错
        printf("ERROR!请求资源过多,超过其最大需求量,无法分配!");
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

// 进行两个资源向量的简单运算, 结果存于a中
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

// 系统安全,输出分配表信息
void print_info()
{
    printf("\n****************************************\n");
    printf("系统是安全的, 模拟分配过程如下:\n");
    printf("进程\t    Work\t    Need\t  Allocation\t Work+Allocation\tFinish\n");
    for(int j = 0 ; j < 4; ++j)
    {
        printf("\t  ");
        for(int k = 0 ; k < m; k++)
            printf("%c  ",(char)('A'+k));
    }
    printf("\n");
    for(int i = 0;i <n;++i)
    {
        printf("P%d\t",Seq[i]);
        for (int j = 0; j < m; ++j)
            printf("%3d",Work[i][j]);
        printf("\t");
        for (int j = 0; j < m; ++j)      
            printf("%3d",Need[Seq[i]][j]);
        printf("\t");
        for (int j = 0; j < m; ++j)
            printf("%3d",Allocation[Seq[i]][j]);
        printf("\t");
        for (int j = 0; j < m; ++j)      
            printf("%3d",Work_and_Allocation[i][j]);
        if(Finish[Seq[i]] == TRUE) printf("\t\ttrue\n");
        else printf("\t\tfalse\n");
    }
    printf("安全序列:");
    for (int j = 0; j < n; ++j)
        printf("P%d ",Seq[j]);
    printf("\n****************************************\n");
}
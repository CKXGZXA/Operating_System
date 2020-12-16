#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

pthread_mutex_t rmutex, wmutex, fair;   
int count = 0;

void * reader(void *args)
{
    int id = *(int *) args;
    while(1)
    {
        pthread_mutex_lock(&fair);
        pthread_mutex_lock(&rmutex);
        if(0 == count) pthread_mutex_lock(&wmutex);
        count++;
        pthread_mutex_unlock(&rmutex);
        pthread_mutex_unlock(&fair);

        printf("读者%d开始读信息!\n",id);
        sleep(rand()%10);

        pthread_mutex_lock(&rmutex);
        printf("读者%d读信息完成!\n",id);
        count--;
        if(count == 0) pthread_mutex_unlock(&wmutex);
        pthread_mutex_unlock(&rmutex);
    }
}

void * writer(void *arg)
{
    int id = *(int *) arg;
    while(1)
    {
        pthread_mutex_lock(&fair);
        pthread_mutex_lock(&wmutex);

        printf("\t写者%d开始写信息\n",id);
        sleep(rand()%10);
        printf("\t写者%d写入信息结束\n",id); 
        pthread_mutex_unlock(&wmutex);                       
        pthread_mutex_unlock(&fair);
        sleep(rand()%10);
    }
}


int main()
{
    int r_count,w_count;
    printf("请输入读者和写者的数量:");
    scanf("%d%d",&r_count, &w_count);
    // 初始化信号量
    pthread_mutex_init(&wmutex, NULL);
    pthread_mutex_init(&rmutex, NULL);
    pthread_mutex_init(&fair,NULL);
    // 新建各个读者和写者进程, 并初始化他们的编号,然后开始执行各自的函数
    pthread_t * r = (pthread_t *)malloc(r_count * sizeof(int));
    pthread_t * w = (pthread_t *)malloc(w_count * sizeof(int));
    int * r_id = (int *)malloc(r_count * sizeof(int));
    int * w_id = (int *)malloc(w_count * sizeof(int));
    int i;
    for(i=0; i<r_count; i++)
        r_id[i] = i;
    for(i=0; i<r_count; i++){
        int *t = r_id + i;
        pthread_create(&r[i], NULL, reader, (void *)t);
    }
    for(i=0; i<w_count; i++)
        w_id[i] = i;
    for(i=0; i<w_count; i++){
        int *t = w_id + i;
        pthread_create(&w[i], NULL, writer, (void *)t);
    }
    for(i = 0;i < w_count;i++)
        pthread_join(w[i], NULL);
    for(i = 0;i<r_count;++i)
        pthread_join(r[i], NULL);
    return 0;
}

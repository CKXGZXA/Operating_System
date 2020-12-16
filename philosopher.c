#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

pthread_mutex_t mutex, chopsticks[5];   // 哲学家拿筷子互斥信号量以及筷子信号量
sem_t remain; // 剩余可进餐哲学家人数

void * eat(void *args)
{

    int t_id = *(int*)args;
    printf("哲学家 %d 开始思考\n",t_id);
    while (1)
    {
        sem_wait(&remain);
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&chopsticks[t_id]);
        pthread_mutex_lock(&chopsticks[(t_id+1)%5]);
        pthread_mutex_unlock(&mutex);

        printf("\t\t哲学家 %d 开始进餐!\n",t_id);
        sleep(rand()%10);
        printf("\t\t哲学家 %d 进餐结束!\n",t_id);

        pthread_mutex_unlock(&chopsticks[t_id]);
        pthread_mutex_unlock(&chopsticks[(t_id+1)%5]);
        sem_post(&remain);
        printf("哲学家 %d 开始思考\n",t_id);
        sleep(rand()%3);
    }
}

int main()
{
    pthread_t process[5];   // 五个哲学家五个进程
    // 初始化信号量
    pthread_mutex_init(&mutex, NULL);   
    int i;
    for ( i = 0; i < 5; ++i)
    {
        pthread_mutex_init(&chopsticks[i],NULL);
    }
    sem_init(&remain,0,4);  // 最多允许4个哲学家同时进餐

    // 给多个哲学家编号,并且启动各自编号的进程
    int thread_id[5];
    for(i=0; i<5; i++)
        thread_id[i] = i;
    for(i=0; i<5; i++)
    {
        int *t = thread_id +i;
        pthread_create(&process[i], NULL, eat, (void*)t);
    }
    for(i = 0;i < 5;++i)
        pthread_join(process[i],NULL);

    // 销毁信号量
    for ( i = 0; i < 5; ++i)
    {
        pthread_mutex_destroy(&chopsticks[i]);
    }
    sem_destroy(&remain);
    return 0;
}

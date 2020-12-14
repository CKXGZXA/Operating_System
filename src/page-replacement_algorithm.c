#include <stdio.h>

// ���ҳ��
#define N 12
// �ڴ�ҳ����
#define Msize  5//4 3
// ȱҳ��
float rate[4];
// ȱҳ��
int lack[4];
// ҳ����������
int Pages[N] = {1,2,3,4,1,2,5,1,2,3,4,5};//{4,3,2,1,4,3,5,4,3,2,1,5};
// �ڴ�ҳ
int memory[Msize];

// ���������е����ֵ�±�
int max(int a[], int size)
{
    int m = 0;
    for (int i = 0; i < size; ++i)
    {
        if (a[i] > a[m])
        {
            m = i;
        } 
    }
    return m;
}
// �����ǰ�ڴ�ҳ�����
void print_m(int cur)
{
    for (int i = 0; i < Msize; ++i)
        if (-1 != memory[i]) printf("%d\t",memory[i]);
    printf("\n");
}
// ��ʼ��ȱҳ��,ȱҳ�����ڴ������Ϣ
void init(int x)
{
    rate[x] = 0;
    lack[x] = 0;
    for (int i = 0; i < Msize; ++i)
    {
        memory[i] = -1;
    }   
}

// �ж��ڴ����Ƿ��п���,�޿��෵��-1,�п��෵�ؿ����ڴ����
int is_full()
{
    for (int i = 0; i < Msize; i++)
    {
        if (-1 == memory[i])
        {
            return i;
        }
    }
    return -1;
}
// �жϵ�ǰҳ�Ƿ������ڴ���,���ڷ���-1, ���ڷ��������ڴ����
int is_in(int page)
{
    for (int i = 0; i < Msize; ++i)
    {
        if(page == memory[i]) return i;
    }
    return -1;
}
// �����ʱ���ʹ�õ����ڴ��ҳ���,��OPTӦ����̭���ڴ��ҳ,���뵱ǰҳ���������кſ�ʼ����
int will_longest_used(int cur)
{
    int i, j;
    int next_time[Msize]  = {0};   //��¼�´γ���ʱ��
    int longest_index = 0;
    for (i = 0; i < Msize; i++) {
        for (j = cur + 1; j < N; j++)
        {
            if(Pages[j] == memory[i]) {
                next_time[i] = j - cur;
                if(next_time[longest_index] < next_time[i]) longest_index = i;
                break;
            }
        }
        if(j == N) return i;
    }
    return longest_index;
}
// LRU֧�ֺ���, ��dwellפ��ʱ�������ṩʱ�䲽��
void dwell_p(int * a, int x)
{
    for (int i = 0; i < Msize; ++i)
    {
        if (-1 != memory[i])
            a[i]++;
        if(x == memory[i]) a[i] = 0;
    }
    
}
// ����û��㷨(OPT)
void OPT()
{
    init(0);
    int cur = 0;    // ��ǰ�������
    for (cur = 0; cur < N; ++cur)
    {
        if(-1 != is_in(Pages[cur]))
        {   // ҳ�����ڴ���
            printf("%d\t|\n",Pages[cur]);
        }
        else if (-1 != is_full())
        {   // �ڴ��п�λִ�д���
            memory[is_full()] = Pages[cur];
            lack[0]++;
            printf("%d\t|",Pages[cur]);
            print_m(cur);
        }
        else if (-1 == is_in(Pages[cur]))
        {   // �Ƿ����ڴ���, ����ִ�д���
            memory[will_longest_used(cur)] = Pages[cur];
            lack[0]++;
            printf("%d\t|",Pages[cur]);
            print_m(cur);
        }
    }
}
// �Ƚ��ȳ�(FIFO)ҳ���û��㷨
void FIFO()
{
    init(1);
    int oldest = 0;
    for (int i = 0; i < N; i++)
    {
        if(-1 != is_in(Pages[i]))
        {   // ҳ�����ڴ���
            printf("%d\t|\n",Pages[i]);
        }
        else if (-1 != is_full())
        {   // �ڴ��п�λִ�д���
            memory[is_full()] = Pages[i];
            lack[1]++;
            printf("%d\t|",Pages[i]);
            print_m(i);
        }
        else if(-1 == is_in(Pages[i]))// �޿�λֱ����̭��������
        {
            memory[oldest] = Pages[i];
            oldest = (++oldest)%Msize;
            lack[1]++;
            printf("%d\t|",Pages[i]);
            print_m(i);
        }

    }
}
// ������δʹ��(Last Recently Used)�û��㷨
void LRU()
{
    int dwell[Msize] = {0};   //�洢ÿ���ڴ�ҳ����δʹ��ʱ�䳤��
    init(2);
    for (int i = 0; i < N; i++)
    {
        if(-1 != is_in(Pages[i]))
        {   // ҳ�����ڴ���
            printf("%d\t|\n",Pages[i]);
        }
        else if (-1 != is_full())
        {   // �ڴ��п�λִ�д���
            memory[is_full()] = Pages[i];
            lack[2]++;
            printf("%d\t|",Pages[i]);
            print_m(i);
        }
        else if(-1 == is_in(Pages[i])) // �޿�λ��̭פ��ʱ�����
        {
            memory[max(dwell, Msize)] = Pages[i];
            lack[2]++;
            printf("%d\t|",Pages[i]);
            print_m(i);
        }

        dwell_p(dwell,Pages[i]);
    }
}
// ΪClock�û��㷨�ҵ��¸���Ҫ��̭���ڴ�ҳ�沢�޸ķ���λ,����Ϊ��ǰʱ��ָ��λ��,����ֵΪ��̭ҳ�����
int next_clock_page(int R[],int *p)
{
    int i,j;
    for ( i = *p,j = 0; j < Msize; i = (++i) % Msize,++j)
    {
        if(R[i] == 0) {
            R[i] = 1;
            *p = (*p + 1) % Msize;
            return i;
        }
    }
    for ( i = 0; i < Msize; i++)
    {
        R[i] = 0;
    }
    i = *p;
    *p = (*p + 1) % Msize;
    return i;
}
// Clock�û��㷨
void CLOCK()
{
    int R[Msize];
    int p = 0;
    init(3);
    for (int i = 0; i < N; i++)
    {
        if(-1 != is_in(Pages[i]))
        {   // ҳ�����ڴ���
            R[is_in(Pages[i])] = 1;
            printf("%d\t|\n",Pages[i]);
        }
        else if (-1 != is_full())
        {   // �ڴ��п�λִ�д���
            R[is_full()] = 1;           // �÷���λΪ 1
            memory[is_full()] = Pages[i];
            // p = (++p) % Msize;
            lack[3]++;
            printf("%d\t|",Pages[i]);
            print_m(i);

        }
        else if(-1 == is_in(Pages[i]))  // �޿�λ�Ҳ����ڴ����������̭����
        {
            memory[next_clock_page(R, &p)] = Pages[i];           
            lack[3]++;
            printf("%d\t|",Pages[i]);
            print_m(i);
        }
    }    
}

// ��ӡ�˵�
void print_menu()
{
    printf("\n=============�˵�================\n");
    printf("0. �˳�\n");
    printf("1. OPT�㷨ʵ��\n");
    printf("2. FIFO�㷨ʵ��\n");
    printf("3. LRU�㷨ʵ��\n");
    printf("4. CLOCK�㷨\n");
    printf("5. ��ʾȱҳ�ʶԱȱ�\n");
    printf("=================================\n");
    printf("������������ѡ��:");
}
// ��ӡȱҳ�ʶԱȱ�
void print_table()
{
    printf("\n**************************************\n");
    printf("�㷨\tȱҳ��\tȱҳ��\n");
    printf("OPT\t");printf("%d\t%.2f\n",lack[0],lack[0]*1.0/N);
    printf("FIFO\t");printf("%d\t%.2f\n",lack[1],lack[1]*1.0/N);
    printf("LRU\t");printf("%d\t%.2f\n",lack[2],lack[2]*1.0/N);
    printf("CLOCK\t");printf("%d\t%.2f\n",lack[3],lack[3]*1.0/N);
    printf("\n**************************************\n");
}
// �������
int main()
{
    printf("***********ҳ����̭/�û��㷨***********\n");
    print_menu();
    int choice;
    scanf("%d", &choice);
    while (choice)
    {
        switch (choice)
        {
        case 1:
            printf("\n**************************************\n");
            printf("OPT�㷨�ڴ�ҳ��������������:\n");
            OPT();
            printf("\n**************************************\n");
            break;
        case 2:
            printf("\n**************************************\n");
            printf("FIFO�㷨�ڴ�ҳ��������������:\n");
            FIFO();
            printf("\n**************************************\n");
            break;
        case 3:
            printf("\n**************************************\n");
            printf("LRU�㷨�ڴ�ҳ��������������:\n");
            LRU();
            printf("\n**************************************\n");
            break;
        case 4:
            printf("\n**************************************\n");
            printf("CLOCK�㷨�ڴ�ҳ��������������:\n");
            CLOCK();
            printf("\n**************************************\n");
            break;
        case 5:
            print_table();
        }
        print_menu();
        scanf("%d",&choice);
    }
    printf("*******************�˳�����!*******************\n");
    return 0;    
}

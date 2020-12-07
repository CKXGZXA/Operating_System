#include <stdio.h>
#define N 10010

int main() {
	int n,i,num[N],j,m,t;
	scanf("%d",&n);
	for ( i = 1; i <= n; i++)
	{
		scanf("%d",&num[i]);
	}
	char op;
	scanf("%d",&t);
	while(t--)
	{
		int k;
		getchar();
		scanf("%c", &op);
	
		scanf("%d%d%d",&i,&j,&m);
		switch (op)
		{
		case '+':
			for(k = i;k<=j;++k)
				num[k] += m;
			break;
		case '-':
			for(k = i;k<=j;++k)
				num[k] -= m;
			break;
		case '=':
			for(k = i;k<=j;++k)
				num[k] = m;
			break;		
		}
	}
	for ( i = 1; i <= n; i++)
	{
		printf("%d ",num[i]);
	}
	return 0;
}


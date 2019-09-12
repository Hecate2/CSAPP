//输出前n个正整数的全排列 
#include<stdio.h>
int a[1000];
int visited[1000];

FILE *fout=fopen("全排列.txt","w");
void perm(int n,int len)
{
	int i,k;
	if(len==n)
	{
		for(i=0;i<n-1;i++)
			//printf("%d ",a[i]);
			fprintf(fout,"%d ",a[i]);
		//printf("%d\n",a[n-1]);
		fprintf(fout,"%d\n",a[n-1]);
	}
	for(k=1;k<=n;k++)
	{
		if(visited[k]==0)
		{
			a[len++]=k;//把当前的数字放进数组，并且本层的len值加1
			visited[k]=1;//把当前的数字加1
			perm(n,len);//递归调用，继续往下深入，如若满足了len==n 就做输出，然后执行下面的还原语句
			len--;//因为当前层次的len值做了加1 要还原本层的len值，就要len--；
			visited[k]=0;//把当前数字的标记去掉，然后返回上一层，继续再往下执行
		}
	}
}
int main()
{
	int n;
	//scanf("%d",&n);
	n=6;
	perm(n,0);
}


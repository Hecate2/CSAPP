//���ǰn����������ȫ���� 
#include<stdio.h>
int a[1000];
int visited[1000];

FILE *fout=fopen("ȫ����.txt","w");
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
			a[len++]=k;//�ѵ�ǰ�����ַŽ����飬���ұ����lenֵ��1
			visited[k]=1;//�ѵ�ǰ�����ּ�1
			perm(n,len);//�ݹ���ã������������룬����������len==n ���������Ȼ��ִ������Ļ�ԭ���
			len--;//��Ϊ��ǰ��ε�lenֵ���˼�1 Ҫ��ԭ�����lenֵ����Ҫlen--��
			visited[k]=0;//�ѵ�ǰ���ֵı��ȥ����Ȼ�󷵻���һ�㣬����������ִ��
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


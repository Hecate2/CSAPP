#include <stdio.h>
//problem 5.17
void *memset_q(void *s, int c,unsigned int n)
{//��ʼ��ַs����Ҫ��������c���ڴ汻c�����λ�ֽڵ���������������Ҫ���ֽ���n
//��Ϊchar�ض�ռ1���ֽ� 
	unsigned long long filler=0xFF & c;
	unsigned char cfiller=filler;
	unsigned int sizel=sizeof(long long);
	unsigned int i;
	for(i=1;i<sizel;++i)
	{
		filler += (unsigned long long)cfiller << (i*8);
	}
	unsigned long long fillerCnt=n/sizel;//fillerCnt�������� 
	unsigned short rem=n%sizel;//���� 
	unsigned long long *lp=(unsigned long long *)s;
	for( ;fillerCnt>0;--fillerCnt)
	{
		*lp=filler;
		lp+=1;
	}
	char *cp=(char *)lp;
	for( ;rem>0;--rem)
	{
		*cp=cfiller;
		cp+=1;
	}
	return s;
}

int main()
{
	unsigned int sizel=sizeof(long long);
	unsigned int sizei=sizeof(int);
	printf("long longռ�ֽ�����%d\t intռ�ֽ�����%d\n\n",sizel,sizei);
	unsigned int n=103;//��Ҫ�����ֽ� 
	unsigned char *s=(char*)malloc(n+1);//��Ϊ���飬�Ȱ�ȫ���ó�һ���ڴ� 
	s[n]=0;//��Ϊ���飬��֤����������ַ��� 
	int c=65304;//1111 1111 0001 1000 ��  FF18
	printf("%d == 0x%X\n\n",c,c);
	s=memset_q(s,c,n);
	unsigned int i;
	for(i=0;i<n;++i)
	{
		printf("%X",s[i]);
	}
}

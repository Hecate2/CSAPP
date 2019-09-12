#include <stdio.h>
//problem 5.17
void *memset_q(void *s, int c,unsigned int n)
{//起始地址s，需要填充的内容c（内存被c的最低位字节的内容填满），需要的字节数n
//认为char必定占1个字节 
	unsigned long long filler=0xFF & c;
	unsigned char cfiller=filler;
	unsigned int sizel=sizeof(long long);
	unsigned int i;
	for(i=1;i<sizel;++i)
	{
		filler += (unsigned long long)cfiller << (i*8);
	}
	unsigned long long fillerCnt=n/sizel;//fillerCnt是整数！ 
	unsigned short rem=n%sizel;//余数 
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
	printf("long long占字节数：%d\t int占字节数：%d\n\n",sizel,sizei);
	unsigned int n=103;//需要多少字节 
	unsigned char *s=(char*)malloc(n+1);//作为试验，先安全地拿出一块内存 
	s[n]=0;//作为试验，保证结束掉这个字符串 
	int c=65304;//1111 1111 0001 1000 或  FF18
	printf("%d == 0x%X\n\n",c,c);
	s=memset_q(s,c,n);
	unsigned int i;
	for(i=0;i<n;++i)
	{
		printf("%X",s[i]);
	}
}

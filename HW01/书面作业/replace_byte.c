#include <stdio.h>

unsigned int replace_byte(unsigned int x,int i,unsigned char b)
{
	unsigned int lsb=(x<<((sizeof(int)-i)<<3))>>((sizeof(int)-i)<<3);
	unsigned int msb=(x>>((i+1)<<3))<<((i+1)<<3);
	//printf("%X\n%X\n",lsb,msb);
	unsigned int bb=b<<(i<<3);
	//printf("%X",bb);
	printf("%X",lsb+bb+msb);
	return lsb+bb+msb;
}

int main()
{
	//printf("%d\n",sizeof(int));
	//printf("%d",1<<10);
	unsigned int a=replace_byte(0x12345678,2,0xAB);
}


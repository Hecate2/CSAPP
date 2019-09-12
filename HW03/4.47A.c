#include <stdio.h>

//problem 4.47 A
void bubble_p(long *data,long count)
{
	long i,last;
	for(last=count-1;last>0;--last)
	{
		for(i=0;i<last;++i)
		{
			long *position=data+i;
			long *position1=position+1;
			if(*(position1)<*(position))
			{
				
				long t=*(position1);
				*(position1)=*(position);
				*(position)=t;
			}
		}
	}
}

int main()
{
	long data[]={-1,124,1324,1342,-14145,124,-4364,-4575,35,657,3245,23,567,456,14,7,34,12,343,7,5,325,2,6426,51,-364252,235326,426,34547};
	long count=sizeof(data)/sizeof(long);
	bubble_p(&data,count);
	long i;
	for(i=0;i<count;++i)
	{
		printf("%d\t",data[i]);
	}
	printf("\n");
}

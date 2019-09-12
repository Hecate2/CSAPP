#define DIM 96
#define pixel	int
#define RIDX(i,j,n) ((i)*(n)+(j))
unsigned int src[DIM][DIM];
unsigned int dst[DIM][DIM];
unsigned int dstStandard[DIM][DIM];

void rotateStandard(int dim,int* src,int* dst)
{
    int i, j;
    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

void rotate(int dim,int* src,int* dst)
{//dim是图片的行数（图片行列数相等） 
    const unsigned int dim2=dim*dim,srcjmp=dim2+1-2*dim;
    register pixel *gatejmp=src+dim2-dim,*gateimg=dst+dim2;
    register pixel *isrc1=src+dim-1,*isrc2=src+dim*2-1;
    register pixel *idst1=dst,*idst2=dst+1;

while(idst2<gateimg)
{
	*idst1=*isrc1;	isrc1+=dim*2;	idst1+=2;
	*idst2=*isrc2;	isrc2+=dim*2;	idst2+=2;
	*idst1=*isrc1;	isrc1+=dim*2;	idst1+=2;
	*idst2=*isrc2;	isrc2+=dim*2;	idst2+=2;
	*idst1=*isrc1;	isrc1+=dim*2;	idst1+=2;
	*idst2=*isrc2;	isrc2+=dim*2;	idst2+=2;
	*idst1=*isrc1;	isrc1+=dim*2;	idst1+=2;
	*idst2=*isrc2;	isrc2+=dim*2;	idst2+=2;

	*idst1=*isrc1;	isrc1+=dim*2;	idst1+=2;
	*idst2=*isrc2;	isrc2+=dim*2;	idst2+=2;
	*idst1=*isrc1;	isrc1+=dim*2;	idst1+=2;
	*idst2=*isrc2;	isrc2+=dim*2;	idst2+=2;
	*idst1=*isrc1;	isrc1+=dim*2;	idst1+=2;
	*idst2=*isrc2;	isrc2+=dim*2;	idst2+=2;
	*idst1=*isrc1;	isrc1+=dim*2;	idst1+=2;
	*idst2=*isrc2;	isrc2+=dim*2;	idst2+=2;

	*idst1=*isrc1;	isrc1+=dim*2;	idst1+=2;
	*idst2=*isrc2;	isrc2+=dim*2;	idst2+=2;
	*idst1=*isrc1;	isrc1+=dim*2;	idst1+=2;
	*idst2=*isrc2;	isrc2+=dim*2;	idst2+=2;
	*idst1=*isrc1;	isrc1+=dim*2;	idst1+=2;
	*idst2=*isrc2;	isrc2+=dim*2;	idst2+=2;
	*idst1=*isrc1;	isrc1+=dim*2;	idst1+=2;
	*idst2=*isrc2;	isrc2+=dim*2;	idst2+=2;

	*idst1=*isrc1;	isrc1+=dim*2;	idst1+=2;
	*idst2=*isrc2;	isrc2+=dim*2;	idst2+=2;
	*idst1=*isrc1;	isrc1+=dim*2;	idst1+=2;
	*idst2=*isrc2;	isrc2+=dim*2;	idst2+=2;
	*idst1=*isrc1;	isrc1+=dim*2;	idst1+=2;
	*idst2=*isrc2;	isrc2+=dim*2;	idst2+=2;
	*idst1=*isrc1;					idst1+=2;
	*idst2=*isrc2;					idst2+=2;

	if(isrc2>=gatejmp)
	{
		isrc1-=srcjmp;
		isrc2-=srcjmp;
	}
	else
	{
		isrc1+=dim*2;
		isrc2+=dim*2;
	}
}
}


void cmp(int dim,int* dst,int* dstStandard)
{
    int i, j;
    for (i = 0; i < dim; i++)
    {
		for (j = 0; j < dim; j++)
		{
		    if(dst[RIDX(i, j, dim)]!=dstStandard[RIDX(i, j, dim)])
		    {
		    	printf("出错于dst[%d][%d]\n",i,j);
		    	//return;
			}
		} 
	}
	printf("正确");
}

int main()
{
	int i,j,k,dim=DIM;
	k=1;
	for(i=0;i<dim;++i)
	for(j=0;j<dim;++j)
		src[i][j]=k++;
	rotateStandard(dim,&src,&dstStandard);
	rotate(dim,&dst,&dstStandard);
	cmp(dim,&dst,&dstStandard);
}

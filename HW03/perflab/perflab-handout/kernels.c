/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "16307130006",              /* Team name */
    "陈幸豪",     /* First member full name */
    "16307130006@fudan.edu.cn",  /* First member email address */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

//void rotate_block(int dim, pixel *src, pixel *src1, pixel *dst)
//{//src和src1两点划出一个矩形图块（包括src和src1的行列，且src1在src的右下角）
////而dst直接就是src经旋转后应到达的位置 
//	const int dimx=(src1-src)%dim, dimy=(src1-src)/dim;	//原图块的x方向和y方向尺寸减1
//	//register pixel *idst=dst, *isrc=src+dimx, *gatejmp=; 
//}


void rotate_cols(int dim, pixel *src, pixel *dst, int cols)
{//图片边长，最左列的最上面像素的地址，dst起始行最左边的地址，要处理的列数 
    const unsigned int dim2=dim*dim,srcjmp=dim2+1-dim;
    register pixel *gatejmp=src+dim2-dim,*isrc=src+cols-1,*gateimg=dst+dim*cols,*idst=dst;
    while(idst<gateimg)
    {
    	*(idst++)=*(isrc);
    	(isrc>=gatejmp)?(isrc-=srcjmp):(isrc+=dim);
    }
}

//struct rotate_args
//{
//	int dim;
//	pixel *src;
//	pixel *dst;
//	int cols;
//};

//void rotate_cols_thread(struct rotate_args *p)
//{//图片边长，最左列的最上面像素的地址，dst起始行最左边的地址，要处理的列数 
//    int dim=p->dim,cols=p->cols;
//    pixel *src=p->src,*dst=p->dst;
//    const unsigned int dim2=dim*dim,srcjmp=dim2+1-dim;
//    register pixel *gatejmp=src+dim2-dim,*isrc=src+cols-1,*gateimg=dst+dim*cols,*idst=dst;
//    while(idst<gateimg)
//    {
//    	*(idst++)=*(isrc);
//    	(isrc>=gatejmp)?(isrc-=srcjmp):(isrc+=dim);
//    }
//}


char rotate_test_descr[] = "临时测试用的函数";
void rotate_test(int dim, pixel *src, pixel *dst) 
{
	//pthread_t t1,t2;
	int dim2=dim*dim;
	int col=dim/2;
	//struct rotate_args arg1={dim,src,dst+dim2/2,dim/2};
	//pthread_create(&t1,NULL,(void*)&rotate_cols,&arg1);
	rotate_cols(dim,src,dst+dim2/2,dim/2);
	rotate_cols(dim,src+dim/2,dst,dim/2);
}

//char rotate_v3_descr[] = "v3: 分块处理\n(也可理解为暴力展开循环，只不过没有把相似语句抄很多遍)\n";
//void rotate_v3(int dim, pixel *src, pixel *dst) 
//{//dim是图片的行数（图片行列数相等） 
//    int i, j, imax, jmax, ;
//    const int block=32;	//块的大小，讲究 
//    for (i = 0; i < dim; i+=block)
//    {
//    	imax=i+block;
//		for (j = 0; j < dim; j+=block)
//		{
//			for
//		}
//	}
//	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
//}

char rotate_block2_descr[] = "分块方法优化";
void rotate_block2(const int dim, pixel *src, pixel *dst)
{
int i, j, i1, j1, im, jm;
pixel *isrc,*idst;
const int block=32;//size 
for(i=0; i<dim; i+=block)
{
	im = i+block; 
	for(j=0; j<dim; j+=block)
	{
		jm = j+block; 
		idst=dst+RIDX(i, j, dim);
		isrc=src+RIDX(j, dim-i-1, dim);

		i1=i;
		do
		{ 
			j1=j;
			do
			{
				*(idst++) = *(isrc);
				isrc+=dim;
				++j1;
			}
			while(j1<jm);

			idst+=(dim-block);
			isrc-=(dim*block+1);
			++i1;
		}
		while(i1<im);
	}
}
}

char rotate_block_descr[] = "实际图像处理使用的分块方法";
void rotate_block(const int dim, pixel *src, pixel *dst)
{
int i, j, i1, j1, im, jm;
const int block=32;//size 
for(i=0; i<dim; i+=block)
{
	im = i+block; 
	for(j=0; j<dim; j+=block)
	{
		jm = j+block; 
		for(i1=i; i1<im; i1++)
		{ 
			for(j1=j; j1<jm; j1++)
				dst[RIDX(i1, j1, dim)] = src[RIDX(j1, dim-i1-1, dim)];
		}
	}
}
}
char rotate_v5_descr[] = "v5: 以32为单位，2个一组，暴力展开v2的循环(保持写缓冲命中率)";
void rotate_v5(const int dim, pixel *src, pixel *dst) 
{//dim是图片的行数（图片行列数相等） 
    const unsigned int dim2=dim*dim,srcjmp=dim2+1-2*dim;
    register pixel *gatejmp=src+dim2-dim,*gateimg=dst+dim2;
    register pixel *isrc1=src+dim-1,*isrc2=isrc1+dim;
    register pixel *idst1=dst,*idst2=idst1+1;

while(idst2<gateimg)
{
	*(idst1)=*(isrc1);	idst1+=2;	isrc1+=dim*2;
	*(idst2)=*(isrc2);	idst2+=2;	isrc2+=dim*2;
	*(idst1)=*(isrc1);	idst1+=2;	isrc1+=dim*2;
	*(idst2)=*(isrc2);	idst2+=2;	isrc2+=dim*2;
	*(idst1)=*(isrc1);	idst1+=2;	isrc1+=dim*2;
	*(idst2)=*(isrc2);	idst2+=2;	isrc2+=dim*2;
	*(idst1)=*(isrc1);	idst1+=2;	isrc1+=dim*2;
	*(idst2)=*(isrc2);	idst2+=2;	isrc2+=dim*2;

	*(idst1)=*(isrc1);	idst1+=2;	isrc1+=dim*2;
	*(idst2)=*(isrc2);	idst2+=2;	isrc2+=dim*2;
	*(idst1)=*(isrc1);	idst1+=2;	isrc1+=dim*2;
	*(idst2)=*(isrc2);	idst2+=2;	isrc2+=dim*2;
	*(idst1)=*(isrc1);	idst1+=2;	isrc1+=dim*2;
	*(idst2)=*(isrc2);	idst2+=2;	isrc2+=dim*2;
	*(idst1)=*(isrc1);	idst1+=2;	isrc1+=dim*2;
	*(idst2)=*(isrc2);	idst2+=2;	isrc2+=dim*2;

	*(idst1)=*(isrc1);	idst1+=2;	isrc1+=dim*2;
	*(idst2)=*(isrc2);	idst2+=2;	isrc2+=dim*2;
	*(idst1)=*(isrc1);	idst1+=2;	isrc1+=dim*2;
	*(idst2)=*(isrc2);	idst2+=2;	isrc2+=dim*2;
	*(idst1)=*(isrc1);	idst1+=2;	isrc1+=dim*2;
	*(idst2)=*(isrc2);	idst2+=2;	isrc2+=dim*2;
	*(idst1)=*(isrc1);	idst1+=2;	isrc1+=dim*2;
	*(idst2)=*(isrc2);	idst2+=2;	isrc2+=dim*2;

	*(idst1)=*(isrc1);	idst1+=2;	isrc1+=dim*2;
	*(idst2)=*(isrc2);	idst2+=2;	isrc2+=dim*2;
	*(idst1)=*(isrc1);	idst1+=2;	isrc1+=dim*2;
	*(idst2)=*(isrc2);	idst2+=2;	isrc2+=dim*2;
	*(idst1)=*(isrc1);	idst1+=2;	isrc1+=dim*2;
	*(idst2)=*(isrc2);	idst2+=2;	isrc2+=dim*2;
	*(idst1)=*(isrc1);	idst1+=2;
	*(idst2)=*(isrc2);	idst2+=2;

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

char rotate_v4_descr[] = "v4: 以32为单位，暴力展开v2的循环(保持写缓冲命中率)";
void rotate_v4(const int dim, pixel *src, pixel *dst) 
{//dim是图片的行数（图片行列数相等） 
    const unsigned int dim2=dim*dim,srcjmp=dim2+1-dim;
    register pixel *gatejmp=src+dim2-dim,*gateimg=dst+dim2;
    register pixel *isrc=src+dim-1;
    register pixel *idst=dst;

while(idst<gateimg)
{
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;

	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;

	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	isrc+=dim;
	*(idst++)=*(isrc);	(isrc>=gatejmp)?(isrc-=srcjmp):(isrc+=dim);
}
}

char rotate_v3_descr[] = "v3: 以32为单位，8个一组，暴力展开v2的循环";
void rotate_v3(const int dim, pixel *src, pixel *dst) 
{//dim是图片的行数（图片行列数相等） 
    const unsigned int dim2=dim*dim,srcjmp=dim2+1-dim*8;
    register pixel *gatejmp=src+dim2-dim,*gateimg=dst+dim2;
    register pixel *isrc1=src+dim-1,*isrc2=isrc1+dim,*isrc3=isrc1+dim*2,*isrc4=isrc1+dim*3,*isrc5=isrc1+dim*4,*isrc6=isrc1+dim*5,*isrc7=isrc1+dim*6,*isrc8=isrc1+dim*7;
	register pixel *idst1=dst,*idst2=idst1+1,*idst3=idst1+2,*idst4=idst1+3,*idst5=idst1+4,*idst6=idst1+5,*idst7=idst1+6,*idst8=idst1+7;

while(idst8<gateimg)
{
    	*(idst1)=*(isrc1);	idst1+=8;	isrc1+=dim*8;
    	*(idst2)=*(isrc2);	idst2+=8;	isrc2+=dim*8;
    	*(idst3)=*(isrc3);	idst3+=8;	isrc3+=dim*8;
    	*(idst4)=*(isrc4);	idst4+=8;	isrc4+=dim*8;
    	*(idst5)=*(isrc5);	idst5+=8;	isrc5+=dim*8;
    	*(idst6)=*(isrc6);	idst6+=8;	isrc6+=dim*8;
    	*(idst7)=*(isrc7);	idst7+=8;	isrc7+=dim*8;
    	*(idst8)=*(isrc8);	idst8+=8;	isrc8+=dim*8;

    	*(idst1)=*(isrc1);	idst1+=8;	isrc1+=dim*8;
    	*(idst2)=*(isrc2);	idst2+=8;	isrc2+=dim*8;
    	*(idst3)=*(isrc3);	idst3+=8;	isrc3+=dim*8;
    	*(idst4)=*(isrc4);	idst4+=8;	isrc4+=dim*8;
    	*(idst5)=*(isrc5);	idst5+=8;	isrc5+=dim*8;
    	*(idst6)=*(isrc6);	idst6+=8;	isrc6+=dim*8;
    	*(idst7)=*(isrc7);	idst7+=8;	isrc7+=dim*8;
    	*(idst8)=*(isrc8);	idst8+=8;	isrc8+=dim*8;

    	*(idst1)=*(isrc1);	idst1+=8;	isrc1+=dim*8;
    	*(idst2)=*(isrc2);	idst2+=8;	isrc2+=dim*8;
    	*(idst3)=*(isrc3);	idst3+=8;	isrc3+=dim*8;
    	*(idst4)=*(isrc4);	idst4+=8;	isrc4+=dim*8;
    	*(idst5)=*(isrc5);	idst5+=8;	isrc5+=dim*8;
    	*(idst6)=*(isrc6);	idst6+=8;	isrc6+=dim*8;
    	*(idst7)=*(isrc7);	idst7+=8;	isrc7+=dim*8;
    	*(idst8)=*(isrc8);	idst8+=8;	isrc8+=dim*8;

    	*(idst1)=*(isrc1);	idst1+=8;
    	*(idst2)=*(isrc2);	idst2+=8;
    	*(idst3)=*(isrc3);	idst3+=8;
    	*(idst4)=*(isrc4);	idst4+=8;
    	*(idst5)=*(isrc5);	idst5+=8;
    	*(idst6)=*(isrc6);	idst6+=8;
    	*(idst7)=*(isrc7);	idst7+=8;
    	*(idst8)=*(isrc8);	idst8+=8;

    	if(isrc8>=gatejmp)
    	{
		isrc1-=srcjmp;
		isrc2-=srcjmp;
		isrc3-=srcjmp;
		isrc4-=srcjmp;
		isrc5-=srcjmp;
		isrc6-=srcjmp;
		isrc7-=srcjmp;
		isrc8-=srcjmp;
	}
	else
	{
		isrc1+=dim*8;
		isrc2+=dim*8;
		isrc3+=dim*8;
		isrc4+=dim*8;
		isrc5+=dim*8;
		isrc6+=dim*8;
		isrc7+=dim*8;
		isrc8+=dim*8;
	}
}
}


char rotate_v2_descr[] = "v2: 增加写缓冲命中率，但牺牲读命中率(有效果!)";
void rotate_v2(int dim, pixel *src, pixel *dst) 
{//dim是图片的行数（图片行列数相等） 
    const unsigned int dim2=dim*dim,srcjmp=dim2+1-dim;
    register pixel *gatejmp=src+dim2-dim,*isrc=src+dim-1,*gateimg=dst+dim2,*idst=dst;
    while(idst<gateimg)
    {
    	*(idst++)=*(isrc);
    	(isrc>=gatejmp)?(isrc-=srcjmp):(isrc+=dim);
	}
}

char rotate_v1_descr[] = "v1: 增加读内存缓冲命中率，但牺牲写内存命中率(负优化！)";
void rotate_v1(int dim, pixel *src, pixel *dst) 
{//dim是图片的行数（图片行列数相等） 
    const unsigned int dim2=dim*dim,dstjmp=dim2+1-dim;
    register pixel *isrc=src,*gateimg=src+dim2,*idst=dst+dim2-dim,*gatejmp=dst+dim;
    //isrc逐行扫描原始图片，这样idst会逐列得出旋转后的图片 
    while(isrc<gateimg)
    {
    	*(idst) = *(isrc++);
    	(idst>=gatejmp)?(idst-=dim):(idst+=dstjmp);
    }
}

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    naive_rotate(dim, src, dst);
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    //add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
    add_rotate_function(&rotate_v1, rotate_v1_descr);
    add_rotate_function(&rotate_v2, rotate_v2_descr);
    add_rotate_function(&rotate_v3, rotate_v3_descr);
    add_rotate_function(&rotate_v4, rotate_v4_descr);
    add_rotate_function(&rotate_v5, rotate_v5_descr);
    add_rotate_function(&rotate_block, rotate_block_descr);
    add_rotate_function(&rotate_block2, rotate_block2_descr);
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/
char smooth_v1_descr[] = "普通地暴力展开";
void smooth_v1(const int dim, pixel *src, pixel *dst)
{
int i,j; 
//先处理非边界的部分
for(i=1;i<dim-1;++i) 
{
	for(j=1;j<dim-1;++j)
	{
		dst[RIDX(i,j,dim)].red=(unsigned short)(((int)src[RIDX(i, j, dim)].red+src[RIDX(i-1, j-1, dim)].red+src[RIDX(i, j-1, dim)].red+src[RIDX(i+1, j-1, dim)].red+src[RIDX(i-1, j, dim)].red+src[RIDX(i+1, j, dim)].red+src[RIDX(i-1, j+1,dim)].red+src[RIDX(i, j+1, dim)].red+src[RIDX(i+1, j+1, dim)].red)/9);
		dst[RIDX(i,j,dim)].green=(unsigned short)(((int)src[RIDX(i, j, dim)].green+src[RIDX(i-1, j-1, dim)].green+src[RIDX(i, j-1, dim)].green+src[RIDX(i+1, j-1, dim)].green+src[RIDX(i-1, j, dim)].green+src[RIDX(i+1, j, dim)].green+src[RIDX(i-1, j+1, dim)].green+src[RIDX(i, j+1, dim)].green+src[RIDX(i+1, j+1, dim)].green)/9);
		dst[RIDX(i,j,dim)].blue=(unsigned short)(((int)src[RIDX(i, j, dim)].blue+src[RIDX(i-1, j-1, dim)].blue+src[RIDX(i, j-1, dim)].blue+src[RIDX(i+1, j-1, dim)].blue+src[RIDX(i-1, j, dim)].blue+src[RIDX(i+1, j, dim)].blue+src[RIDX(i-1, j+1, dim)].blue+src[RIDX(i, j+1, dim)].blue+src[RIDX(i+1, j+1, dim)].blue)/9);
	}
}
//下面处理四个角落
	dst[RIDX(0,0,dim)].red=(unsigned short)(((int)src[RIDX(0, 0, dim)].red+src[RIDX(0, 1, dim)].red+src[RIDX(1, 0, dim)].red+src[RIDX(1, 1, dim)].red) >> 2);
	dst[RIDX(0,0,dim)].green=(unsigned short)(((int)src[RIDX(0, 0, dim)].green+src[RIDX(0, 1, dim)].green+src[RIDX(1, 0, dim)].green+src[RIDX(1, 1, dim)].green) >> 2);
	dst[RIDX(0,0,dim)].blue=(unsigned short)(((int)src[RIDX(0, 0, dim)].blue+src[RIDX(0, 1, dim)].blue+src[RIDX(1, 0, dim)].blue+src[RIDX(1, 1, dim)].blue) >> 2);

	dst[RIDX(dim-1,dim-1,dim)].red=(unsigned short)(((int)src[RIDX(dim-1,dim-1, dim)].red+src[RIDX(dim-1,dim-2, dim)].red+src[RIDX(dim-2,dim-1, dim)].red+src[RIDX(dim-2,dim-2, dim)].red) >> 2);
	dst[RIDX(dim-1,dim-1,dim)].green=(unsigned short)(((int)src[RIDX(dim-1,dim-1, dim)].green+src[RIDX(dim-1,dim-2, dim)].green+src[RIDX(dim-2,dim-1, dim)].green+src[RIDX(dim-2,dim-2, dim)].green) >> 2);
	dst[RIDX(dim-1,dim-1,dim)].blue=(unsigned short)(((int)src[RIDX(dim-1,dim-1, dim)].blue+src[RIDX(dim-1,dim-2, dim)].blue+src[RIDX(dim-2,dim-1, dim)].blue+src[RIDX(dim-2,dim-2, dim)].blue) >> 2);

	dst[RIDX(dim-1,0,dim)].red=(unsigned short)(((int)src[RIDX(dim-1,0, dim)].red+src[RIDX(dim-2,0, dim)].red+src[RIDX(dim-1,1, dim)].red+src[RIDX(dim-2,1, dim)].red) >> 2);
	dst[RIDX(dim-1,0,dim)].green=(unsigned short)(((int)src[RIDX(dim-1,0, dim)].green+src[RIDX(dim-2,0, dim)].green+src[RIDX(dim-1,1, dim)].green+src[RIDX(dim-2,1, dim)].green) >> 2);
	dst[RIDX(dim-1,0,dim)].blue=(unsigned short)(((int)src[RIDX(dim-1,0, dim)].blue+src[RIDX(dim-2,0, dim)].blue+src[RIDX(dim-1,1, dim)].blue+src[RIDX(dim-2,1, dim)].blue) >> 2);

	dst[RIDX(0,dim-1,dim)].red=(unsigned short)(((int)src[RIDX(0,dim-1, dim)].red+src[RIDX(0,dim-2, dim)].red+src[RIDX(1,dim-1, dim)].red+src[RIDX(1,dim-2, dim)].red) >> 2);
	dst[RIDX(0,dim-1,dim)].green=(unsigned short)(((int)src[RIDX(0,dim-1, dim)].green+src[RIDX(0,dim-2, dim)].green+src[RIDX(1,dim-1, dim)].green+src[RIDX(1,dim-2, dim)].green) >> 2);
	dst[RIDX(0,dim-1,dim)].blue=(unsigned short)(((int)src[RIDX(0,dim-1, dim)].blue+src[RIDX(0,dim-2, dim)].blue+src[RIDX(1,dim-1, dim)].blue+src[RIDX(1,dim-2, dim)].blue) >> 2);

//下面处理四条边
j=0;
for(i=1;i<dim-1;++i) 
{
	dst[RIDX(i,j,dim)].red=(unsigned short)(((int)src[RIDX(i, j, dim)].red+src[RIDX(i-1, j, dim)].red+src[RIDX(i+1, j, dim)].red+src[RIDX(i-1, j+1,dim)].red+src[RIDX(i, j+1, dim)].red+src[RIDX(i+1, j+1, dim)].red)/6);
	dst[RIDX(i,j,dim)].green=(unsigned short)(((int)src[RIDX(i, j, dim)].green+src[RIDX(i-1, j, dim)].green+src[RIDX(i+1, j, dim)].green+src[RIDX(i-1, j+1, dim)].green+src[RIDX(i, j+1, dim)].green+src[RIDX(i+1, j+1, dim)].green)/6);
	dst[RIDX(i,j,dim)].blue=(unsigned short)(((int)src[RIDX(i, j, dim)].blue+src[RIDX(i-1, j, dim)].blue+src[RIDX(i+1, j, dim)].blue+src[RIDX(i-1, j+1, dim)].blue+src[RIDX(i, j+1, dim)].blue+src[RIDX(i+1, j+1, dim)].blue)/6);
}

j=dim-1;
for(i=1;i<dim-1;++i) 
{
	dst[RIDX(i,j,dim)].red=(unsigned short)(((int)src[RIDX(i, j, dim)].red+src[RIDX(i-1, j-1, dim)].red+src[RIDX(i, j-1, dim)].red+src[RIDX(i+1, j-1, dim)].red+src[RIDX(i-1, j, dim)].red+src[RIDX(i+1, j, dim)].red)/6);
	dst[RIDX(i,j,dim)].green=(unsigned short)(((int)src[RIDX(i, j, dim)].green+src[RIDX(i-1, j-1, dim)].green+src[RIDX(i, j-1, dim)].green+src[RIDX(i+1, j-1, dim)].green+src[RIDX(i-1, j, dim)].green+src[RIDX(i+1, j, dim)].green)/6);
	dst[RIDX(i,j,dim)].blue=(unsigned short)(((int)src[RIDX(i, j, dim)].blue+src[RIDX(i-1, j-1, dim)].blue+src[RIDX(i, j-1, dim)].blue+src[RIDX(i+1, j-1, dim)].blue+src[RIDX(i-1, j, dim)].blue+src[RIDX(i+1, j, dim)].blue)/6);
}

i=0;
for(j=1;j<dim-1;++j)
{
	dst[RIDX(i,j,dim)].red=(unsigned short)(((int)src[RIDX(i, j, dim)].red+src[RIDX(i, j-1, dim)].red+src[RIDX(i+1, j-1, dim)].red+src[RIDX(i+1, j, dim)].red+src[RIDX(i, j+1, dim)].red+src[RIDX(i+1, j+1, dim)].red)/6);
	dst[RIDX(i,j,dim)].green=(unsigned short)(((int)src[RIDX(i, j, dim)].green+src[RIDX(i, j-1, dim)].green+src[RIDX(i+1, j-1, dim)].green+src[RIDX(i+1, j, dim)].green+src[RIDX(i, j+1, dim)].green+src[RIDX(i+1, j+1, dim)].green)/6);
	dst[RIDX(i,j,dim)].blue=(unsigned short)(((int)src[RIDX(i, j, dim)].blue+src[RIDX(i, j-1, dim)].blue+src[RIDX(i+1, j-1, dim)].blue+src[RIDX(i+1, j, dim)].blue+src[RIDX(i, j+1, dim)].blue+src[RIDX(i+1, j+1, dim)].blue)/6);
}

i=dim-1;
for(j=1;j<dim-1;++j)
{
	dst[RIDX(i,j,dim)].red=(unsigned short)(((int)src[RIDX(i, j, dim)].red+src[RIDX(i-1, j-1, dim)].red+src[RIDX(i, j-1, dim)].red+src[RIDX(i-1, j, dim)].red+src[RIDX(i-1, j+1,dim)].red+src[RIDX(i, j+1, dim)].red)/6);
	dst[RIDX(i,j,dim)].green=(unsigned short)(((int)src[RIDX(i, j, dim)].green+src[RIDX(i-1, j-1, dim)].green+src[RIDX(i, j-1, dim)].green+src[RIDX(i-1, j, dim)].green+src[RIDX(i-1, j+1, dim)].green+src[RIDX(i, j+1, dim)].green)/6);
	dst[RIDX(i,j,dim)].blue=(unsigned short)(((int)src[RIDX(i, j, dim)].blue+src[RIDX(i-1, j-1, dim)].blue+src[RIDX(i, j-1, dim)].blue+src[RIDX(i-1, j, dim)].blue+src[RIDX(i-1, j+1, dim)].blue+src[RIDX(i, j+1, dim)].blue)/6);
}
}
/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    smooth_v1(dim, src, dst);
}


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
    add_smooth_function(&smooth_v1, smooth_v1_descr);
}

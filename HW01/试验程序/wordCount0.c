//使用字典树(Trie)和一个指针数组来完成。
//字典树节点名为node，用于查找单词是否出现过。
//如果出现过则计数+1。计数存放于数组中 
//指针数组sortHelper指向存储各个单词以及单词出现次数的struct 
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <windows.h>

typedef struct linknode//定义指针数组所指的struct 
{
	char *word;
	unsigned short count;
}Linknode;
int sizeofLinknode=sizeof(Linknode);

typedef struct node//定义字典树节点 
{
	unsigned short link;//这个单词是否出现过。没出现过就是0，出现过就是这个单词在指针数组里的下标 
	//如果出现过，则link应指向对应链表节点。如果没出现过，则link==NULL。此时应新建链表节点 
	struct node* next[26];//后面可能接26个英文字母的任意一种 
}Node;
Node dummy,*proot=&dummy,*p;
//根部无用节点dummy，根部地址proot，工作指针p 
int sizeofNode=sizeof(Node);

#define MAXCOUNT	65535
//允许不同单词的总数 
Linknode* sortHelper[MAXCOUNT];//指针数组
unsigned int maxcount=0;//当前指针数组用掉多少了 

Node* newNode()//新建字典树节点 
{
	Node* ptmp;
	if(ptmp = (Node*)malloc(sizeofNode))
	{
		//如果malloc成功，即不是空指针 
		ptmp->link=MAXCOUNT-1;
		register int i;
		for(i=0;i<26;++i)
			ptmp->next[i]=NULL;
		return ptmp;
	}
	printf("创建字典树节点失败！可能内存不足！");
	exit(EXIT_FAILURE);
}

Linknode* newLinknode(char* word)//新建指针数组所指的结构 
{
	Linknode* ptmp;
	if(ptmp = (Linknode*)malloc(sizeofLinknode))
	{
		ptmp->count=1;
		ptmp->word=(char*)malloc(strlen(word)+1);
		strcpy(ptmp->word,word);
		return ptmp;
	}
	printf("创建链表节点失败！可能内存不足！");
	exit(EXIT_FAILURE);
}

unsigned char isLetter(register char letter)
//判断字符是不是字母。如果是，则转为小写字母输出 
//如果不是，则输出'\0'
{
	if(96<letter && letter<123)//是小写字母 
	{
		return letter;
	}
	if(64<letter && letter<91)//是大写字母
	{
		return letter+32;//返回小写字母 
	} 
	//不是大写或小写字母 
	return 0;
}

char *get1word(register char *fin,register char *pend,register char *word)
//从文件读1个单词放在word字符串 
{
	//从文件当前位置往后找到下一个字母
	register char letter; 
	register char i=1;
	while(fin<pend)//如果文件没读完 
	{
		letter=isLetter(*(fin++));//isLetter函数只会输出小写字母或'\0' 
		if(letter)//letter是字母
		{
			word[0]=letter;
			goto inputWord;
		}
	}
	word[0]=0;//如果能执行这一句，说明文件读完了 
	return 0;
inputWord://已经发现了第一个字母，应把整个单词都输入word数组 
	do
	{
		if(*(fin)!='\'')
		{
	   		letter=isLetter(*(fin++));
	   		word[i++]=letter;
		}
		else
		{
			goto gotQuote;
		}
	}
	while(letter);
	return fin;
gotQuote:
	++fin;
	do
	{
		letter=isLetter(*(fin++));
		word[i++]=letter;
	}
	while(letter);
	return fin;
}

unsigned char trie(char *word)//字典树匹配 
{
	p=proot;//工作指针p指示单词word对应字典树哪个节点 
	register char i=0;//当整数用，指示word数组中目前要处理哪个字母 
	register char tmpchar;//指示字典树里的字母 
	while(tmpchar=word[i++])
	{
		tmpchar-=97;//'a'的ascii是97 
		if(p->next[tmpchar])
		{
			p=p->next[tmpchar];
		}
		else
		{
			p=p->next[tmpchar]=newNode();//新建节点，且把字典树p往下移动
		}
	}
	if(p->link==MAXCOUNT-1)//是新单词 
	{
		if(maxcount<MAXCOUNT)//指针数组还够用 
		{
			p->link=maxcount;
			sortHelper[maxcount++]=newLinknode(word);
		}
		else//指针数组用完了 
		{
			printf("指针数组用完了。请增大指针数组");
			exit(EXIT_FAILURE);
		}
	}
	else//不是新单词 
	{
		++(sortHelper[p->link]->count);
	}
	//return 1;
}

//堆排序函数 
//建堆
void BuildHeap(Linknode **a,int n){
    for (int i = n - 1; i > -1; --i) {
        HeapAdjust(a, i, n);
    }
}

//交换两个数
void swap(Linknode **a,Linknode **b){
    Linknode **temp;
    temp = *a;    *a = *b;    *b = temp;
}

//堆排序
void HeapSort(Linknode **a,int n){
    BuildHeap(a, n);
    for (int i = n - 1; i >= 0; i--) {
        //交换堆顶和最后一个元素，即每次将剩余元素中的最大者放到后面；
        swap(&a[0], &a[i]);
        //重新调整堆为大顶堆；
        HeapAdjust(a, 0, i );
    }
}

//调整堆
void HeapAdjust(Linknode **a,int i,int n)
{
    int lchild = 2*i;//左孩子
    int rchild = 2*i+1;//右孩子
    if (i <= n) {
	    int max = i;
        if (lchild < n && a[lchild]->count > a[max]->count) {
            max = lchild;
        }
        if (rchild < n && a[rchild]->count > a[max]->count) {
            max = rchild;
        }
        if (i != max) {
            swap(&a[i], &a[max]);
            //避免调整之后以max为父节点的子树不是堆；
            HeapAdjust(a, max, n);
        }
    }
}

int main(int argc, char** argv)
{
//设定计时器
	clock_t start, finish; 
	/* 测量一个事件持续的时间*/ 
	start = clock(); 
	


//主程序开始---------------------------------- 
//使用内存文件映射来读取input.txt。fopen方法实在太慢了 
    //打开一个文件
    register HANDLE hin = CreateFileA("input.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hin == INVALID_HANDLE_VALUE)
    {
        printf("打开文件失败\n");
        system("pause");
        exit(0);
    }
    //获取文件大小
    register unsigned int filesize = GetFileSize(hin, NULL);
    //printf("文件长度:%d\n", filesize);
    HANDLE hmap = CreateFileMappingA(hin,NULL,PAGE_READWRITE | SEC_COMMIT,0,
		filesize + 1,NULL);
    if (hmap == NULL)
    {
        printf("文件映射失败\n");
        CloseHandle(hin);
        system("pause");
        exit(0);
    }
    //创建一个指针，存储映射以后的首地址
    register PVOID pvfile = MapViewOfFile(hmap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
    if (hmap == NULL)
    {
        printf("指针映射失败\n");
        CloseHandle(hin);CloseHandle(hmap);
        system("pause");
        exit(0);
    }
    //printf("文件映射成功\n");
    register char *pstart = (char*)pvfile; //首地址
    register char *pend = pstart + filesize; //结束地址+1
    //system("pause");
    register char *fin=pstart;
    // 从映射的文件中读内容
    //*(pstart + pointer)即为input.txt的第pointer个字符

	register FILE *fout=fopen("output.txt","wt");//获取输出文件的指针 
	char word[127];//单个单词的缓冲区 
	//get1word(fin,pend,word);
	//printf("%s\n",word);
	
	
	
//文件已准备完成。下面用字典树统计每个单词的次数----------------------------------
	while(fin=get1word(fin,pend,word))
	{
		//printf("%s\n",word);
		trie(word);//在字典树里匹配 
	}
	//统计完成 
	
	//关闭input.txt 
    UnmapViewOfFile(pvfile);CloseHandle(hin);CloseHandle(hmap);CloseHandle(hin);	
	//对sortHelper排序
	HeapSort(sortHelper,maxcount);
	//输出sortHelper
	register int i;
	for(i=0;i<maxcount;++i)
	{
		fprintf(fout,"%s %d\n",sortHelper[i]->word,sortHelper[i]->count);
	} 



//停止计时器---------------------------------- 
	finish = clock(); 
	double Total_time = (double)(finish-start) / CLOCKS_PER_SEC; 
	printf( "%f seconds", Total_time); 
	return 0;
}

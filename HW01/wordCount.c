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
	unsigned int count;
}Linknode;
int sizeofLinknode=sizeof(Linknode);

typedef struct node//定义字典树节点 
{
	unsigned short link;//这个单词是否出现过。没出现过就是MAXCOUNT，出现过就是这个单词在指针数组里的下标 
	//如果出现过，则link应指向对应链表节点。如果没出现过，则link==NULL。此时应新建链表节点 
	struct node* next[26];//后面可能接26个英文字母的任意一种 
}Node;
Node *proot,*p;
//根节点，工作指针p 
int sizeofNode=sizeof(Node);

#define MAXCOUNT	65535
//允许不同单词的总数 
//注意！MAXCOUNT不要超出unsigned short的取值范围！ 
Linknode* sortHelper[MAXCOUNT];//指针数组
unsigned int maxcount=0;//当前指针数组用掉多少了 

Node* newNode()//新建字典树节点 
{
	Node* ptmp;
	if(ptmp = (Node*)malloc(sizeofNode))
	{
		//如果malloc成功，即不是空指针 
		ptmp->link=MAXCOUNT;
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

void trieGo(register char letter)
//根据输入的字母，让字典树工作指针p向下移动  
{
	letter-=97;
	(p->next[letter])?
		(p=p->next[letter]):
		(p=p->next[letter]=newNode());
}

void wordCount(register char *word)//给单词计数 
//如果是新单词，则新建linkNode存储该单词。
//如果不是新单词（ linkNode已有该单词的节点），则计数+1 
//字典树中工作指针p所指节点的link值能代表这个单词是否出现过 
{
	if(p->link==MAXCOUNT)//是新单词 
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
}

//下面是堆排序函数 
//交换两个数
void swap(Linknode **a,Linknode **b){
    Linknode *temp;
    temp = *a;    *a = *b;    *b = temp;
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

//建堆
void BuildHeap(Linknode **a,int n){
    for (int i = n - 1; i > -1; --i) {
        HeapAdjust(a, i, n);
    }
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
	
	
	
//文件已准备完成。下面用字典树统计每个单词的次数-------------------------------
//从文件读1个单词，同时在字典树里查找这个单词 
//接下来有很多goto，当成有限状态机来理解。有三种状态：
//未遇上单词状态start，遇上单词状态gotLetter，遇上单词中有缩写的'\''状态gotQuote 
	proot=newNode();
	register unsigned short i;//指示word数组里当前应该往哪一格填数 
	register char letter,tmpletter;
start://开始 
	p=proot;
	while(fin<pend)//如果文件没读完 
	{
		letter=isLetter(*(fin++));//isLetter函数只会输出小写字母或'\0' 
		if(letter)//letter是字母
		{
			trieGo(letter);
			word[0]=letter;
			goto gotLetter;
		}
	}
	word[0]=0;//如果能执行这一句，说明文件读完了，且没遇上单词 
	goto end;
gotLetter://遇上了字母，进入读单词状态 
	i=1;//指示word数组里当前应该往哪一格填数，兼作单词长度 
	while(fin<pend)
	{
		letter=*(fin++);
		if(tmpletter=isLetter(letter))//是字母 
		{
			word[i++]=tmpletter;
			trieGo(tmpletter);
			continue;
		}
		if(letter=='-')//是连字符 
		{
			continue;
		}
		if(letter=='\'')//是引号（表示缩写） 
		{
			goto gotQuote;
		}
		//执行到这里说明一个单词结束了 
		word[i]=0;
		wordCount(word);
		goto start;//继续读文件 
	}
	//执行到这里说明文件结束了 
	word[i]=0;
	wordCount(word);
	goto end;
gotQuote://之前一直是字母，且现在遇上了缩写用的单引号 
//与gotLetter状态的区别仅仅在于，如果再遇上引号就算单词结束了 
	while(fin<pend)
	{
		letter=*(fin++);
		if(tmpletter=isLetter(letter))//是字母 
		{
			word[i++]=tmpletter;
			trieGo(tmpletter);
			continue;
		}
		if(letter=='-')
		{
			continue;
		}
		//执行到这里说明单词结束了 
		word[i]=0;
		wordCount(word);
		goto start;
	}	
	//执行到这里说明文件结束了 
	word[i]=0;
	wordCount(word);
	goto end;
end:
	//统计完成 
	//关闭input.txt 
    UnmapViewOfFile(pvfile);CloseHandle(hin);CloseHandle(hmap);CloseHandle(hin);	
	
	//对sortHelper排序
	HeapSort(sortHelper,maxcount);
	//输出sortHelper
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

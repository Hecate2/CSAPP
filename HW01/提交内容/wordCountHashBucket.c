//使用哈希查找！ 
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <windows.h>

typedef struct linknode//用来保存单词及次数 
{
	char *word;
	unsigned int count;
	struct linknode *next;
}Linknode;
Linknode *p;//工作指针 
int sizeofLinknode=sizeof(Linknode);

#define HASHSPACE	1048575
//用HASHSPACE那么多的格子存放不同的单词 
Linknode *hashspace[HASHSPACE]; 

#define MAXCOUNT 200000
//一个单词最多允许出现MAXCOUNT次（二十万） 

Linknode* newLinknode(char* word)//新建指针数组所指的结构 
{
	Linknode* ptmp;
	if(ptmp = (Linknode*)malloc(sizeofLinknode))
	{
		ptmp->next=NULL;
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

unsigned short wordCount(char *word,unsigned short length)//单词本身和单词长度 
{
	register unsigned long long hashResult=word[0]-97;
	register int i;
	for(i=1;i<length;++i)
	{
		hashResult += (i*word[i]*word[i-1]);
	}
	hashResult=hashResult % HASHSPACE;
	if(hashspace[hashResult]==NULL)
	{
		hashspace[hashResult]=newLinknode(word);
		return 0;
	}
	else
	{
		p=hashspace[hashResult];
		while(strcmp(p->word,word))
		{
			if(p->next)
			{
				p=p->next;
			}
			else
			{
				p->next=newLinknode(word);
				return 1;
			}
		}
		p->count+=1;
		return 0;
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

	register FILE *fout=fopen("outputHashBucket.txt","wt");//获取输出文件的指针 
	char word[127];//单个单词的缓冲区 
	//get1word(fin,pend,word);
	//printf("%s\n",word);
	
	
	
//文件已准备完成。下面开始读文件，哈希，计数----------------------------------
	register unsigned long long i;//指示word数组里当前应该往哪一格填数 
	register char letter,tmpletter;
	register unsigned short conflict=0;//统计哈希冲突次数 
	for(i=0;i<HASHSPACE;++i)
	{
		hashspace[i]=NULL;
	}
start://开始 
	while(fin<pend)//如果文件没读完 
	{
		letter=isLetter(*(fin++));//isLetter函数只会输出小写字母或'\0' 
		if(letter)//letter是字母
		{
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
		word[i++]=0;
		conflict+=wordCount(word,i);
		goto start;//继续读文件 
	}
	//执行到这里说明文件结束了 
	word[i++]=0;
	conflict+=wordCount(word,i);
	goto end;
gotQuote://之前一直是字母，且现在遇上了缩写用的单引号 
//与gotLetter状态的区别仅仅在于，如果再遇上引号就算单词结束了 
	while(fin<pend)
	{
		letter=*(fin++);
		if(tmpletter=isLetter(letter))//是字母 
		{
			word[i++]=tmpletter;
			continue;
		}
		if(letter=='-')
		{
			continue;
		}
		//执行到这里说明单词结束了 
		word[i++]=0;
		conflict+=wordCount(word,i);
		goto start;
	}	
	//执行到这里说明文件结束了 
	word[i++]=0;
	conflict+=wordCount(word,i);
	goto end;
end:
	//统计完成 
	//关闭input.txt 
    UnmapViewOfFile(pvfile);CloseHandle(hin);CloseHandle(hmap);CloseHandle(hin);	
    Linknode* bucket[MAXCOUNT];
	//MAXCOUNT列。每一列是一个链表的头指针 
	//链表里存储出现次数等于列数的单词 
	for(i=0;i<MAXCOUNT;++i){bucket[i]=NULL;}
    register unsigned long long j=0;//遍历HASHSPACE 
    register Linknode *pbuck;//bucket中的工作指针 
	for(;j<HASHSPACE;++j)
	{
		if(hashspace[j])
		{
			p=hashspace[j];
			do
			{
				if(p->count==1)
				{
					fprintf(fout,"%s %d\n",p->word,p->count);
					p=p->next;
				}
				else
				{
					pbuck=p;
					p=p->next;
					pbuck->next=bucket[pbuck->count];
					bucket[pbuck->count]=pbuck;
				}
			}
			while(p);
		}
	}
	//输出bucket
	for(i=0;i<MAXCOUNT;++i)
	{
		if(bucket[i])
		{
			pbuck=bucket[i];
			do
			{
				fprintf(fout,"%s %d\n",pbuck->word,i);
				pbuck=pbuck->next;
			}
			while(pbuck);
		}
	}



//停止计时器---------------------------------- 
	finish = clock(); 
	double Total_time = (double)(finish-start) / CLOCKS_PER_SEC; 
	printf( "%f seconds\n", Total_time); 
	printf("哈希冲突次数:%d",conflict);
	return 0;
}

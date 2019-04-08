//使用字典树+两条链表来完成。
//字典树节点名为node，用于查找单词是否出现过。如果出现过则计数+1。计数存放于链表中 
//第一条链表用于存储不同单词的出现次数，以及插入排序 
//第二条链表用来辅助插入排序。存储 
#include <stdio.h>
#include <malloc.h>
#include <Windows.h>

#define Node	struct node
#define Linknode	struct linknode
struct node//定义字典树节点 
{
	Linknode* link;//这个单词是否出现过。
	//如果出现过，则link应指向对应链表节点。如果没出现过，则link==NULL。此时应新建链表节点 
	Node* next[26];//后面可能接26个英文字母的任意一种 
}*proot;
int sizeofNode=sizeof(Node);

struct linknode//定义链表节点 
{
	const char* word;
	Linknode* next;//链表的下一个节点 
	int count;
}*ptail,*phead;
int sizeofLinknode=sizeof(Linknode);

Node* newNode()//新建字典树节点 
{
	Node* p = (Node*)malloc(sizeofNode);//如果malloc成功，即不是空指针 
	int i;
	for(i=0;i<26;++i)
		p->next[i]=NULL;
	return p;
}

Linknode* newLinknode()//新建链表节点 
{
	Linknode* p = (Linknode*)malloc(sizeofLinknode);
	p->next=NULL;
	return p;
}

int main(int argc, char** argv)
{
	HANDLE hfile=INVALID_HANDLE_VALUE;
	hfile=CreateFile(L"input.txt",GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_FLAG_OVERLAPPED| FILE_ATTRIBUTE_NORMAL,NULL);
	if(hfile==INVALID_HANDLE_VALUE)
	{
		printf("读input.txt失败！");
		goto inputFailed;
	}
	char buf[1024];
	int readed;
	OVERLAPPED ov;
	HANDLE hevent=CreateEvent(NULL,FALSE,FALSE,NULL);
	memset(&ov,0,sizeof(ov));
	ov.hEvent=hevent;
	ov.Offset=0;
	ReadFile(hfile,buf,1024,&readed,&ov);
	if(GetLastError()==ERROR_IO_PENDING)
	{
		WaitForSingleObject(hevent,INFINITE);
		readed=ov.InternalHigh;
		buf[readed]=0;
		printf("%s\n",buf);
	}
inputFailed:
	system("pause");
	return 1;
}

//ʹ���ֵ���+������������ɡ�
//�ֵ����ڵ���Ϊnode�����ڲ��ҵ����Ƿ���ֹ���������ֹ������+1����������������� 
//��һ���������ڴ洢��ͬ���ʵĳ��ִ������Լ��������� 
//�ڶ����������������������򡣴洢 
#include <stdio.h>
#include <malloc.h>
#include <Windows.h>

#define Node	struct node
#define Linknode	struct linknode
struct node//�����ֵ����ڵ� 
{
	Linknode* link;//��������Ƿ���ֹ���
	//������ֹ�����linkӦָ���Ӧ����ڵ㡣���û���ֹ�����link==NULL����ʱӦ�½�����ڵ� 
	Node* next[26];//������ܽ�26��Ӣ����ĸ������һ�� 
}*proot;
int sizeofNode=sizeof(Node);

struct linknode//��������ڵ� 
{
	const char* word;
	Linknode* next;//�������һ���ڵ� 
	int count;
}*ptail,*phead;
int sizeofLinknode=sizeof(Linknode);

Node* newNode()//�½��ֵ����ڵ� 
{
	Node* p = (Node*)malloc(sizeofNode);//���malloc�ɹ��������ǿ�ָ�� 
	int i;
	for(i=0;i<26;++i)
		p->next[i]=NULL;
	return p;
}

Linknode* newLinknode()//�½�����ڵ� 
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
		printf("��input.txtʧ�ܣ�");
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

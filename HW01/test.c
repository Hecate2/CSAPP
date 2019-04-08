//ʹ���ֵ���(Trie)��һ��ָ����������ɡ�
//�ֵ����ڵ���Ϊnode�����ڲ��ҵ����Ƿ���ֹ���
//������ֹ������+1����������������� 
//ָ������sortHelperָ��洢���������Լ����ʳ��ִ�����struct 
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <windows.h>

typedef struct linknode//����ָ��������ָ��struct 
{
	char *word;
	unsigned short count;
}Linknode;
int sizeofLinknode=sizeof(Linknode);

typedef struct node//�����ֵ����ڵ� 
{
	unsigned short link;//��������Ƿ���ֹ���û���ֹ�����MAXCOUNT�����ֹ��������������ָ����������±� 
	//������ֹ�����linkӦָ���Ӧ����ڵ㡣���û���ֹ�����link==NULL����ʱӦ�½�����ڵ� 
	struct node* next[26];//������ܽ�26��Ӣ����ĸ������һ�� 
}Node;
Node *proot,*p;
//���ڵ㣬����ָ��p 
int sizeofNode=sizeof(Node);

#define MAXCOUNT	65535
//����ͬ���ʵ����� 
//ע�⣡MAXCOUNT��Ҫ����unsigned short��ȡֵ��Χ�� 
Linknode* sortHelper[MAXCOUNT];//ָ������
unsigned int maxcount=0;//��ǰָ�������õ������� 

Node* newNode()//�½��ֵ����ڵ� 
{
	Node* ptmp;
	if(ptmp = (Node*)malloc(sizeofNode))
	{
		//���malloc�ɹ��������ǿ�ָ�� 
		ptmp->link=MAXCOUNT;
		register int i;
		for(i=0;i<26;++i)
			ptmp->next[i]=NULL;
		return ptmp;
	}
	printf("�����ֵ����ڵ�ʧ�ܣ������ڴ治�㣡");
	exit(EXIT_FAILURE);
}

Linknode* newLinknode(char* word)//�½�ָ��������ָ�Ľṹ 
{
	Linknode* ptmp;
	if(ptmp = (Linknode*)malloc(sizeofLinknode))
	{
		ptmp->count=1;
		ptmp->word=(char*)malloc(strlen(word)+1);
		strcpy(ptmp->word,word);
		return ptmp;
	}
	printf("��������ڵ�ʧ�ܣ������ڴ治�㣡");
	exit(EXIT_FAILURE);
}

unsigned char isLetter(register char letter)
//�ж��ַ��ǲ�����ĸ������ǣ���תΪСд��ĸ��� 
//������ǣ������'\0'
{
	if(96<letter && letter<123)//��Сд��ĸ 
	{
		return letter;
	}
	if(64<letter && letter<91)//�Ǵ�д��ĸ
	{
		return letter+32;//����Сд��ĸ 
	} 
	//���Ǵ�д��Сд��ĸ 
	return 0;
}

void trieGo(register char letter)
//�����������ĸ�����ֵ�������ָ��p�����ƶ�  
{
	letter-=97;
	(p->next[letter])?
		(p=p->next[letter]):
		(p=p->next[letter]=newNode());
}

void wordCount(register char *word)//�����ʼ��� 
//������µ��ʣ����½�linkNode�洢�õ��ʡ�
//��������µ��ʣ� linkNode���иõ��ʵĽڵ㣩�������+1 
//�ֵ����й���ָ��p��ָ�ڵ��linkֵ�ܴ�����������Ƿ���ֹ� 
{
	if(p->link==MAXCOUNT)//���µ��� 
	{
		if(maxcount<MAXCOUNT)//ָ�����黹���� 
		{
			p->link=maxcount;
			sortHelper[maxcount++]=newLinknode(word);
		}
		else//ָ������������ 
		{
			printf("ָ�����������ˡ�������ָ������");
			exit(EXIT_FAILURE);
		}
	}
	else//�����µ��� 
	{
		++(sortHelper[p->link]->count);
	}
}

//�����Ƕ������� 
//����������
void swap(Linknode **a,Linknode **b){
    Linknode **temp;
    temp = *a;    *a = *b;    *b = temp;
}

//������
void HeapAdjust(Linknode **a,int i,int n)
{
    int lchild = 2*i;//����
    int rchild = 2*i+1;//�Һ���
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
            //�������֮����maxΪ���ڵ���������Ƕѣ�
            HeapAdjust(a, max, n);
        }
    }
}

//����
void BuildHeap(Linknode **a,int n){
    for (int i = n - 1; i > -1; --i) {
        HeapAdjust(a, i, n);
    }
}

//������
void HeapSort(Linknode **a,int n){
    BuildHeap(a, n);
    for (int i = n - 1; i >= 0; i--) {
        //�����Ѷ������һ��Ԫ�أ���ÿ�ν�ʣ��Ԫ���е�����߷ŵ����棻
        swap(&a[0], &a[i]);
        //���µ�����Ϊ�󶥶ѣ�
        HeapAdjust(a, 0, i );
    }
}

int main(int argc, char** argv)
{
//�趨��ʱ��
	clock_t start, finish; 
	/* ����һ���¼�������ʱ��*/ 
	start = clock(); 
	


//������ʼ---------------------------------- 
//ʹ���ڴ��ļ�ӳ������ȡinput.txt��fopen����ʵ��̫���� 
    //��һ���ļ�
    register HANDLE hin = CreateFileA("test.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hin == INVALID_HANDLE_VALUE)
    {
        printf("���ļ�ʧ��\n");
        system("pause");
        exit(0);
    }
    //��ȡ�ļ���С
    register unsigned int filesize = GetFileSize(hin, NULL);
    //printf("�ļ�����:%d\n", filesize);
    HANDLE hmap = CreateFileMappingA(hin,NULL,PAGE_READWRITE | SEC_COMMIT,0,
		filesize + 1,NULL);
    if (hmap == NULL)
    {
        printf("�ļ�ӳ��ʧ��\n");
        CloseHandle(hin);
        system("pause");
        exit(0);
    }
    //����һ��ָ�룬�洢ӳ���Ժ���׵�ַ
    register PVOID pvfile = MapViewOfFile(hmap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
    if (hmap == NULL)
    {
        printf("ָ��ӳ��ʧ��\n");
        CloseHandle(hin);CloseHandle(hmap);
        system("pause");
        exit(0);
    }
    //printf("�ļ�ӳ��ɹ�\n");
    register char *pstart = (char*)pvfile; //�׵�ַ
    register char *pend = pstart + filesize; //������ַ+1
    //system("pause");
    register char *fin=pstart;
    // ��ӳ����ļ��ж�����
    //*(pstart + pointer)��Ϊinput.txt�ĵ�pointer���ַ�

	register FILE *fout=fopen("outputTest.txt","wt");//��ȡ����ļ���ָ�� 
	char word[127];//�������ʵĻ����� 
	//get1word(fin,pend,word);
	//printf("%s\n",word);
	
	
	
//�ļ���׼����ɡ��������ֵ���ͳ��ÿ�����ʵĴ���-------------------------------
//���ļ���1�����ʣ�ͬʱ���ֵ��������������� 
//�������кܶ�goto����������״̬������⡣������״̬��
//δ���ϵ���״̬start�����ϵ���״̬gotLetter�����ϵ���������д��'\''״̬gotQuote 
	proot=newNode();
	register unsigned short i;//ָʾword�����ﵱǰӦ������һ������ 
	register char letter;
start://��ʼ 
	p=proot;
	while(fin<pend)//����ļ�û���� 
	{
		letter=isLetter(*(fin++));//isLetter����ֻ�����Сд��ĸ��'\0' 
		if(letter)//letter����ĸ
		{
			trieGo(letter);
			word[0]=letter;
			goto gotLetter;
		}
	}
	word[0]=0;//�����ִ����һ�䣬˵���ļ������ˣ���û���ϵ��� 
	goto end;
gotLetter://��������ĸ�����������״̬ 
	i=1;//ָʾword�����ﵱǰӦ������һ������ 
	while(fin<pend)
	{
		letter=*(fin++);
		if(letter=='\'')//������ 
		{
			goto gotQuote;
		}
		if(letter=isLetter(letter))//����ĸ 
		{
			trieGo(letter);
			word[i++]=letter;
		}
		else
		{
			word[i]=0;
			wordCount(word);
			goto start;//�������ļ� 
		}
	}
	//ִ�е�����˵����ǰ�����˵��ʣ�����������ʽ���ʱ�ļ�Ҳ������ 
	word[i]=0;
	wordCount(word);
	goto end;
gotQuote://֮ǰһֱ����ĸ����������������д�õĵ����� 
//��gotLetter״̬������������ڣ�������������ž��㵥�ʽ����� 
	while(fin<pend)
	{
		letter=*(fin++);
		if(letter=isLetter(letter))//����ĸ 
		{
			trieGo(letter);
			word[i++]=letter;
		}
		else
		{
			word[i]=0;
			wordCount(word);
			goto start;
		}
	}	
	//ִ�е�����˵����ǰ�����˵��ʣ�����������ʽ���ʱ�ļ�Ҳ������ 
	word[i]=0;
	wordCount(word);
	goto end;
end:
	//ͳ����� 
	//�ر�input.txt 
    UnmapViewOfFile(pvfile);CloseHandle(hin);CloseHandle(hmap);CloseHandle(hin);	
	
	//��sortHelper����
	HeapSort(sortHelper,maxcount);
	//���sortHelper
	for(i=0;i<maxcount;++i)
	{
		fprintf(fout,"%s %d\n",sortHelper[i]->word,sortHelper[i]->count);
	} 


//ֹͣ��ʱ��---------------------------------- 
	finish = clock(); 
	double Total_time = (double)(finish-start) / CLOCKS_PER_SEC; 
	printf( "%f seconds", Total_time); 
	return 0;
}

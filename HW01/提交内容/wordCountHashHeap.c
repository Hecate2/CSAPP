//ʹ�ù�ϣ���ң� 
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <windows.h>

typedef struct linknode//�������浥�ʼ����� 
{
	char *word;
	unsigned int count;
	struct linknode *next;
}Linknode;
Linknode *p;//����ָ�� 
int sizeofLinknode=sizeof(Linknode);

#define HASHSPACE	1048575
//��HASHSPACE��ô��ĸ��Ӵ�Ų�ͬ�ĵ��� 
Linknode *hashspace[HASHSPACE]; 

Linknode* newLinknode(char* word)//�½�ָ��������ָ�Ľṹ 
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

unsigned short wordCount(char *word,unsigned short length)//���ʱ���͵��ʳ��� 
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

//�����Ƕ������� 
//����������
void swap(Linknode *a,Linknode *b){
    Linknode temp;
    temp = *a;    *a = *b;    *b = temp;
}

//������
void HeapAdjust(Linknode *a,int i,int n)
{
    int lchild = 2*i;//����
    int rchild = 2*i+1;//�Һ���
    if (i <= n) {
	    int max = i;
        if (lchild < n && a[lchild].count > a[max].count) {
            max = lchild;
        }
        if (rchild < n && a[rchild].count > a[max].count) {
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
void BuildHeap(Linknode *a,int n){
    for (int i = n - 1; i > -1; --i) {
        HeapAdjust(a, i, n);
    }
}

//������
void HeapSort(Linknode *a,int n){
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
    register HANDLE hin = CreateFileA("input.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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

	register FILE *fout=fopen("outputHashHeap.txt","wt");//��ȡ����ļ���ָ�� 
	char word[127];//�������ʵĻ����� 
	//get1word(fin,pend,word);
	//printf("%s\n",word);
	
	
	
//�ļ���׼����ɡ����濪ʼ���ļ�����ϣ������----------------------------------
	register unsigned long long i;//ָʾword�����ﵱǰӦ������һ������ 
	register char letter,tmpletter;
	register unsigned short conflict=0;//ͳ�ƹ�ϣ��ͻ���� 
	for(i=0;i<HASHSPACE;++i)
	{
		hashspace[i]=NULL;
	}
start://��ʼ 
	while(fin<pend)//����ļ�û���� 
	{
		letter=isLetter(*(fin++));//isLetter����ֻ�����Сд��ĸ��'\0' 
		if(letter)//letter����ĸ
		{
			word[0]=letter;
			goto gotLetter;
		}
	}
	word[0]=0;//�����ִ����һ�䣬˵���ļ������ˣ���û���ϵ��� 
	goto end;
gotLetter://��������ĸ�����������״̬ 
	i=1;//ָʾword�����ﵱǰӦ������һ���������������ʳ��� 
	while(fin<pend)
	{
		letter=*(fin++);
		if(tmpletter=isLetter(letter))//����ĸ 
		{
			word[i++]=tmpletter;
			continue;
		}
		if(letter=='-')//�����ַ� 
		{
			continue;
		}
		if(letter=='\'')//�����ţ���ʾ��д�� 
		{
			goto gotQuote;
		}
		//ִ�е�����˵��һ�����ʽ����� 
		word[i++]=0;
		conflict+=wordCount(word,i);
		goto start;//�������ļ� 
	}
	//ִ�е�����˵���ļ������� 
	word[i++]=0;
	conflict+=wordCount(word,i);
	goto end;
gotQuote://֮ǰһֱ����ĸ����������������д�õĵ����� 
//��gotLetter״̬������������ڣ�������������ž��㵥�ʽ����� 
	while(fin<pend)
	{
		letter=*(fin++);
		if(tmpletter=isLetter(letter))//����ĸ 
		{
			word[i++]=tmpletter;
			continue;
		}
		if(letter=='-')
		{
			continue;
		}
		//ִ�е�����˵�����ʽ����� 
		word[i++]=0;
		conflict+=wordCount(word,i);
		goto start;
	}	
	//ִ�е�����˵���ļ������� 
	word[i++]=0;
	conflict+=wordCount(word,i);
	goto end;
end:
	//ͳ����� 
	//�ر�input.txt 
    UnmapViewOfFile(pvfile);CloseHandle(hin);CloseHandle(hmap);CloseHandle(hin);	
    Linknode sortHelper[65535];
    register unsigned long long j=0;//����HASHSPACE 
    i=0;//����sortHelper 
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
				}
				else
				{
					sortHelper[i++]=*p;
				}
				p=p->next;
			}
			while(p);
		}
	}
	//��sortHelper����
	HeapSort(sortHelper,i);
	//���sortHelper
	j=i;
	for(i=0;i<j;++i)
	{
		fprintf(fout,"%s %d\n",sortHelper[i].word,sortHelper[i].count);
	}



//ֹͣ��ʱ��---------------------------------- 
	finish = clock(); 
	double Total_time = (double)(finish-start) / CLOCKS_PER_SEC; 
	printf( "%f seconds\n", Total_time); 
	printf("��ϣ��ͻ����:%d",conflict);
	return 0;
}

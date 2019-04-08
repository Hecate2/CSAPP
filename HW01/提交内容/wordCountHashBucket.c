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

#define MAXCOUNT 200000
//һ����������������MAXCOUNT�Σ���ʮ�� 

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

	register FILE *fout=fopen("outputHashBucket.txt","wt");//��ȡ����ļ���ָ�� 
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
    Linknode* bucket[MAXCOUNT];
	//MAXCOUNT�С�ÿһ����һ�������ͷָ�� 
	//������洢���ִ������������ĵ��� 
	for(i=0;i<MAXCOUNT;++i){bucket[i]=NULL;}
    register unsigned long long j=0;//����HASHSPACE 
    register Linknode *pbuck;//bucket�еĹ���ָ�� 
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
	//���bucket
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



//ֹͣ��ʱ��---------------------------------- 
	finish = clock(); 
	double Total_time = (double)(finish-start) / CLOCKS_PER_SEC; 
	printf( "%f seconds\n", Total_time); 
	printf("��ϣ��ͻ����:%d",conflict);
	return 0;
}

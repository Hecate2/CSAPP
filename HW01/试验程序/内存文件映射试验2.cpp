#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

char path[128] = "input.txt";

int main()
{
    //��һ���ļ�
    HANDLE hfile = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hfile == INVALID_HANDLE_VALUE)
    {
        printf("���ļ�ʧ��\n");
        system("pause");
        exit(0);
    }
    //��ȡ�ļ���С
    unsigned int filesize = GetFileSize(hfile, NULL);

    printf("�ļ�����:%d\n", filesize);

    HANDLE hmap = CreateFileMappingA(hfile, //�ļ�ӳ��
        NULL,
        PAGE_READWRITE | SEC_COMMIT,
        0,
        filesize + 1,
        NULL);

    if (hmap == NULL)
    {
        printf("ӳ��ʧ��\n");
        CloseHandle(hfile);
        system("pause");
        exit(0);
    }

    //����һ��ָ�룬�洢ӳ���Ժ���׵�ַ
    PVOID pvfile = MapViewOfFile(hmap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

    if (hmap == NULL)
    {
        printf("ָ��ӳ��ʧ��\n");
        CloseHandle(hfile);
        CloseHandle(hmap);
        system("pause");
        exit(0);
    }

    printf("�ļ�ӳ��ɹ�\n");

    char *pstart = (char*)pvfile; //�׵�ַ
    char *pend = pstart + filesize; //������ַ
    //system("pause");
    int i;

    // ��ӳ����ļ��ж�����
    for (i = 0; i < 200; i++)
    {
        putchar(*(pstart + i));
    }

    printf("\n");
    system("pause");

    UnmapViewOfFile(pvfile);
    CloseHandle(hfile);
    CloseHandle(hmap);
    CloseHandle(hfile);

    system("pause");
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

char path[128] = "input.txt";

int main()
{
    //打开一个文件
    HANDLE hfile = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hfile == INVALID_HANDLE_VALUE)
    {
        printf("打开文件失败\n");
        system("pause");
        exit(0);
    }
    //获取文件大小
    unsigned int filesize = GetFileSize(hfile, NULL);

    printf("文件长度:%d\n", filesize);

    HANDLE hmap = CreateFileMappingA(hfile, //文件映射
        NULL,
        PAGE_READWRITE | SEC_COMMIT,
        0,
        filesize + 1,
        NULL);

    if (hmap == NULL)
    {
        printf("映射失败\n");
        CloseHandle(hfile);
        system("pause");
        exit(0);
    }

    //创建一个指针，存储映射以后的首地址
    PVOID pvfile = MapViewOfFile(hmap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

    if (hmap == NULL)
    {
        printf("指针映射失败\n");
        CloseHandle(hfile);
        CloseHandle(hmap);
        system("pause");
        exit(0);
    }

    printf("文件映射成功\n");

    char *pstart = (char*)pvfile; //首地址
    char *pend = pstart + filesize; //结束地址
    //system("pause");
    int i;

    // 从映射的文件中读内容
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


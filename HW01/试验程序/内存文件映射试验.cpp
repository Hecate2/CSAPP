#define  _CRT_SECURE_NO_WARNINGS 1
 
#include <stdio.h>
 
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
 
 
#define LOGD printf
 
#if _WIN32
#include <windows.h>
#include <strsafe.h>
#include <io.h>
#define stat _stat
 
#else
 
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif
 
 
int getFileSizeByStat(char * strFileName)
{
	struct stat temp;
	stat(strFileName, &temp);
	return temp.st_size;
}
 
 
#if _WIN32
int test_mmaping(char *filename)
{
	int time = GetTickCount();
	HANDLE hFile = CreateFileA(filename,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		printf("CreateFile : %u\n", GetLastError());
		//ErrorExit((LPTSTR)"CreateFile");
		return -1;
	}
	int filesize = getFileSizeByStat(filename);
 
	//读写文件
	HANDLE hMap = CreateFileMapping(hFile,
		NULL,
		PAGE_READWRITE,
		0,
		filesize,
		NULL
		);
	if (!hMap) {
		printf("CreateFileMapping : %u\n", GetLastError());
		//ErrorExit((LPTSTR)"CreateFileMapping");
		CloseHandle(hFile);
		return -1;
	}
 
	char *buf = (char *)MapViewOfFile(hMap,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		filesize);
	if (!buf) {
		printf("MapViewOfFile : %u\n", GetLastError());
		CloseHandle(hMap);
		CloseHandle(hFile);
		return -1;
	}
	int i = 0;
	for (i = 0; i < filesize; i++)
	{
		buf[i] = buf[i] ^ 55;
	}
	/**
	char *dummy = (char *)malloc(filesize);
	memcpy(dummy, buf, filesize);
	free(dummy);
	*/
 
	UnmapViewOfFile(buf);
	CloseHandle(hMap);
	CloseHandle(hFile);
 
 
	time = GetTickCount() - time;
	printf("test_mmaping cost time:%d ms\n", time);
	return 0;
}
 
#else
 
//获取到毫秒
long getCurrentTime()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
 
#define handle_error(msg) \
do { perror(msg); exit(EXIT_FAILURE); } while (0)
 
int test_mmaping(char *filename)
{
	long t_start, t_end;
	t_start=getCurrentTime();
 
	char *memblock;
	int fd;
	struct stat sb;
 
	//读写文件
	fd = open(filename,  O_RDWR);
	fstat(fd, &sb);
	int filesize=(uint64_t)sb.st_size;
	printf("Size: %lu\n", filesize);
 
	memblock = mmap(NULL, filesize, PROT_WRITE, MAP_SHARED, fd, 0);
 
	if (memblock == MAP_FAILED)
		handle_error("mmap");
	close(fd);
 
	int i=0;
	for ( i = 0; i < filesize; i++)
	{
		// printf("[%lu]=%X ", i, memblock[i]);
		memblock[i] = memblock[i] ^55;
	}
	printf("\n");
	if ((msync((void *)memblock, filesize, MS_SYNC)) == -1)
	{
		perror("msync");
	}
	munmap(memblock,filesize);
 
 
	t_end=getCurrentTime();
	long cha=t_end-t_start;
	LOGD("test_mmaping finish! take time=%d ms\n",cha);
 
 
	return 0;
}
 
#endif
 
/**
//获取到微秒
struct timeval start, end;
gettimeofday( &start, NULL );
sleep(3);
gettimeofday( &end, NULL );
int timeuse = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;
printf("time: %d us\n", timeuse);
*/
 
int test_crt(char *name)
{
#if _WIN32
	int t_start = GetTickCount();
#else
	long t_start, t_end;
	t_start=getCurrentTime();
#endif
 
	FILE *fp = fopen(name, "rb+");
	if (!fp) {
		perror("fopen");
		return -1;
	}
	int filesize = getFileSizeByStat(name);
	char *dummy = (char *)malloc(filesize);
	fread(dummy, filesize, 1, fp);
 
	int i = 0;
	for (i = 0; i < filesize; i++)
	{
		dummy[i] = dummy[i] ^ 55;
	}
	rewind(fp);
	fwrite(dummy, filesize, 1, fp);
 
	fclose(fp);
	free(dummy);
 
	// sleep(3);
#if _WIN32
	int t_end = GetTickCount();
	int cha = t_end - t_start;
#else
	t_end=getCurrentTime();
	long cha=t_end-t_start;
#endif
	printf("test_crt   cost time:%d ms\n", cha);
 
	return 0;
}
 
//c语言实现mmap内存映射读取文件和文件加密
int main()
{
	clock_t start, finish; 
	double Total_time; 
	//test 500MB file
	//test_crt("input.txt");
	printf("========\n");
	start = clock(); 
	test_mmaping("input.txt");
	finish = clock(); 
	Total_time = (double)(finish-start) / CLOCKS_PER_SEC; 
	printf( "%f seconds", Total_time); 
	
	/**
	test_crt("wangzhe.apk");
	printf("========\n");
	test_mmaping("wangzhe.apk");
	*/
	return 0;
}


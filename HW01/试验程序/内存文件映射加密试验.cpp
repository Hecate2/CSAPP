#define  _CRT_SECURE_NO_WARNINGS 1
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
 
#if _WIN32
#include <io.h>
#define stat _stat
#endif
int getFileSize(char * strFileName)
{
	struct stat temp;
	stat(strFileName, &temp);
	return temp.st_size;
}
 
//大文件简单加密处理
extern void jiajieWithPwd(char *source, char *out, char *pwd) {
 
 
	int fileSize = getFileSize(source);
	printf("fileSize=%d\n", fileSize);
	int length = strlen(pwd);
	FILE *fsource = fopen(source, "rb");
	FILE *fout = fopen(out, "wb");
	if (fsource == NULL || fout == NULL) {
		perror("fsource == NULL || fout == NULL");
		return;
	}
	int i = 0;
	int j = 0;
	for (; i < fileSize / length; i++) {
		for (j = 0; j < length; j++) {
			int ch = fgetc(fsource);
			ch = ch ^ pwd[j];
			fputc(ch, fout);
		}
	}
	j = 0;
	for (; j < fileSize % length; j++) {
		int ch = fgetc(fsource);
		ch = ch ^ pwd[j];
		fputc(ch, fout);
	}
	fclose(fsource);
	fclose(fout);
}
 
 
int main()
{
	char key[] = "1024";
	jiajieWithPwd("d:/test/wangzhe.apk", "d:/test/wangzhe.zip", key);
 
	jiajieWithPwd("d:/test/wangzhe.zip", "d:/test/wangzhe_out.apk", key);
 
 
	return 0;
}


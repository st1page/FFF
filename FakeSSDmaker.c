#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define SECTOR_SIZE (512)
int SECTOR_NUM;
char sector_null[SECTOR_SIZE];
#define PATH ("data")
char filename[100];
void gen(){
	mkdir(PATH,0777);
	chdir(PATH);
	system("rm *.sector");
	for(int i=0;i<SECTOR_NUM;i++){
		sprintf(filename, "%d", i);
		strcat(filename, ".sector");
		FILE *fp = fopen(filename, "w");
		for(int j=0;j<SECTOR_SIZE;j++) fputs( "00 ", fp );
		fclose(fp);
	}
	FILE *fp = fopen("property", "w");
	fprintf(fp,"%d\n%d",SECTOR_SIZE,SECTOR_NUM);
	return ;
}
int main(){
	int x;
	puts("Warning : 生成FSSD会将\\data内文件清空");
	printf("输入磁盘大小（单位 MB） : ");
	scanf("%d",&x);
	printf("%d\n",x);
	if(x<=0) {
		puts("喵喵喵？");
		return 0;
	}
	if(x>16) {
		puts("SSD太贵，16MB以下吧w\n");
		return 0;
	}
	SECTOR_NUM = 1024*1024/SECTOR_SIZE*x; 
	gen();
	return 0;
}
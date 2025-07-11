#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include "com.h"
#include "com.c"
unsigned char func(unsigned char a);
int main(void)
{
	unsigned char tmp[1]= {0}; //用于存放读取出来的数据的缓冲区
	int rl; //读取数据的长度（单位：字节）
	int i;
 
	fd = openSerial("/dev/ttyUSB0"); //打开串口，ttyUSB0是串口文件
	
	if(fd < 0)
	{
		printf("open com fail!\n");
		return 0;
	}
	
	EpollInit(fd); //初始化终端事件触发函数epoll,设置要监听的事件及相关参数等
	
	unsigned char buf[]={0xAA,0x55,0x02,0x00,0x02,0x00,0x00,0x04,0x00,0x06,0x01,0x04,0x00,0x09,0x00,0x00,0x00,0x00};
	if(write(fd,buf,14)<=0) printf("write failed!\n");
	int count=0;
	int start=1000;
	unsigned char temp; 
	
	while(1){
		//bzero(tmp,sizeof(tmp)); 				//置零
		rl = ComRead(tmp,1);					//读取数据
		printf(" %02x", tmp[0]);
		//识别前导AA 55
		if(count>=2){
			count++;
			if(count==3) start=(int)tmp[0];		//起始位置
			if(count>=start) buf[count-start+2]=tmp[0];
			if(count-start==3) {
				write(fd,buf,6);
				//printf("\n write %2x %2x %2x %2x %2x %2x\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
				count=0;				//清0
			}
		}
		if(tmp[0]==0x55&&temp==0xAA) {
			count=2;
			printf("\n");
		}
		temp=tmp[0];						//上一个字节
	}
	close(epid);
	close(fd);
	return 0;
}



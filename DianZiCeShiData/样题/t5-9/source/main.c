#include "STC15F2K60S2.H"
#include "sys.H"
#include "displayer.H"  
 
code unsigned long SysClock=11059200;
 
#ifdef _displayer_H_
code char decode_table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x08,0x40,0x01,0x76,0x38};   
                /* 序号:   0    1    2    3    4    5    6    7    8    9    10     11	 12   13   14   15  */
                /* 显示:   0    1    2    3    4    5    6    7    8    9    (无)   下-  中-  上-   H    L  */  
#endif
 
long min=0,sec=0,ms=0,temp=0;
char oput[8]={0};
 
void fun(){
	temp++;
	ms=temp;
	sec=(temp/1000)%60;
	min=temp/60000;
	oput[0]=ms%10;
	oput[1]=(ms/10)%10;
	oput[2]=(ms/100)%10;
	oput[3]=12;
	oput[4]=sec%10;
	oput[5]=(sec/10)%10;
	oput[6]=12;
	oput[7]=min;
	Seg7Print(oput[7],oput[6],oput[5],oput[4],oput[3],oput[2],oput[1],oput[0]);
}
 
int main(){
	DisplayerInit(); 	
	SetDisplayerArea(0,7);
	Seg7Print(0,0,0,0,0,0,0,0);
	LedPrint(0);
	SetEventCallBack(enumEventSys1mS, fun);
	MySTC_Init();
	while(1)
		MySTC_OS();
}
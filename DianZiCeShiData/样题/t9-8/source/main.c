#include "STC15F2K60S2.H"
#include "sys.H" 
#include "displayer.H"
#include "key.H"
#include "adc.h"  
 
code unsigned long SysClock=11059200;
 
#ifdef _displayer_H_
code char decode_table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x08,0x40,0x01,0x76,0x38};   
	             /* 序号:   0    1    2    3    4    5    6    7    8    9    10     11	 12   13   14   15  */
                 /* 显示:   0    1    2    3    4    5    6    7    8    9    (无)   下-  中-  上-   H    L  */ 
#endif
 
unsigned char temp[]={0xc0,0x60,0x30,0x18,0x0c,0x06,0x03,0x81};
int i=3;
void mydh_callback()
{
	if(GetAdcNavAct(enumAdcNavKeyLeft)==enumKeyPress)
	{
		if(i>0)
			i--;
		else i=7;
	}
	else if(GetAdcNavAct(enumAdcNavKeyRight)==enumKeyPress)
	{
		if(i<7)
			i++;
		else i=0;
	}
	LedPrint(temp[i]);
}
 
 
void main(){
	DisplayerInit();
	KeyInit();
	AdcInit(ADCexpEXT);
	SetDisplayerArea(0,7);
	LedPrint(temp[3]);
	Seg7Print(10,10,10,10,10,10,10,10);
	SetEventCallBack(enumEventNav, mydh_callback);
	MySTC_Init();
	while(1)
		MySTC_OS();
}
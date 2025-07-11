#include "STC15F2K60S2.H"
#include "sys.H"
#include "displayer.H"
#include "beep.h"
#include "adc.h"
 
code unsigned long SysClock=11059200;
 
#ifdef _displayer_H_ 
code char decode_table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x08,0x40,0x01,0x76,0x38};   
	             /* 序号:   0    1    2    3    4    5    6    7    8    9    10     11	 12   13   14   15  */
                 /* 显示:   0    1    2    3    4    5    6    7    8    9    (无)   下-  中-  上-   H    L  */ 
 
void Rop_callback(){
	if(GetADC().Rop<20)
		SetBeep(1200,150);
	else if(GetADC().Rop>100){
		SetBeep(2000,150);
	}
}
void Per100ms_displayer(){
	char gewei = (GetADC().Rop % 10);
    char shiwei = ((GetADC().Rop) / 10) % 10;
    char baiwei = ((GetADC().Rop) / 100) % 10;
	if(GetADC().Rop>100){
		Seg7Print(10,10,10,10,10,baiwei,shiwei,gewei);
	}
	else{
		Seg7Print(10,10,10,10,10,10,shiwei,gewei);
	}
}
 
int main(){
	DisplayerInit(); 
	BeepInit();
	AdcInit();
	SetDisplayerArea(0,7);
	Seg7Print(10,10,10,10,10,10,10,10);
	LedPrint(0);
	SetEventCallBack(enumEventSys100mS, Per100ms_displayer);
	SetEventCallBack(enumEventXADC, Rop_callback);
    MySTC_Init();
    while(1){
     MySTC_OS();
	}		
}
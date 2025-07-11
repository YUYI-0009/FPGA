#include "STC15F2K60S2.H"
#include "sys.H"
#include "beep.H"   
#include "Vib.h"
 
code unsigned long SysClock=11059200;
 
void mysv_callback()
{
	if(GetVibAct()==enumVibQuake)
		SetBeep(1200,100);
}
 
 
void main(){
	
	BeepInit();
	VibInit();
	SetEventCallBack(enumEventVib, mysv_callback);
	MySTC_Init();
	while(1)
		MySTC_OS();
	
}
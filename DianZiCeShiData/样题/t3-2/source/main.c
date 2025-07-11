#include "STC15F2K60S2.H"
#include "sys.H"
#include "beep.H"
 
code unsigned long SysClock=11059200;
 
int main(){
	
	BeepInit();
	SetBeep(1000,150);
	MySTC_Init();
	while(1)
		MySTC_OS();
 
}
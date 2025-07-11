#include "STC15F2K60S2.H"
#include "sys.H"
#include "displayer.H"            
#include "key.H" 
 
code unsigned long SysClock=11059200;
 
#ifdef _displayer_H_                          
code char decode_table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x08,0x40,0x01,0x76,0x38};
                 /* 序号:   0    1    2    3    4    5    6    7    8    9    10     11	 12   13   14   15  */
                 /* 显示:   0    1    2    3    4    5    6    7    8    9    (无)   下-  中-  上-   H    L  */  
 
#endif
 
char a=10;
 
void myKey_callback()
{   char k;
	k=GetKeyAct(enumKey1);
	if( k == enumKeyPress ) a=14;
	  else if( k == enumKeyRelease ) a=10;
	Seg7Print(10,10,10,10,10,10,10,a);
}
 
//void my10mS_callback(){
//Seg7Print(10,10,10,10,10,10,10,a);}
 
void main(){
	 DisplayerInit();   
	 KeyInit(); 
	 LedPrint(0);
	 SetDisplayerArea(0,7);
	 //SetEventCallBack(enumEventSys10mS, my10mS_callback);
	 SetEventCallBack(enumEventKey, myKey_callback);
	 MySTC_Init();
	 while(1){
		 MySTC_OS();
	 }
}
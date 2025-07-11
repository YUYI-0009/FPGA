#include "STC15F2K60S2.H"        //���롣
#include "sys.H"                 //���롣
#include "displayer.H" 
#include "hall.h"
#include "beep.h"

code unsigned long SysClock=11059200;         //���롣����ϵͳ����ʱ��Ƶ��(Hz)���û������޸ĳ���ʵ�ʹ���Ƶ�ʣ�����ʱѡ��ģ�һ��
#ifdef _displayer_H_                          //��ʾģ��ѡ��ʱ���롣���������ʾ������Ñ����޸ġ����ӵȣ� 
code char decode_table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x08,0x40,0x01, 0x41, 0x48, 
	              /* ���:   0   1    2	   3    4	    5    6	  7   8	   9	 10	   11		12   13    14     15     */
                /* ��ʾ:   0   1    2    3    4     5    6    7   8    9  (��)   ��-  ��-  ��-  ����-  ����-   */  
	                       0x3f|0x80,0x06|0x80,0x5b|0x80,0x4f|0x80,0x66|0x80,0x6d|0x80,0x7d|0x80,0x07|0x80,0x7f|0x80,0x6f|0x80 };  
             /* ��С����     0         1         2         3         4         5         6         7         8         9        */
#endif
unsigned char a;
void myHall_callback()
{ if( GetHallAct() == enumHallGetClose) 
		{	a=1;
			SetBeep(1000,20);
		}
}

void my100mS_callback()
{ if (a == 0xff) a=0;
	if (a != 0) a=(a<<1)+1;
	LedPrint(a);
}

void main() 
{ DisplayerInit();  
	HallInit();
	BeepInit();
	SetEventCallBack(enumEventHall,myHall_callback);
	SetEventCallBack(enumEventSys100mS,my100mS_callback);
	SetDisplayerArea(0,7);	
  Seg7Print(1,2,3,4,5,6,7,8);	
  MySTC_Init();	    
	while(1)             	
		{ MySTC_OS();    
		}	             
}                 
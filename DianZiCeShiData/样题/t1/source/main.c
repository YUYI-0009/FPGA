#include "STC15F2K60S2.H"
#include "displayer.H" 
#include "sys.H"
 
 
code unsigned long SysClock=11059200;         //必须。 （定义系统工作时钟频率，用户可以修改，且必须与实际工作频率（下载时选择的）一致
 
#ifdef _displayer_H_                          //显示模块选用时必须。（数码管显示译码表，用戶可修改、增加等） 
code char decode_table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x08,0x40,0x01,0x77,0x7c,0x39,0x5e,0x79,0x78,0x76,0x38,0x54,0x5c,0x73,0x67,0x3e,0x6e};  
	              /* 序号:   0    1    2    3    4    5    6    7    8    9    10     11	 12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27 */
                  /* 显示:   0    1    2    3    4    5    6    7    8    9    (无)   下-    中-   上-   A    b    C    d    E    F    H    L    n    o    P    q    U    y */  
#endif
 
 
 
void main()
{  				
	
	displayerInit();                              
	LedPrint(0);
    SetDisplayerArea(0,6);                           
	Seg7Print(1,2,20,21,12,2,1);	 
	MySTC_Init();
	while(1)
		MySTC_OS();
}
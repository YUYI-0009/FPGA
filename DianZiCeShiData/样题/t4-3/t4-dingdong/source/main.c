#include "STC15F2K60S2.H"
#include "sys.H"
#include "displayer.H" 
#include "Beep.H"
#include "music.H"
 
code unsigned long SysClock=11059200;
 
#ifdef _displayer_H_ 
code char decode_table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x08,0x40,0x01,0x76,0x38};
                 /* 序号:   0    1    2    3    4    5    6    7    8    9    10     11	 12   13   14   15  */
                 /* 显示:   0    1    2    3    4    5    6    7    8    9    (无)   下-  中-  上-   H    L  */  
 
#endif
 
// 修改音乐数据为两声"叮咚"
code unsigned char arr[]={
    0x31, 0x20,  // "叮" (中音C, 1/2拍)
    0x00, 0x10,  // 休止符 (1/4拍)
    0x35, 0x20,   // "咚" (低音G, 1/2拍)
	0x00, 0x20,
	 0x31, 0x20,  // "叮" (中音C, 1/2拍)
    0x00, 0x10,  // 休止符 (1/4拍)
    0x35, 0x20   // "咚" (低音G, 1/2拍)
};

int main(){
    DisplayerInit();
    LedPrint(0);
    SetDisplayerArea(0,7);
    BeepInit();
    MusicPlayerInit();
    SetPlayerMode(enumModePlay);
    SetMusic(100, 0xFA, arr, 14, enumMscDrvSeg7andLed); // 注意长度改为6
    MySTC_Init();
    
    while(1){
        MySTC_OS();
    }
}
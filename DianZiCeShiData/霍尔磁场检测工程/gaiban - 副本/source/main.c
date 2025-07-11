 #include "STC15F2K60S2.H"
#include "sys.H"
#include "displayer.H" 
#include "hall.H"
#include "beep.h"

code unsigned long SysClock = 11059200;
#ifdef _displayer_H_                         
code char decode_table[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x00, 0x08, 0x40, 0x01, 0x41, 0x48,
    0x3f|0x80, 0x06|0x80, 0x5b|0x80, 0x4f|0x80, 0x66|0x80, 0x6d|0x80, 0x7d|0x80, 0x07|0x80, 0x7f|0x80, 0x6f|0x80
};
#endif

// 流水灯相关变量
unsigned char flowLightState = 0;     // 流水灯当前状态（8位）
unsigned char flowLightCounter = 0;   // 流水灯更新计数器
bit magnetState = 0;                  // 磁场状态标志 (0:离开, 1:靠近)

int c=0;
void myHall_callback()
{
    if (GetHallAct() == enumHallGetClose) 
    {
        magnetState = 1;              // 设置磁场靠近标志
       // flowLightState = 0x0F;        // 初始化：低4位亮（4个灯同时亮）
        //flowLightCounter = 0;         // 重置计数器
       // SetBeep(1000, 20);            // 蜂鸣器报警
    }
    else
    {

         SetBeep(1000, 20);            // 蜂鸣器报警
        magnetState = 0;              // 清除磁场靠近标志
        flowLightState = 0x0F;  
              // 关闭所有LED
    }
    LedPrint(flowLightState);         // 立即更新LED显示
}

void my100mS_callback()
{
    if (!magnetState)  // 如果磁场靠近，执行流水灯效果

    {
        SetBeep(1000, 20);   

        if(c==1) {
            magnetState=1;
        }
        flowLightCounter++;
        
        // 每3个周期更新一次流水灯状态（约300ms移动一次）
        if (flowLightCounter >= 3)
        {
            flowLightCounter = 0;
            
            // 8位循环左移（4个灯同时移动）
            // 最高位移到最低位实现循环效果
            flowLightState = flowLightState >> 1;
             c=1;
        }
    }
    
    // 更新LED显示（8位全控制）
    LedPrint(flowLightState);
}

void main() 
{
    DisplayerInit();  
    HallInit();
   // BeepInit();
    
    // 设置事件回调函数
    SetEventCallBack(enumEventHall, myHall_callback);
    SetEventCallBack(enumEventSys100mS, my100mS_callback);
    
    // 设置显示区域（根据实际硬件调整）
    SetDisplayerArea(0, 7);	
    
    // 初始化显示（可选）
    Seg7Print(10, 10, 10,10, 10, 10, 10, 10);	
    
    // 系统初始化
    MySTC_Init();	    
    
    // 主循环
    while (1)
    { 
        MySTC_OS();  // 运行系统任务
    }
}
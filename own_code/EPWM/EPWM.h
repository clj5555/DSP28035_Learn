#ifndef EPWM_H_
#define EPWM_H_

#include "DSP2803x_Device.h"     // DSP2833x 头文件
#include "DSP2803x_Examples.h"   // DSP2833x 例子相关头文件



interrupt void epwm1_timer_isr(void);
interrupt void epwm2_timer_isr(void);
void EPWM1_Init(Uint16 tbprd);
void EPwm1A_SetCompare(Uint16 val);
void EPwm1B_SetCompare(Uint16 val);
void EPWM2_Init(Uint16 tbprd);
void EPwm2A_SetCompare(Uint16 val);
void EPwm2B_SetCompare(Uint16 val);








#endif


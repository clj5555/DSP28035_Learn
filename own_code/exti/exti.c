/*
 * exti.c
 *
 *  Created on: 2021-1-25
 *      Author: Administrator
 */


#include "DSP2803x_Device.h"     // DSP2833x 头文件
#include "DSP2803x_Examples.h"   // DSP2833x 例子相关头文件

// 定义要控制的变量
extern volatile Uint16 controlValue;

void EXTI1_Init(void)
{
    EALLOW;
    //KEY端口配置
    GpioCtrlRegs.GPBMUX1.bit.GPIO44=0;
    GpioCtrlRegs.GPBDIR.bit.GPIO44=0;
   // GpioCtrlRegs.GPBPUD.bit.GPIO44=0;
/*
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 0;        // 外部中断1与系统时钟SYSCLKOUT同步
    GpioCtrlRegs.GPBQSEL1.bit.GPIO44 = 2;        // 外部中断1（XINT1）输入限定6个采样窗口
    GpioCtrlRegs.GPBCTRL.bit.QUALPRD0= 0xFF;   // 每个采样窗口的周期为510*SYSCLKOUT
*/
    EDIS;

    EALLOW;
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 44;   // 指定XINT1是GPIO44
    XIntruptRegs.XINT1CR.bit.POLARITY = 1;      // 上升沿触发中断
    XIntruptRegs.XINT1CR.bit.ENABLE= 1;        // 使能XINT1
    EDIS;

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // 使能PIE组1的INT4
    IER |= M_INT1;                              // 使能CPU中断1（INT1）

    EALLOW; // 修改被保护的寄存器，修改前应添加EALLOW语句
    PieVectTable.XINT1 = &EXTI1_IRQn;
    EDIS;   // EDIS的意思是不允许修改被保护的寄存器
}

interrupt void EXTI1_IRQn(void)
{
    // 按键按下，变量值加 1
    if(controlValue < 65535)
    {
        controlValue++;
    }


/*    PieCtrlRegs.PIEACK.bit.ACK1=1;*/
    // 清除中断标志
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 0; // 清除 GPIO 中断标志
    XIntruptRegs.XINT1CR.bit.ENABLE = 1;     // 重新使能中断
}


/*
void EXTI2_Init(void)
{
//  EALLOW;
//  SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;    // GPIO input clock
//  EDIS;

    EALLOW;
    //KEY端口配置
    GpioCtrlRegs.GPAMUX2.bit.GPIO19=0;
    GpioCtrlRegs.GPADIR.bit.GPIO19=0;
    GpioCtrlRegs.GPAPUD.bit.GPIO19=0;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 2;        // 外部中断2（XINT2）输入限定6个采样窗口
    GpioCtrlRegs.GPACTRL.bit.QUALPRD1 = 0xFF;   // 每个采样窗口的周期为510*SYSCLKOUT
    EDIS;

    EALLOW;
    GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = 19;   // XINT2是GPIO19
    EDIS;

    EALLOW; // 修改被保护的寄存器，修改前应添加EALLOW语句
    PieVectTable.XINT2 = &EXTI2_IRQn;
    EDIS;   // EDIS的意思是不允许修改被保护的寄存器

    //PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
    PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          // 使能PIE组1的INT5

    XIntruptRegs.XINT2CR.bit.POLARITY = 0;      // 下降沿触发中断
    XIntruptRegs.XINT2CR.bit.ENABLE = 1;        // 使能XINT2

    IER |= M_INT1;                              // 使能CPU中断1（INT1）
    EINT;                                       // 开全局中断
    ERTM;
}

interrupt void EXTI2_IRQn(void)
{
    Uint32 i;
    for(i=0;i<10000;i++);    //键盘消抖动
    while(!KEY_H2);
    Solar_value-=100;
    LED3_TOGGLE;
    PieCtrlRegs.PIEACK.bit.ACK1=1;
}
*/

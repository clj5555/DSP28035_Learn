/*
 * led.c
 *
 *  Created on: 2018-1-20
 *      Author: Administrator
 */

#include "DSP2803x_Device.h"     // DSP2833x 头文件
#include "DSP2803x_Examples.h"   // DSP2833x 例子相关头文件
/*******************************************************************************
* 函 数 名         : LED_Init
* 函数功能         : LED初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void LED_Init(void)
{
    EALLOW;
    //SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// 开启GPIO时钟

    //LED1端口配置
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;//设置为通用GPIO功能
    GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;     //设置GPIO方向为输出
    GpioCtrlRegs.GPAPUD.bit.GPIO3=0;       //使能GPIO上拉电阻
    GpioDataRegs.GPASET.bit.GPIO3=1;

    //LED2端口配置
    GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 0;//设置为通用GPIO功能
    GpioCtrlRegs.GPBDIR.bit.GPIO44 = 1;     //设置GPIO方向为输出
    GpioCtrlRegs.GPBPUD.bit.GPIO44=0;       //使能GPIO上拉电阻
    GpioDataRegs.GPBSET.bit.GPIO44=1;

    //LED3端口配置
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 0;//设置为通用GPIO功能
    GpioCtrlRegs.GPADIR.bit.GPIO23 = 1;     //设置GPIO方向为输出
    GpioCtrlRegs.GPAPUD.bit.GPIO23=0;       //使能GPIO上拉电阻
    GpioDataRegs.GPASET.bit.GPIO23=1;
    EDIS;
}

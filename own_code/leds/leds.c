/*
 * led.c
 *
 *  Created on: 2018-1-20
 *      Author: Administrator
 */

#include "DSP2803x_Device.h"     // DSP2833x ͷ�ļ�
#include "DSP2803x_Examples.h"   // DSP2833x �������ͷ�ļ�
/*******************************************************************************
* �� �� ��         : LED_Init
* ��������         : LED��ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void LED_Init(void)
{
    EALLOW;
    //SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// ����GPIOʱ��

    //LED1�˿�����
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;//����Ϊͨ��GPIO����
    GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;     //����GPIO����Ϊ���
    GpioCtrlRegs.GPAPUD.bit.GPIO3=0;       //ʹ��GPIO��������
    GpioDataRegs.GPASET.bit.GPIO3=1;

    //LED2�˿�����
    GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 0;//����Ϊͨ��GPIO����
    GpioCtrlRegs.GPBDIR.bit.GPIO44 = 1;     //����GPIO����Ϊ���
    GpioCtrlRegs.GPBPUD.bit.GPIO44=0;       //ʹ��GPIO��������
    GpioDataRegs.GPBSET.bit.GPIO44=1;

    //LED3�˿�����
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 0;//����Ϊͨ��GPIO����
    GpioCtrlRegs.GPADIR.bit.GPIO23 = 1;     //����GPIO����Ϊ���
    GpioCtrlRegs.GPAPUD.bit.GPIO23=0;       //ʹ��GPIO��������
    GpioDataRegs.GPASET.bit.GPIO23=1;
    EDIS;
}

/*
 * key.c
 *
 *  Created on: 2018-1-22
 *      Author: Administrator
 */

#include "DSP2803x_Device.h"     // DSP2833x ͷ�ļ�
#include "DSP2803x_Examples.h"   // DSP2833x �������ͷ�ļ�

void KEY_Init(void)
{
    EALLOW;
    //SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;// ����GPIOʱ��

    //KEY�˿�����
    GpioCtrlRegs.GPAMUX1.bit.GPIO12=0;
    GpioCtrlRegs.GPADIR.bit.GPIO12=0;
    GpioCtrlRegs.GPAPUD.bit.GPIO12=0;

    GpioCtrlRegs.GPAMUX2.bit.GPIO19=0;
    GpioCtrlRegs.GPADIR.bit.GPIO19=0;
    GpioCtrlRegs.GPAPUD.bit.GPIO19=0;

    GpioCtrlRegs.GPAMUX2.bit.GPIO17=0;
    GpioCtrlRegs.GPADIR.bit.GPIO17=0;
    GpioCtrlRegs.GPAPUD.bit.GPIO17=0;

    GpioCtrlRegs.GPAMUX2.bit.GPIO18=0;
    GpioCtrlRegs.GPADIR.bit.GPIO18=0;
    GpioCtrlRegs.GPAPUD.bit.GPIO18=0;

    EDIS;
}

/*char KEY_Scan(char mode)
{

    static char keyl1=1;
    static char keyl2=1;
    static char keyl3=1;

    //��1��ɨ��
    KEY_L1_SetL;
    KEY_L2_SetH;
    KEY_L3_SetH;
    if(keyl1==1&&(KEY_H1==0||KEY_H2==0||KEY_H3==0))
    {
        DELAY_US(10000);
        keyl1=0;
        if(KEY_H1==0)
        {
            return KEY1_PRESS;
        }
        else if(KEY_H2==0)
        {
            return KEY4_PRESS;
        }
        else if(KEY_H3==0)
        {
            return KEY7_PRESS;
        }
    }
    else if(KEY_H1==1&&KEY_H2==1&&KEY_H3==1)
    {
        keyl1=1;
    }
    if(mode)
        keyl1=1;


    //��2��ɨ��
    KEY_L2_SetL;
    KEY_L1_SetH;
    KEY_L3_SetH;
    if(keyl2==1&&(KEY_H1==0||KEY_H2==0||KEY_H3==0))
    {
        DELAY_US(10000);
        keyl2=0;
        if(KEY_H1==0)
        {
            return KEY2_PRESS;
        }
        else if(KEY_H2==0)
        {
            return KEY5_PRESS;
        }
        else if(KEY_H3==0)
        {
            return KEY8_PRESS;
        }
    }
    else if(KEY_H1==1&&KEY_H2==1&&KEY_H3==1)
    {
        keyl2=1;
    }
    if(mode)
        keyl2=1;


    //��3��ɨ��
    KEY_L3_SetL;
    KEY_L1_SetH;
    KEY_L2_SetH;
    if(keyl3==1&&(KEY_H1==0||KEY_H2==0||KEY_H3==0))
    {
        DELAY_US(10000);
        keyl3=0;
        if(KEY_H1==0)
        {
            return KEY3_PRESS;
        }
        else if(KEY_H2==0)
        {
            return KEY6_PRESS;
        }
        else if(KEY_H3==0)
        {
            return KEY9_PRESS;
        }
    }
    else if(KEY_H1==1&&KEY_H2==1&&KEY_H3==1)
    {
        keyl3=1;
    }
    if(mode)
        keyl3=1;

    return KEY_UNPRESS;
}*/

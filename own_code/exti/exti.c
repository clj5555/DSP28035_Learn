/*
 * exti.c
 *
 *  Created on: 2021-1-25
 *      Author: Administrator
 */


#include "DSP2803x_Device.h"     // DSP2833x ͷ�ļ�
#include "DSP2803x_Examples.h"   // DSP2833x �������ͷ�ļ�

// ����Ҫ���Ƶı���
extern volatile Uint16 controlValue;

void EXTI1_Init(void)
{
    EALLOW;
    //KEY�˿�����
    GpioCtrlRegs.GPBMUX1.bit.GPIO44=0;
    GpioCtrlRegs.GPBDIR.bit.GPIO44=0;
   // GpioCtrlRegs.GPBPUD.bit.GPIO44=0;
/*
//  GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 0;        // �ⲿ�ж�1��ϵͳʱ��SYSCLKOUTͬ��
    GpioCtrlRegs.GPBQSEL1.bit.GPIO44 = 2;        // �ⲿ�ж�1��XINT1�������޶�6����������
    GpioCtrlRegs.GPBCTRL.bit.QUALPRD0= 0xFF;   // ÿ���������ڵ�����Ϊ510*SYSCLKOUT
*/
    EDIS;

    EALLOW;
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 44;   // ָ��XINT1��GPIO44
    XIntruptRegs.XINT1CR.bit.POLARITY = 1;      // �����ش����ж�
    XIntruptRegs.XINT1CR.bit.ENABLE= 1;        // ʹ��XINT1
    EDIS;

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // ʹ��PIE��1��INT4
    IER |= M_INT1;                              // ʹ��CPU�ж�1��INT1��

    EALLOW; // �޸ı������ļĴ������޸�ǰӦ���EALLOW���
    PieVectTable.XINT1 = &EXTI1_IRQn;
    EDIS;   // EDIS����˼�ǲ������޸ı������ļĴ���
}

interrupt void EXTI1_IRQn(void)
{
    // �������£�����ֵ�� 1
    if(controlValue < 65535)
    {
        controlValue++;
    }


/*    PieCtrlRegs.PIEACK.bit.ACK1=1;*/
    // ����жϱ�־
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 0; // ��� GPIO �жϱ�־
    XIntruptRegs.XINT1CR.bit.ENABLE = 1;     // ����ʹ���ж�
}


/*
void EXTI2_Init(void)
{
//  EALLOW;
//  SysCtrlRegs.PCLKCR3.bit.GPIOINENCLK = 1;    // GPIO input clock
//  EDIS;

    EALLOW;
    //KEY�˿�����
    GpioCtrlRegs.GPAMUX2.bit.GPIO19=0;
    GpioCtrlRegs.GPADIR.bit.GPIO19=0;
    GpioCtrlRegs.GPAPUD.bit.GPIO19=0;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 2;        // �ⲿ�ж�2��XINT2�������޶�6����������
    GpioCtrlRegs.GPACTRL.bit.QUALPRD1 = 0xFF;   // ÿ���������ڵ�����Ϊ510*SYSCLKOUT
    EDIS;

    EALLOW;
    GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = 19;   // XINT2��GPIO19
    EDIS;

    EALLOW; // �޸ı������ļĴ������޸�ǰӦ���EALLOW���
    PieVectTable.XINT2 = &EXTI2_IRQn;
    EDIS;   // EDIS����˼�ǲ������޸ı������ļĴ���

    //PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
    PieCtrlRegs.PIEIER1.bit.INTx5 = 1;          // ʹ��PIE��1��INT5

    XIntruptRegs.XINT2CR.bit.POLARITY = 0;      // �½��ش����ж�
    XIntruptRegs.XINT2CR.bit.ENABLE = 1;        // ʹ��XINT2

    IER |= M_INT1;                              // ʹ��CPU�ж�1��INT1��
    EINT;                                       // ��ȫ���ж�
    ERTM;
}

interrupt void EXTI2_IRQn(void)
{
    Uint32 i;
    for(i=0;i<10000;i++);    //����������
    while(!KEY_H2);
    Solar_value-=100;
    LED3_TOGGLE;
    PieCtrlRegs.PIEACK.bit.ACK1=1;
}
*/

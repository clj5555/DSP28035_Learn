/*
 * adc.c
 *
 *  Created on: 2018-1-29
 *      Author: Administrator
 */

#include "adc.h"


// ȫ�ֱ��������ڴ洢ѭ������
Uint16 LoopCount;
Uint16 ConversionCount; //ȫ�ֱ��������ڴ洢ת������
Uint16 Voltage1[10];
Uint16 Voltage2[10];


// ��ʼ��ADCģ��
void ADC_Init(void)
{
    EALLOW; // ��������ܱ����ļĴ���
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;    // ADC ʹ������ʱ��
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;   // Disable TBCLK within the ePWM
    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;  // ePWM1
    EDIS; // ��ֹ�����ܱ����ļĴ���

    // Specific clock setting for this example:28335
//    EALLOW;
//    SysCtrlRegs.HISPCP.all = ADC_MODCLK;    // HSPCLK = SYSCLKOUT/2*ADC_MODCLK
//    EDIS;

    // �ж�ӳ������
    EALLOW;
    PieVectTable.ADCINT1 = &adc_isr; // ��ADCINT1�ж�ӳ�䵽adc_isr�жϷ�������
    EDIS;

    InitAdc();  // ��ʼ��ADCģ��
    AdcOffsetSelfCal(); // ִ��ADCƫ����У׼

    // ��ʼ���ж�
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;  // ��PIE��ʹ��INT 1.1�ж�
    IER |= M_INT1; // ʹ��CPU�ж�1
    EINT; // ȫ��ʹ���ж�INTM
    ERTM; // ȫ��ʹ��ʵʱ�ж�DBGM

    LoopCount = 0; // ��ʼ��ѭ����������
    ConversionCount = 0; // ��ʼ��ת����������

    // ���ڲ��¶ȴ��������ӵ�ADCINA5ͨ��
    EALLOW;
    AdcRegs.ADCCTL1.bit.TEMPCONV = 1;
    EDIS;

    // ����ADC
    EALLOW;
    AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1; // ADCINT1��AdcResults���浽�����Ĵ���ǰ1�����ڴ���
    AdcRegs.INTSEL1N2.bit.INT1E = 1; // ʹ��ADCINT1
    AdcRegs.INTSEL1N2.bit.INT1CONT = 0; // ����ADCINT1����ģʽ
    AdcRegs.INTSEL1N2.bit.INT1SEL = 2; // ����EOC2����ADCINT1

    AdcRegs.ADCSOC0CTL.bit.CHSEL = 0; // ����SOC0ͨ��ѡ��ADCINA0
    AdcRegs.ADCSOC1CTL.bit.CHSEL = 0; // ����SOC1ͨ��ѡ��ADCINA0
    AdcRegs.ADCSOC2CTL.bit.CHSEL = 5; // ����SOC2ͨ��ѡ��ADCINA5
    AdcRegs.ADCSOC3CTL.bit.CHSEL = 5; // ����SOC3ͨ��ѡ��ADCINA6

    AdcRegs.ADCSOC0CTL.bit.TRIGSEL = 5;
    AdcRegs.ADCSOC1CTL.bit.TRIGSEL = 5; // ����SOC1��EPWM1A����
    AdcRegs.ADCSOC2CTL.bit.TRIGSEL = 5; // ����SOC2��EPWM1A����

    AdcRegs.ADCSOC0CTL.bit.ACQPS = 6;
    AdcRegs.ADCSOC1CTL.bit.ACQPS = 6; // ����SOC1�������ִ���Ϊ7��ADCʱ������
    AdcRegs.ADCSOC2CTL.bit.ACQPS = 6; // ����SOC2�������ִ���Ϊ7��ADCʱ������
    EDIS;

    // ����ePWM1ʱ���Ѿ���InitSysCtrl();������
    EPwm1Regs.ETSEL.bit.SOCAEN = 1; // ʹ��A��SOC
    EPwm1Regs.ETSEL.bit.SOCASEL = 4; // ѡ���CPMA�ϼ���ѡ��SOC
    EPwm1Regs.ETPS.bit.SOCAPRD = 1; // �ڵ�һ���¼�����������
    EPwm1Regs.CMPA.half.CMPA = 0x0080; // ���ñȽ�Aֵ
    EPwm1Regs.TBPRD = 0xFFFF; // ����ePWM1����
    EPwm1Regs.TBCTL.bit.CTRMODE = 0; // ���ü���ģʽΪ���ϼ�������ʼ
}

// �жϷ������̣����ڴ���ADC�ж�
interrupt void  adc_isr(void)
{
    // ʹ��static�ؼ���ȷ��i���жϷ������֮�䱣����ֵ
    static Uint16 i=0;
    i++; // ���Ӽ�����i��ֵ

    // �洢ADCת�������ȫ�ֱ��������У�����ADCRESULT0��Ϊ��������ķ���
    Voltage1[ConversionCount] = AdcResult.ADCRESULT1;
    Voltage2[ConversionCount] = AdcResult.ADCRESULT2;

    // ����Ѿ���¼��20��ת�������ü�����
    if(ConversionCount == 9)
    {
       ConversionCount = 0;
    }
    else
    {
        ConversionCount++; // ��������ת����������ֵ
    }

    // ���ADCINT1��־λ��Ϊ��һ��SOC��Start of Conversion����׼��
    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    // ��PIE��Programmable Interrupt Controller��ȷ���жϴ������
    PieCtrlRegs.PIEACK.bit.ACK1 = 1;
}

/**
 * ��ȡADCͨ��0��ֵ
 * 
 * �˺������ڶ�ȡADC��ģ��ת������ͨ��0��ת�����
 * ���ȴ�ADC���ת����Ȼ���ȡ���������������4λ����Ӧ12λ��ADC���
 * 
 * @return ת�����12λADCֵ
 */
Uint16 Read_ADC_CH0_Value(void)
{
    // �ȴ�ADC����1��ת�����
    //while (AdcRegs.ADCST.bit.INT_SEQ1== 0);
    
    // ���ADC����1���жϱ�־
    //AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
    
    // ����ADCͨ��0��ת�����������4λ����Ӧ12λ��ADC���
    //return AdcRegs.ADCRESULT0>>4;
    
    // ��ǰʵ���У�����ֱ�ӷ���0
    return 0;
}

//�ú���ʹ�ô洢��OTP�еĲο�������ת��ԭʼ�¶�
//����������Ϊ��
int16 GetTemperatureC(int16 sensorSample)
{
    return ((sensorSample - getTempOffset())*(int32)getTempSlope() + FP_ROUND + KELVIN_OFF)/FP_SCALE - KELVIN;
}

//����������ΪK
int16 GetTemperatureK(int16 sensorSample)
{
    return ((sensorSample - getTempOffset())*(int32)getTempSlope() + FP_ROUND + KELVIN_OFF)/FP_SCALE;
}





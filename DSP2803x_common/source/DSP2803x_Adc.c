#include "DSP2803x_Device.h"     // ���� DSP2803x �豸ͷ�ļ�
#include "DSP2803x_Examples.h"   // ���� DSP2803x ʾ��ͷ�ļ�

#define ADC_usDELAY  1000L       // �����ӳ�ʱ�䳣������λΪ΢��

//---------------------------------------------------------------------------
// InitAdc:
//---------------------------------------------------------------------------
// �˺����� ADC ��ʼ������֪״̬��
//
// ע�⣺2803x �豸�� ADC ��ʼ�������� 28x �豸��ͬ
//
void InitAdc(void)
{
    extern void DSP28x_usDelay(Uint32 Count); // �����ⲿ��ʱ����

    // *��Ҫ*
    // Device_cal �����Ὣ ADC У׼ֵ�� TI Ԥ���� OTP ���Ƶ� ADCREFSEL �� ADCOFFTRIM �Ĵ����С��˲��������� ROM ���Զ���ɡ�������Թ������ƹ������� ROM ���룬�������ô˺�����ȷ�� ADC ���չ淶������ADC ʱ�ӱ����ڵ��ô˺���֮ǰʹ�ܡ�

    EALLOW; // ��������ܱ����ļĴ���
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1; // ʹ�� ADC ʱ��
    (*Device_cal)(); // ����У׼����
    EDIS; // ��ֹ�����ܱ����ļĴ���

    // Ϊ������ ADC�������������� ADCENCLK λ������ʱ�ӣ�Ȼ������������϶���ο���·�� ADC �ںˡ��״�ת��ǰ��Ҫ�ȴ����� 5ms ���ӳ٣��Ա�����ģ���·���㹻��ʱ���ϵ粢�ȶ�������

    // ע�⣺������ʱ��������ȷ���������� DSP2803x_Examples.h �ļ��е� CPU_RATE �궨�壬�ú�Ӧ������ȷ�� CPU ʱ�����ڣ����룩��
    EALLOW;
    AdcRegs.ADCCTL1.bit.ADCBGPWD  = 1;      // ���� ADC ��϶��Դ
    AdcRegs.ADCCTL1.bit.ADCREFPWD = 1;      // ���òο���ѹ��Դ
    AdcRegs.ADCCTL1.bit.ADCPWDN   = 1;      // ���� ADC �ں˵�Դ
    AdcRegs.ADCCTL1.bit.ADCENABLE = 1;      // ���� ADC
    AdcRegs.ADCCTL1.bit.ADCREFSEL = 0;      // ѡ���ڲ���϶�ο�
    EDIS;

    DELAY_US(ADC_usDELAY);         // ��ת�� ADC ͨ��ǰ�����ӳ�
}

void InitAdcAio()
{
   EALLOW; // ��������ܱ����ļĴ���

/* ʹ�� AIO �Ĵ������� ADC ����*/
// ��ָ������Щ���ܵ� AIO ���Ž�������Ϊģ���������š�
// ע�⣺AIO1,3,5,7-9,11,13,15 ������ AIOMUX1 �����µ�ģ���������š���ע�͵�����Ҫ���С�

    GpioCtrlRegs.AIOMUX1.bit.AIO2 = 2;    // �� AIO2 ����Ϊ A2��ģ�����룩ģʽ
    GpioCtrlRegs.AIOMUX1.bit.AIO4 = 2;    // �� AIO4 ����Ϊ A4��ģ�����룩ģʽ
    GpioCtrlRegs.AIOMUX1.bit.AIO6 = 2;    // �� AIO6 ����Ϊ A6��ģ�����룩ģʽ
    GpioCtrlRegs.AIOMUX1.bit.AIO10 = 2;   // �� AIO10 ����Ϊ B2��ģ�����룩ģʽ
    GpioCtrlRegs.AIOMUX1.bit.AIO12 = 2;   // �� AIO12 ����Ϊ B4��ģ�����룩ģʽ
    GpioCtrlRegs.AIOMUX1.bit.AIO14 = 2;   // �� AIO14 ����Ϊ B6��ģ�����룩ģʽ

    EDIS; // ��ֹ�����ܱ����ļĴ���
}


/* AdcOffsetSelfCal-
   �˺���ͨ��ʹ�� VREFLO �ο���ѹ�� ADC ������ƫ���������У׼�����޸� ADCOFFTRIM �Ĵ�����VREFLO ͨ���ڲ���·����ѡ�����ӵ� A5�����������ⲿ ADC ���š��˺�����������������������
   - AdcChanSelect(channel) �� ѡ��Ҫת���� ADC ͨ��
   - AdcConversion() �� ������� ADC ת��������ƽ��ֵ
*/
void AdcOffsetSelfCal()
{
    Uint16 AdcConvMean;
    EALLOW;
    AdcRegs.ADCCTL1.bit.ADCREFSEL = 0;                  // ѡ���ڲ��ο�ģʽ
    AdcRegs.ADCCTL1.bit.VREFLOCONV = 1;                 // ѡ�� VREFLO �ڲ������� B5
    AdcChanSelect(13);                                  // ѡ������ SOC �� B5 ͨ��
    AdcRegs.ADCOFFTRIM.bit.OFFTRIM = 80;                // Ӧ���˹�ƫ�� (+80)���Ե��� ADC �ں��п��ܳ��ֵĸ�ƫ��
    AdcConvMean = AdcConversion();                      // �� VREFLO ���� ADC ת������ȡƽ��ֵ
    AdcRegs.ADCOFFTRIM.bit.OFFTRIM = 80 - AdcConvMean;  // ���� offtrim �Ĵ�������ֵ�����Ƴ��˹�ƫ�� (+80) ������ƫ�����Ķ����Ʋ��룩
    AdcRegs.ADCCTL1.bit.VREFLOCONV = 0;                 // ѡ���ⲿ ADCIN5 ����������Ϊ B5
    EDIS;
}

/*  AdcChanSelect-
    �˺���ͨ���������� SOC ��ͨ��ѡ����ѡ��Ҫת���� ADC ͨ����

     * ע�� * �˺����Ḳ��֮ǰ�� SOC ͨ��ѡ�����á����鱣��֮ǰ�����á�
 */
void AdcChanSelect(Uint16 ch_no)
{
    AdcRegs.ADCSOC0CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC1CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC2CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC3CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC4CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC5CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC6CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC7CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC8CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC9CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC10CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC11CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC12CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC13CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC14CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC15CTL.bit.CHSEL= ch_no;
} // end AdcChanSelect

/* AdcConversion -
   �˺���������� ADC ת��������ƽ��ֵ����ʹ�� ADCINT1 �� ADCINT2 �� SOC0-7 �� SOC8-15 ֮�䡰ƹ�ҡ�������

     * ע�� * �˺����Ḳ��֮ǰ�� ADC ���á����鱣��֮ǰ�����á�
*/
Uint16 AdcConversion(void)
{
    Uint16 index, SampleSize, Mean, ACQPS_Value;
    Uint32 Sum;

    index       = 0;            // ��ʼ������Ϊ 0
    SampleSize  = 256;          // ����������СΪ 256��ע�⣺������С������ 2^x �ı��������� x �Ǵ��ڵ��� 4 ��������
    Sum         = 0;            // ��ʼ����ͱ���Ϊ 0
    Mean        = 999;          // ��ʼ����ֵΪһ����ֵ֪

    // ���� ADC ��������Ϊ����ֵ���������� = ACQPS + 1��
    ACQPS_Value = 6;
    AdcRegs.ADCSOC0CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC1CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC2CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC3CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC4CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC5CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC6CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC7CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC8CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC9CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC10CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC11CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC12CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC13CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC14CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC15CTL.bit.ACQPS = ACQPS_Value;


    // ����ƹ�Ҳ���

    // ���� ADCINT1 �� ADCINT2
    AdcRegs.INTSEL1N2.bit.INT1E = 1;
    AdcRegs.INTSEL1N2.bit.INT2E = 1;

    // ���� ADCINT1 �� ADCINT2 ����������
    AdcRegs.INTSEL1N2.bit.INT1CONT = 0;
    AdcRegs.INTSEL1N2.bit.INT2CONT = 0;

    AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1;    // ADCINTs ��ת������ʱ����

    // ���� ADCINT1 �� ADCINT2 ����Դ
    AdcRegs.INTSEL1N2.bit.INT1SEL = 6;      // EOC6 ���� ADCINT1
    AdcRegs.INTSEL1N2.bit.INT2SEL = 14;     // EOC14 ���� ADCINT2

    // ����ÿ�� SOC �� ADCINT ����Դ
    AdcRegs.ADCINTSOCSEL1.bit.SOC0  = 2;    // ADCINT2 ���� SOC0-7
    AdcRegs.ADCINTSOCSEL1.bit.SOC1  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC2  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC3  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC4  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC5  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC6  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC7  = 2;
    AdcRegs.ADCINTSOCSEL2.bit.SOC8  = 1;    // ADCINT1 ���� SOC8-15
    AdcRegs.ADCINTSOCSEL2.bit.SOC9  = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC10 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC11 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC12 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC13 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC14 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC15 = 1;

    DELAY_US(ADC_usDELAY);                  // ��ת�� ADC ͨ��ǰ�����ӳ�


    // ADC ת��

    AdcRegs.ADCSOCFRC1.all = 0x00FF;  // ǿ������ SOC0-7 �Կ�ʼƹ�Ҳ���

    while( index < SampleSize ){

        // �ȴ� ADCINT1 ������Ȼ�� ADCRESULT0-7 �Ĵ�����ֵ�ӵ��ܺ���
        while (AdcRegs.ADCINTFLG.bit.ADCINT1 == 0){}
        AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;   // ������� ADCINT1 ��־����Ϊ INT1CONT = 0
        Sum += AdcResult.ADCRESULT0;
        Sum += AdcResult.ADCRESULT1;
        Sum += AdcResult.ADCRESULT2;
        Sum += AdcResult.ADCRESULT3;
        Sum += AdcResult.ADCRESULT4;
        Sum += AdcResult.ADCRESULT5;
        Sum += AdcResult.ADCRESULT6;
        Sum += AdcResult.ADCRESULT7;

        // �ȴ� ADCINT2 ������Ȼ�� ADCRESULT8-15 �Ĵ�����ֵ�ӵ��ܺ���
        while (AdcRegs.ADCINTFLG.bit.ADCINT2 == 0){}
        AdcRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;   // ������� ADCINT2 ��־����Ϊ INT2CONT = 0
        Sum += AdcResult.ADCRESULT8;
        Sum += AdcResult.ADCRESULT9;
        Sum += AdcResult.ADCRESULT10;
        Sum += AdcResult.ADCRESULT11;
        Sum += AdcResult.ADCRESULT12;
        Sum += AdcResult.ADCRESULT13;
        Sum += AdcResult.ADCRESULT14;
        Sum += AdcResult.ADCRESULT15;

        index+=16;

    } // ���ݲɼ�����

    // ���� ADCINT1 �� ADCINT2 ��ֹͣƹ�Ҳ���
    AdcRegs.INTSEL1N2.bit.INT1E = 0;
    AdcRegs.INTSEL1N2.bit.INT2E = 0;

    Mean = Sum / SampleSize;    // ����ƽ�� ADC ����ֵ

    return Mean;                // ����ƽ��ֵ

}// end AdcConversion

//===========================================================================
// �ļ�����






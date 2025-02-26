// TI �ļ��汾 $Revision: /main/7 $
// �ύ���� $Date: November 10, 2009   14:05:15 $
//###########################################################################
//
// �ļ���: DSP2803x_SysCtrl.c
//
// ����: DSP2803x �豸ϵͳ���Ƴ�ʼ����֧�ֺ�����
//
// ����:
//
//         ϵͳ��Դ��ʾ����ʼ����
//
//###########################################################################
// $TI ����: 2803x C/C++ ͷ�ļ� V1.21 $
// $��������: December 1, 2009 $
//###########################################################################

#include "DSP2803x_Device.h"     // �����豸ͷ�ļ�
#include "DSP2803x_Examples.h"   // ����ʾ���ӳ���ͷ�ļ�

// ��Ҫ�� RAM �����еĺ�����Ҫ���䵽��ͬ�ĶΡ��öν�ͨ�������������ļ�ӳ�䵽���غ����е�ַ��

#pragma CODE_SECTION(InitFlash, "ramfuncs");

//---------------------------------------------------------------------------
// InitSysCtrl:
//---------------------------------------------------------------------------
// �˺�����ϵͳ���ƼĴ�����ʼ��Ϊ��֪״̬��
// - ���ÿ��Ź�
// - ���� PLLCR �Ի����ȷ�� SYSCLKOUT Ƶ��
// - ���øߡ���Ƶ����ʱ�ӵ�Ԥ��Ƶ��
// - ��������ʱ��

void InitSysCtrl(void)
{
    // ���ÿ��Ź�
    DisableDog();

    // *��Ҫ*
    // Device_cal �����Ὣ ADC ������У׼ֵ�� TI ������ OTP ���Ƶ���Ӧ�ĵ����Ĵ�����
    // ������������� ROM ���Զ���ɡ�����ڵ��Թ������ƹ������� ROM ���룬�������ô˺�����
    // �Ա� ADC ���������չ淶�������ڵ��ô˺���֮ǰ���������� ADC ��ʱ�ӡ�
    // ������Ϣ������豸�����ֲ��/�� ADC �ο��ֲᡣ

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1; // ���� ADC ����ʱ��
    (*Device_cal)();                      // ����У׼����
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 0; // �ָ� ADC ʱ�ӵ�ԭʼ״̬
    EDIS;

    // ѡ���ڲ����� 1 ��Ϊʱ��Դ��Ĭ�ϣ������ر�����δʹ�õ�ʱ���Խ�ʡ���ġ�
    IntOsc1Sel();

    // ��ʼ�� PLL ���ƣ�PLLCR �� CLKINDIV
    // DSP28_PLLCR �� DSP28_DIVSEL �� DSP2803x_Examples.h �ж���
    InitPll(DSP28_PLLCR,DSP28_DIVSEL);

    // ��ʼ������ʱ��
    InitPeripheralClocks();
}

//---------------------------------------------------------------------------
// ʾ��: InitFlash:
//---------------------------------------------------------------------------
// �˺�����ʼ�� Flash ���ƼĴ���

//                   ע��
// �˺��������� RAM ��ִ�С��� OTP/Flash ��ִ�����ᵼ�²���Ԥ��Ľ��

void InitFlash(void)
{
    EALLOW;
    // ���� Flash �ܵ�ģʽ����ߴ� Flash ִ�д�������ܡ�
    FlashRegs.FOPT.bit.ENPIPE = 1;

    //                ע��
    // ������ TI �Ը��� CPU �����µ� Flash ��С�ȴ�״̬���б�����
    // ����������ֲ��ȡ������Ϣ��

    // ���� Flash �ķ�ҳ�ȴ�״̬
    FlashRegs.FBANKWAIT.bit.PAGEWAIT = 2;

    // ���� Flash ������ȴ�״̬
    FlashRegs.FBANKWAIT.bit.RANDWAIT = 2;

    // ���� OTP �ĵȴ�״̬
    FlashRegs.FOTPWAIT.bit.OTPWAIT = 2;

    //                ע��
    // ��Ӧʹ����Щ�Ĵ�����Ĭ��ֵ
    FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;
    FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;
    EDIS;

    // ǿ��ˢ����ˮ�ߣ���ȷ�������һ�����üĴ�����д���ڷ���֮ǰ������

    asm(" RPT #7 || NOP");
}

//---------------------------------------------------------------------------
// ʾ��: ServiceDog:
//---------------------------------------------------------------------------
// �˺������ÿ��Ź���ʱ����
// ���Ӧ�ó�����ʹ�� ServiceDog�������ô˹���

void ServiceDog(void)
{
    EALLOW;
    SysCtrlRegs.WDKEY = 0x0055;
    SysCtrlRegs.WDKEY = 0x00AA;
    EDIS;
}

//---------------------------------------------------------------------------
// ʾ��: DisableDog:
//---------------------------------------------------------------------------
// �˺������ÿ��Ź���ʱ����

void DisableDog(void)
{
    EALLOW;
    SysCtrlRegs.WDCR = 0x0068;
    EDIS;
}

//---------------------------------------------------------------------------
// ʾ��: InitPll:
//---------------------------------------------------------------------------
// �˺�����ʼ�� PLLCR �Ĵ�����

void InitPll(Uint16 val, Uint16 divsel)
{
    volatile Uint16 iVol;

    // ȷ�� PLL ����������ģʽ
    if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 0)
    {
        EALLOW;
        // ��⵽ OSCCLKSRC1 ���ϣ�PLL ��������ģʽ��
        // �������ö�ʧʱ���߼���
        SysCtrlRegs.PLLSTS.bit.MCLKCLR = 1;
        EDIS;
        // �滻Ϊ�ʵ��� SystemShutdown() �������á�
        asm("        ESTOP0");     // ���ڵ���Ŀ��
    }

    // �ڸ��� PLLCR ֮ǰ��DIVSEL ����Ϊ 0
    // �ⲿ��λ XRSn ��������Ϊ 0�����ʹ���ǽ��� 1/4 ģʽ
    if (SysCtrlRegs.PLLSTS.bit.DIVSEL != 0)
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;
        EDIS;
    }

    // ���� PLLCR
    if (SysCtrlRegs.PLLCR.bit.DIV != val)
    {
        EALLOW;
        // ������ PLLCR ֮ǰ�رն�ʧʱ�Ӽ���߼�
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
        SysCtrlRegs.PLLCR.bit.DIV = val;
        EDIS;

        // ��ѡ���ȴ� PLL ������
        // �ڴ��ڼ䣬CPU ���л��� OSCCLK/2 ֱ�� PLL �ȶ���
        // һ�� PLL �ȶ���CPU ���л����µ� PLL ֵ��
        //
        // �������ʱ���� PLL ������������ء�
        //
        // ���벻��Ҫ�ڴ˵ȴ� PLL ������
        // Ȼ�����������ִ�е���ʱ��ؼ�������������Ҫ��ȷ��ʱ������������õȴ��л���ɡ�

        // �ȴ� PLL ����λ�����á�

        // �ڴ�ѭ��ǰ���ÿ��Ź�������ѭ����ͨ�� ServiceDog() ι����

        // ȡ��ע���Խ��ÿ��Ź�
        DisableDog();

        while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1)
        {
            // ȡ��ע����ι��
            // ServiceDog();
        }

        EALLOW;
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
        EDIS;
    }

    // ����л��� 1/2
    if((divsel == 1)||(divsel == 2))
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = divsel;
        EDIS;
    }

    // ����л��� 1/1
    // * ���л��� 1/2 ���õ�Դ�ȶ�
    //   �ȶ������ʱ��ȡ����ϵͳ������ֻ��һ��ʾ��
    // * Ȼ���л��� 1/1
    if(divsel == 3)
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 2;
        DELAY_US(50L);
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 3;
        EDIS;
    }
}

//---------------------------------------------------------------------------
// ʾ��: InitPeripheralClocks:
//---------------------------------------------------------------------------
// �˺�����ʼ������ģ���ʱ�ӡ�
// �������øߡ���ʱ��Ԥ��Ƶ��
// Ȼ������ÿ�������ʱ�ӡ�
// Ϊ�˼��ٹ��ģ���ʹ�õ�����ʱ��Ӧ���ֹر�
//
// ע�⣺���δ��������ʱ�ӣ����޷���ȡ��д�������ļĴ���

void InitPeripheralClocks(void)
{
    EALLOW;

    // LOSPCP Ԥ��Ƶ�Ĵ������ã�ͨ������ΪĬ��ֵ

    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;  // GPIO18 = XCLKOUT
    SysCtrlRegs.LOSPCP.all = 0x0002;

    // XCLKOUT �� SYSCLKOUT �ı��ʡ�Ĭ������� XCLKOUT = 1/4 SYSCLKOUT
    SysCtrlRegs.XCLK.bit.XCLKOUTDIV = 2;

    // ����ѡ�������ʱ�ӡ�
    // �����ʹ��ĳ�����裬��ر���ʱ���Խ�ʡ���ġ�
    //
    // ע�⣺�������� 2803x ������Ʒ�������������衣
    // ���������ʹ���豸�������ֲᡣ
    //
    // ���������Ǹ�Ч�Ĵ���ʾ����

    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;      // ADC
    SysCtrlRegs.PCLKCR3.bit.COMP1ENCLK = 1;    // COMP1
    SysCtrlRegs.PCLKCR3.bit.COMP2ENCLK = 1;    // COMP2
    SysCtrlRegs.PCLKCR3.bit.COMP3ENCLK = 1;    // COMP3
    SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 1;    // eCAP1
    SysCtrlRegs.PCLKCR0.bit.ECANAENCLK = 1;    // eCAN-A
    SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 1;    // eQEP1
    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;    // ePWM1
    SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;    // ePWM2
    SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1;    // ePWM3
    SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 1;    // ePWM4
    SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 1;    // ePWM5
    SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 1;    // ePWM6
    SysCtrlRegs.PCLKCR1.bit.EPWM7ENCLK = 1;    // ePWM7
    SysCtrlRegs.PCLKCR0.bit.HRPWMENCLK = 1;    // HRPWM
    SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 1;     // I2C
    SysCtrlRegs.PCLKCR0.bit.LINAENCLK = 1;     // LIN-A
    SysCtrlRegs.PCLKCR3.bit.CLA1ENCLK = 1;     // CLA1
    SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;     // SCI-A
    SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1;     // SPI-A
    SysCtrlRegs.PCLKCR0.bit.SPIBENCLK = 1;     // SPI-B

    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;     // ���� ePWM �ڲ� TBCLK

    EDIS;
}

//---------------------------------------------------------------------------
// ʾ��: CsmUnlock:
//---------------------------------------------------------------------------
// �˺������� CSM���û������õ�ǰ�����滻 0xFFFF���ɹ��������� 1��

#define STATUS_FAIL          0
#define STATUS_SUCCESS       1

Uint16 CsmUnlock()
{
    volatile Uint16 temp;

    // ʹ�õ�ǰ���������Կ�Ĵ�����0xFFFF ���������롣�û�Ӧ�滻Ϊ��ȷ�����롣

    EALLOW;
    CsmRegs.KEY0 = 0xFFFF;
    CsmRegs.KEY1 = 0xFFFF;
    CsmRegs.KEY2 = 0xFFFF;
    CsmRegs.KEY3 = 0xFFFF;
    CsmRegs.KEY4 = 0xFFFF;
    CsmRegs.KEY5 = 0xFFFF;
    CsmRegs.KEY6 = 0xFFFF;
    CsmRegs.KEY7 = 0xFFFF;
    EDIS;

    // ִ������λ�õ������ȡ
    // �������ƥ����Կֵ��CSM ������

    temp = CsmPwl.PSWD0;
    temp = CsmPwl.PSWD1;
    temp = CsmPwl.PSWD2;
    temp = CsmPwl.PSWD3;
    temp = CsmPwl.PSWD4;
    temp = CsmPwl.PSWD5;
    temp = CsmPwl.PSWD6;
    temp = CsmPwl.PSWD7;

    // ��� CSM ���������سɹ������򷵻�ʧ�ܡ�
    if (CsmRegs.CSMSCR.bit.SECURE == 0) return STATUS_SUCCESS;
    else return STATUS_FAIL;
}

//---------------------------------------------------------------------------
// ʾ��: IntOsc1Sel:
//---------------------------------------------------------------------------
// �˺����л����ڲ����� 1 ���ر���������ʱ��Դ����С������

void IntOsc1Sel (void) {
    EALLOW;
    SysCtrlRegs.CLKCTL.bit.INTOSC1OFF = 0;
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL = 0;  // ʱ��Դ = INTOSC1
    SysCtrlRegs.CLKCTL.bit.XCLKINOFF = 1;     // �ر� XCLKIN
    SysCtrlRegs.CLKCTL.bit.XTALOSCOFF = 1;    // �ر� XTALOSC
    SysCtrlRegs.CLKCTL.bit.INTOSC2OFF = 1;    // �ر� INTOSC2
    EDIS;
}

//---------------------------------------------------------------------------
// ʾ��: IntOsc2Sel:
//---------------------------------------------------------------------------
// �˺������ⲿ�����л����ڲ����� 2�����ر���������ʱ��Դ����С������
// ע�⣺���û���ⲿʱ�����ӣ��ڴ� INTOSC1 �л��� INTOSC2 ֮ǰ��
//       ����ر� EXTOSC �� XLCKIN

void IntOsc2Sel (void) {
    EALLOW;
    SysCtrlRegs.CLKCTL.bit.INTOSC2OFF = 0;     // ���� INTOSC2
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRC2SEL = 1;  // �л��� INTOSC2
    SysCtrlRegs.CLKCTL.bit.XCLKINOFF = 1;      // �ر� XCLKIN
    SysCtrlRegs.CLKCTL.bit.XTALOSCOFF = 1;     // �ر� XTALOSC
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL = 1;   // �л����ڲ����� 2
    SysCtrlRegs.CLKCTL.bit.WDCLKSRCSEL = 1;    // �л����Ź�ʱ��Դ�� INTOSC2
    SysCtrlRegs.CLKCTL.bit.INTOSC1OFF = 1;     // �ر� INTOSC1
    EDIS;
}
//---------------------------------------------------------------------------
// ʾ��: XtalOscSel:
//---------------------------------------------------------------------------
// �˺����л����ⲿ�����������ر���������ʱ��Դ����С�����ġ�
// ��ѡ����ܲ������������豸��װ

void XtalOscSel (void)  {
    EALLOW;
    SysCtrlRegs.CLKCTL.bit.XTALOSCOFF = 0;     // ���� XTALOSC
    SysCtrlRegs.CLKCTL.bit.XCLKINOFF = 1;      // �ر� XCLKIN
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRC2SEL = 0;  // �л����ⲿʱ��
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL = 1;   // �л����ⲿʱ��
    SysCtrlRegs.CLKCTL.bit.WDCLKSRCSEL = 1;    // �л����Ź�ʱ��Դ���ⲿʱ��
    SysCtrlRegs.CLKCTL.bit.INTOSC2OFF = 1;     // �ر� INTOSC2
    SysCtrlRegs.CLKCTL.bit.INTOSC1OFF = 1;     // �ر� INTOSC1
    EDIS;
}

//---------------------------------------------------------------------------
// ʾ��: ExtOscSel:
//---------------------------------------------------------------------------
// �˺����л����ⲿ�������ر���������ʱ��Դ����С�����ġ�

void ExtOscSel (void)  {
    EALLOW;
    SysCtrlRegs.XCLK.bit.XCLKINSEL = 1;       // 1-GPIO19 = XCLKIN, 0-GPIO38 = XCLKIN
    SysCtrlRegs.CLKCTL.bit.XTALOSCOFF = 1;    // Turn on XTALOSC
    SysCtrlRegs.CLKCTL.bit.XCLKINOFF = 0;     // Turn on XCLKIN
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRC2SEL = 0; // Switch to external clock
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL = 1;  // Switch from INTOSC1 to INTOSC2/ext clk
    SysCtrlRegs.CLKCTL.bit.WDCLKSRCSEL = 1;   // Switch Watchdog Clk Src to external clock
    SysCtrlRegs.CLKCTL.bit.INTOSC2OFF = 1;    // Turn off INTOSC2
    SysCtrlRegs.CLKCTL.bit.INTOSC1OFF = 1;    // Turn off INTOSC1
    EDIS;
}
//===========================================================================
// End of file.
//===========================================================================

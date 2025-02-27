#include "EPWM.h"

void EPWM1_Init(Uint16 tbprd)
{
    //ʹ��ePWM����
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;          // ʧ��ePWM����ʱ��
    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;      // ʹ��ePWM1
    EDIS;

    // Disable Sync
    EPwm1Regs.TBCTL.bit.SYNCOSEL = 11;  // Pass through
    // Initially disable Free/Soft Bits
    EPwm1Regs.TBCTL.bit.FREE_SOFT = 0;

    //����ePWM1��ӦGPIOʱ�Ӽ���ʼ������
    InitEPwm1Gpio();
    //����ePWM1��ӦTZ����
    InitTzGpio();

//    ISR functions found within this file.
//    EALLOW;
//    PieVectTable.EPWM1_TZINT = &epwm1_tzint_isr;
//    EDIS;

//    EALLOW;
//    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;      // Stop all the TB clocks
//    EDIS;

   //Enable TZ1 and TZ2 as one shot trip sources
   EALLOW;
   EPwm1Regs.TZSEL.bit.CBC1 = 1;
   EPwm1Regs.TZSEL.bit.CBC2 = 1;

   // What do we want the TZ1 and TZ2 to do?
   EPwm1Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
   EPwm1Regs.TZCTL.bit.TZB = TZ_FORCE_LO;

   // Enable TZ interrupt ����ҪTZ�жϷ�����ʱʹ��
   //EPwm1Regs.TZEINT.bit.OST = 1;


   EDIS;

    //��ʼ��ʱ��ģ��
    //ʱ��ͬ����������
//    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;   // �첽
    // Allow each timer to be sync'ed
//    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;                   //��ʹ����λ
//    EPwm1Regs.TBPHS.half.TBPHS = 0;                           //��λֵ
    EPwm1Regs.TBCTR = 0x0000;                                   //���������
    EPwm1Regs.TBPRD = tbprd;                                    //��������
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;                  //���ϼ���
    EPwm1Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;                      //���÷�Ƶ
    EPwm1Regs.TBCTL.bit.CLKDIV=TB_DIV1;

    //��ʼ���Ƚ�ģ��
    // Setup shadow register load on ZERO [CC]
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;//SA Ӱ�ӼĴ���
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;//SB Ӱ�ӼĴ���
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;//CTL=0ʱ����
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;//CTL=0ʱ����

    EPwm1Regs.ETSEL.bit.SOCAEN = 1; // ʹ��A��SOC
    EPwm1Regs.ETSEL.bit.SOCASEL = 4; // ѡ���CPMA�ϼ���ѡ��SOC
    EPwm1Regs.ETPS.bit.SOCAPRD = 1; // �ڵ�һ���¼�����������

    // ���ñȽ�ֵ
    EPwm1Regs.CMPA.half.CMPA =0;    // Set compare A value
//    EPwm1Regs.CMPB = 0;                     // Set Compare B value

    //��ʼ�����޶�ģ��[AQ]
    EPwm1Regs.AQCTLA.bit.PRD = AQ_SET;            // Set PWM1A on Zero
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;       // Clear PWM1A on event A, up count

//    EPwm1Regs.AQCTLB.bit.ZRO = AQ_CLEAR;       // Set PWM1B on Zero
//    EPwm1Regs.AQCTLB.bit.CAU = AQ_SET;            // Clear PWM1B on event B, up count
    //��ʼ���¼�����ģ��[ET]
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;  // Select INT on Zero event
    EPwm1Regs.ETSEL.bit.INTEN = 1;                      // Enable INT
    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;            // ����1���¼�������1���ж�

    EALLOW;
    PieVectTable.EPWM1_INT = &epwm1_timer_isr;
    EDIS;
    //�������á�ն������
    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    // �������ӳ�������˿ڲ���ת���������ӳ�������˿ڷ�ת��
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    //����8*1/60MHz
    EPwm1Regs.DBFED = 8;
    //����8*1/60MHz
    EPwm1Regs.DBRED = 8;

    //ʹ��ʱ������ʱ��
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;         // Start all the timers synced
    EDIS;

    // Enable CPU INT3 which is connected to EPWM1-3 INT:
    IER |= M_INT3;
    // Enable EPWM INTn in the PIE: Group 3 interrupt 1-6
    PieCtrlRegs.PIEIER3.bit.INTx1 = PWM1_INT_ENABLE;

    // Enable EPWM INTn in the PIE: Group 3 interrupt 1-3
//    PieCtrlRegs.PIEIER2.bit.INTx1 = 1;

    EINT;
    ERTM;
}

void EPWM2_Init(Uint16 tbprd)
{
    // ʹ�� ePWM ����
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;          // ʧ�� ePWM ����ʱ��
    SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;         // ʹ�� ePWM2
    EDIS;

    // ����ͬ���ź�
    EPwm2Regs.TBCTL.bit.SYNCOSEL = 11;  // ѡ��ͬ���ź�Ϊֱͨģʽ
    // ��ʼ�����Զ�����/������λ
    EPwm2Regs.TBCTL.bit.FREE_SOFT = 0;

    // ���� ePWM2 ��Ӧ GPIO ʱ�Ӽ���ʼ������
    InitEPwm2Gpio();
    // ���� ePWM2 ��Ӧ TZ ����
    InitTzGpio();

    // ����TZ1��TZ2��Ϊ���δ���Դ
    EALLOW;
    EPwm2Regs.TZSEL.bit.CBC1 = 1;
    EPwm2Regs.TZSEL.bit.CBC2 = 1;

    // ����TZ1��TZ2�Ķ���
    EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_LO;

    // Enable TZ interrupt ����Ҫ TZ �жϷ�����ʱʹ��
    // EPwm2Regs.TZEINT.bit.OST = 1;

    EDIS;

    // ��ʼ��ʱ��ģ��
    EPwm2Regs.TBCTR = 0x0000;                                   // ���������
    EPwm2Regs.TBPRD = tbprd;                                    // ��������
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;                  // ���ϼ���
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;                    // ���÷�Ƶ
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    // ��ʼ���Ƚ�ģ��
    // ����Ӱ�ӼĴ�������ģʽΪ��������Ϊ0ʱ�����
    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW; // SA Ӱ�ӼĴ���
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW; // SB Ӱ�ӼĴ���
    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // CTL=0 ʱ����
    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // CTL=0 ʱ����

    // ���ñȽ�ֵ
    EPwm2Regs.CMPA.half.CMPA = 0;    // ���ñȽ�ֵAΪ0
    // EPwm2Regs.CMPB = 0;                     // Set Compare B value

    // ��ʼ�����޶�ģ�� [AQ]
    EPwm2Regs.AQCTLA.bit.PRD = AQ_SET;            // �ڼ������ﵽ����ʱ���ø�
    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // �ڼ��������ϼ�����CMPAʱ���õ�

    // EPwm2Regs.AQCTLB.bit.ZRO = AQ_CLEAR;       // �ڼ�����Ϊ0ʱ����0
    // EPwm2Regs.AQCTLB.bit.CAU = AQ_SET;         // �ڼ���������CMPBʱ���ø�

    // ��ʼ���¼�����ģ�� [ET]
    EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // ѡ���ж��ڼ�����Ϊ0ʱ�򴥷�
    EPwm2Regs.ETSEL.bit.INTEN = 1;                //
    EPwm2Regs.ETPS.bit.INTPRD = ET_1ST;           // ���� 1 ���¼������� 1 ���ж�

    EALLOW;
    PieVectTable.EPWM2_INT = &epwm2_timer_isr;
    EDIS;

    // �������á�ն������
    EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    // �������ӳ�������˿ڲ���ת���������ӳ�������˿ڷ�ת��
    EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    // ���� 8 * 1/60MHz
    EPwm2Regs.DBFED = 8;
    // ���� 8 * 1/60MHz
    EPwm2Regs.DBRED = 8;

    // ʹ��ʱ������ʱ��
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;         // Start all the timers synced
    EDIS;

    // Enable CPU INT3 which is connected to EPWM1-3 INT:
    IER |= M_INT3;
    // Enable EPWM INTn in the PIE: Group 3 interrupt 1-6
    PieCtrlRegs.PIEIER3.bit.INTx2 = PWM2_INT_ENABLE;

    EINT;
    ERTM;
}


void EPwm1A_SetCompare(Uint16 val)    //����EPWM1Aռ�ձ�
{
    EPwm1Regs.CMPA.half.CMPA = val;  //����ռ�ձ�
}


void EPwm1B_SetCompare(Uint16 val)  //����EPWM1Bռ�ձ�
{
    EPwm1Regs.CMPB = val;  //����ռ�ձ�
}

void EPwm2A_SetCompare(Uint16 val)    //����EPWM2Aռ�ձ�
{
    EPwm2Regs.CMPA.half.CMPA = val;  //����ռ�ձ�
}


void EPwm2B_SetCompare(Uint16 val)  //����EPWM2Bռ�ձ�
{
    EPwm2Regs.CMPB = val;  //����ռ�ձ�
}

interrupt void epwm1_timer_isr(void)
{
    static Uint16 cnt=0;
    cnt++;
    if(cnt==20000)
    {
        cnt=0;
    }

    // ����˶�ʱ�����жϱ�־
    EPwm1Regs.ETCLR.bit.INT = 1;
    // ȷ�ϴ��жϣ��Ա����3���ո����ж�
    PieCtrlRegs.PIEACK.bit.ACK3 = 1;
}

interrupt void epwm2_timer_isr(void)
{
    static Uint16 cnt2=0;
    cnt2++;
    if(cnt2==20000)
    {
        cnt2=0;
    }

    // ����˶�ʱ�����жϱ�־
    EPwm2Regs.ETCLR.bit.INT = 1;
    // ȷ�ϴ��жϣ�
    PieCtrlRegs.PIEACK.bit.ACK3 = 1;
}

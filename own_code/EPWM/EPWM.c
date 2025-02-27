#include "EPWM.h"

void EPWM1_Init(Uint16 tbprd)
{
    //使能ePWM外设
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;          // 失能ePWM外设时钟
    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;      // 使能ePWM1
    EDIS;

    // Disable Sync
    EPwm1Regs.TBCTL.bit.SYNCOSEL = 11;  // Pass through
    // Initially disable Free/Soft Bits
    EPwm1Regs.TBCTL.bit.FREE_SOFT = 0;

    //开启ePWM1对应GPIO时钟及初始化配置
    InitEPwm1Gpio();
    //开启ePWM1对应TZ配置
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

   // Enable TZ interrupt 当需要TZ中断服务函数时使用
   //EPwm1Regs.TZEINT.bit.OST = 1;


   EDIS;

    //初始化时基模块
    //时钟同步输入设置
//    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;   // 异步
    // Allow each timer to be sync'ed
//    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;                   //不使用相位
//    EPwm1Regs.TBPHS.half.TBPHS = 0;                           //相位值
    EPwm1Regs.TBCTR = 0x0000;                                   //清除计数器
    EPwm1Regs.TBPRD = tbprd;                                    //设置周期
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;                  //向上计数
    EPwm1Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;                      //设置分频
    EPwm1Regs.TBCTL.bit.CLKDIV=TB_DIV1;

    //初始化比较模块
    // Setup shadow register load on ZERO [CC]
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;//SA 影子寄存器
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;//SB 影子寄存器
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;//CTL=0时加载
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;//CTL=0时加载

    EPwm1Regs.ETSEL.bit.SOCAEN = 1; // 使能A组SOC
    EPwm1Regs.ETSEL.bit.SOCASEL = 4; // 选择从CPMA上计数选择SOC
    EPwm1Regs.ETPS.bit.SOCAPRD = 1; // 在第一个事件上生成脉冲

    // 设置比较值
    EPwm1Regs.CMPA.half.CMPA =0;    // Set compare A value
//    EPwm1Regs.CMPB = 0;                     // Set Compare B value

    //初始动作限定模块[AQ]
    EPwm1Regs.AQCTLA.bit.PRD = AQ_SET;            // Set PWM1A on Zero
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;       // Clear PWM1A on event A, up count

//    EPwm1Regs.AQCTLB.bit.ZRO = AQ_CLEAR;       // Set PWM1B on Zero
//    EPwm1Regs.AQCTLB.bit.CAU = AQ_SET;            // Clear PWM1B on event B, up count
    //初始化事件触发模块[ET]
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;  // Select INT on Zero event
    EPwm1Regs.ETSEL.bit.INTEN = 1;                      // Enable INT
    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;            // 发生1次事件，产生1个中断

    EALLOW;
    PieVectTable.EPWM1_INT = &epwm1_timer_isr;
    EDIS;
    //死区设置、斩波设置
    EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    // 上升沿延迟器输出端口不翻转，下升沿延迟器输出端口翻转；
    EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    //死区8*1/60MHz
    EPwm1Regs.DBFED = 8;
    //死区8*1/60MHz
    EPwm1Regs.DBRED = 8;

    //使能时基计数时钟
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
    // 使能 ePWM 外设
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;          // 失能 ePWM 外设时钟
    SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1;         // 使能 ePWM2
    EDIS;

    // 禁用同比信号
    EPwm2Regs.TBCTL.bit.SYNCOSEL = 11;  // 选择同步信号为直通模式
    // 初始禁用自动运行/软启动位
    EPwm2Regs.TBCTL.bit.FREE_SOFT = 0;

    // 开启 ePWM2 对应 GPIO 时钟及初始化配置
    InitEPwm2Gpio();
    // 开启 ePWM2 对应 TZ 配置
    InitTzGpio();

    // 启用TZ1和TZ2作为单次触发源
    EALLOW;
    EPwm2Regs.TZSEL.bit.CBC1 = 1;
    EPwm2Regs.TZSEL.bit.CBC2 = 1;

    // 设置TZ1和TZ2的动作
    EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_LO;

    // Enable TZ interrupt 当需要 TZ 中断服务函数时使用
    // EPwm2Regs.TZEINT.bit.OST = 1;

    EDIS;

    // 初始化时基模块
    EPwm2Regs.TBCTR = 0x0000;                                   // 清除计数器
    EPwm2Regs.TBPRD = tbprd;                                    // 设置周期
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;                  // 向上计数
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;                    // 设置分频
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    // 初始化比较模块
    // 设置影子寄存器加载模式为当计数器为0时候加载
    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW; // SA 影子寄存器
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW; // SB 影子寄存器
    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // CTL=0 时加载
    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO; // CTL=0 时加载

    // 设置比较值
    EPwm2Regs.CMPA.half.CMPA = 0;    // 设置比较值A为0
    // EPwm2Regs.CMPB = 0;                     // Set Compare B value

    // 初始动作限定模块 [AQ]
    EPwm2Regs.AQCTLA.bit.PRD = AQ_SET;            // 在计数器达到周期时候置高
    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // 在计数器向上计数到CMPA时候置低

    // EPwm2Regs.AQCTLB.bit.ZRO = AQ_CLEAR;       // 在计数器为0时候置0
    // EPwm2Regs.AQCTLB.bit.CAU = AQ_SET;         // 在计数器到达CMPB时候置高

    // 初始化事件触发模块 [ET]
    EPwm2Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // 选择中断在计数器为0时候触发
    EPwm2Regs.ETSEL.bit.INTEN = 1;                //
    EPwm2Regs.ETPS.bit.INTPRD = ET_1ST;           // 发生 1 次事件，产生 1 个中断

    EALLOW;
    PieVectTable.EPWM2_INT = &epwm2_timer_isr;
    EDIS;

    // 死区设置、斩波设置
    EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    // 上升沿延迟器输出端口不翻转，下升沿延迟器输出端口翻转；
    EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;
    // 死区 8 * 1/60MHz
    EPwm2Regs.DBFED = 8;
    // 死区 8 * 1/60MHz
    EPwm2Regs.DBRED = 8;

    // 使能时基计数时钟
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


void EPwm1A_SetCompare(Uint16 val)    //设置EPWM1A占空比
{
    EPwm1Regs.CMPA.half.CMPA = val;  //设置占空比
}


void EPwm1B_SetCompare(Uint16 val)  //设置EPWM1B占空比
{
    EPwm1Regs.CMPB = val;  //设置占空比
}

void EPwm2A_SetCompare(Uint16 val)    //设置EPWM2A占空比
{
    EPwm2Regs.CMPA.half.CMPA = val;  //设置占空比
}


void EPwm2B_SetCompare(Uint16 val)  //设置EPWM2B占空比
{
    EPwm2Regs.CMPB = val;  //设置占空比
}

interrupt void epwm1_timer_isr(void)
{
    static Uint16 cnt=0;
    cnt++;
    if(cnt==20000)
    {
        cnt=0;
    }

    // 清除此定时器的中断标志
    EPwm1Regs.ETCLR.bit.INT = 1;
    // 确认此中断，以便从组3接收更多中断
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

    // 清除此定时器的中断标志
    EPwm2Regs.ETCLR.bit.INT = 1;
    // 确认此中断，
    PieCtrlRegs.PIEACK.bit.ACK3 = 1;
}

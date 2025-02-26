// TI File $Revision: /main/3 $
// Checkin $Date: November 10, 2009   14:05:11 $
//###########################################################################
//
// FILE:   DSP2803x_EPwm.c
//
// TITLE:  DSP2803x EPwm Initialization & Support Functions.
//
//###########################################################################
// $TI Release: 2803x C/C++ Header Files V1.21 $
// $Release Date: December 1, 2009 $
//###########################################################################

#include "DSP2803x_Device.h"     // DSP2803x Headerfile Include File
#include "DSP2803x_EPwm_defines.h"
#include "DSP2803x_Examples.h"   // DSP2803x Examples Include File

//---------------------------------------------------------------------------
// InitEPwm:
//---------------------------------------------------------------------------
// This function initializes the EPwm(s) to a known state.
//
void InitEPwm(void)
{
   // Initialize EPwm1/2/3/4/5/6/7

   //tbd...

}

//---------------------------------------------------------------------------
// Example: InitEPwmGpio:
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as EPwm pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
//

void InitEPwmGpio(void)
{
   InitEPwm1Gpio();
   InitEPwm2Gpio();
   InitEPwm3Gpio();
#if DSP28_EPWM4
   InitEPwm4Gpio();
#endif // endif DSP28_EPWM4
#if DSP28_EPWM5
   InitEPwm5Gpio();
#endif // endif DSP28_EPWM5
#if DSP28_EPWM6
   InitEPwm6Gpio();
#endif // endif DSP28_EPWM6
#if DSP28_EPWM7
   InitEPwm7Gpio();
#endif // endif DSP28_EPWM7
}

void InitEPwm1Gpio(void)
{
   EALLOW;

/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;    // Disable pull-up on GPIO1 (EPWM1B)

/* Configure EPWM-1 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM1 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B

    EDIS;
}

void InitEPwm2Gpio(void)
{
   EALLOW;

/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // Disable pull-up on GPIO2 (EPWM2A)
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;    // Disable pull-up on GPIO3 (EPWM2B)

/* Configure EPwm-2 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM2 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B

    EDIS;
}

void InitEPwm3Gpio(void)
{
   EALLOW;

/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;    // Disable pull-up on GPIO4 (EPWM3A)
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;    // Disable pull-up on GPIO5 (EPWM3B)

/* Configure EPwm-3 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM3 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B

    EDIS;
}

#if DSP28_EPWM4
void InitEPwm4Gpio(void)
{
   EALLOW;
/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;    // Disable pull-up on GPIO6 (EPWM4A)
    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;    // Disable pull-up on GPIO7 (EPWM4B)

/* Configure EPWM-4 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM4 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // Configure GPIO6 as EPWM4A
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // Configure GPIO7 as EPWM4B

    EDIS;
}
#endif // endif DSP28_EPWM4

#if DSP28_EPWM5
void InitEPwm5Gpio(void)
{
   EALLOW;
/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;    // Disable pull-up on GPIO8 (EPWM5A)
    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;    // Disable pull-up on GPIO9 (EPWM5B)

/* Configure EPWM-5 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM5 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // Configure GPIO8 as EPWM5A
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // Configure GPIO9 as EPWM5B

    EDIS;
}
#endif // endif DSP28_EPWM5

#if DSP28_EPWM6
void InitEPwm6Gpio(void)
{
   EALLOW;
/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1;    // Disable pull-up on GPIO10 (EPWM6A)
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;    // Disable pull-up on GPIO11 (EPWM6B)

/* Configure EPWM-6 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM6 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;   // Configure GPIO10 as EPWM6A
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;   // Configure GPIO11 as EPWM6B

    EDIS;
}
#endif // endif DSP28_EPWM6

#if DSP28_EPWM7
void InitEPwm7Gpio(void)
{
   EALLOW;
/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPBPUD.bit.GPIO40 = 1;    // Disable pull-up on GPIO40 (EPWM7A)
    GpioCtrlRegs.GPBPUD.bit.GPIO41 = 1;    // Disable pull-up on GPIO41 (EPWM7B)

/* Configure EPWM-7 pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPWM7 functional pins.
// Comment out other unwanted lines.

    GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 1;   // Configure GPIO40 as EPWM7A
    GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 1;   // Configure GPIO41 as EPWM7B

    EDIS;
}
#endif // endif DSP28_EPWM7


//---------------------------------------------------------------------------
// Example: InitEPwmSyncGpio:
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as EPwm Synch pins
//

void InitEPwmSyncGpio(void)
{

//   EALLOW;

/* Configure EPWMSYNCI  */

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

// GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;    // Enable pull-up on GPIO6 (EPWMSYNCI)
   GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;   // Enable pull-up on GPIO32 (EPWMSYNCI)

/* Set qualification for selected pins to asynch only */
// This will select synch to SYSCLKOUT for the selected pins.
// Comment out other unwanted lines.

// GpioCtrlRegs.GPAQSEL1.bit.GPIO6 = 0;   // Synch to SYSCLKOUT GPIO6 (EPWMSYNCI)
   GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 0;  // Synch to SYSCLKOUT GPIO32 (EPWMSYNCI)

/* Configure EPwmSync pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be EPwmSync functional pins.
// Comment out other unwanted lines.

// GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 2;    // Configures GPIO6 for EPWMSYNCI operation
   GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 2;   // Configures GPIO32 for EPWMSYNCI operation.

/* Configure EPWMSYNC0  */

/* Disable internal pull-up for the selected output pins
   for reduced power consumption */
// Pull-ups can be enabled or disabled by the user.
// Comment out other unwanted lines.

// GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;    // Disable pull-up on GPIO6 (EPWMSYNCO)
   GpioCtrlRegs.GPBPUD.bit.GPIO33 = 1;   // Disable pull-up on GPIO33 (EPWMSYNCO)

// GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 3;   // Configures GPIO6 for EPWMSYNCO
   GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 2;  // Configures GPIO33 for EPWMSYNCO

}

//---------------------------------------------------------------------------
// Example: InitTzGpio:
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as Trip Zone (TZ) pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
//

void InitTzGpio(void)
{
   EALLOW;

   /* 启用选定引脚的内部上拉电阻 */
   // 上拉电阻可以由用户启用或禁用。
   // 以下代码将为指定的引脚启用上拉电阻。
   // 注释掉不需要的行。
   GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;    // 为 GPIO12 (TZ1) 启用上拉电阻
   // GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;    // 为 GPIO15 (TZ1) 启用上拉电阻
   GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;    // 为 GPIO13 (TZ2) 启用上拉电阻
   // GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;    // 为 GPIO16 (TZ2) 启用上拉电阻
   // GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;    // 为 GPIO28 (TZ2) 启用上拉电阻
   GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;    // 为 GPIO14 (TZ3) 启用上拉电阻
   // GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;    // 为 GPIO17 (TZ3) 启用上拉电阻
   // GpioCtrlRegs.GPAPUD.bit.GPIO29 = 0;    // 为 GPIO29 (TZ3) 启用上拉电阻

   /* 设置选定引脚的输入资格为异步 */
   // 输入默认同步到 SYSCLKOUT。
   // 以下代码将选择异步（无资格）作为选定引脚的输入模式。
   // 注释掉不需要的行。
   GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 3;  // GPIO12 (TZ1) 设置为异步输入
   // GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;  // GPIO15 (TZ1) 设置为异步输入
   GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3;  // GPIO13 (TZ2) 设置为异步输入
   // GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3;  // GPIO16 (TZ2) 设置为异步输入
   // GpioCtrlRegs.GPAQSEL2.bit.GPIO28 = 3;  // GPIO28 (TZ2) 设置为异步输入
   GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 3;  // GPIO14 (TZ3) 设置为异步输入
   // GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3;  // GPIO17 (TZ3) 设置为异步输入
   // GpioCtrlRegs.GPAQSEL2.bit.GPIO29 = 3;  // GPIO29 (TZ3) 设置为异步输入

   /* 使用 GPIO 寄存器配置 TZ 引脚 */
   // 以下代码指定了哪些 GPIO 引脚将作为 TZ 功能引脚使用。
   // 注释掉不需要的行。
   GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;  // 配置 GPIO12 作为 TZ1
   // GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;  // 配置 GPIO15 作为 TZ1
   GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;  // 配置 GPIO13 作为 TZ2
   // GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 3;  // 配置 GPIO16 作为 TZ2
   // GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 3;  // 配置 GPIO28 作为 TZ2
   GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;  // 配置 GPIO14 作为 TZ3
   // GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 3;  // 配置 GPIO17 作为 TZ3
   // GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 3;  // 配置 GPIO29 作为 TZ3

   EDIS;
}


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

//===========================================================================
// End of file.
//===========================================================================

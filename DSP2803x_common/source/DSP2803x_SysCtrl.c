// TI 文件版本 $Revision: /main/7 $
// 提交日期 $Date: November 10, 2009   14:05:15 $
//###########################################################################
//
// 文件名: DSP2803x_SysCtrl.c
//
// 标题: DSP2803x 设备系统控制初始化和支持函数。
//
// 描述:
//
//         系统资源的示例初始化。
//
//###########################################################################
// $TI 发布: 2803x C/C++ 头文件 V1.21 $
// $发布日期: December 1, 2009 $
//###########################################################################

#include "DSP2803x_Device.h"     // 包含设备头文件
#include "DSP2803x_Examples.h"   // 包含示例子程序头文件

// 需要在 RAM 中运行的函数需要分配到不同的段。该段将通过链接器命令文件映射到加载和运行地址。

#pragma CODE_SECTION(InitFlash, "ramfuncs");

//---------------------------------------------------------------------------
// InitSysCtrl:
//---------------------------------------------------------------------------
// 此函数将系统控制寄存器初始化为已知状态。
// - 禁用看门狗
// - 设置 PLLCR 以获得正确的 SYSCLKOUT 频率
// - 设置高、低频外设时钟的预分频器
// - 启用外设时钟

void InitSysCtrl(void)
{
    // 禁用看门狗
    DisableDog();

    // *重要*
    // Device_cal 函数会将 ADC 和振荡器校准值从 TI 保留的 OTP 复制到相应的调整寄存器，
    // 这个过程在引导 ROM 中自动完成。如果在调试过程中绕过了引导 ROM 代码，则必须调用此函数，
    // 以便 ADC 和振荡器按照规范工作。在调用此函数之前，必须启用 ADC 的时钟。
    // 更多信息请参阅设备数据手册和/或 ADC 参考手册。

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1; // 启用 ADC 外设时钟
    (*Device_cal)();                      // 调用校准函数
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 0; // 恢复 ADC 时钟到原始状态
    EDIS;

    // 选择内部振荡器 1 作为时钟源（默认），并关闭所有未使用的时钟以节省功耗。
    IntOsc1Sel();

    // 初始化 PLL 控制：PLLCR 和 CLKINDIV
    // DSP28_PLLCR 和 DSP28_DIVSEL 在 DSP2803x_Examples.h 中定义
    InitPll(DSP28_PLLCR,DSP28_DIVSEL);

    // 初始化外设时钟
    InitPeripheralClocks();
}

//---------------------------------------------------------------------------
// 示例: InitFlash:
//---------------------------------------------------------------------------
// 此函数初始化 Flash 控制寄存器

//                   注意
// 此函数必须在 RAM 中执行。在 OTP/Flash 中执行它会导致不可预测的结果

void InitFlash(void)
{
    EALLOW;
    // 启用 Flash 管道模式以提高从 Flash 执行代码的性能。
    FlashRegs.FOPT.bit.ENPIPE = 1;

    //                注意
    // 必须由 TI 对给定 CPU 速率下的 Flash 最小等待状态进行表征。
    // 请参阅数据手册获取最新信息。

    // 设置 Flash 的分页等待状态
    FlashRegs.FBANKWAIT.bit.PAGEWAIT = 2;

    // 设置 Flash 的随机等待状态
    FlashRegs.FBANKWAIT.bit.RANDWAIT = 2;

    // 设置 OTP 的等待状态
    FlashRegs.FOTPWAIT.bit.OTPWAIT = 2;

    //                注意
    // 仅应使用这些寄存器的默认值
    FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;
    FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;
    EDIS;

    // 强制刷新流水线，以确保对最后一个配置寄存器的写入在返回之前发生。

    asm(" RPT #7 || NOP");
}

//---------------------------------------------------------------------------
// 示例: ServiceDog:
//---------------------------------------------------------------------------
// 此函数重置看门狗定时器。
// 如果应用程序中使用 ServiceDog，请启用此功能

void ServiceDog(void)
{
    EALLOW;
    SysCtrlRegs.WDKEY = 0x0055;
    SysCtrlRegs.WDKEY = 0x00AA;
    EDIS;
}

//---------------------------------------------------------------------------
// 示例: DisableDog:
//---------------------------------------------------------------------------
// 此函数禁用看门狗定时器。

void DisableDog(void)
{
    EALLOW;
    SysCtrlRegs.WDCR = 0x0068;
    EDIS;
}

//---------------------------------------------------------------------------
// 示例: InitPll:
//---------------------------------------------------------------------------
// 此函数初始化 PLLCR 寄存器。

void InitPll(Uint16 val, Uint16 divsel)
{
    volatile Uint16 iVol;

    // 确保 PLL 不处于跛行模式
    if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 0)
    {
        EALLOW;
        // 检测到 OSCCLKSRC1 故障，PLL 处于跛行模式。
        // 重新启用丢失时钟逻辑。
        SysCtrlRegs.PLLSTS.bit.MCLKCLR = 1;
        EDIS;
        // 替换为适当的 SystemShutdown() 函数调用。
        asm("        ESTOP0");     // 用于调试目的
    }

    // 在更改 PLLCR 之前，DIVSEL 必须为 0
    // 外部复位 XRSn 将其设置为 0，这会使我们进入 1/4 模式
    if (SysCtrlRegs.PLLSTS.bit.DIVSEL != 0)
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;
        EDIS;
    }

    // 更改 PLLCR
    if (SysCtrlRegs.PLLCR.bit.DIV != val)
    {
        EALLOW;
        // 在设置 PLLCR 之前关闭丢失时钟检测逻辑
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
        SysCtrlRegs.PLLCR.bit.DIV = val;
        EDIS;

        // 可选：等待 PLL 锁定。
        // 在此期间，CPU 将切换到 OSCCLK/2 直到 PLL 稳定。
        // 一旦 PLL 稳定，CPU 将切换到新的 PLL 值。
        //
        // 这个锁定时间由 PLL 锁定计数器监控。
        //
        // 代码不需要在此等待 PLL 锁定。
        // 然而，如果代码执行的是时间关键操作，并且需要正确的时钟锁定，则最好等待切换完成。

        // 等待 PLL 锁定位被设置。

        // 在此循环前禁用看门狗，或在循环内通过 ServiceDog() 喂狗。

        // 取消注释以禁用看门狗
        DisableDog();

        while(SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1)
        {
            // 取消注释以喂狗
            // ServiceDog();
        }

        EALLOW;
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
        EDIS;
    }

    // 如果切换到 1/2
    if((divsel == 1)||(divsel == 2))
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = divsel;
        EDIS;
    }

    // 如果切换到 1/1
    // * 先切换到 1/2 并让电源稳定
    //   稳定所需的时间取决于系统，这里只是一个示例
    // * 然后切换到 1/1
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
// 示例: InitPeripheralClocks:
//---------------------------------------------------------------------------
// 此函数初始化外设模块的时钟。
// 首先设置高、低时钟预分频器
// 然后启用每个外设的时钟。
// 为了减少功耗，不使用的外设时钟应保持关闭
//
// 注意：如果未启用外设时钟，则无法读取或写入该外设的寄存器

void InitPeripheralClocks(void)
{
    EALLOW;

    // LOSPCP 预分频寄存器设置，通常设置为默认值

    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 3;  // GPIO18 = XCLKOUT
    SysCtrlRegs.LOSPCP.all = 0x0002;

    // XCLKOUT 到 SYSCLKOUT 的比率。默认情况下 XCLKOUT = 1/4 SYSCLKOUT
    SysCtrlRegs.XCLK.bit.XCLKOUTDIV = 2;

    // 启用选定外设的时钟。
    // 如果不使用某个外设，请关闭其时钟以节省功耗。
    //
    // 注意：并非所有 2803x 衍生产品都包含所有外设。
    // 请参阅您所使用设备的数据手册。
    //
    // 本函数不是高效的代码示例。

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

    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;     // 启用 ePWM 内部 TBCLK

    EDIS;
}

//---------------------------------------------------------------------------
// 示例: CsmUnlock:
//---------------------------------------------------------------------------
// 此函数解锁 CSM。用户必须用当前密码替换 0xFFFF。成功解锁返回 1。

#define STATUS_FAIL          0
#define STATUS_SUCCESS       1

Uint16 CsmUnlock()
{
    volatile Uint16 temp;

    // 使用当前密码加载密钥寄存器。0xFFFF 是虚拟密码。用户应替换为正确的密码。

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

    // 执行密码位置的虚拟读取
    // 如果它们匹配密钥值，CSM 将解锁

    temp = CsmPwl.PSWD0;
    temp = CsmPwl.PSWD1;
    temp = CsmPwl.PSWD2;
    temp = CsmPwl.PSWD3;
    temp = CsmPwl.PSWD4;
    temp = CsmPwl.PSWD5;
    temp = CsmPwl.PSWD6;
    temp = CsmPwl.PSWD7;

    // 如果 CSM 解锁，返回成功，否则返回失败。
    if (CsmRegs.CSMSCR.bit.SECURE == 0) return STATUS_SUCCESS;
    else return STATUS_FAIL;
}

//---------------------------------------------------------------------------
// 示例: IntOsc1Sel:
//---------------------------------------------------------------------------
// 此函数切换到内部振荡器 1 并关闭所有其他时钟源以最小化功耗

void IntOsc1Sel (void) {
    EALLOW;
    SysCtrlRegs.CLKCTL.bit.INTOSC1OFF = 0;
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL = 0;  // 时钟源 = INTOSC1
    SysCtrlRegs.CLKCTL.bit.XCLKINOFF = 1;     // 关闭 XCLKIN
    SysCtrlRegs.CLKCTL.bit.XTALOSCOFF = 1;    // 关闭 XTALOSC
    SysCtrlRegs.CLKCTL.bit.INTOSC2OFF = 1;    // 关闭 INTOSC2
    EDIS;
}

//---------------------------------------------------------------------------
// 示例: IntOsc2Sel:
//---------------------------------------------------------------------------
// 此函数从外部振荡器切换到内部振荡器 2，并关闭所有其他时钟源以最小化功耗
// 注意：如果没有外部时钟连接，在从 INTOSC1 切换到 INTOSC2 之前，
//       必须关闭 EXTOSC 和 XLCKIN

void IntOsc2Sel (void) {
    EALLOW;
    SysCtrlRegs.CLKCTL.bit.INTOSC2OFF = 0;     // 启用 INTOSC2
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRC2SEL = 1;  // 切换到 INTOSC2
    SysCtrlRegs.CLKCTL.bit.XCLKINOFF = 1;      // 关闭 XCLKIN
    SysCtrlRegs.CLKCTL.bit.XTALOSCOFF = 1;     // 关闭 XTALOSC
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL = 1;   // 切换到内部振荡器 2
    SysCtrlRegs.CLKCTL.bit.WDCLKSRCSEL = 1;    // 切换看门狗时钟源到 INTOSC2
    SysCtrlRegs.CLKCTL.bit.INTOSC1OFF = 1;     // 关闭 INTOSC1
    EDIS;
}
//---------------------------------------------------------------------------
// 示例: XtalOscSel:
//---------------------------------------------------------------------------
// 此函数切换到外部晶体振荡器并关闭所有其他时钟源以最小化功耗。
// 此选项可能不适用于所有设备封装

void XtalOscSel (void)  {
    EALLOW;
    SysCtrlRegs.CLKCTL.bit.XTALOSCOFF = 0;     // 启用 XTALOSC
    SysCtrlRegs.CLKCTL.bit.XCLKINOFF = 1;      // 关闭 XCLKIN
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRC2SEL = 0;  // 切换到外部时钟
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL = 1;   // 切换到外部时钟
    SysCtrlRegs.CLKCTL.bit.WDCLKSRCSEL = 1;    // 切换看门狗时钟源到外部时钟
    SysCtrlRegs.CLKCTL.bit.INTOSC2OFF = 1;     // 关闭 INTOSC2
    SysCtrlRegs.CLKCTL.bit.INTOSC1OFF = 1;     // 关闭 INTOSC1
    EDIS;
}

//---------------------------------------------------------------------------
// 示例: ExtOscSel:
//---------------------------------------------------------------------------
// 此函数切换到外部振荡器并关闭所有其他时钟源以最小化功耗。

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

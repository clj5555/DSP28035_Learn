#include "DSP2803x_Device.h"     // 包含 DSP2803x 设备头文件
#include "DSP2803x_Examples.h"   // 包含 DSP2803x 示例头文件

#define ADC_usDELAY  1000L       // 定义延迟时间常量，单位为微秒

//---------------------------------------------------------------------------
// InitAdc:
//---------------------------------------------------------------------------
// 此函数将 ADC 初始化到已知状态。
//
// 注意：2803x 设备的 ADC 初始化与其他 28x 设备不同
//
void InitAdc(void)
{
    extern void DSP28x_usDelay(Uint32 Count); // 声明外部延时函数

    // *重要*
    // Device_cal 函数会将 ADC 校准值从 TI 预留的 OTP 复制到 ADCREFSEL 和 ADCOFFTRIM 寄存器中。此操作在引导 ROM 中自动完成。如果调试过程中绕过了引导 ROM 代码，则必须调用此函数以确保 ADC 按照规范工作。ADC 时钟必须在调用此函数之前使能。

    EALLOW; // 允许访问受保护的寄存器
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1; // 使能 ADC 时钟
    (*Device_cal)(); // 调用校准函数
    EDIS; // 禁止访问受保护的寄存器

    // 为了启动 ADC，必须首先设置 ADCENCLK 位以启用时钟，然后依次启动带隙、参考电路和 ADC 内核。首次转换前需要等待至少 5ms 的延迟，以便所有模拟电路有足够的时间上电并稳定下来。

    // 注意：以下延时函数的正确运行依赖于 DSP2803x_Examples.h 文件中的 CPU_RATE 宏定义，该宏应包含正确的 CPU 时钟周期（纳秒）。
    EALLOW;
    AdcRegs.ADCCTL1.bit.ADCBGPWD  = 1;      // 启用 ADC 带隙电源
    AdcRegs.ADCCTL1.bit.ADCREFPWD = 1;      // 启用参考电压电源
    AdcRegs.ADCCTL1.bit.ADCPWDN   = 1;      // 启用 ADC 内核电源
    AdcRegs.ADCCTL1.bit.ADCENABLE = 1;      // 启用 ADC
    AdcRegs.ADCCTL1.bit.ADCREFSEL = 0;      // 选择内部带隙参考
    EDIS;

    DELAY_US(ADC_usDELAY);         // 在转换 ADC 通道前进行延迟
}

void InitAdcAio()
{
   EALLOW; // 允许访问受保护的寄存器

/* 使用 AIO 寄存器配置 ADC 引脚*/
// 这指定了哪些可能的 AIO 引脚将被配置为模拟输入引脚。
// 注意：AIO1,3,5,7-9,11,13,15 是所有 AIOMUX1 配置下的模拟输入引脚。请注释掉不需要的行。

    GpioCtrlRegs.AIOMUX1.bit.AIO2 = 2;    // 将 AIO2 配置为 A2（模拟输入）模式
    GpioCtrlRegs.AIOMUX1.bit.AIO4 = 2;    // 将 AIO4 配置为 A4（模拟输入）模式
    GpioCtrlRegs.AIOMUX1.bit.AIO6 = 2;    // 将 AIO6 配置为 A6（模拟输入）模式
    GpioCtrlRegs.AIOMUX1.bit.AIO10 = 2;   // 将 AIO10 配置为 B2（模拟输入）模式
    GpioCtrlRegs.AIOMUX1.bit.AIO12 = 2;   // 将 AIO12 配置为 B4（模拟输入）模式
    GpioCtrlRegs.AIOMUX1.bit.AIO14 = 2;   // 将 AIO14 配置为 B6（模拟输入）模式

    EDIS; // 禁止访问受保护的寄存器
}


/* AdcOffsetSelfCal-
   此函数通过使用 VREFLO 参考电压对 ADC 进行零偏移误差重新校准，并修改 ADCOFFTRIM 寄存器。VREFLO 通过内部多路复用选择连接到 A5，无需牺牲外部 ADC 引脚。此函数调用了两个其他函数：
   - AdcChanSelect(channel) — 选择要转换的 ADC 通道
   - AdcConversion() — 触发多次 ADC 转换并返回平均值
*/
void AdcOffsetSelfCal()
{
    Uint16 AdcConvMean;
    EALLOW;
    AdcRegs.ADCCTL1.bit.ADCREFSEL = 0;                  // 选择内部参考模式
    AdcRegs.ADCCTL1.bit.VREFLOCONV = 1;                 // 选择 VREFLO 内部连接至 B5
    AdcChanSelect(13);                                  // 选择所有 SOC 的 B5 通道
    AdcRegs.ADCOFFTRIM.bit.OFFTRIM = 80;                // 应用人工偏移 (+80)，以抵消 ADC 内核中可能出现的负偏移
    AdcConvMean = AdcConversion();                      // 对 VREFLO 进行 ADC 转换并获取平均值
    AdcRegs.ADCOFFTRIM.bit.OFFTRIM = 80 - AdcConvMean;  // 设置 offtrim 寄存器的新值（即移除人工偏移 (+80) 并创建偏移误差的二进制补码）
    AdcRegs.ADCCTL1.bit.VREFLOCONV = 0;                 // 选择外部 ADCIN5 输入引脚作为 B5
    EDIS;
}

/*  AdcChanSelect-
    此函数通过设置所有 SOC 的通道选择来选择要转换的 ADC 通道。

     * 注意 * 此函数会覆盖之前的 SOC 通道选择设置。建议保存之前的设置。
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
   此函数触发多次 ADC 转换并返回平均值。它使用 ADCINT1 和 ADCINT2 在 SOC0-7 和 SOC8-15 之间“乒乓”采样。

     * 注意 * 此函数会覆盖之前的 ADC 设置。建议保存之前的设置。
*/
Uint16 AdcConversion(void)
{
    Uint16 index, SampleSize, Mean, ACQPS_Value;
    Uint32 Sum;

    index       = 0;            // 初始化索引为 0
    SampleSize  = 256;          // 设置样本大小为 256（注意：样本大小必须是 2^x 的倍数，其中 x 是大于等于 4 的整数）
    Sum         = 0;            // 初始化求和变量为 0
    Mean        = 999;          // 初始化均值为一个已知值

    // 设置 ADC 采样窗口为所需值（采样窗口 = ACQPS + 1）
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


    // 启用乒乓采样

    // 启用 ADCINT1 和 ADCINT2
    AdcRegs.INTSEL1N2.bit.INT1E = 1;
    AdcRegs.INTSEL1N2.bit.INT2E = 1;

    // 禁用 ADCINT1 和 ADCINT2 的连续采样
    AdcRegs.INTSEL1N2.bit.INT1CONT = 0;
    AdcRegs.INTSEL1N2.bit.INT2CONT = 0;

    AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1;    // ADCINTs 在转换结束时触发

    // 设置 ADCINT1 和 ADCINT2 触发源
    AdcRegs.INTSEL1N2.bit.INT1SEL = 6;      // EOC6 触发 ADCINT1
    AdcRegs.INTSEL1N2.bit.INT2SEL = 14;     // EOC14 触发 ADCINT2

    // 设置每个 SOC 的 ADCINT 触发源
    AdcRegs.ADCINTSOCSEL1.bit.SOC0  = 2;    // ADCINT2 触发 SOC0-7
    AdcRegs.ADCINTSOCSEL1.bit.SOC1  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC2  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC3  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC4  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC5  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC6  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC7  = 2;
    AdcRegs.ADCINTSOCSEL2.bit.SOC8  = 1;    // ADCINT1 触发 SOC8-15
    AdcRegs.ADCINTSOCSEL2.bit.SOC9  = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC10 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC11 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC12 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC13 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC14 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC15 = 1;

    DELAY_US(ADC_usDELAY);                  // 在转换 ADC 通道前进行延迟


    // ADC 转换

    AdcRegs.ADCSOCFRC1.all = 0x00FF;  // 强制启动 SOC0-7 以开始乒乓采样

    while( index < SampleSize ){

        // 等待 ADCINT1 触发，然后将 ADCRESULT0-7 寄存器的值加到总和中
        while (AdcRegs.ADCINTFLG.bit.ADCINT1 == 0){}
        AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;   // 必须清除 ADCINT1 标志，因为 INT1CONT = 0
        Sum += AdcResult.ADCRESULT0;
        Sum += AdcResult.ADCRESULT1;
        Sum += AdcResult.ADCRESULT2;
        Sum += AdcResult.ADCRESULT3;
        Sum += AdcResult.ADCRESULT4;
        Sum += AdcResult.ADCRESULT5;
        Sum += AdcResult.ADCRESULT6;
        Sum += AdcResult.ADCRESULT7;

        // 等待 ADCINT2 触发，然后将 ADCRESULT8-15 寄存器的值加到总和中
        while (AdcRegs.ADCINTFLG.bit.ADCINT2 == 0){}
        AdcRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;   // 必须清除 ADCINT2 标志，因为 INT2CONT = 0
        Sum += AdcResult.ADCRESULT8;
        Sum += AdcResult.ADCRESULT9;
        Sum += AdcResult.ADCRESULT10;
        Sum += AdcResult.ADCRESULT11;
        Sum += AdcResult.ADCRESULT12;
        Sum += AdcResult.ADCRESULT13;
        Sum += AdcResult.ADCRESULT14;
        Sum += AdcResult.ADCRESULT15;

        index+=16;

    } // 数据采集结束

    // 禁用 ADCINT1 和 ADCINT2 以停止乒乓采样
    AdcRegs.INTSEL1N2.bit.INT1E = 0;
    AdcRegs.INTSEL1N2.bit.INT2E = 0;

    Mean = Sum / SampleSize;    // 计算平均 ADC 样本值

    return Mean;                // 返回平均值

}// end AdcConversion

//===========================================================================
// 文件结束






/*
 * adc.c
 *
 *  Created on: 2018-1-29
 *      Author: Administrator
 */

#include "adc.h"


// 全局变量，用于存储循环计数
Uint16 LoopCount;
Uint16 ConversionCount; //全局变量，用于存储转换计数
Uint16 Voltage1[10];
Uint16 Voltage2[10];


// 初始化ADC模块
void ADC_Init(void)
{
    EALLOW; // 允许访问受保护的寄存器
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;    // ADC 使能外设时钟
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;   // Disable TBCLK within the ePWM
    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1;  // ePWM1
    EDIS; // 禁止访问受保护的寄存器

    // Specific clock setting for this example:28335
//    EALLOW;
//    SysCtrlRegs.HISPCP.all = ADC_MODCLK;    // HSPCLK = SYSCLKOUT/2*ADC_MODCLK
//    EDIS;

    // 中断映射设置
    EALLOW;
    PieVectTable.ADCINT1 = &adc_isr; // 将ADCINT1中断映射到adc_isr中断服务例程
    EDIS;

    InitAdc();  // 初始化ADC模块
    AdcOffsetSelfCal(); // 执行ADC偏移自校准

    // 初始化中断
    PieCtrlRegs.PIEIER1.bit.INTx1 = 1;  // 在PIE中使能INT 1.1中断
    IER |= M_INT1; // 使能CPU中断1
    EINT; // 全局使能中断INTM
    ERTM; // 全局使能实时中断DBGM

    LoopCount = 0; // 初始化循环计数变量
    ConversionCount = 0; // 初始化转换计数变量

    // 将内部温度传感器连接到ADCINA5通道
    EALLOW;
    AdcRegs.ADCCTL1.bit.TEMPCONV = 1;
    EDIS;

    // 配置ADC
    EALLOW;
    AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1; // ADCINT1在AdcResults锁存到其结果寄存器前1个周期触发
    AdcRegs.INTSEL1N2.bit.INT1E = 1; // 使能ADCINT1
    AdcRegs.INTSEL1N2.bit.INT1CONT = 0; // 禁用ADCINT1连续模式
    AdcRegs.INTSEL1N2.bit.INT1SEL = 5; // 设置EOC5触发ADCINT1


    AdcRegs.ADCSOC0CTL.bit.CHSEL = 0; // 设置SOC0通道选择ADCINA0
    AdcRegs.ADCSOC1CTL.bit.CHSEL = 0; // 设置SOC1通道选择ADCINA0
    AdcRegs.ADCSOC2CTL.bit.CHSEL = 1; // 设置SOC2通道选择ADCINA1
    AdcRegs.ADCSOC3CTL.bit.CHSEL = 3; // 设置SOC3通道选择ADCINA1
    AdcRegs.ADCSOC4CTL.bit.CHSEL = 5; // 设置SOC4通道选择ADCINA5
    AdcRegs.ADCSOC5CTL.bit.CHSEL = 6; // 设置SOC5通道选择ADCINA6

    AdcRegs.ADCSOC0CTL.bit.TRIGSEL = 5;
    AdcRegs.ADCSOC1CTL.bit.TRIGSEL = 5; // 设置SOC1由EPWM1A触发
    AdcRegs.ADCSOC2CTL.bit.TRIGSEL = 5; // 设置SOC2由EPWM1A触发
    AdcRegs.ADCSOC3CTL.bit.TRIGSEL = 5; // 设置SOC3由EPWM1A触发
    AdcRegs.ADCSOC4CTL.bit.TRIGSEL = 5; // 设置SOC4由EPWM1A触发
    AdcRegs.ADCSOC5CTL.bit.TRIGSEL = 5; // 设置SOC5由EPWM1A触发  //暂时由EPWM1触发，后续需要修改成EPWM3

    AdcRegs.ADCSOC0CTL.bit.ACQPS = 6;
    AdcRegs.ADCSOC1CTL.bit.ACQPS = 6; // 设置SOC1采样保持窗口为7个ADC时钟周期
    AdcRegs.ADCSOC2CTL.bit.ACQPS = 6; // 设置SOC2采样保持窗口为7个ADC时钟周期
    AdcRegs.ADCSOC3CTL.bit.ACQPS = 6; // 设置SOC3采样保持窗口为7个ADC时钟周期
    AdcRegs.ADCSOC4CTL.bit.ACQPS = 6; // 设置SOC4采样保持窗口为7个ADC时钟周期
    AdcRegs.ADCSOC5CTL.bit.ACQPS = 6; // 设置SOC5采样保持窗口为7个ADC时钟周期
    EDIS;

// 假设ePWM1时钟已经在InitSysCtrl();中启用 此处代码已经移动至EPWM中，无需再使用
//    EPwm1Regs.ETSEL.bit.SOCAEN = 1; // 使能A组SOC
//    EPwm1Regs.ETSEL.bit.SOCASEL = 4; // 选择从CPMA上计数选择SOC
//    EPwm1Regs.ETPS.bit.SOCAPRD = 1; // 在第一个事件上生成脉冲
//    EPwm1Regs.CMPA.half.CMPA = 0x0080; // 设置比较A值
//    EPwm1Regs.TBPRD = 0xFFFF; // 设置ePWM1周期
//    EPwm1Regs.TBCTL.bit.CTRMODE = 0; // 设置计数模式为向上计数并开始

}


// 中断服务例程，用于处理ADC中断
interrupt void  adc_isr(void)
{
    // 使用static关键字确保i在中断服务程序之间保持其值
    static Uint16 i=0;
    i++; // 增加计数器i的值

    ADCSample();
    PwmReflash();

    // 清除ADCINT1标志位，为下一次SOC（Start of Conversion）做准备
    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    // 向PIE（Programmable Interrupt Controller）确认中断处理完成
    PieCtrlRegs.PIEACK.bit.ACK1 = 1;
}


/*
** ===================================================================
**     Funtion Name :   void ADCSample(void)
**     Description :    采样输出电压、输出电流、滑动变阻器电压值（输出电压参考值设置）并对其求平均
**     Parameters  :
**     Returns     :
** ===================================================================
*/
void ADCSample(void)
{


      // 存储ADC转换结果到全局变量数组中，忽略ADCRESULT0作为修正错误的方法
      Voltage1[ConversionCount] = AdcResult.ADCRESULT1;
      Voltage2[ConversionCount] = AdcResult.ADCRESULT2;

      // 如果已经记录了20次转换，重置计数器
      if(ConversionCount == 9)
      {
         ConversionCount = 0;
      }
      else
      {
          ConversionCount++; // 否则，增加转换计数器的值
      }
}




/*
** ===================================================================
**     Funtion Name : void PwmReflash(void)
**     Description :  PWM寄存器更新函数
** ===================================================================
*/
#define PSFB_HALF 600

void PwmReflash(void)
{
    //定义频率对应的周期量
  //  static int PhaseShift;

//    //Q15移相角控制量更改成实际PWM数字量
//    PhaseShift = (int)((ID2DCtrValue.Phase*PSFB_HALF)>>15);
//
//    //更新寄存器，保持EPWE1不动，调整EPWM2，实现移相
//    EALLOW;
//    EPwm2Regs.CMPA.half.CMPA=PhaseShift;
//    EPwm2Regs.CMPB=PSFB_HALF+PhaseShift;
//    //更改ADC触发点
//    EPwm3Regs.CMPA.half.CMPA=(PhaseShift>>1);
//    EDIS;
}



//该函数使用存储在OTP中的参考数据来转换原始温度
//传感器读数为℃
int16 GetTemperatureC(int16 sensorSample)
{
    return ((sensorSample - getTempOffset())*(int32)getTempSlope() + FP_ROUND + KELVIN_OFF)/FP_SCALE - KELVIN;
}

//传感器读数为K
int16 GetTemperatureK(int16 sensorSample)
{
    return ((sensorSample - getTempOffset())*(int32)getTempSlope() + FP_ROUND + KELVIN_OFF)/FP_SCALE;
}











/*
 * adc.h
 *
 *  Created on: 2018-1-29
 *      Author: Administrator
 */

#ifndef ADC_H_
#define ADC_H_

#include "DSP2803x_Device.h"     // DSP2833x 头文件
#include "DSP2803x_Examples.h"   // DSP2833x 例子相关头文件



#define ADC_MODCLK 3

#define BUF_SIZE   16  // Sample buffer size
extern Uint16 SampleTable[BUF_SIZE];

//内部温度采集相关变量
//温度传感器变化率 (deg. C / ADC code).  存储在定点Q15格式
#define getTempSlope() (*(int (*)(void))0x3D7E82)()
//ADC代码对应温度传感器输出在0 deg. C
#define getTempOffset() (*(int (*)(void))0x3D7E85)()
#define FP_SCALE 32768       //定点数字的比例因子Q15 (2^15)
#define FP_ROUND FP_SCALE/2  //在转换为整数之前添加到Q15数字以四舍五入该数字
// Q15开尔文
#define KELVIN 273
#define KELVIN_OFF FP_SCALE*KELVIN


void ADC_Init(void);
Uint16 Read_ADC_CH0_Value(void);
void Read_ADC_SEQ1_Value_OVD(void);
interrupt void  adc_isr(void);

void ADCSample(void);
void PwmReflash(void);

//内部温度采集函数
int16 GetTemperatureC(int16 sensorSample);
int16 GetTemperatureK(int16 sensorSample);
#endif /* ADC_H_ */

/*
 * adc.h
 *
 *  Created on: 2018-1-29
 *      Author: Administrator
 */

#ifndef ADC_H_
#define ADC_H_

#include "DSP2803x_Device.h"     // DSP2833x ͷ�ļ�
#include "DSP2803x_Examples.h"   // DSP2833x �������ͷ�ļ�



#define ADC_MODCLK 3

#define BUF_SIZE   16  // Sample buffer size
extern Uint16 SampleTable[BUF_SIZE];

//�ڲ��¶Ȳɼ���ر���
//�¶ȴ������仯�� (deg. C / ADC code).  �洢�ڶ���Q15��ʽ
#define getTempSlope() (*(int (*)(void))0x3D7E82)()
//ADC�����Ӧ�¶ȴ����������0 deg. C
#define getTempOffset() (*(int (*)(void))0x3D7E85)()
#define FP_SCALE 32768       //�������ֵı�������Q15 (2^15)
#define FP_ROUND FP_SCALE/2  //��ת��Ϊ����֮ǰ��ӵ�Q15�������������������
// Q15������
#define KELVIN 273
#define KELVIN_OFF FP_SCALE*KELVIN


void ADC_Init(void);
Uint16 Read_ADC_CH0_Value(void);
void Read_ADC_SEQ1_Value_OVD(void);
interrupt void  adc_isr(void);

void ADCSample(void);
void PwmReflash(void);

//�ڲ��¶Ȳɼ�����
int16 GetTemperatureC(int16 sensorSample);
int16 GetTemperatureK(int16 sensorSample);
#endif /* ADC_H_ */

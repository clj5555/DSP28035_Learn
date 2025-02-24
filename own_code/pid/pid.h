/*
 * pid.h
 *
 *  Created on: 2018-1-29
 *      Author: Administrator
 */

#ifndef PID_H_
#define PID_H_


typedef struct
{
  float Kp;                       //����ϵ��Proportional
  float Ki;                       //����ϵ��Integral
  float Kd;                       //΢��ϵ��Derivative

  float Ek;                       //��ǰ���
  float Ek1;                      //ǰһ����� e(k-1)
  float Ek2;                      //��ǰһ����� e(k-2)
}PID_IncTypeDef;


float PID_Inc(float SetValue, float ActualValue, PID_IncTypeDef *PID);


#endif

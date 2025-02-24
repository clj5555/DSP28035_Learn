/************************************************
�������� ��PID_Inc
��    �� ��PID����(Increment)����
��    �� ��SetValue ------ ����ֵ(����ֵ)
            ActualValue --- ʵ��ֵ(����ֵ)
            PID ----------- PID���ݽṹ
�� �� ֵ ��PIDInc -------- ����PID����(+/-)
��    �� ��strongerHuang
*************************************************/

#include "pid.h"


float PID_Inc(float SetValue, float ActualValue, PID_IncTypeDef *PID)
{
    float PIDInc;                                  //����

    PID->Ek = SetValue - ActualValue;
    PIDInc = (PID->Kp * PID->Ek) - (PID->Ki * PID->Ek1) + (PID->Kd * PID->Ek2);

    PID->Ek2 = PID->Ek1;
    PID->Ek1 = PID->Ek;

    return PIDInc;
}

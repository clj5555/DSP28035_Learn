/************************************************
函数名称 ：PID_Inc
功    能 ：PID增量(Increment)计算
参    数 ：SetValue ------ 设置值(期望值)
            ActualValue --- 实际值(反馈值)
            PID ----------- PID数据结构
返 回 值 ：PIDInc -------- 本次PID增量(+/-)
作    者 ：strongerHuang
*************************************************/

#include "pid.h"


float PID_Inc(float SetValue, float ActualValue, PID_IncTypeDef *PID)
{
    float PIDInc;                                  //增量

    PID->Ek = SetValue - ActualValue;
    PIDInc = (PID->Kp * PID->Ek) - (PID->Ki * PID->Ek1) + (PID->Kd * PID->Ek2);

    PID->Ek2 = PID->Ek1;
    PID->Ek1 = PID->Ek;

    return PIDInc;
}

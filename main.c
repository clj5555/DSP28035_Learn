#include "DSP28x_Project.h"
#include <string.h>
#include <stdint.h>
#include "oled.h"
#include "iic.h"
#include "adc.h"
#include "sci.h"

// 定义要控制的变量
volatile Uint16 controlValue = 0;

extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadSize;
extern Uint16 RamfuncsRunStart;

//中断接收用的标志和缓存变量定义
extern  Uint16   usart1_rx_length;
extern  Uint16   Frame_Length;

// 全局变量
Uint16 Solar_value=500;
Uint16 VoltageAD=0;
extern unsigned short Voltage1[10],Voltage2[10];
int16 temp; //raw temperature sensor reading
int16 degC; //temperature in deg. C
int16 degK; //temperature in deg. K
int32 Time0=1000000;//4294967295
int16 Vin=0;

extern Uint16  COM1_RxBuff[USART1_RX_LENGTH_MAX];

void delay_loop()
{
    long      i;
    for (i = 0; i < 1000000; i++) {}
}




int main(void)
{
    // Step 1. 系统初始化
    InitSysCtrl();   //系统时钟初始化:PLL, WatchDog, enable Peripheral Clocks
    memcpy((uint16_t *)&RamfuncsRunStart, (uint16_t *)&RamfuncsLoadStart, (unsigned long)&RamfuncsLoadSize);
    InitFlash();
    InitPieCtrl();    // 初始化PIE control
    // 关闭 CPU interrupts and clear all CPU interrupt flags:
    IER = 0x0000;//关闭全局总中断全部位
    IFR = 0x0000;//关闭中断标志位全部位
    InitPieVectTable(); // PIE 向量初始化 Interrupt Service Routines (ISR)
    Init_I2CA();

    //EXTI1_Init();
    OLED_Init();//OLED初始化

    USARTA_Init(38400);
    Prints("hello wrold!");
    Prints("USART Test Begin:\r\n");
    Prints("Send DSP Some data,It Will Send Them Bcak!\r\n");
    TIM0_Init(60, Time0);
    ADC_Init();
    EPWM1_Init(3000);//20k
    EPWM2_Init(3000);//20k

    // 全局中断使能
    EINT;
    ERTM;
    
    OLED_Clear();
    OLED_ShowString(1,0,"This is an OLED",8);//
    OLED_ShowString(1,1,"example using",8);//
    OLED_ShowString(1,2,"hardware I2C",8);//
    while(1)
    {
        int32 voltage_int = (int32)(Vin * 1000);


 //       PrintLongInt(voltage_int);
       // Vin=3.3/4095.0*910.0/51.0*Voltage1[2];
        Vin=3.3/4095.0*Voltage1[2]*10.0;
        EPwm1A_SetCompare(1500);
        EPwm2A_SetCompare(1500);
        degC = GetTemperatureC(Voltage2[2]);
        degK = GetTemperatureK(Voltage2[2]);
        OLED_ShowNum(0,3,Voltage1[2],5,16);
        OLED_ShowNum(40,3,Vin,3,16);
        OLED_ShowNum(0,5,degC,3,16);
        OLED_ShowNum(40,5,degK,3,16);
//         delay_loop();
//         LED1_ON;
//         delay_loop();
//         LED1_OFF;
        //接收到的数据原路发送回去
//        // 检查是否有接收到数据
//        if (usart1_rx_length > 0)
//        {
//            Uint16 i;
//            // 发送接收到的数据
//            for (i = 0; i < usart1_rx_length; i++)
//            {
//                USART_Transmit(COM1_RxBuff[i]);
//            }
//
//            // 重置接收数据长度计数器
//            usart1_rx_length = 0;
//        }



    }
}


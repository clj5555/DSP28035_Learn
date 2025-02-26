/*
 * modbus.h
 *
 *  Created on: 2022��3��19��
 *      Author: Administrator
 */

#ifndef MYBSP_MODBUS_MODBUS_H_
#define MYBSP_MODBUS_MODBUS_H_

#include "DSP28x_Project.h"

#define REG_LAST_ADDR     0x0088

//extern  Uint32 MODBUS_BaudRate[10];
extern  Uint16   Flash_Modbus_ADDR;
extern  Uint16   Flash_Usart_BaudRate;

#define Delay_US()      DELAY_US(1)
#define Delay_nUS(n)    DELAY_US(n)

void Delay_nMS(Uint16 nms);

void MODBUS_Send(Uint16 *buf,unsigned int length);
void MODBUS(Uint16 *buf,unsigned int length,unsigned int UsartNUM);
void MODBUS_ReadFromEEPROM(void);
void MODBUS_event(void);
void MODBUS_datain(void);
void MODBUS_03_Return(unsigned int startaddr,unsigned int number);
void MODBUS_10_Return(unsigned int addr,unsigned int code);
unsigned int CRC_Calculate(Uint16 *str,unsigned int usDataLen);
static Uint16 get_crc16(Uint16 *pbuff, Uint16 len);
extern unsigned short modbus_reg[];

//
static void modbus_01_solve(void);
static void modbus_03_solve(void);
static void modbus_05_solve(void);
static void modbus_06_solve(void);
static void modbus_10_solve(void);

void modbus_05_handle(Uint16 *buf);
void modbus_06_handle(Uint16 *buf);

#endif /* MYBSP_MODBUS_MODBUS_H_ */

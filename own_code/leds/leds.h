/*
 * led.h
 *
 *  Created on: 2018-1-20
 *      Author: Administrator
 */

#ifndef _LEDS_H_
#define _LEDS_H_




#define LED1_OFF        (GpioDataRegs.GPASET.bit.GPIO3=1)
#define LED1_ON         (GpioDataRegs.GPACLEAR.bit.GPIO3=1)
#define LED1_TOGGLE     (GpioDataRegs.GPATOGGLE.bit.GPIO3=1)

#define LED2_OFF        (GpioDataRegs.GPBSET.bit.GPIO44=1)
#define LED2_ON         (GpioDataRegs.GPBCLEAR.bit.GPIO44=1)
#define LED2_TOGGLE     (GpioDataRegs.GPBTOGGLE.bit.GPIO44=1)

#define LED3_OFF        (GpioDataRegs.GPASET.bit.GPIO23=1)
#define LED3_ON         (GpioDataRegs.GPACLEAR.bit.GPIO23=1)
#define LED3_TOGGLE     (GpioDataRegs.GPATOGGLE.bit.GPIO23=1)

#define LED4_OFF        (GpioDataRegs.GPBSET.bit.GPIO33=1)
#define LED4_ON         (GpioDataRegs.GPBCLEAR.bit.GPIO33=1)
#define LED4_TOGGLE     (GpioDataRegs.GPBTOGGLE.bit.GPIO33=1)


void LED_Init(void);


#endif /* LED_H_ */

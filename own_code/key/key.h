/*
 * key.h
 *  Created on: 2018-1-22
 *      Author: Administrator
 */

#ifndef KEY_H_
#define KEY_H_


#define KEY_L1_SetL         (GpioDataRegs.GPACLEAR.bit.GPIO12=1)
#define KEY_L2_SetL         (GpioDataRegs.GPACLEAR.bit.GPIO19=1)
#define KEY_L3_SetL         (GpioDataRegs.GPACLEAR.bit.GPIO17=1)

#define KEY_L1_SetH         (GpioDataRegs.GPASET.bit.GPIO12=1)
#define KEY_L2_SetH         (GpioDataRegs.GPASET.bit.GPIO19=1)
#define KEY_L3_SetH         (GpioDataRegs.GPASET.bit.GPIO17=1)

#define KEY_H1          (GpioDataRegs.GPADAT.bit.GPIO12)
#define KEY_H2          (GpioDataRegs.GPADAT.bit.GPIO19)
#define KEY_H3          (GpioDataRegs.GPADAT.bit.GPIO17)

#define KEY1_PRESS      1
#define KEY2_PRESS      2
#define KEY3_PRESS      3
#define KEY4_PRESS      4
#define KEY5_PRESS      5
#define KEY6_PRESS      6
#define KEY7_PRESS      7
#define KEY8_PRESS      8
#define KEY9_PRESS      9
#define KEY_UNPRESS     0


void KEY_Init(void);
/*char KEY_Scan(char mode);*/

#endif /* KEY_H_ */

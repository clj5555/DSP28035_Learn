/*
 * time.h
 *
 *  Created on: 2018-1-24
 *      Author: Administrator
 */

#ifndef TIM_H_
#define TIM_H_






void TIM0_Init(float Freq, float Period);
interrupt void TIM0_IRQn(void);

void TIM1_Init(float Freq, float Period);
interrupt void TIM1_IRQn(void);

void TIM2_Init(float Freq, float Period);
interrupt void TIM2_IRQn(void);


#endif /* TIME_H_ */

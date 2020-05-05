/*
 * timers.h
 *
 *  Created on: 27 июля 2018 г.
 *      Author: vasilek
 */

#ifndef TIMERS_H_
#define TIMERS_H_
#include "stm32f4xx_hal.h"

#if defined(__STM32F4xx_HAL_TIM_H) || defined(STM32F4xx_HAL_TIM_H)

extern void tim_UpdatePeriod(void);
extern unsigned int tim_GetPeriod(void);
extern void tim_StartTimer(TIM_HandleTypeDef *par_htim);
extern unsigned int tim_GetTimeFromStartMS(void);
extern unsigned int tim_GetTimeFromStartSEC(void);
extern void tim_InitTimer(TIM_HandleTypeDef *par_htim);

#endif /* defined(__STM32F4xx_HAL_TIM_H) || defined(STM32F4xx_HAL_TIM_H) */
#endif /* TIMERS_H_ */

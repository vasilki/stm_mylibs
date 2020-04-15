/*
 * adc.h
 *
 *  Created on: 26 июля 2018 г.
 *      Author: vasilek
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32f4xx_hal.h"

#ifdef __STM32F4xx_ADC_H

unsigned int adc_GetValue(ADC_HandleTypeDef *par_hadc);
unsigned int adc_GetNormalizeValue(ADC_HandleTypeDef *par_hadc, unsigned int par_coeff);

#endif /* __STM32F4xx_ADC_H */
#endif /* ADC_H_ */

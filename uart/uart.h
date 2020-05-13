/*
 * uart.h
 *
 *  Created on: 28 июля 2018 г.
 *      Author: vasilek
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f4xx_hal.h"

#ifdef __STM32F4xx_HAL_UART_H

void uart_Init(UART_HandleTypeDef *par_uart);
void uart_Printf(const uint8_t *par_string);
void uart_PrintfInteger(int par_value, const char *par_base);
void uart_PrintfBuildVersion(void);
void uart_Scanf(uint8_t *par_buffer, uint32_t *par_size);

#define UART_PRINTFINTEGER(A,B) \
{ \
extern void uart_PrintfInteger(int par_value, const char *par_base); \
uart_PrintfInteger(A,B); \
}


#endif /* __STM32F4xx_HAL_UART_H */
#endif /* UART_H_ */
